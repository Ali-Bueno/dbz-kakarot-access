// UAT_UIXcmnAgreement focused decompile (2026-07-17, pass 2).
// The auto-walk in agreement_find.java crossed class boundaries; the real agreement
// class is the exec cluster around 0x1417bd/0x1417bf that carries Loop_Arrow +
// DebugChangeLangage + SettoUICommonInput (the boot "Change language" screen).
// This decompiles each candidate EXEC thunk + its IMPL (first .text call) + the
// impl's .text callees (one level), so the struct offsets (0x3A8 SubjectImage,
// 0x3B0 HeadImage, 0x508 ImageTextureMap, and the 0x3E0..0x508 tail state) reveal
// which cluster is the agreement and where the page/doc indices live.
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
import java.util.ArrayList;
import java.util.LinkedHashMap;
import java.util.LinkedHashSet;
import java.util.List;
import java.util.Map;
import java.util.Set;

public class agree2 extends GhidraScript {
    static final String OUT_DIR =
        "D:\\code\\unreal\\dragon ball kakarot access\\code\\decompiled";

    // Candidate exec thunks (from _agreement_index.txt, the Loop_Arrow clusters),
    // labelled by their registered UFunction name.
    static final long[][] EXECS = {
        // cluster A @0x14407a7b8 / 0x14407aba8  (execs 0x1417bd5xx / 0x1417bd9xx)
        {0x1417bd5a0L, 0}, {0x1417bd5d0L, 1}, {0x1417bd600L, 2}, {0x1417bd630L, 3},
        {0x1417bd660L, 4}, {0x1417bd690L, 5}, {0x1417bd6c0L, 6}, {0x1417bd6f0L, 7},
        {0x1417bd990L, 8}, {0x1417bd9c0L, 9}, {0x1417bd9f0L, 10}, {0x1417bda20L, 11},
        // cluster B @0x14407c260 / 0x14407d870  (execs 0x1417bexx / 0x1417bfxxx)
        {0x1417befe0L, 12}, {0x1417bf000L, 13}, {0x1417bf020L, 14}, {0x1417bf040L, 15},
        {0x1417bf0f0L, 16}, {0x1417bf110L, 17}, {0x1417bf130L, 18}, {0x1417bf150L, 19},
        {0x1417bf170L, 20}, {0x1417bf190L, 21}, {0x1417bf1b0L, 22}, {0x1417bf340L, 23},
        {0x1417bf4d0L, 24}, {0x1417bf660L, 25}, {0x1417bf680L, 26}, {0x1417bf6a0L, 27},
        {0x1417bf6c0L, 28}, {0x1417bf6e0L, 29}, {0x1417bf700L, 30}, {0x1417bf720L, 31},
        {0x1417bf740L, 32},
    };
    static final String[] NAMES = {
        "A:CheckHovered","A:In_Headder","A:In_Win","A:Loop_Arrow","A:MouseClickDecide",
        "A:MouseClickTabLeft","A:MouseClickTabRight","A:Out_Headder",
        "A2:CheckHovered","A2:Loop_Arrow","A2:MouseClickDecide","A2:MouseClickTabLeft",
        "B:CheckHovered","B2:CheckHovered","B3:CheckHovered","B:DebugChangeLangage",
        "B:In_Headder","B:In_Win","B2:In_Win","B:Loop_Arrow","B2:Loop_Arrow","B3:Loop_Arrow",
        "B:MouseClickDecide","B2:MouseClickDecide","B3:MouseClickDecide",
        "B:MouseClickTabLeft","B2:MouseClickTabLeft","B:MouseClickTabRight","B2:MouseClickTabRight",
        "B:Out_Headder","B:Out_Win","B2:Out_Win","B:SettoUICommonInput",
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

        Map<Long, String> funcs = new LinkedHashMap<>();
        for (long[] e : EXECS) {
            long ex = e[0];
            String nm = NAMES[(int) e[1]];
            funcs.putIfAbsent(ex, "EXEC " + nm);
            Address ea = space.getAddress(ex);
            Function ef = getOrCreate(ea);
            Address impl = firstCall(ef);
            if (impl != null) {
                funcs.putIfAbsent(impl.getOffset(), "IMPL " + nm);
                Function imf = getOrCreate(impl);
                for (Address c : allCalls(imf)) {
                    funcs.putIfAbsent(c.getOffset(), "CALLEE-of " + nm);
                }
            }
        }

        PrintWriter pw = new PrintWriter(new FileWriter(new File(OUT_DIR, "agreement2.c")));
        pw.println("// UAT_UIXcmnAgreement focused decompile (exec + impl + impl callees).");
        pw.println("// Anchors: +0x3A8 SubjectImage, +0x3B0 HeadImage, +0x3C0 LeftArrow,");
        pw.println("//   +0x3C8 RightArrow, +0x508 ImageTextureMap; tail 0x3E0..0x508 = state.");
        pw.println();
        for (Map.Entry<Long, String> en : funcs.entrySet()) {
            Address a = space.getAddress(en.getKey());
            Function f = getOrCreate(a);
            if (f == null) { pw.println("// !! no func @ 0x" + Long.toHexString(en.getKey())
                    + " (" + en.getValue() + ")"); continue; }
            pw.println("// ======== " + en.getValue() + "  @ " + f.getEntryPoint() + " ========");
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
        println("AGREE2: done, " + funcs.size() + " funcs");
    }

    Function getOrCreate(Address a) {
        if (a == null) return null;
        Function f = fm.getFunctionAt(a);
        if (f == null) { try { f = createFunction(a, null); } catch (Exception e) {} }
        return f;
    }

    Address firstCall(Function f) {
        if (f == null) return null;
        try {
            InstructionIterator ii = currentProgram.getListing().getInstructions(f.getBody(), true);
            while (ii.hasNext()) {
                Instruction ins = ii.next();
                if (ins.getMnemonicString().toLowerCase().startsWith("call")) {
                    for (Reference r : ins.getReferencesFrom()) {
                        Address t = r.getToAddress();
                        Function g = fm.getFunctionAt(t);
                        if (g != null && !g.equals(f)) return t;
                    }
                }
            }
        } catch (Exception e) {}
        return null;
    }

    List<Address> allCalls(Function f) {
        List<Address> out = new ArrayList<>();
        if (f == null) return out;
        Set<Long> seen = new LinkedHashSet<>();
        try {
            InstructionIterator ii = currentProgram.getListing().getInstructions(f.getBody(), true);
            while (ii.hasNext()) {
                Instruction ins = ii.next();
                if (ins.getMnemonicString().toLowerCase().startsWith("call")) {
                    for (Reference r : ins.getReferencesFrom()) {
                        Address t = r.getToAddress();
                        Function g = fm.getFunctionAt(t);
                        if (g != null && !g.equals(f) && seen.add(t.getOffset())) out.add(t);
                    }
                }
            }
        } catch (Exception e) {}
        return out;
    }
}
