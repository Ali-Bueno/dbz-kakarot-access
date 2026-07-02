// Decompile an explicit comma-separated list of addresses (hex, e.g.
// "0x1416f7bd0,0x1415bcbc0"). Writes decompiled/manual_<addr>.c each.
// Usage: -postScript "decompile_addrs.java 0x1416f7bd0,0x1415bcbc0"
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

public class decompile_addrs extends GhidraScript {
    static final String OUT_DIR =
        "D:\\code\\unreal\\dragon ball kakarot access\\code\\decompiled";

    @Override
    public void run() throws Exception {
        new File(OUT_DIR).mkdirs();
        String[] args = getScriptArgs();
        if (args.length == 0) { println("DECADDR: no addresses given"); return; }
        FunctionManager fm = currentProgram.getFunctionManager();
        ConsoleTaskMonitor mon = new ConsoleTaskMonitor();
        DecompInterface decomp = new DecompInterface();
        decomp.openProgram(currentProgram);

        for (String tok : args[0].split(",")) {
            tok = tok.trim();
            if (tok.isEmpty()) continue;
            long off = Long.parseLong(tok.replaceFirst("^0x", ""), 16);
            Address a = currentProgram.getAddressFactory().getDefaultAddressSpace().getAddress(off);
            Function f = fm.getFunctionAt(a);
            if (f == null) {
                try { f = createFunction(a, null); } catch (Exception e) {}
            }
            if (f == null) { println("DECADDR: no function at " + tok); continue; }
            String c;
            try {
                DecompileResults res = decomp.decompileFunction(f, 180, mon);
                c = (res != null && res.decompileCompleted())
                    ? res.getDecompiledFunction().getC() : "// <decompile failed>";
            } catch (Exception e) { c = "// <decompile error> " + e; }
            File out = new File(OUT_DIR, "manual_" + tok.replaceFirst("^0x", "") + ".c");
            PrintWriter pw = new PrintWriter(new FileWriter(out));
            pw.println("// " + f.getName() + " @ " + f.getEntryPoint());
            pw.println(c);
            pw.close();
            println("DECADDR: wrote " + out.getName());
        }
        println("DECADDR: done");
    }
}
