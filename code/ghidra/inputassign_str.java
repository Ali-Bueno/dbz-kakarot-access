// Anchor on the slot-name STRINGS. UE code that touches a named member at
// runtime builds a static FName from the literal, so any CODE xref to
// "Controller_Btn_*" / "InputAssign" / the option-config table names is a
// function that manipulates the layout. Property-registration tables are data
// xrefs and are reported separately (they name the struct's own metadata).
//@category Kakarot
import ghidra.app.script.GhidraScript;
import ghidra.app.decompiler.DecompInterface;
import ghidra.app.decompiler.DecompileResults;
import ghidra.program.model.address.Address;
import ghidra.program.model.address.AddressSpace;
import ghidra.program.model.listing.Function;
import ghidra.program.model.listing.FunctionManager;
import ghidra.program.model.listing.Instruction;
import ghidra.program.model.listing.InstructionIterator;
import ghidra.program.model.mem.Memory;
import ghidra.program.model.mem.MemoryBlock;
import ghidra.program.model.symbol.Reference;
import ghidra.program.model.symbol.ReferenceManager;
import ghidra.util.task.ConsoleTaskMonitor;
import java.io.File;
import java.io.FileWriter;
import java.io.PrintWriter;
import java.util.ArrayList;
import java.util.LinkedHashMap;
import java.util.LinkedHashSet;
import java.util.List;
import java.util.Map;
import java.util.Set;

public class inputassign_str extends GhidraScript {
    static final String OUT_DIR =
        "D:\\code\\unreal\\dragon ball kakarot access\\code\\decompiled";

    static final String[] ANCHORS = {
        "Controller_Btn_B",
        "Controller_Btn_Start",
        "Controller_Btn_R3",
        "InputAssign",
        "Table_OptionControllerConfigParam",
        "OptionControllerConfigParam",
        "DynamicAssignInputControllerId",
        "KeyConfigList",
        "Keyboard_Type",
    };

    Memory mem; FunctionManager fm; AddressSpace space;
    DecompInterface decomp; ConsoleTaskMonitor mon;
    PrintWriter pw; PrintWriter iw;
    Set<Long> emitted = new LinkedHashSet<>();

    long u64(byte[] b, int i) {
        long v = 0; for (int k = 0; k < 8; k++) v |= ((long)(b[i+k] & 0xff)) << (8*k); return v;
    }
    List<Address> findAll(byte[] needle, int cap) throws Exception {
        List<Address> out = new ArrayList<>();
        Address from = mem.getMinAddress();
        while (true) { Address hit = find(from, needle); if (hit == null) break;
            out.add(hit); from = hit.add(1); if (out.size() >= cap) break; }
        return out;
    }
    void emit(Function f, String tag) {
        if (!emitted.add(f.getEntryPoint().getOffset())) { pw.println("// (already emitted) " + tag + " -> " + f.getName()); return; }
        pw.println("// ================= " + tag + "  " + f.getName() + " @ " + f.getEntryPoint()
                   + "  size=0x" + Long.toHexString(f.getBody().getNumAddresses()) + " =================");
        StringBuilder cs = new StringBuilder();
        for (Function c : f.getCallingFunctions(mon)) {
            cs.append(c.getName()).append("@").append(c.getEntryPoint()).append(" ");
            if (cs.length() > 500) { cs.append("..."); break; }
        }
        pw.println("// callers: " + (cs.length() == 0 ? "<none>" : cs.toString()));
        try {
            DecompileResults res = decomp.decompileFunction(f, 180, mon);
            pw.println(res != null && res.decompileCompleted()
                ? res.getDecompiledFunction().getC() : "// <decompile failed>");
        } catch (Exception e) { pw.println("// <decompile error> " + e); }
        pw.println(); pw.flush();
    }

    // every code reference to `addr`, direct or through an 8-byte pointer slot
    void chase(Address addr, String tag, int depth) {
        ReferenceManager rm = currentProgram.getReferenceManager();
        for (Reference r : rm.getReferencesTo(addr)) {
            Address from = r.getFromAddress();
            Function f = fm.getFunctionContaining(from);
            if (f != null) {
                iw.printf("    CODE xref from %s in %s%n", from, f.getEntryPoint());
                emit(f, tag + " <- " + from);
            } else {
                iw.printf("    DATA xref from %s%n", from);
                if (depth > 0) chase(from, tag + "(ptr)", depth - 1);
            }
        }
    }

    @Override
    public void run() throws Exception {
        new File(OUT_DIR).mkdirs();
        mem = currentProgram.getMemory();
        fm = currentProgram.getFunctionManager();
        space = currentProgram.getAddressFactory().getDefaultAddressSpace();
        mon = new ConsoleTaskMonitor();
        decomp = new DecompInterface(); decomp.openProgram(currentProgram);
        pw = new PrintWriter(new FileWriter(new File(OUT_DIR, "_inputassign_str.c")));
        iw = new PrintWriter(new FileWriter(new File(OUT_DIR, "_inputassign_str_index.txt")));

        // pointer slots in .rdata/.data pointing at a given address (FName param
        // tables reference the literal through a pointer, so a plain xref misses it)
        List<byte[]> bl = new ArrayList<>(); List<Long> bs = new ArrayList<>();
        for (String bn : new String[]{".rdata", ".data"}) {
            MemoryBlock b = mem.getBlock(bn);
            if (b == null) continue;
            byte[] arr = new byte[(int) b.getSize()];
            b.getBytes(b.getStart(), arr);
            bl.add(arr); bs.add(b.getStart().getOffset());
        }

        for (String a : ANCHORS) {
            iw.println("==== ANCHOR " + a + " ====");
            List<Address> hits = new ArrayList<>();
            hits.addAll(findAll((a + "\0").getBytes("ISO-8859-1"), 40));
            byte[] wide = new byte[(a.length() + 1) * 2];
            for (int i = 0; i < a.length(); i++) wide[i*2] = (byte) a.charAt(i);
            hits.addAll(findAll(wide, 40));
            for (Address h : hits) {
                iw.println("  hit @ " + h + "  block=" +
                    (mem.getBlock(h) == null ? "?" : mem.getBlock(h).getName()));
                chase(h, a, 1);
                // pointer slots holding this address
                long target = h.getOffset();
                for (int bi = 0; bi < bl.size(); bi++) {
                    byte[] arr = bl.get(bi); long base = bs.get(bi);
                    for (int i = 0; i + 8 <= arr.length; i += 8) {
                        if (u64(arr, i) != target) continue;
                        Address slot = space.getAddress(base + i);
                        iw.println("    ptr slot @ " + slot);
                        chase(slot, a + "(slot)", 1);
                    }
                }
            }
        }
        iw.close(); pw.close();
        println("STR done, " + emitted.size() + " functions emitted");
    }
}
