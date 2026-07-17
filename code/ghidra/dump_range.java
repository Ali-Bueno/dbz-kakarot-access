// Decompile every function whose entry is in [START, END). Args: "START,END" hex.
// Output -> decompiled/_range_<START>.txt
//@category Kakarot
import ghidra.app.script.GhidraScript;
import ghidra.app.decompiler.DecompInterface;
import ghidra.app.decompiler.DecompileResults;
import ghidra.program.model.address.Address;
import ghidra.program.model.listing.Function;
import ghidra.program.model.listing.FunctionManager;
import ghidra.util.task.ConsoleTaskMonitor;
import java.io.File;
import java.io.FileWriter;
import java.io.PrintWriter;

public class dump_range extends GhidraScript {
    static final String OUT_DIR =
        "D:\\code\\unreal\\dragon ball kakarot access\\code\\decompiled";
    @Override
    public void run() throws Exception {
        String[] a = String.join(",", getScriptArgs()).split(",");
        long start = Long.parseLong(a[0].replaceFirst("^0x",""),16);
        long end = Long.parseLong(a[1].replaceFirst("^0x",""),16);
        FunctionManager fm = currentProgram.getFunctionManager();
        DecompInterface d = new DecompInterface(); d.openProgram(currentProgram);
        ConsoleTaskMonitor mon = new ConsoleTaskMonitor();
        PrintWriter pw = new PrintWriter(new FileWriter(new File(OUT_DIR,"_range_"+a[0].replaceFirst("^0x","")+".txt")));
        for (Function f : fm.getFunctions(true)) {
            long ep = f.getEntryPoint().getOffset();
            if (ep < start || ep >= end) continue;
            String c;
            try { DecompileResults r = d.decompileFunction(f,120,mon);
                c = (r!=null&&r.decompileCompleted())?r.getDecompiledFunction().getC():"// <fail>"; }
            catch (Exception e){ c="// <err>"; }
            pw.println("// ===== "+f.getName()+" @ "+f.getEntryPoint()+" size=0x"+Long.toHexString(f.getBody().getNumAddresses())+" =====");
            pw.println(c); pw.println();
        }
        pw.close(); println("RANGE: done");
    }
}
