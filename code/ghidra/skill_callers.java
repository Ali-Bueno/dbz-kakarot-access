// List (and decompile) the callers of a comma-separated list of addresses.
// Usage: -postScript "skill_callers.java 0x14144efc0,0x14145e140"
// -> decompiled/_skill_callers.txt  +  decompiled/skill_callers.c
//@category Kakarot
import ghidra.app.script.GhidraScript;
import ghidra.app.decompiler.DecompInterface;
import ghidra.app.decompiler.DecompileResults;
import ghidra.program.model.address.Address;
import ghidra.program.model.listing.Function;
import ghidra.program.model.listing.FunctionManager;
import ghidra.program.model.symbol.Reference;
import ghidra.program.model.symbol.ReferenceIterator;
import ghidra.util.task.ConsoleTaskMonitor;
import java.io.File;
import java.io.FileWriter;
import java.io.PrintWriter;
import java.util.LinkedHashSet;
import java.util.Set;

public class skill_callers extends GhidraScript {
    static final String OUT_DIR =
        "D:\\code\\unreal\\dragon ball kakarot access\\code\\decompiled";

    @Override
    public void run() throws Exception {
        String[] raw = getScriptArgs();
        if (raw.length == 0) { println("SKILLCALL: no addresses"); return; }
        String all = String.join(",", raw);
        FunctionManager fm = currentProgram.getFunctionManager();
        ConsoleTaskMonitor mon = new ConsoleTaskMonitor();
        DecompInterface decomp = new DecompInterface();
        decomp.openProgram(currentProgram);

        PrintWriter idx = new PrintWriter(new FileWriter(new File(OUT_DIR, "_skill_callers.txt")));
        PrintWriter pw = new PrintWriter(new FileWriter(new File(OUT_DIR, "skill_callers.c")));
        Set<Address> done = new LinkedHashSet<>();

        for (String tok : all.split(",")) {
            tok = tok.trim();
            if (tok.isEmpty()) continue;
            long off = Long.parseLong(tok.replaceFirst("^0x", ""), 16);
            Address a = currentProgram.getAddressFactory().getDefaultAddressSpace().getAddress(off);
            idx.println("=== callers of " + tok + " ===");
            ReferenceIterator it = currentProgram.getReferenceManager().getReferencesTo(a);
            while (it.hasNext()) {
                Reference r = it.next();
                Function c = fm.getFunctionContaining(r.getFromAddress());
                if (c == null) continue;
                idx.println("  " + c.getEntryPoint() + "   (from " + r.getFromAddress() + ")");
                if (!done.add(c.getEntryPoint())) continue;
                pw.println("// ======== caller " + c.getEntryPoint() + " of " + tok + " ========");
                String cstr;
                try {
                    DecompileResults res = decomp.decompileFunction(c, 180, mon);
                    cstr = (res != null && res.decompileCompleted())
                        ? res.getDecompiledFunction().getC() : "// <decompile failed>";
                } catch (Exception e) { cstr = "// <decompile error> " + e; }
                pw.println(cstr);
                pw.println();
            }
        }
        idx.close();
        pw.close();
        println("SKILLCALL: done, " + done.size() + " unique callers");
    }
}
