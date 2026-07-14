// Skill-tree / skill-ownership native hunt (2026-07-14).
// Anchors on skill-related UFUNCTION name strings, locates their
// {const char* Name, FNativeFuncPtr exec} registration entries, and for the
// "walk" anchors also walks the CONTIGUOUS class table both directions to
// recover THAT class's whole exec set (that is how we get AT_UISkillTree's
// OnInputDPad*/OnInputDecide/Open without confusing them with the dozens of
// other classes that also export those generic names).
// Writes _skill_index.txt (table maps) and skill_natives.c (decompiled).
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
import ghidra.program.model.mem.Memory;
import ghidra.program.model.mem.MemoryBlock;
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

public class skill_find extends GhidraScript {
    static final String OUT_DIR =
        "D:\\code\\unreal\\dragon ball kakarot access\\code\\decompiled";

    // Unique-enough names: take ONLY the matching table entry, no walk.
    static final String[] DIRECT = {
        "HasSkill",
        "HasSkill_BPCharaCode",
        "ATDebugSetSkillLevel",
        "LearnCharacterSkill",
        "RequestImportantLogSkillUnlockCheck",
    };
    // Names that identify a skill class: walk the whole contiguous class table.
    static final String[] WALK = {
        "OnInputGotoSkillCustomize",   // -> AT_UISkillTree
        "OnIn_Line",                   // -> AT_UISkillTreePanel (verify by neighbours)
    };
    static final int WALK_CAP = 48;     // entries each direction
    static final int FUNC_CAP = 160;    // total unique funcs to decompile

    Memory mem;
    FunctionManager fm;
    AddressSpace space;
    DecompInterface decomp;
    ConsoleTaskMonitor mon;
    List<byte[]> bl = new ArrayList<>();
    List<Long> bs = new ArrayList<>();

    long u64(byte[] b, int i) {
        long v = 0;
        for (int k = 0; k < 8; k++) v |= ((long)(b[i+k] & 0xff)) << (8*k);
        return v;
    }

    String asciiAt(long ptr) {
        try {
            Address a = space.getAddress(ptr);
            byte[] buf = new byte[80];
            mem.getBytes(a, buf);
            StringBuilder sb = new StringBuilder();
            for (byte c : buf) {
                if (c == 0) return sb.length() >= 2 ? sb.toString() : null;
                if (c < 0x20 || c > 0x7e) return null;
                sb.append((char) c);
            }
        } catch (Exception e) {}
        return null;
    }

    boolean inText(long ptr) {
        try {
            MemoryBlock b = mem.getBlock(space.getAddress(ptr));
            return b != null && b.isExecute();
        } catch (Exception e) { return false; }
    }

