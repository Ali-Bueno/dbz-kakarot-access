// Decompile all CALLERS of the community-board socket hit-test FUN_1414f2ab0
// (and of the pick/place FUN_1414f38f0). One of them is the per-tick cursor
// mover — it reads the stick, scales by EditCursorSpeed and writes the cursor
// widget's RenderTransform — and its guards tell us when the cursor is LOCKED
// (the user's stick did nothing during the last live pass).
//@category Kakarot
import ghidra.app.script.GhidraScript;
import ghidra.app.decompiler.DecompInterface;
import ghidra.app.decompiler.DecompileResults;
import ghidra.program.model.address.Address;
import ghidra.program.model.listing.Function;
import ghidra.program.model.symbol.Reference;
import ghidra.program.model.symbol.ReferenceIterator;
import ghidra.util.task.ConsoleTaskMonitor;
import java.io.File;
import java.io.FileWriter;
import java.io.PrintWriter;
import java.util.LinkedHashSet;
import java.util.Set;

public class dig_commu_callers extends GhidraScript {
    static final String OUT_DIR =
        "D:\\code\\unreal\\dragon ball kakarot access\\code\\decompiled";
    static final long[] TARGETS = { 0x1414e3170L };
    static final int MAX_CALLERS = 15;

    @Override
    public void run() throws Exception {
        new File(OUT_DIR).mkdirs();
        DecompInterface decomp = new DecompInterface();
        decomp.openProgram(currentProgram);
        ConsoleTaskMonitor mon = new ConsoleTaskMonitor();

        File out = new File(OUT_DIR, "commu_tick_callers.c");
        PrintWriter pw = new PrintWriter(new FileWriter(out));
        for (long t : TARGETS) {
            Address ta = currentProgram.getAddressFactory().getDefaultAddressSpace().getAddress(t);
            Set<Function> callers = new LinkedHashSet<>();
            ReferenceIterator ri = currentProgram.getReferenceManager().getReferencesTo(ta);
            while (ri.hasNext() && callers.size() < MAX_CALLERS) {
                Reference r = ri.next();
                Function f = getFunctionContaining(r.getFromAddress());
                if (f != null) callers.add(f);
            }
            pw.println("// ################ callers of 0x" + Long.toHexString(t)
                       + " (" + callers.size() + ") ################");
            for (Function f : callers) {
                pw.println("// ======== " + f.getName() + " @ " + f.getEntryPoint() + " ========");
                String c;
                try {
                    DecompileResults res = decomp.decompileFunction(f, 180, mon);
                    c = (res != null && res.decompileCompleted())
                        ? res.getDecompiledFunction().getC() : "// <decompile failed>";
                } catch (Exception e) { c = "// <decompile error> " + e; }
                pw.println(c);
                pw.println();
            }
        }
        pw.close();
        println("DIGCALLERS: done");
    }
}
