// Find the pause selection offset the same way StartTop's was found: locate the
// function(s) that index UAT_UIXCmnPause.ListBarArray (data @0x3A8, count @0x3B0)
// by a selection index. Filter = functions whose instructions reference BOTH the
// 0x3a8 and 0x3b0 scalars (the TArray data+count pair), then decompile them so we
// can read `idx = *(int*)(this+off); item = array[idx]`.
//@category Kakarot
import ghidra.app.script.GhidraScript;
import ghidra.app.decompiler.DecompInterface;
import ghidra.app.decompiler.DecompileResults;
import ghidra.program.model.address.Address;
import ghidra.program.model.listing.Function;
import ghidra.program.model.listing.FunctionIterator;
import ghidra.program.model.listing.Instruction;
import ghidra.program.model.listing.InstructionIterator;
import ghidra.program.model.scalar.Scalar;
import ghidra.util.task.ConsoleTaskMonitor;
import java.io.File;
import java.io.FileWriter;
import java.io.PrintWriter;
import java.util.ArrayList;
import java.util.List;

public class scan_disp extends GhidraScript {
    static final String OUT_DIR =
        "D:\\code\\unreal\\dragon ball kakarot access\\code\\decompiled";
    // (data offset, count offset) pairs of the TArray to hunt
    static final long[][] PAIRS = {
        {0x3a8, 0x3b0},   // UAT_UIXCmnPause.ListBarArray
        {0x3b8, 0x3c0},   // UAT_UIGameover.SelectionWidgetArray (cross-check; has reflected idx @0x3E0)
    };

    boolean uses(Function f, long scalar) {
        InstructionIterator ii = currentProgram.getListing().getInstructions(f.getBody(), true);
        while (ii.hasNext()) {
            Instruction ins = ii.next();
            int n = ins.getNumOperands();
            for (int op = 0; op < n; op++) {
                Object[] objs = ins.getOpObjects(op);
                for (Object o : objs) {
                    if (o instanceof Scalar && ((Scalar) o).getUnsignedValue() == scalar) return true;
                }
            }
        }
        return false;
    }

    @Override
    public void run() throws Exception {
        new File(OUT_DIR).mkdirs();
        DecompInterface decomp = new DecompInterface();
        decomp.openProgram(currentProgram);
        ConsoleTaskMonitor mon = new ConsoleTaskMonitor();
        StringBuilder idx = new StringBuilder();

        for (long[] pair : PAIRS) {
            long dataOff = pair[0], cntOff = pair[1];
            idx.append("=== TArray data 0x").append(Long.toHexString(dataOff))
               .append(" count 0x").append(Long.toHexString(cntOff)).append(" ===\n");
            List<Function> hits = new ArrayList<>();
            FunctionIterator fit = currentProgram.getFunctionManager().getFunctions(true);
            int scanned = 0;
            while (fit.hasNext()) {
                Function f = fit.next();
                scanned++;
                if (uses(f, dataOff) && uses(f, cntOff)) hits.add(f);
                if (hits.size() >= 25) break;
            }
            idx.append("  functions using both: ").append(hits.size()).append("\n");
            File out = new File(OUT_DIR, "scan_" + Long.toHexString(dataOff) + ".c");
            PrintWriter pw = new PrintWriter(new FileWriter(out));
            pw.println("// Functions referencing both 0x" + Long.toHexString(dataOff)
                     + " (TArray data) and 0x" + Long.toHexString(cntOff) + " (count)");
            for (Function f : hits) {
                String c;
                try {
                    DecompileResults r = decomp.decompileFunction(f, 120, mon);
                    c = (r != null && r.decompileCompleted()) ? r.getDecompiledFunction().getC() : "// <failed>";
                } catch (Exception e) { c = "// <error> " + e; }
                pw.println("// ---- " + f.getName() + " @ " + f.getEntryPoint() + " ----");
                pw.println(c);
                pw.println();
                idx.append("   ").append(f.getEntryPoint()).append("\n");
            }
            pw.close();
        }
        PrintWriter iw = new PrintWriter(new FileWriter(new File(OUT_DIR, "_scan_index.txt")));
        iw.print(idx.toString());
        iw.close();
        println("SCANDISP: done");
    }
}