    List<Address> strHits(String s) throws Exception {
        byte[] needle = (s + "\0").getBytes("ISO-8859-1");
        List<Address> out = new ArrayList<>();
        Address from = mem.getMinAddress();
        while (true) {
            Address hit = find(from, needle);
            if (hit == null) break;
            out.add(hit);
            from = hit.add(1);
            if (out.size() > 200) break;
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

        for (String bn : new String[]{".rdata", ".data"}) {
            MemoryBlock b = mem.getBlock(bn);
            if (b == null) continue;
            byte[] arr = new byte[(int) b.getSize()];
            b.getBytes(b.getStart(), arr);
            bl.add(arr); bs.add(b.getStart().getOffset());
        }

        StringBuilder idx = new StringBuilder();
        Map<Long, String> allFuncs = new LinkedHashMap<>();

        for (String anchor : DIRECT) {
            List<Address> hits = strHits(anchor);
            idx.append("DIRECT '").append(anchor).append("': ").append(hits.size()).append(" string hits\n");
            for (Address sa : hits) {
                long target = sa.getOffset();
                for (int bi = 0; bi < bl.size(); bi++) {
                    byte[] arr = bl.get(bi);
                    long base = bs.get(bi);
                    for (int i = 0; i + 16 <= arr.length; i += 8) {
                        if (u64(arr, i) != target) continue;
                        long exec = u64(arr, i + 8);
                        if (!inText(exec)) continue;
                        idx.append(String.format("  entry @ 0x%x  exec 0x%x%n", base + i, exec));
                        allFuncs.putIfAbsent(exec, anchor);
                    }
                }
            }
        }

        for (String anchor : WALK) {
            List<Address> hits = strHits(anchor);
            idx.append("WALK '").append(anchor).append("': ").append(hits.size()).append(" string hits\n");
            Set<Long> seenEntry = new LinkedHashSet<>();
            for (Address sa : hits) {
                long target = sa.getOffset();
                for (int bi = 0; bi < bl.size(); bi++) {
                    byte[] arr = bl.get(bi);
                    long base = bs.get(bi);
                    for (int i = 0; i + 16 <= arr.length; i += 8) {
                        if (u64(arr, i) != target) continue;
                        long exec = u64(arr, i + 8);
                        if (!inText(exec)) continue;
                        if (!seenEntry.add(base + i)) continue;
                        idx.append(String.format("  --- class table containing entry @ 0x%x ---%n", base + i));
                        int lo = i, hi = i, n = 0;
                        while (lo - 16 >= 0 && n < WALK_CAP) {
                            if (asciiAt(u64(arr, lo - 16)) == null || !inText(u64(arr, lo - 8))) break;
                            lo -= 16; n++;
                        }
                        n = 0;
                        while (hi + 32 <= arr.length && n < WALK_CAP) {
                            if (asciiAt(u64(arr, hi + 16)) == null || !inText(u64(arr, hi + 24))) break;
                            hi += 16; n++;
                        }
                        for (int j = lo; j <= hi; j += 16) {
                            String nm = asciiAt(u64(arr, j));
                            long ex = u64(arr, j + 8);
                            if (nm == null) continue;
                            idx.append(String.format("    %-44s exec 0x%x%n", nm, ex));
                            if (allFuncs.size() < FUNC_CAP) allFuncs.putIfAbsent(ex, anchor + "::" + nm);
                        }
                    }
                }
            }
        }

        File out = new File(OUT_DIR, "skill_natives.c");
        PrintWriter pw = new PrintWriter(new FileWriter(out));
        pw.println("// Skill-tree / skill-ownership native registration tables (exec thunk + impl)");
        for (Map.Entry<Long, String> e : allFuncs.entrySet()) {
            Address execAddr = space.getAddress(e.getKey());
            Function f = fm.getFunctionAt(execAddr);
            if (f == null) { try { f = createFunction(execAddr, null); } catch (Exception ex) {} }
            if (f == null) { pw.println("// !! no function at 0x" + Long.toHexString(e.getKey()) + " for " + e.getValue()); continue; }
            pw.println("// ======== " + e.getValue() + "  exec @ " + f.getEntryPoint() + " ========");
            emit(pw, f);
            Address impl = firstCallTarget(f);
            if (impl != null) {
                Function g = fm.getFunctionAt(impl);
                if (g != null) {
                    pw.println("// -------- " + e.getValue() + "  impl @ " + impl + " --------");
                    emit(pw, g);
                }
            }
        }
        pw.close();

        PrintWriter iw = new PrintWriter(new FileWriter(new File(OUT_DIR, "_skill_index.txt")));
        iw.print(idx.toString());
        iw.close();
        println("SKILLFIND: done, " + allFuncs.size() + " unique exec funcs");
    }

    void emit(PrintWriter pw, Function f) {
        String c;
        try {
            DecompileResults res = decomp.decompileFunction(f, 120, mon);
            c = (res != null && res.decompileCompleted())
                ? res.getDecompiledFunction().getC() : "// <decompile failed>";
        } catch (Exception e) { c = "// <decompile error> " + e; }
        pw.println(c);
        pw.println();
    }

    Address firstCallTarget(Function f) {
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
}
