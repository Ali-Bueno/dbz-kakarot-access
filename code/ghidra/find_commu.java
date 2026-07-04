// Community-board cursor hunt (2026-07-04). Goal: find where the board's free
// cursor position / hovered-socket index live, so the mod can read them.
//
// "CheckHovered"/"MouseClickDecide" exist in 40+ classes, so the generic
// find_ufunc name->pointer scan drowned. Anchor instead on names UNIQUE to
// UAT_UICommunityBoard ("PlayInCommRank"/"PlayOutCommRank"), locate their
// {const char* Name, FNativeFuncPtr exec} registration entry, then walk the
// CONTIGUOUS class table in both directions - that yields THIS class's
// CheckHovered / MouseClickDecide / SetSelectTab exec thunks unambiguously.
// Decompile each thunk + its impl (first .text call target).
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
import java.util.List;
import java.util.Map;

public class find_commu extends GhidraScript {
    static final String OUT_DIR =
        "D:\\code\\unreal\\dragon ball kakarot access\\code\\decompiled";
    static final String[] ANCHORS = { "PlayInCommRank", "PlayOutCommRank" };

    Memory mem;
    FunctionManager fm;
    AddressSpace space;
    DecompInterface decomp;
    ConsoleTaskMonitor mon;

    long u64(byte[] b, int i) {
        long v = 0;
        for (int k = 0; k < 8; k++) v |= ((long)(b[i+k] & 0xff)) << (8*k);
        return v;
    }

    // Read a NUL-terminated ASCII string at addr; null if not plausible.
    String asciiAt(long ptr) {
        try {
            Address a = space.getAddress(ptr);
            byte[] buf = new byte[64];
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

    @Override
    public void run() throws Exception {
        new File(OUT_DIR).mkdirs();
        mem = currentProgram.getMemory();
        fm = currentProgram.getFunctionManager();
        space = currentProgram.getAddressFactory().getDefaultAddressSpace();
        mon = new ConsoleTaskMonitor();
        decomp = new DecompInterface();
        decomp.openProgram(currentProgram);

        // cache .rdata + .data for the pointer scan
        String[] wanted = {".rdata", ".data"};
        List<byte[]> bl = new ArrayList<>();
        List<Long> bs = new ArrayList<>();
        for (String bn : wanted) {
            MemoryBlock b = mem.getBlock(bn);
            if (b == null) continue;
            byte[] arr = new byte[(int) b.getSize()];
            b.getBytes(b.getStart(), arr);
            bl.add(arr); bs.add(b.getStart().getOffset());
        }

        StringBuilder idx = new StringBuilder();
        // name -> exec, in table order (LinkedHashMap keeps it readable)
        Map<String, Long> table = new LinkedHashMap<>();

        for (String anchor : ANCHORS) {
            byte[] needle = (anchor + "\0").getBytes("ISO-8859-1");
            List<Address> strAddrs = new ArrayList<>();
            Address from = mem.getMinAddress();
            while (true) {
                Address hit = find(from, needle);
                if (hit == null) break;
                strAddrs.add(hit);
                from = hit.add(1);
                if (strAddrs.size() > 200) break;
            }
            idx.append("anchor '").append(anchor).append("': ")
               .append(strAddrs.size()).append(" string hits\n");

            for (Address sa : strAddrs) {
                long target = sa.getOffset();
                for (int bi = 0; bi < bl.size(); bi++) {
                    byte[] arr = bl.get(bi);
                    long base = bs.get(bi);
                    for (int i = 0; i + 16 <= arr.length; i += 8) {
                        if (u64(arr, i) != target) continue;
                        long exec = u64(arr, i + 8);
                        if (!inText(exec)) continue;
                        idx.append("  entry @ 0x").append(Long.toHexString(base + i)).append("\n");
                        // walk the contiguous {name, exec} table both ways
                        int lo = i, hi = i;
                        while (lo - 16 >= 0) {
                            String nm = asciiAt(u64(arr, lo - 16));
                            if (nm == null || !inText(u64(arr, lo - 8))) break;
                            lo -= 16;
                        }
                        while (hi + 32 <= arr.length) {
                            String nm = asciiAt(u64(arr, hi + 16));
                            if (nm == null || !inText(u64(arr, hi + 24))) break;
                            hi += 16;
                        }
                        for (int j = lo; j <= hi; j += 16) {
                            String nm = asciiAt(u64(arr, j));
                            long ex = u64(arr, j + 8);
                            if (nm == null) continue;
                            table.put(nm, ex);
                            idx.append(String.format("    %-28s exec 0x%x%n", nm, ex));
                        }
                    }
                }
            }
        }

        // decompile every table function: exec thunk + first-call impl
        File out = new File(OUT_DIR, "commu_board_natives.c");
        PrintWriter pw = new PrintWriter(new FileWriter(out));
        pw.println("// UAT_UICommunityBoard native registration table (anchored on PlayIn/OutCommRank)");
        for (Map.Entry<String, Long> e : table.entrySet()) {
            Address execAddr = space.getAddress(e.getValue());
            Function f = fm.getFunctionAt(execAddr);
            if (f == null) { try { f = createFunction(execAddr, null); } catch (Exception ex) {} }
            if (f == null) { pw.println("// !! no function at 0x" + Long.toHexString(e.getValue()) + " for " + e.getKey()); continue; }
            pw.println("// ======== " + e.getKey() + " exec @ " + f.getEntryPoint() + " ========");
            emit(pw, f);
            Address impl = firstCallTarget(f);
            if (impl != null) {
                Function g = fm.getFunctionAt(impl);
                if (g != null) {
                    pw.println("// -------- " + e.getKey() + " impl @ " + impl + " --------");
                    emit(pw, g);
                }
            }
        }
        pw.close();

        PrintWriter iw = new PrintWriter(new FileWriter(new File(OUT_DIR, "_commu_index.txt")));
        iw.print(idx.toString());
        iw.close();
        println("FINDCOMMU: done, " + table.size() + " entries");
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
