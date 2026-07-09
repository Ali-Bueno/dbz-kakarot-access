// UAT_UIMapWorld exec-table hunt (2026-07-09). Anchor on a name UNIQUE to this
// class ("OnClosedInfoWindow" / "OnCursorMove"), find its {const char* Name,
// FNativeFuncPtr exec} registration entry, walk the CONTIGUOUS class table both
// directions to recover UAT_UIMapWorld's exec thunks (InputConfirm,
// MouseClickDecide, CheckHovered, ...), then decompile each thunk + its impl
// (first .text call target) AND the second/third call targets so we follow the
// confirm path. Writes _mapworld_index.txt and mapworld_natives.c.
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

public class find_mapworld extends GhidraScript {
    static final String OUT_DIR =
        "D:\\code\\unreal\\dragon ball kakarot access\\code\\decompiled";
    static final String[] ANCHORS = { "OnClosedInfoWindow", "OnCursorMove" };
    // For these functions, also decompile the 2nd/3rd call targets (confirm path).
    static final String[] DEEP = { "InputConfirm", "MouseClickDecide", "CheckHovered" };

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

    @Override
    public void run() throws Exception {
        new File(OUT_DIR).mkdirs();
        mem = currentProgram.getMemory();
        fm = currentProgram.getFunctionManager();
        space = currentProgram.getAddressFactory().getDefaultAddressSpace();
        mon = new ConsoleTaskMonitor();
        decomp = new DecompInterface();
        decomp.openProgram(currentProgram);

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
        Map<Long, String> allFuncs = new LinkedHashMap<>();

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

            Set<Long> seenEntry = new LinkedHashSet<>();
            for (Address sa : strAddrs) {
                long target = sa.getOffset();
                for (int bi = 0; bi < bl.size(); bi++) {
                    byte[] arr = bl.get(bi);
                    long base = bs.get(bi);
                    for (int i = 0; i + 16 <= arr.length; i += 8) {
                        if (u64(arr, i) != target) continue;
                        long exec = u64(arr, i + 8);
                        if (!inText(exec)) continue;
                        long entryAddr = base + i;
                        if (!seenEntry.add(entryAddr)) continue;
                        idx.append("  entry @ 0x").append(Long.toHexString(entryAddr)).append("\n");
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
                            idx.append(String.format("    %-34s exec 0x%x%n", nm, ex));
                            allFuncs.putIfAbsent(ex, nm);
                        }
                    }
                }
            }
        }

        File out = new File(OUT_DIR, "mapworld_natives.c");
        PrintWriter pw = new PrintWriter(new FileWriter(out));
        pw.println("// UAT_UIMapWorld native registration table (anchored on OnClosedInfoWindow/OnCursorMove)");
        for (Map.Entry<Long, String> e : allFuncs.entrySet()) {
            Address execAddr = space.getAddress(e.getKey());
            Function f = fm.getFunctionAt(execAddr);
            if (f == null) { try { f = createFunction(execAddr, null); } catch (Exception ex) {} }
            if (f == null) { pw.println("// !! no function at 0x" + Long.toHexString(e.getKey()) + " for " + e.getValue()); continue; }
            pw.println("// ======== " + e.getValue() + " exec @ " + f.getEntryPoint() + " ========");
            emit(pw, f);
            boolean deep = false;
            for (String d : DEEP) if (d.equals(e.getValue())) deep = true;
            List<Address> calls = callTargets(f, deep ? 4 : 1);
            int n = 0;
            for (Address impl : calls) {
                Function g = fm.getFunctionAt(impl);
                if (g == null) continue;
                pw.println("// -------- " + e.getValue() + " call#" + (n++) + " @ " + impl + " --------");
                emit(pw, g);
            }
        }
        pw.close();

        PrintWriter iw = new PrintWriter(new FileWriter(new File(OUT_DIR, "_mapworld_index.txt")));
        iw.print(idx.toString());
        iw.close();
        println("FINDMW: done, " + allFuncs.size() + " unique exec funcs");
    }

    void emit(PrintWriter pw, Function f) {
        String c;
        try {
            DecompileResults res = decomp.decompileFunction(f, 150, mon);
            c = (res != null && res.decompileCompleted())
                ? res.getDecompiledFunction().getC() : "// <decompile failed>";
        } catch (Exception e) { c = "// <decompile error> " + e; }
        pw.println(c);
        pw.println();
    }

    // Return up to `max` distinct .text call targets in body order.
    List<Address> callTargets(Function f, int max) {
        List<Address> res = new ArrayList<>();
        Set<Long> seen = new LinkedHashSet<>();
        try {
            InstructionIterator ii = currentProgram.getListing().getInstructions(f.getBody(), true);
            while (ii.hasNext() && res.size() < max) {
                Instruction ins = ii.next();
                if (!ins.getMnemonicString().toLowerCase().startsWith("call")) continue;
                for (Reference r : ins.getReferencesFrom()) {
                    Address t = r.getToAddress();
                    Function g = fm.getFunctionAt(t);
                    if (g != null && !g.equals(f) && seen.add(t.getOffset())) {
                        res.add(t);
                        break;
                    }
                }
            }
        } catch (Exception e) {}
        return res;
    }
}
