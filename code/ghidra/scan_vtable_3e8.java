// Enumerate vtables in .rdata/.data (runs of consecutive .text pointers), and for
// every vtable long enough to have a slot at byte offset 0x3E8 (index 125), record
// the function pointer at that slot. Then keep only slot-0x3E8 targets whose body
// loads [this+0x100] (StatusInstance) -> those are the attribute-component level/status
// getters the aura power-compare code calls via [[char+0x8E8]+0x3E8]().
// Output -> decompiled/_vtable_3e8.txt
//@category Kakarot
import ghidra.app.script.GhidraScript;
import ghidra.program.model.address.Address;
import ghidra.program.model.listing.Function;
import ghidra.program.model.listing.FunctionManager;
import ghidra.program.model.listing.Instruction;
import ghidra.program.model.mem.Memory;
import ghidra.program.model.mem.MemoryBlock;
import ghidra.program.model.scalar.Scalar;
import java.io.File;
import java.io.FileWriter;
import java.io.PrintWriter;
import java.util.LinkedHashMap;
import java.util.Map;
import java.util.List;
import java.util.ArrayList;

public class scan_vtable_3e8 extends GhidraScript {
    static final String OUT_DIR =
        "D:\\code\\unreal\\dragon ball kakarot access\\code\\decompiled";
    static final long SLOT = 0x3E8;

    Memory mem;
    long textStart, textEnd;
    FunctionManager fm;

    boolean inText(long a) { return a >= textStart && a < textEnd; }
    long u64(byte[] b, int i) {
        long v = 0;
        for (int k = 0; k < 8; k++) v |= ((long)(b[i+k] & 0xff)) << (8*k);
        return v;
    }

    @Override
    public void run() throws Exception {
        new File(OUT_DIR).mkdirs();
        mem = currentProgram.getMemory();
        fm = currentProgram.getFunctionManager();
        MemoryBlock text = mem.getBlock(".text");
        textStart = text.getStart().getOffset();
        textEnd = text.getEnd().getOffset();

        String[] wanted = {".rdata", ".data"};
        // map slotTarget -> list of vtableBase(s)
        Map<Long, List<Long>> slotTargets = new LinkedHashMap<>();
        for (String bn : wanted) {
            MemoryBlock b = mem.getBlock(bn);
            if (b == null) continue;
            byte[] arr = new byte[(int) b.getSize()];
            b.getBytes(b.getStart(), arr);
            long base = b.getStart().getOffset();
            int i = 0;
            while (i + 8 <= arr.length) {
                // find start of a run of .text pointers preceded by non-.text
                boolean prevText = (i >= 8) && inText(u64(arr, i - 8));
                if (inText(u64(arr, i)) && !prevText) {
                    int start = i;
                    int j = i;
                    while (j + 8 <= arr.length && inText(u64(arr, j))) j += 8;
                    int runBytes = j - start;
                    if (runBytes > SLOT) {
                        long target = u64(arr, start + (int)SLOT);
                        long vbase = base + start;
                        slotTargets.computeIfAbsent(target, k -> new ArrayList<>()).add(vbase);
                    }
                    i = j;
                } else {
                    i += 8;
                }
            }
        }

        PrintWriter pw = new PrintWriter(new FileWriter(new File(OUT_DIR, "_vtable_3e8.txt")));
        pw.println("// distinct functions at vtable slot 0x3E8 (index 125): " + slotTargets.size());
        int flagged = 0;
        for (Map.Entry<Long, List<Long>> e : slotTargets.entrySet()) {
            long target = e.getKey();
            Function f = fm.getFunctionAt(currentProgram.getAddressFactory().getDefaultAddressSpace().getAddress(target));
            String fname = f == null ? "(no func)" : f.getName();
            long readOff = readsThis100Off(f);
            String tag = (readOff >= 0) ? ("  <== reads [[this+0x100]+0x" + Long.toHexString(readOff) + "]") : "";
            if (readOff >= 0) flagged++;
            StringBuilder sb = new StringBuilder();
            sb.append("slot0x3E8 target 0x").append(Long.toHexString(target)).append("  ").append(fname)
              .append("  usedByVtables=").append(e.getValue().size()).append(tag);
            // show first couple vtable bases
            for (int k = 0; k < Math.min(3, e.getValue().size()); k++)
                sb.append("  vb=0x").append(Long.toHexString(e.getValue().get(k)));
            pw.println(sb.toString());
        }
        pw.println("// flagged (read [this+0x100] then offset): " + flagged);
        pw.close();
        println("VT3E8: done, distinct=" + slotTargets.size() + " flagged=" + flagged);
    }

    // If function loads [this(rcx)+0x100] into a reg and then reads a small offset off it,
    // return that small offset; else -1.
    long readsThis100Off(Function f) {
        if (f == null) return -1;
        try {
            long size = f.getBody().getNumAddresses();
            if (size > 0x200) return -1;
            List<Instruction> insns = new ArrayList<>();
            var ii = currentProgram.getListing().getInstructions(f.getBody(), true);
            while (ii.hasNext()) insns.add(ii.next());
            for (int i = 0; i < insns.size(); i++) {
                Instruction ins = insns.get(i);
                if (!ins.getMnemonicString().equalsIgnoreCase("MOV")) continue;
                String reg = movLoadReg(ins, 0x100);
                if (reg == null) continue;
                for (int j = i+1; j < insns.size() && j <= i+14; j++) {
                    Instruction n = insns.get(j);
                    if (!n.getMnemonicString().equalsIgnoreCase("MOV")) continue;
                    long disp = movLoadFromBase(n, reg);
                    if (disp >= 0 && disp < 0x400) return disp;
                }
            }
        } catch (Exception e) {}
        return -1;
    }

    String movLoadReg(Instruction ins, long dispWanted) {
        try {
            Object[] o0 = ins.getOpObjects(0);
            Object[] o1 = ins.getOpObjects(1);
            if (o0.length != 1 || !(o0[0] instanceof ghidra.program.model.lang.Register)) return null;
            boolean baseRcx = false, hasDisp = false;
            for (Object o : o1) {
                if (o instanceof ghidra.program.model.lang.Register
                    && ((ghidra.program.model.lang.Register)o).getName().equalsIgnoreCase("RCX")) baseRcx = true;
                if (o instanceof Scalar && ((Scalar)o).getUnsignedValue() == dispWanted) hasDisp = true;
            }
            // require the StatusInstance load to be off RCX (the 'this' pointer)
            if (baseRcx && hasDisp) return ((ghidra.program.model.lang.Register)o0[0]).getName();
        } catch (Exception e) {}
        return null;
    }

    long movLoadFromBase(Instruction ins, String baseRegName) {
        try {
            Object[] o0 = ins.getOpObjects(0);
            Object[] o1 = ins.getOpObjects(1);
            if (o0.length != 1 || !(o0[0] instanceof ghidra.program.model.lang.Register)) return -1;
            boolean baseMatch = false; long disp = 0; boolean hasScalar = false;
            for (Object o : o1) {
                if (o instanceof ghidra.program.model.lang.Register &&
                    ((ghidra.program.model.lang.Register)o).getName().equalsIgnoreCase(baseRegName)) baseMatch = true;
                if (o instanceof Scalar) { disp = ((Scalar)o).getUnsignedValue(); hasScalar = true; }
            }
            if (baseMatch) return hasScalar ? disp : 0;
        } catch (Exception e) {}
        return -1;
    }
}
