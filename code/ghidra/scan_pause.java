// Targeted hunt for the battle-pause selection offset. A function that navigates
// UAT_UIXCmnPause reads its ListBarArray (data @0x3a8) AND a tail selection index
// in 0x438..0x4fc. So: find functions that reference BOTH the scalar 0x3a8 and at
// least one scalar in [0x438,0x4fc], restricted to the game-UI code region, and
// report which tail scalar(s) each uses, then decompile them.
//@category Kakarot
import ghidra.app.script.GhidraScript;
import ghidra.app.decompiler.DecompInterface;
import ghidra.app.decompiler.DecompileResults;
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
import java.util.TreeSet;

public class scan_pause extends GhidraScript {
    static final String OUT_DIR =
        "D:\\code\\unreal\\dragon ball kakarot access\\code\\decompiled";
    static final long ARRAY_OFF = 0x3a8;      // ListBarArray data ptr
    static final long TAIL_LO = 0x438, TAIL_HI = 0x4fc;
    static final long CODE_LO = 0x141000000L, CODE_HI = 0x142000000L; // game UI .text band

    @Override
    public void run() throws Exception {
        new File(OUT_DIR).mkdirs();
        DecompInterface decomp = new DecompInterface();
        decomp.openProgram(currentProgram);
        ConsoleTaskMonitor mon = new ConsoleTaskMonitor();

        StringBuilder idx = new StringBuilder();
        List<Function> hits = new ArrayList<>();
        List<TreeSet<Long>> tails = new ArrayList<>();

        FunctionIterator fit = currentProgram.getFunctionManager().getFunctions(true);
        while (fit.hasNext()) {
            Function f = fit.next();
            long ep = f.getEntryPoint().getOffset();
            if (ep < CODE_LO || ep >= CODE_HI) continue;
            boolean hasArray = false;
            TreeSet<Long> tailHits = new TreeSet<>();
            InstructionIterator ii = currentProgram.getListing().getInstructions(f.getBody(), true);
            while (ii.hasNext()) {
                Instruction ins = ii.next();
                for (int op = 0; op < ins.getNumOperands(); op++) {
                    for (Object o : ins.getOpObjects(op)) {
                        if (o instanceof Scalar) {
                            long v = ((Scalar) o).getUnsignedValue();
                            if (v == ARRAY_OFF) hasArray = true;
                            else if (v >= TAIL_LO && v <= TAIL_HI) tailHits.add(v);
                        }
                    }
                }
            }
            if (hasArray && !tailHits.isEmpty()) { hits.add(f); tails.add(tailHits); }
        }

        idx.append("functions referencing 0x3a8 + a tail scalar [0x438,0x4fc]: ").append(hits.size()).append("\n");
        File out = new File(OUT_DIR, "scan_pause.c");
        PrintWriter pw = new PrintWriter(new FileWriter(out));
        for (int k = 0; k < hits.size(); k++) {
            Function f = hits.get(k);
            StringBuilder t = new StringBuilder();
            for (Long v : tails.get(k)) t.append(" 0x").append(Long.toHexString(v));
            idx.append("   ").append(f.getEntryPoint()).append("  tail:").append(t).append("\n");
            String c;
            try {
                DecompileResults r = decomp.decompileFunction(f, 120, mon);
                c = (r != null && r.decompileCompleted()) ? r.getDecompiledFunction().getC() : "// <failed>";
            } catch (Exception e) { c = "// <error> " + e; }
            pw.println("// ---- " + f.getName() + " @ " + f.getEntryPoint() + "   tail scalars:" + t + " ----");
            pw.println(c);
            pw.println();
        }
        pw.close();
        PrintWriter iw = new PrintWriter(new FileWriter(new File(OUT_DIR, "_scan_pause_index.txt")));
        iw.print(idx.toString());
        iw.close();
        println("SCANPAUSE: done, hits=" + hits.size());
    }
}
