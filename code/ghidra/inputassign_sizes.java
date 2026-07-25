// Size-immediate hunt. FATSaveSystemOption is 0x66C bytes: any working copy of
// the option block MUST go through a real memcpy (too big to inline), and the
// same routine almost certainly copies InputAssign (0x1E0) next to it. Also
// looks for the whole-save sizes so UATSaveSystem code can be identified.
//@category Kakarot
import ghidra.app.script.GhidraScript;
import ghidra.app.decompiler.DecompInterface;
import ghidra.app.decompiler.DecompileResults;
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
import ghidra.util.task.ConsoleTaskMonitor;
import java.io.File;
import java.io.FileWriter;
import java.io.PrintWriter;
import java.util.LinkedHashMap;
import java.util.LinkedHashSet;
import java.util.Map;
import java.util.Set;

public class inputassign_sizes extends GhidraScript {
    static final String OUT_DIR =
        "D:\\code\\unreal\\dragon ball kakarot access\\code\\decompiled";

    // FATSaveSystemOption 0x66C, +InputAssign 0x84C, UATSaveSystem 0x9AF98,
    // Headers 0x9A630, DLC offset 0x9AF30, InputAssign offset 0x720/size 0x1E0.
    static final long[] SIZES = { 0x66CL, 0x670L, 0x84CL, 0x850L, 0x9A630L, 0x9AF30L, 0x9AF98L, 0x9AF48L };

    Map<Long, Set<String>> hits = new LinkedHashMap<>();
    PrintWriter pw;
    DecompInterface decomp; ConsoleTaskMonitor mon; FunctionManager fm;

    @Override
    public void run() throws Exception {
        new File(OUT_DIR).mkdirs();
        Memory mem = currentProgram.getMemory();
        Listing lst = currentProgram.getListing();
        fm = currentProgram.getFunctionManager();
        mon = new ConsoleTaskMonitor();
        decomp = new DecompInterface(); decomp.openProgram(currentProgram);
        pw = new PrintWriter(new FileWriter(new File(OUT_DIR, "_inputassign_sizes.c")));

        Set<Long> wanted = new LinkedHashSet<>();
        for (long s : SIZES) wanted.add(s);

        long n = 0;
        for (MemoryBlock b : mem.getBlocks()) {
            if (!b.isExecute()) continue;
            InstructionIterator it = lst.getInstructions(b.getStart(), true);
            while (it.hasNext()) {
                Instruction ins = it.next();
                if (ins.getAddress().compareTo(b.getEnd()) > 0) break;
                if ((++n % 4000000) == 0) println("  ..." + n);
                for (int oi = 0; oi < ins.getNumOperands(); oi++) {
                    for (Object o : ins.getOpObjects(oi)) {
                        if (!(o instanceof Scalar)) continue;
                        long v = ((Scalar) o).getUnsignedValue();
                        if (!wanted.contains(v)) continue;
                        // skip pure stack-frame adjustments
                        String m = ins.getMnemonicString().toUpperCase();
                        String txt = ins.toString();
                        if ((m.equals("SUB") || m.equals("ADD")) && txt.contains("RSP")) continue;
                        Function f = fm.getFunctionContaining(ins.getAddress());
                        long key = f == null ? 0 : f.getEntryPoint().getOffset();
                        hits.computeIfAbsent(key, k -> new LinkedHashSet<>())
                            .add(String.format("0x%x  %-44s [0x%x] -> %s",
                                ins.getAddress().getOffset(), txt, v, nextCall(ins, 10)));
                    }
                }
            }
        }
        println("SIZES done, " + n + " ins, " + hits.size() + " funcs");

        PrintWriter iw = new PrintWriter(new FileWriter(new File(OUT_DIR, "_inputassign_sizes.txt")));
        for (Map.Entry<Long, Set<String>> e : hits.entrySet()) {
            Function f = e.getKey() == 0 ? null : fm.getFunctionAt(toAddr(e.getKey()));
            iw.printf("FUNC 0x%x  %s%n", e.getKey(), f == null ? "<none>" : f.getName());
            for (String s : e.getValue()) iw.println("    " + s);
        }
        iw.close();

        // decompile everything that is not a pure stack-size hit
        int cap = 0;
        for (Map.Entry<Long, Set<String>> e : hits.entrySet()) {
            if (e.getKey() == 0 || cap++ > 40) continue;
            Function f = fm.getFunctionAt(toAddr(e.getKey()));
            if (f == null) continue;
            pw.println("// ============ " + f.getName() + " @ " + f.getEntryPoint()
                       + "  size=0x" + Long.toHexString(f.getBody().getNumAddresses()) + " ============");
            for (String s : e.getValue()) pw.println("//   " + s);
            StringBuilder cs = new StringBuilder();
            for (Function c : f.getCallingFunctions(mon)) {
                cs.append(c.getName()).append("@").append(c.getEntryPoint()).append(" ");
                if (cs.length() > 400) break;
            }
            pw.println("// callers: " + cs);
            try {
                DecompileResults res = decomp.decompileFunction(f, 180, mon);
                pw.println(res != null && res.decompileCompleted()
                    ? res.getDecompiledFunction().getC() : "// <decompile failed>");
            } catch (Exception ex) { pw.println("// <err> " + ex); }
            pw.flush();
        }
        pw.close();
        println("WROTE _inputassign_sizes.{txt,c}");
    }

    String nextCall(Instruction ins, int n) {
        Instruction cur = ins;
        for (int i = 0; i < n && cur != null; i++) {
            cur = cur.getNext();
            if (cur == null) break;
            if (cur.getMnemonicString().toLowerCase().startsWith("call")) {
                for (Reference r : cur.getReferencesFrom()) {
                    Function g = fm.getFunctionAt(r.getToAddress());
                    if (g != null) return g.getName() + "@" + r.getToAddress();
                }
                return "indirect";
            }
        }
        return "-";
    }
}
