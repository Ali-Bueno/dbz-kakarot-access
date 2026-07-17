// UAT_UIXcmnAgreement pass 3 (2026-07-17): decompile the cluster-B input-handler
// IMPLS (this in RCX) and follow their .text callees recursively (to DEPTH), so we
// capture the page-index tail member write in MouseClickTab*, its clamp against a
// page count, and the texture-refresh routine that reads ImageTextureMap @0x508 and
// computes its uint32 key. Seeds are the real impls (from the exec thunks in
// agreement2.c), NOT the FFrame exec thunks.
//@category Kakarot
import ghidra.app.script.GhidraScript;
import ghidra.app.decompiler.DecompInterface;
import ghidra.app.decompiler.DecompileResults;
import ghidra.program.model.address.Address;
import ghidra.program.model.address.AddressSpace;
import ghidra.program.model.listing.Function;
import ghidra.program.model.listing.FunctionManager;
import ghidra.program.model.listing.Instruction;
import ghidra.program.model.listing.InstructionIterator;
import ghidra.program.model.symbol.Reference;
import ghidra.util.task.ConsoleTaskMonitor;
import java.io.File;
import java.io.FileWriter;
import java.io.PrintWriter;
import java.util.ArrayDeque;
import java.util.ArrayList;
import java.util.LinkedHashMap;
import java.util.List;
import java.util.Map;

public class agree3 extends GhidraScript {
    static final String OUT_DIR =
        "D:\\code\\unreal\\dragon ball kakarot access\\code\\decompiled";
    static final int DEPTH = 3;      // seed=0
    static final int CAP = 150;

    // Cluster-B agreement impls (name -> impl addr), from agreement2.c exec thunks.
    static final Object[][] SEEDS = {
        {"CheckHovered",       0x1416b6650L},
        {"In_Headder",         0x1416bea70L},
        {"In_Win",             0x1416beb70L},
        {"Loop_Arrow",         0x1416c2180L},
        {"MouseClickDecide",   0x1416c2300L},
        {"MouseClickTabLeft",  0x1416c2370L},
        {"MouseClickTabRight", 0x1416c23a0L},
        {"Out_Headder",        0x1416c6c00L},
    };

    FunctionManager fm;
    AddressSpace space;
    DecompInterface decomp;
    ConsoleTaskMonitor mon;

    @Override
    public void run() throws Exception {
        new File(OUT_DIR).mkdirs();
        fm = currentProgram.getFunctionManager();
        space = currentProgram.getAddressFactory().getDefaultAddressSpace();
        mon = new ConsoleTaskMonitor();
        decomp = new DecompInterface();
        decomp.openProgram(currentProgram);

        // BFS over callees to DEPTH.
        Map<Long, String> role = new LinkedHashMap<>();
        Map<Long, Integer> depthOf = new LinkedHashMap<>();
        ArrayDeque<Long> q = new ArrayDeque<>();
        for (Object[] s : SEEDS) {
            long a = (Long) s[1];
            role.put(a, "SEED " + s[0]);
            depthOf.put(a, 0);
            q.add(a);
        }
        while (!q.isEmpty() && role.size() < CAP) {
            long cur = q.poll();
            int d = depthOf.get(cur);
            if (d >= DEPTH) continue;
            Function f = getOrCreate(space.getAddress(cur));
            for (Address c : allCalls(f)) {
                long ca = c.getOffset();
                if (role.containsKey(ca)) continue;
                if (role.size() >= CAP) break;
                role.put(ca, "d" + (d + 1) + " callee-of 0x" + Long.toHexString(cur));
                depthOf.put(ca, d + 1);
                q.add(ca);
            }
        }

        PrintWriter pw = new PrintWriter(new FileWriter(new File(OUT_DIR, "agreement3.c")));
        pw.println("// UAT_UIXcmnAgreement cluster-B impls + callees (depth " + DEPTH + ").");
        pw.println("// this@RCX. Reflected: +0x3A8 SubjectImage, +0x3B0 HeadImage,");
        pw.println("//   +0x3C0 LeftArrow, +0x3C8 RightArrow, +0x508 ImageTextureMap.");
        pw.println("//   Tail 0x3E0..0x508 = unreflected state.");
        pw.println();
        for (Map.Entry<Long, String> e : role.entrySet()) {
            Function f = getOrCreate(space.getAddress(e.getKey()));
            if (f == null) { pw.println("// !! no func @ 0x" + Long.toHexString(e.getKey())); continue; }
            pw.println("// ======== " + e.getValue() + "  @ " + f.getEntryPoint() + " ========");
            String c;
            try {
                DecompileResults r = decomp.decompileFunction(f, 120, mon);
                c = (r != null && r.decompileCompleted())
                    ? r.getDecompiledFunction().getC() : "// <decompile failed>";
            } catch (Exception ex) { c = "// <err> " + ex; }
            pw.println(c);
            pw.println();
        }
        pw.close();
        println("AGREE3: done, " + role.size() + " funcs");
    }

    Function getOrCreate(Address a) {
        if (a == null) return null;
        Function f = fm.getFunctionAt(a);
        if (f == null) { try { f = createFunction(a, null); } catch (Exception e) {} }
        return f;
    }

    List<Address> allCalls(Function f) {
        List<Address> out = new ArrayList<>();
        if (f == null) return out;
        try {
            InstructionIterator ii = currentProgram.getListing().getInstructions(f.getBody(), true);
            while (ii.hasNext()) {
                Instruction ins = ii.next();
                if (ins.getMnemonicString().toLowerCase().startsWith("call")) {
                    for (Reference r : ins.getReferencesFrom()) {
                        Address t = r.getToAddress();
                        Function g = fm.getFunctionAt(t);
                        if (g != null && !g.equals(f)) out.add(t);
                    }
                }
            }
        } catch (Exception e) {}
        return out;
    }
}
