// Refined pass: OBJECT-field accesses (base register != RSP/RBP) inside the
// UATSaveSystem::InputAssign window [0x720, 0x900) — the first scan drowned in
// RBP-relative stack frames. Also fingerprints any function that touches >= 6
// distinct 8-byte-aligned displacements 0x180..0x1D8 apart from a common base
// (the 12 controller slots inside ANY copy of FATSaveSystemInputAssign,
// whatever its host offset).
//@category Kakarot
import ghidra.app.script.GhidraScript;
import ghidra.program.model.address.Address;
import ghidra.program.model.lang.Register;
import ghidra.program.model.listing.Function;
import ghidra.program.model.listing.FunctionManager;
import ghidra.program.model.listing.Instruction;
import ghidra.program.model.listing.InstructionIterator;
import ghidra.program.model.listing.Listing;
import ghidra.program.model.mem.Memory;
import ghidra.program.model.mem.MemoryBlock;
import ghidra.program.model.scalar.Scalar;
import java.io.File;
import java.io.FileWriter;
import java.io.PrintWriter;
import java.util.ArrayList;
import java.util.LinkedHashMap;
import java.util.List;
import java.util.Map;
import java.util.TreeMap;
import java.util.TreeSet;

public class inputassign_scan2 extends GhidraScript {
    static final String OUT_DIR =
        "D:\\code\\unreal\\dragon ball kakarot access\\code\\decompiled";

    static final long WIN_LO = 0x720, WIN_HI = 0x900;   // InputAssign inside UATSaveSystem

    static class F {
        TreeSet<Long> win = new TreeSet<>();      // displacements in the save-system window
        List<String> lines = new ArrayList<>();
        Map<String, TreeSet<Long>> byBase = new LinkedHashMap<>(); // base reg -> all displacements
    }

    @Override
    public void run() throws Exception {
        new File(OUT_DIR).mkdirs();
        Memory mem = currentProgram.getMemory();
        Listing lst = currentProgram.getListing();
        FunctionManager fm = currentProgram.getFunctionManager();
        Map<Long, F> map = new TreeMap<>();
        long n = 0;

        for (MemoryBlock b : mem.getBlocks()) {
            if (!b.isExecute()) continue;
            InstructionIterator it = lst.getInstructions(b.getStart(), true);
            while (it.hasNext()) {
                Instruction ins = it.next();
                if (ins.getAddress().compareTo(b.getEnd()) > 0) break;
                if ((++n % 4000000) == 0) println("  ..." + n);
                int nops = ins.getNumOperands();
                for (int oi = 0; oi < nops; oi++) {
                    Object[] objs = ins.getOpObjects(oi);
                    if (objs.length < 2) continue;          // need base + displacement
                    String base = null; long disp = -1; boolean stack = false;
                    for (Object o : objs) {
                        if (o instanceof Register) {
                            String rn = ((Register) o).getName();
                            if (rn.equals("RSP") || rn.equals("RBP") || rn.equals("ESP") || rn.equals("EBP"))
                                stack = true;
                            else if (base == null) base = rn;
                        } else if (o instanceof Scalar) {
                            long v = ((Scalar) o).getUnsignedValue();
                            if (v > disp) disp = v;
                        }
                    }
                    if (stack || base == null || disp < 0) continue;
                    Function f = fm.getFunctionContaining(ins.getAddress());
                    if (f == null) continue;
                    long key = f.getEntryPoint().getOffset();
                    F rec = map.get(key);
                    if (rec == null) { rec = new F(); map.put(key, rec); }
                    rec.byBase.computeIfAbsent(base, k -> new TreeSet<>()).add(disp);
                    if (disp >= WIN_LO && disp < WIN_HI && (disp % 8) == 0) {
                        rec.win.add(disp);
                        if (rec.lines.size() < 24)
                            rec.lines.add(ins.getAddress() + "  " + ins);
                    }
                }
            }
        }
        println("SCAN2 done, " + n + " ins, " + map.size() + " funcs");

        PrintWriter pw = new PrintWriter(new FileWriter(new File(OUT_DIR, "_inputassign_scan2.txt")));
        pw.println("=== A. object-field access to EXACTLY +0x720 (base != stack) ===");
        for (Map.Entry<Long, F> e : map.entrySet()) {
            if (!e.getValue().win.contains(0x720L)) continue;
            Function f = fm.getFunctionAt(toAddr(e.getKey()));
            pw.printf("FUNC 0x%x  %s   window disps: %s%n", e.getKey(),
                f == null ? "?" : f.getName(), e.getValue().win);
            for (String l : e.getValue().lines) pw.println("    " + l);
        }
        pw.println();
        pw.println("=== B. functions touching >=3 distinct displacements inside [0x720,0x900) ===");
        for (Map.Entry<Long, F> e : map.entrySet()) {
            if (e.getValue().win.size() < 3) continue;
            Function f = fm.getFunctionAt(toAddr(e.getKey()));
            pw.printf("FUNC 0x%x  %s   %s%n", e.getKey(), f == null ? "?" : f.getName(), e.getValue().win);
            for (String l : e.getValue().lines) pw.println("    " + l);
        }
        pw.println();
        pw.println("=== C. 12-controller-slot fingerprint: same base, >=6 disps 8-apart spanning 0x58 ===");
        for (Map.Entry<Long, F> e : map.entrySet()) {
            for (Map.Entry<String, TreeSet<Long>> be : e.getValue().byBase.entrySet()) {
                List<Long> ds = new ArrayList<>(be.getValue());
                for (int i = 0; i < ds.size(); i++) {
                    int run = 1;
                    long start = ds.get(i);
                    int j = i;
                    while (j + 1 < ds.size() && ds.get(j + 1) - ds.get(j) == 8) { j++; run++; }
                    if (run >= 6 && ds.get(j) - start >= 0x28) {
                        Function f = fm.getFunctionAt(toAddr(e.getKey()));
                        pw.printf("FUNC 0x%x  %s  base=%s run=%d  0x%x..0x%x%n",
                            e.getKey(), f == null ? "?" : f.getName(), be.getKey(), run, start, ds.get(j));
                    }
                    i = j;
                }
            }
        }
        pw.close();
        println("WROTE _inputassign_scan2.txt");
    }
}
