// Find the overworld ring item -> section mapping. The ring items are image-only
// (no text) and the header is a fixed "Main Menu" title, so the per-item name must
// come from the game's data. UAT_UIStartTop's selection index lives at 0x4e4; the
// method that maps index -> section (EXCmnHeaderFontType / an asset) references it.
// So: decompile every UI-band function that references the scalar 0x4e4.
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

public class dig_starttop extends GhidraScript {
    static final String OUT_DIR =
        "D:\\code\\unreal\\dragon ball kakarot access\\code\\decompiled";
    static final long SEL_OFF = 0x4e4;
    static final long CODE_LO = 0x141000000L, CODE_HI = 0x142000000L;

    @Override
    public void run() throws Exception {
        new File(OUT_DIR).mkdirs();
        DecompInterface decomp = new DecompInterface();
        decomp.openProgram(currentProgram);
        ConsoleTaskMonitor mon = new ConsoleTaskMonitor();

        List<Function> hits = new ArrayList<>();
        FunctionIterator fit = currentProgram.getFunctionManager().getFunctions(true);
        while (fit.hasNext()) {
            Function f = fit.next();
            long ep = f.getEntryPoint().getOffset();
            if (ep < CODE_LO || ep >= CODE_HI) continue;
            boolean hit = false;
            InstructionIterator ii = currentProgram.getListing().getInstructions(f.getBody(), true);
            while (ii.hasNext() && !hit) {
                Instruction ins = ii.next();
                for (int op = 0; op < ins.getNumOperands(); op++) {
                    for (Object o : ins.getOpObjects(op)) {
                        if (o instanceof Scalar && ((Scalar) o).getUnsignedValue() == SEL_OFF) { hit = true; break; }
                    }
                    if (hit) break;
                }
            }
            if (hit) hits.add(f);
            if (hits.size() >= 40) break;
        }

        StringBuilder idx = new StringBuilder();
        idx.append("UI-band functions referencing 0x4e4: ").append(hits.size()).append("\n");
        File out = new File(OUT_DIR, "dig_starttop.c");
        PrintWriter pw = new PrintWriter(new FileWriter(out));
        for (Function f : hits) {
            idx.append("   ").append(f.getEntryPoint()).append("\n");
            String c;
            try {
                DecompileResults r = decomp.decompileFunction(f, 150, mon);
                c = (r != null && r.decompileCompleted()) ? r.getDecompiledFunction().getC() : "// <failed>";
            } catch (Exception e) { c = "// <error> " + e; }
            pw.println("// ---- " + f.getName() + " @ " + f.getEntryPoint() + " ----");
            pw.println(c);
            pw.println();
        }
        pw.close();
        PrintWriter iw = new PrintWriter(new FileWriter(new File(OUT_DIR, "_dig_starttop_index.txt")));
        iw.print(idx.toString());
        iw.close();
        println("DIGSTARTTOP: done hits=" + hits.size());
    }
}
