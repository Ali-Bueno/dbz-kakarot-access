// Boot agreement viewer (UAT_UIXcmnAgreement) native hunt (2026-07-17).
// Anchors on the class's reflected UFUNCTION name strings, finds their
// {const char* Name, FNativeFuncPtr exec} registration records, and walks the
// CONTIGUOUS per-class native table (StaticRegisterNatives block) both directions
// to recover THIS class's whole exec set. Then it SCORES each distinct table by how
// many anchor names it contains and decompiles every function in the winning table:
// exec thunk + impl (first call target) + one level of the impl's callees (to catch
// the texture-refresh routine that reads ImageTextureMap @0x508 and computes the key).
// Writes _agreement_index.txt (table maps + scores) and agreement_natives.c.
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

public class agreement_find extends GhidraScript {
    static final String OUT_DIR =
        "D:\\code\\unreal\\dragon ball kakarot access\\code\\decompiled";

    // Reflected UFunctions of UAT_UIXcmnAgreement (from CXX dump / task brief).
    static final String[] ANCHORS = {
        "MouseClickTabRight", "MouseClickTabLeft", "MouseClickDecide",
        "Loop_Arrow", "CheckHovered",
    };
    static final int WALK_CAP = 64;     // entries each direction
    static final int FUNC_CAP = 220;    // total unique funcs to decompile

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
            byte[] buf = new byte[96];
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
            if (out.size() > 300) break;
        }
        return out;
    }

    // A recovered contiguous class table: its byte span key + ordered members.
    static class Tbl {
        long spanKey;                 // block-relative lo offset, unique per table
        List<String> names = new ArrayList<>();
        List<Long> execs = new ArrayList<>();
        int score;
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

        Set<String> anchorSet = new LinkedHashSet<>();
        for (String a : ANCHORS) anchorSet.add(a);

        StringBuilder idx = new StringBuilder();
        Map<Long, Tbl> tables = new LinkedHashMap<>();  // spanKey -> table

        for (String anchor : ANCHORS) {
            List<Address> hits = strHits(anchor);
            idx.append("ANCHOR '").append(anchor).append("': ").append(hits.size()).append(" string hits\n");
            for (Address sa : hits) {
                long target = sa.getOffset();
                for (int bi = 0; bi < bl.size(); bi++) {
                    byte[] arr = bl.get(bi);
                    long base = bs.get(bi);
                    for (int i = 0; i + 16 <= arr.length; i += 8) {
                        if (u64(arr, i) != target) continue;
                        long exec = u64(arr, i + 8);
                        if (!inText(exec)) continue;
                        // walk contiguous table around [i]
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
                        long spanKey = base + lo;
                        if (tables.containsKey(spanKey)) continue;
                        Tbl t = new Tbl();
                        t.spanKey = spanKey;
                        for (int j = lo; j <= hi; j += 16) {
                            String nm = asciiAt(u64(arr, j));
                            long ex = u64(arr, j + 8);
                            if (nm == null) continue;
                            t.names.add(nm);
                            t.execs.add(ex);
                            if (anchorSet.contains(nm)) t.score++;
                        }
                        tables.put(spanKey, t);
                    }
                }
            }
        }

        // Report every distinct table + score; pick the best (highest anchor score).
        Tbl best = null;
        idx.append("\n==== distinct contiguous tables recovered ====\n");
        for (Tbl t : tables.values()) {
            idx.append(String.format("--- table @ 0x%x  members=%d  anchorScore=%d ---%n",
                    t.spanKey, t.names.size(), t.score));
            for (int k = 0; k < t.names.size(); k++) {
                idx.append(String.format("    %-46s exec 0x%x%n", t.names.get(k), t.execs.get(k)));
            }
            if (best == null || t.score > best.score
                    || (t.score == best.score && t.names.size() < best.names.size())) {
                best = t;
            }
        }

        if (best == null) {
            idx.append("\n!! no table found\n");
            PrintWriter iw = new PrintWriter(new FileWriter(new File(OUT_DIR, "_agreement_index.txt")));
            iw.print(idx.toString()); iw.close();
            println("AGREEMENT: no table"); return;
        }

        idx.append(String.format("%n==== WINNER table @ 0x%x  score=%d  members=%d ====%n",
                best.spanKey, best.score, best.names.size()));

        // Collect functions to decompile: for each member exec -> exec thunk + impl,
        // and for each impl, its .text callees (one level) to catch the refresh/key routine.
        Map<Long, String> allFuncs = new LinkedHashMap<>();  // addr -> role label
        for (int k = 0; k < best.names.size(); k++) {
            String nm = best.names.get(k);
            long exec = best.execs.get(k);
            allFuncs.putIfAbsent(exec, "EXEC " + nm);
            Address execAddr = space.getAddress(exec);
            Function ef = fm.getFunctionAt(execAddr);
            if (ef == null) { try { ef = createFunction(execAddr, null); } catch (Exception e) {} }
            Address impl = firstCallTarget(ef);
            if (impl != null) {
                allFuncs.putIfAbsent(impl.getOffset(), "IMPL " + nm);
                Function imf = fm.getFunctionAt(impl);
                if (imf == null) { try { imf = createFunction(impl, null); } catch (Exception e) {} }
                for (Address cal : allCallTargets(imf)) {
                    if (allFuncs.size() >= FUNC_CAP) break;
                    allFuncs.putIfAbsent(cal.getOffset(), "CALLEE-of " + nm);
                }
            }
            if (allFuncs.size() >= FUNC_CAP) break;
        }

        File out = new File(OUT_DIR, "agreement_natives.c");
        PrintWriter pw = new PrintWriter(new FileWriter(out));
        pw.println("// UAT_UIXcmnAgreement native registration table (exec + impl + impl callees).");
        pw.println("// Winner table @ 0x" + Long.toHexString(best.spanKey)
                + "  anchorScore=" + best.score + "  members=" + best.names.size());
        pw.println("// Reflected anchors: +0x398 RootPanel, +0x3A8 SubjectImage, +0x3B0 HeadImage,");
        pw.println("//   +0x3C0 LeftArrow, +0x3C8 RightArrow, +0x508 ImageTextureMap(TMap<u32,UTexture2D*>).");
        pw.println("//   Tail 0x3E0..0x508 = unreflected state (currentPage / currentDoc live here).");
        pw.println();
        for (Map.Entry<Long, String> e : allFuncs.entrySet()) {
            Address a = space.getAddress(e.getKey());
            Function f = fm.getFunctionAt(a);
            if (f == null) { try { f = createFunction(a, null); } catch (Exception ex) {} }
            if (f == null) { pw.println("// !! no function at 0x" + Long.toHexString(e.getKey())
                    + " (" + e.getValue() + ")"); continue; }
            pw.println("// ======== " + e.getValue() + "  @ " + f.getEntryPoint() + " ========");
            emit(pw, f);
        }
        pw.close();

        PrintWriter iw = new PrintWriter(new FileWriter(new File(OUT_DIR, "_agreement_index.txt")));
        iw.print(idx.toString());
        iw.close();
        println("AGREEMENT: done, " + allFuncs.size() + " funcs, winner @ 0x"
                + Long.toHexString(best.spanKey));
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

    List<Address> allCallTargets(Function f) {
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
