// Ghidra headless POST-script (Java).
// For a curated set of UE UI classes (the ones behind the two Lua dead ends),
// locate their RTTI vftable via the symbol table, collect the virtual methods
// plus any function that references the vftable (ctor / handlers), decompile
// them and write readable C to code/decompiled/<Class>.c + _index.txt.
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
import ghidra.program.model.symbol.ReferenceIterator;
import ghidra.program.model.symbol.ReferenceManager;
import ghidra.program.model.symbol.Symbol;
import ghidra.program.model.symbol.SymbolIterator;
import ghidra.program.model.symbol.SymbolTable;
import ghidra.util.task.ConsoleTaskMonitor;
import java.io.File;
import java.io.FileWriter;
import java.io.PrintWriter;
import java.util.ArrayList;
import java.util.Comparator;
import java.util.HashMap;
import java.util.LinkedHashSet;
import java.util.List;
import java.util.Map;
import java.util.Set;

public class export_ui extends GhidraScript {
    static final String OUT_DIR =
        "D:\\code\\unreal\\dragon ball kakarot access\\code\\decompiled";
    static final String[] TARGETS = {
        "AT_UIXCmnPause", "AT_UIGameoverBar", "AT_UIGameover", "AT_UIXCmnHeader",
        "AT_UIStartTop", "AT_UIStartTopList", "AT_UIStartTopListZ",
        "AT_UIMenuListBase00", "AT_UIMenuListBase01", "AT_UIMenuListBase03",
        "AT_UITips", "MenuManager", "RootMenu", "BaseMenu",
    };

    @Override
    public void run() throws Exception {
        new File(OUT_DIR).mkdirs();
        SymbolTable st = currentProgram.getSymbolTable();
        FunctionManager fm = currentProgram.getFunctionManager();
        ReferenceManager rm = currentProgram.getReferenceManager();
        Memory mem = currentProgram.getMemory();
        AddressSpace space = currentProgram.getAddressFactory().getDefaultAddressSpace();
        ConsoleTaskMonitor mon = new ConsoleTaskMonitor();

        Map<String, List<Address>> vftables = new HashMap<>();
        Map<String, List<Address>> symAddrs = new HashMap<>();
        for (String t : TARGETS) {
            vftables.put(t, new ArrayList<Address>());
            symAddrs.put(t, new ArrayList<Address>());
        }

        SymbolIterator it = st.getAllSymbols(true);
        while (it.hasNext()) {
            Symbol s = it.next();
            String nm = s.getName(true);
            for (String t : TARGETS) {
                if (nm.contains(t)) {
                    symAddrs.get(t).add(s.getAddress());
                    String low = nm.toLowerCase();
                    if (low.contains("vftable") || low.contains("vtable")) {
                        vftables.get(t).add(s.getAddress());
                    }
                }
            }
        }

        DecompInterface decomp = new DecompInterface();
        decomp.openProgram(currentProgram);

        StringBuilder index = new StringBuilder();
        for (String t : TARGETS) {
            Set<Address> funcs = new LinkedHashSet<>();
            index.append("=== ").append(t).append(" ===\n");
            for (Address vaddr : vftables.get(t)) {
                index.append("  vftable @ ").append(vaddr).append("\n");
                Address a = vaddr;
                for (int i = 0; i < 400; i++) {
                    long v;
                    try {
                        v = mem.getLong(a);
                    } catch (Exception e) {
                        break;
                    }
                    if (v == 0) break;
                    Address tgt;
                    try {
                        tgt = space.getAddress(v);
                    } catch (Exception e) {
                        break;
                    }
                    Function f = fm.getFunctionAt(tgt);
                    if (f == null) break;
                    funcs.add(tgt);
                    a = a.add(8);
                }
                ReferenceIterator ri = rm.getReferencesTo(vaddr);
                while (ri.hasNext()) {
                    Reference r = ri.next();
                    Function f = fm.getFunctionContaining(r.getFromAddress());
                    if (f != null) funcs.add(f.getEntryPoint());
                }
            }
            for (Address a : symAddrs.get(t)) {
                Function f = fm.getFunctionContaining(a);
                if (f != null) funcs.add(f.getEntryPoint());
            }
            index.append("  functions: ").append(funcs.size()).append("\n");
            if (funcs.isEmpty()) continue;

            List<Address> sorted = new ArrayList<>(funcs);
            sorted.sort(Comparator.comparingLong(x -> x.getOffset()));
            File out = new File(OUT_DIR, t + ".c");
            PrintWriter pw = new PrintWriter(new FileWriter(out));
            pw.println("// Decompiled functions related to " + t + " (vftable virtuals + xrefs)");
            pw.println("// Source: AT-Win64-Shipping.exe.unpacked.exe (Steamless), Ghidra 12 RTTI/demangler");
            pw.println();
            int count = 0;
            for (Address fa : sorted) {
                Function f = fm.getFunctionAt(fa);
                if (f == null) continue;
                String c;
                try {
                    DecompileResults res = decomp.decompileFunction(f, 90, mon);
                    if (res != null && res.decompileCompleted()) {
                        c = res.getDecompiledFunction().getC();
                    } else {
                        c = "// <decompile failed> " + f.getName();
                    }
                } catch (Exception e) {
                    c = "// <decompile error> " + f.getName() + " : " + e;
                }
                pw.println("// ---- " + f.getName() + " @ " + f.getEntryPoint() + " ----");
                pw.println(c);
                pw.println();
                count++;
                index.append("    ").append(f.getEntryPoint()).append("  ").append(f.getName()).append("\n");
            }
            pw.close();
            index.append("  wrote ").append(out).append(" (").append(count).append(" funcs)\n");
        }
        PrintWriter iw = new PrintWriter(new FileWriter(new File(OUT_DIR, "_index.txt")));
        iw.print(index.toString());
        iw.close();
        println("export_ui done");
    }
}
