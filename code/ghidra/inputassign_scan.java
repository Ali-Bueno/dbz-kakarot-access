// Pending (unsaved) controller/keyboard assignment hunt (2026-07-25).
//
// Established facts this builds on:
//   FATSaveSystemInputAssign = 0x1E0 bytes, lives at UATSaveSystem+0x720.
//   Layout: 47 FName action entries 0x000..0x174, ECFKeyboardLayoutType at 0x178,
//           12 FName controller slots at 0x180..0x1D8.
//
// Strategy (anchor-free): the commit path must copy 0x1E0 bytes into <obj>+0x720,
// so scan every instruction in executable blocks for
//   (a) the scalar 0x1E0            (memcpy size / struct size)
//   (b) the scalar 0x3C / 0x78      (rep movsq / rep movsd counts for 0x1E0)
//   (c) the displacement 0x720      (the InputAssign member)
//   (d) the displacement 0x180/0x1D8 (the controller-slot window)
// and intersect per enclosing function. Writes an index; no decompile here
// (a second pass decompiles the survivors), so this stays cheap.
//@category Kakarot
import ghidra.app.script.GhidraScript;
import ghidra.program.model.address.Address;
import ghidra.program.model.lang.OperandType;
import ghidra.program.model.listing.Function;
import ghidra.program.model.listing.FunctionManager;
import ghidra.program.model.listing.Instruction;
import ghidra.program.model.listing.InstructionIterator;
import ghidra.program.model.listing.Listing;
import ghidra.program.model.mem.Memory;
import ghidra.program.model.mem.MemoryBlock;
import ghidra.program.model.scalar.Scalar;
import ghidra.program.model.symbol.Reference;
import java.io.File;
import java.io.FileWriter;
import java.io.PrintWriter;
import java.util.ArrayList;
import java.util.LinkedHashMap;
import java.util.List;
import java.util.Map;
import java.util.TreeMap;

public class inputassign_scan extends GhidraScript {
    static final String OUT_DIR =
        "D:\\code\\unreal\\dragon ball kakarot access\\code\\decompiled";

    static final long SZ   = 0x1E0;   // struct size
    static final long CNT8 = 0x3C;    // 0x1E0/8  rep movsq
    static final long CNT4 = 0x78;    // 0x1E0/4  rep movsd
    static final long OFF  = 0x720;   // UATSaveSystem::InputAssign
    static final long SLOT0 = 0x180;  // first controller slot inside the struct
    static final long SLOTL = 0x1D8;  // last controller slot

    static class Info {
        boolean sz, cnt, off, slot;
        List<String> notes = new ArrayList<>();
    }

    Map<Long, Info> byFunc = new TreeMap<>();
    List<String> szSites = new ArrayList<>();

    Info info(Function f) {
        long k = f.getEntryPoint().getOffset();
        Info i = byFunc.get(k);
        if (i == null) { i = new Info(); byFunc.put(k, i); }
        return i;
    }

    // Next CALL within `n` instructions, for "mov r8d,0x1e0 ; call memcpy".
    String nextCall(Instruction ins, int n) {
        Instruction cur = ins;
        for (int i = 0; i < n && cur != null; i++) {
            cur = cur.getNext();
            if (cur == null) break;
            if (cur.getMnemonicString().toLowerCase().startsWith("call")) {
                for (Reference r : cur.getReferencesFrom()) {
                    Function g = currentProgram.getFunctionManager().getFunctionAt(r.getToAddress());
                    if (g != null) return g.getName() + "@" + r.getToAddress();
                }
                return "indirect@" + cur.getAddress();
            }
        }
        return "-";
    }

