// Community-board cursor: find the native cursor-move / hit-test code WITHOUT
// going through UFunctions (pad navigation is C++-direct on this game).
// The board host UAT_UICommunityBoard keeps its tuning as reflected members:
//   0x3F0 EditCursorSpeed, 0x44C CursorAdjust, 0x458 HitRange, 0x460 HitBaseOffset,
//   0x468 LeaderHitBaseOffset, 0x45C LeaderHitRange.
// Any function referencing TWO OR MORE of these scalars on the same object is the
// cursor integrator or the socket hit-test. Decompile them all.
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
import java.util.HashSet;
import java.util.List;
import java.util.Set;

public class dig_commu_cursor extends GhidraScript {
    static final String OUT_DIR =
        "D:\\code\\unreal\\dragon ball kakarot access\\code\\decompiled";
    static final long[] OFFS = { 0x3F0, 0x44C, 0x458, 0x460, 0x468, 0x45C };
    static final long CODE_LO = 0x140000000L, CODE_HI = 0x143000000L;
    static final int MIN_DISTINCT = 2;
    static final int MAX_HITS = 25;

    @Override
    public void run() throws Exception {
        new File(OUT_DIR).mkdirs();
        DecompInterface decomp = new DecompInterface();
        decomp.openProgram(currentProgram);
        ConsoleTaskMonitor mon = new ConsoleTaskMonitor();

        List<Function> hits = new ArrayList<>();
        FunctionIterator fit = currentProgram.getFunctionManager().getFunctions(true);
        while (fit.hasNext() && hits.size() < MAX_HITS) {
            Function f = fit.next();
            long ep = f.getEntryPoint().getOffset();
            if (ep < CODE_LO || ep >= CODE_HI) continue;
            Set<Long> seen = new HashSet<>();
            InstructionIterator ii = currentProgram.getListing().getInstructions(f.getBody(), true);
            while (ii.hasNext() && seen.size() < OFFS.length) {
                Instruction ins = ii.next();
                for (int op = 0; op < ins.getNumOperands(); op++) {
                    for (Object o : ins.getOpObjects(op)) {
                        if (!(o instanceof Scalar)) continue;
                        long v = ((Scalar) o).getUnsignedValue();
                        for (long w : OFFS) if (v == w) seen.add(w);
                    }
                }
            }
            if (seen.size() >= MIN_DISTINCT) hits.add(f);
        }

        StringBuilder idx = new StringBuilder();
        idx.append("functions referencing >=").append(MIN_DISTINCT)
           .append(" of the board cursor offsets: ").append(hits.size()).append("\n");
        File out = new File(OUT_DIR, "commu_cursor_code.c");
        PrintWriter pw = new PrintWriter(new FileWriter(out));
        pw.println("// Functions referencing >=2 of {0x3F0,0x44C,0x458,0x460,0x468,0x45C}");
        for (Function f : hits) {
            idx.append("  ").append(f.getName()).append(" @ ")
               .append(f.getEntryPoint()).append("\n");
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
        pw.close();
        PrintWriter iw = new PrintWriter(new FileWriter(new File(OUT_DIR, "_commu_cursor_index.txt")));
        iw.print(idx.toString());
        iw.close();
        println("DIGCURSOR: done, " + hits.size() + " functions");
    }
}
