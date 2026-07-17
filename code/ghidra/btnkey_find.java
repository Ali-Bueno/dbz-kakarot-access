// Btn_Key direction-texture mapping hunt (accessibility: N -> d-pad direction).
// The icon-data asset stores INDEXED glyph names "Btn_Key_1..6", but the Xbox
// atlas only has named direction textures Btn_Key_Up/Dwn/L/R/Ud/Lr. So native
// code (or a table) must translate the numbered IconName -> a direction texture
// at rich-text render time. This script:
//   1. finds every occurrence of the direction/base strings,
//   2. finds all code references to each string,
//   3. reports + decompiles the unique containing functions.
// Writes _btnkey_index.txt (xref map) and btnkey_natives.c (decompiled funcs).
//@category Kakarot
import ghidra.app.script.GhidraScript;
import ghidra.app.decompiler.DecompInterface;
import ghidra.app.decompiler.DecompileResults;
import ghidra.program.model.address.Address;
import ghidra.program.model.address.AddressSpace;
import ghidra.program.model.listing.Function;
import ghidra.program.model.listing.FunctionManager;
import ghidra.program.model.mem.Memory;
import ghidra.program.model.symbol.Reference;
import ghidra.program.model.symbol.ReferenceManager;
import ghidra.util.task.ConsoleTaskMonitor;
import java.io.File;
import java.io.FileWriter;
import java.io.PrintWriter;
import java.util.ArrayList;
import java.util.LinkedHashMap;
import java.util.List;
import java.util.Map;

public class btnkey_find extends GhidraScript {
    static final String OUT_DIR =
        "D:\\code\\unreal\\dragon ball kakarot access\\code\\decompiled";

    // Exact-match strings (null-terminated) that would be built or switched on.
    static final String[] NEEDLES = {
        "Btn_Key_Up", "Btn_Key_Dwn", "Btn_Key_Ud", "Btn_Key_Lr",
        "Btn_Key_L", "Btn_Key_R", "Btn_Key",
        "Btn_Key_%d", "Btn_Key_",
        "PLAT_X",
    };

    Memory mem;
    FunctionManager fm;
    AddressSpace space;
    DecompInterface decomp;
    ConsoleTaskMonitor mon;

    List<Address> strHits(String s) throws Exception {
        byte[] needle = (s + "\0").getBytes("ISO-8859-1");
        List<Address> out = new ArrayList<>();
        Address from = mem.getMinAddress();
        while (true) {
            Address hit = find(from, needle);
            if (hit == null) break;
            out.add(hit);
            from = hit.add(1);
            if (out.size() > 400) break;
        }
        return out;
    }

    @Override
    public void run() throws Exception {
        new File(OUT_DIR).mkdirs();
        mem = currentProgram.getMemory();
        fm = currentProgram.getFunctionManager();
        space = currentProgram.getAddressFactory().getDefaultAddressSpace();
        mon = new ConsoleTaskMonitor();
        decomp = new DecompInterface();
        decomp.openProgram(currentProgram);
        ReferenceManager rm = currentProgram.getReferenceManager();

        StringBuilder idx = new StringBuilder();
        Map<Long, String> funcs = new LinkedHashMap<>();

        for (String needle : NEEDLES) {
            List<Address> hits = strHits(needle);
            idx.append("=== '").append(needle).append("' : ")
               .append(hits.size()).append(" occurrence(s) ===\n");
            for (Address sa : hits) {
                idx.append(String.format("  str @ %s%n", sa));
                // references to this string address
                java.util.Iterator<Reference> it = rm.getReferencesTo(sa).iterator();
                boolean any = false;
                while (it.hasNext()) {
                    Reference r = it.next();
                    any = true;
                    Address from = r.getFromAddress();
                    Function f = fm.getFunctionContaining(from);
                    String fn = (f == null) ? "?" : (f.getName() + " @ " + f.getEntryPoint());
                    idx.append(String.format("    xref from %s  in %s%n", from, fn));
                    if (f != null) funcs.putIfAbsent(f.getEntryPoint().getOffset(),
                        needle + " <- " + from);
                }
                if (!any) idx.append("    (no direct refs)\n");
            }
        }

        File out = new File(OUT_DIR, "btnkey_natives.c");
        PrintWriter pw = new PrintWriter(new FileWriter(out));
        pw.println("// Btn_Key direction-texture mapping: functions referencing the strings.");
        for (Map.Entry<Long, String> e : funcs.entrySet()) {
            Address a = space.getAddress(e.getKey());
            Function f = fm.getFunctionAt(a);
            if (f == null) continue;
            pw.println("// ======== " + f.getName() + " @ " + f.getEntryPoint()
                + "   (via " + e.getValue() + ") ========");
            String c;
            try {
                DecompileResults res = decomp.decompileFunction(f, 120, mon);
                c = (res != null && res.decompileCompleted())
                    ? res.getDecompiledFunction().getC() : "// <decompile failed>";
            } catch (Exception ex) { c = "// <decompile error> " + ex; }
            pw.println(c);
            pw.println();
        }
        pw.close();

        PrintWriter iw = new PrintWriter(new FileWriter(new File(OUT_DIR, "_btnkey_index.txt")));
        iw.print(idx.toString());
        iw.close();
        println("BTNKEY: done, " + funcs.size() + " unique functions reference the strings");
    }
}