    @Override
    public void run() throws Exception {
        new File(OUT_DIR).mkdirs();
        Memory mem = currentProgram.getMemory();
        Listing lst = currentProgram.getListing();
        FunctionManager fm = currentProgram.getFunctionManager();

        long nIns = 0;
        for (MemoryBlock b : mem.getBlocks()) {
            if (!b.isExecute()) continue;
            println("SCAN block " + b.getName() + " " + b.getStart() + ".." + b.getEnd());
            InstructionIterator it = lst.getInstructions(b.getStart(), true);
            while (it.hasNext()) {
                Instruction ins = it.next();
                if (ins.getAddress().compareTo(b.getEnd()) > 0) break;
                nIns++;
                if ((nIns % 2000000) == 0) println("  ..." + nIns + " ins @ " + ins.getAddress());

                boolean hSz = false, hCnt = false, hOff = false, hSlot = false;
                int nops = ins.getNumOperands();
                for (int oi = 0; oi < nops; oi++) {
                    for (Object o : ins.getOpObjects(oi)) {
                        if (!(o instanceof Scalar)) continue;
                        long v = ((Scalar) o).getUnsignedValue();
                        if (v == SZ) {
                            // an immediate 0x1e0 is a size; a displacement 0x1e0 is not
                            if (OperandType.isScalar(ins.getOperandType(oi))) hSz = true;
                        } else if (v == CNT8 || v == CNT4) {
                            if (OperandType.isScalar(ins.getOperandType(oi))) hCnt = true;
                        } else if (v == OFF) {
                            hOff = true;
                        } else if (v == SLOT0 || v == SLOTL) {
                            hSlot = true;
                        }
                    }
                }
                if (!(hSz || hCnt || hOff || hSlot)) continue;

                Function f = fm.getFunctionContaining(ins.getAddress());
                if (f == null) continue;
                Info i = info(f);
                if (hSz)   { i.sz = true;
                             szSites.add(String.format("0x%s  %-40s in %s  -> %s",
                                 ins.getAddress(), ins.toString(),
                                 f.getEntryPoint(), nextCall(ins, 8))); }
                if (hCnt)  i.cnt = true;
                if (hOff)  { i.off = true;
                             if (i.notes.size() < 12)
                                 i.notes.add("0x720 @ " + ins.getAddress() + "  " + ins); }
                if (hSlot) { i.slot = true;
                             if (i.notes.size() < 12)
                                 i.notes.add("slot  @ " + ins.getAddress() + "  " + ins); }
            }
        }
        println("SCAN done, " + nIns + " instructions, " + byFunc.size() + " hit functions");

        PrintWriter pw = new PrintWriter(new FileWriter(new File(OUT_DIR, "_inputassign_scan.txt")));
        pw.println("# instructions scanned: " + nIns);
        pw.println();
        pw.println("=== A. functions with BOTH a 0x1E0/0x3C/0x78 copy size AND a 0x720 offset ===");
        for (Map.Entry<Long, Info> e : byFunc.entrySet()) {
            Info i = e.getValue();
            if (!((i.sz || i.cnt) && i.off)) continue;
            Function f = fm.getFunctionAt(toAddr(e.getKey()));
            pw.printf("FUNC 0x%x  %s  size=%b cnt=%b off720=%b slot=%b%n",
                e.getKey(), f == null ? "?" : f.getName(), i.sz, i.cnt, i.off, i.slot);
            for (String n : i.notes) pw.println("    " + n);
        }
        pw.println();
        pw.println("=== B. functions with a 0x720 offset AND the 0x180/0x1D8 slot window ===");
        for (Map.Entry<Long, Info> e : byFunc.entrySet()) {
            Info i = e.getValue();
            if (!(i.off && i.slot)) continue;
            Function f = fm.getFunctionAt(toAddr(e.getKey()));
            pw.printf("FUNC 0x%x  %s%n", e.getKey(), f == null ? "?" : f.getName());
            for (String n : i.notes) pw.println("    " + n);
        }
        pw.println();
        pw.println("=== C. every immediate 0x1E0 site (size arg) + following call ===");
        pw.println("# total: " + szSites.size());
        for (String s : szSites) pw.println(s);
        pw.println();
        pw.println("=== D. all functions touching offset 0x720 (count only) ===");
        int n720 = 0;
        for (Info i : byFunc.values()) if (i.off) n720++;
        pw.println("# " + n720 + " functions reference displacement 0x720");
        pw.close();
        println("WROTE _inputassign_scan.txt");
    }
}
