// Pending input-assign buffer: decompile the 0x1E0-copy candidates found by
// inputassign_scan.java, name them via their referenced strings, and walk the
// UFunction native-registration table for the option-menu classes so we can
// attach the copy to a class.
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

public class inputassign_dig extends GhidraScript {
    static final String OUT_DIR =
        "D:\\code\\unreal\\dragon ball kakarot access\\code\\decompiled";

    // From _inputassign_scan.txt (section C / A):
    static final long[] TARGETS = {
        0x141a82450L,  // tiny helper LEA map  (called by the default-setters)
        0x141a84ae0L,  // tiny helper LEA map
        0x141a57b50L,  // tiny helper LEA map (option screens)
        0x141a8e3b0L,  // tiny helper LEA map (option screens)
        0x141a51e70L,  // heavy map access (Add / FindOrAdd?)
        0x141a66300L,  // map Find (many UI callers)
    };

    // Reflected function names of the option-menu classes (AT.hpp) -> class tables.
    static final String[] WALK = {
        "MouseClickDecide",        // UOptionMenu
        "MenuActivate",            // UOptionMenuComponent
        "CheckHovered",            // UAT_UIStartOption
        "In_List",                 // UAT_UIStartOption
    };
    static final int WALK_CAP = 64;

    Memory mem; FunctionManager fm; AddressSpace space;
    DecompInterface decomp; ConsoleTaskMonitor mon;
    List<byte[]> bl = new ArrayList<>(); List<Long> bs = new ArrayList<>();
    PrintWriter pw;

    long u64(byte[] b, int i) {
        long v = 0; for (int k = 0; k < 8; k++) v |= ((long)(b[i+k] & 0xff)) << (8*k); return v;
    }
    String asciiAt(long ptr) {
        try {
            Address a = space.getAddress(ptr);
            byte[] buf = new byte[80]; mem.getBytes(a, buf);
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
        try { MemoryBlock b = mem.getBlock(space.getAddress(ptr)); return b != null && b.isExecute(); }
        catch (Exception e) { return false; }
    }
    List<Address> strHits(String s) throws Exception {
        byte[] needle = (s + "\0").getBytes("ISO-8859-1");
        List<Address> out = new ArrayList<>();
        Address from = mem.getMinAddress();
        while (true) { Address hit = find(from, needle); if (hit == null) break;
            out.add(hit); from = hit.add(1); if (out.size() > 100) break; }
        return out;
    }

    // Strings (ascii or utf16) referenced from inside a function -> a cheap name.
    Set<String> stringsIn(Function f) {
        Set<String> out = new LinkedHashSet<>();
        try {
            InstructionIterator ii = currentProgram.getListing().getInstructions(f.getBody(), true);
            while (ii.hasNext()) {
                Instruction ins = ii.next();
                for (Reference r : ins.getReferencesFrom()) {
                    Address t = r.getToAddress();
                    String s = asciiAt(t.getOffset());
                    if (s == null) s = utf16At(t.getOffset());
                    if (s != null && s.length() >= 4) out.add(s);
                    if (out.size() > 40) return out;
                }
            }
        } catch (Exception e) {}
        return out;
    }
    String utf16At(long ptr) {
        try {
            Address a = space.getAddress(ptr);
            byte[] buf = new byte[120]; mem.getBytes(a, buf);
            StringBuilder sb = new StringBuilder();
            for (int i = 0; i + 1 < buf.length; i += 2) {
                int c = (buf[i] & 0xff) | ((buf[i+1] & 0xff) << 8);
                if (c == 0) return sb.length() >= 3 ? sb.toString() : null;
                if (c < 0x20 || c > 0x7e) return null;
                sb.append((char) c);
            }
        } catch (Exception e) {}
        return null;
    }

    void emit(Function f, String tag) {
        pw.println("// ================= " + tag + "  " + f.getName() + " @ " + f.getEntryPoint()
                   + "  size=0x" + Long.toHexString(f.getBody().getNumAddresses()) + " =================");
        Set<String> ss = stringsIn(f);
        if (!ss.isEmpty()) pw.println("// strings: " + ss);
        StringBuilder cs = new StringBuilder();
        for (Function c : f.getCallingFunctions(mon)) {
            cs.append(c.getName()).append("@").append(c.getEntryPoint()).append(" ");
            if (cs.length() > 700) { cs.append("..."); break; }
        }
        pw.println("// callers: " + (cs.length() == 0 ? "<none>" : cs.toString()));
        try {
            DecompileResults res = decomp.decompileFunction(f, 180, mon);
            pw.println(res != null && res.decompileCompleted()
                ? res.getDecompiledFunction().getC() : "// <decompile failed>");
        } catch (Exception e) { pw.println("// <decompile error> " + e); }
        pw.println();
        pw.flush();
    }

    @Override
    public void run() throws Exception {
        new File(OUT_DIR).mkdirs();
        mem = currentProgram.getMemory();
        fm = currentProgram.getFunctionManager();
        space = currentProgram.getAddressFactory().getDefaultAddressSpace();
        mon = new ConsoleTaskMonitor();
        decomp = new DecompInterface(); decomp.openProgram(currentProgram);
        pw = new PrintWriter(new FileWriter(new File(OUT_DIR, "_inputassign_dig4.c")));

        for (long t : TARGETS) {
            Address a = space.getAddress(t);
            Function f = fm.getFunctionContaining(a);
            if (f == null) { try { f = createFunction(a, null); } catch (Exception e) {} }
            if (f == null) { pw.println("// !! no function at 0x" + Long.toHexString(t)); continue; }
            println("DIG " + f.getEntryPoint());
            emit(f, "TARGET 0x" + Long.toHexString(t));
            // one level of callers, decompiled too (context: who commits?)
            int n = 0;
            for (Function c : f.getCallingFunctions(mon)) {
                if (n++ >= 3) break;
                emit(c, "  CALLER-OF 0x" + Long.toHexString(t));
            }
        }

        for (String bn : new String[]{".rdata", ".data"}) {
            MemoryBlock b = mem.getBlock(bn);
            if (b == null) continue;
            byte[] arr = new byte[(int) b.getSize()];
            b.getBytes(b.getStart(), arr);
            bl.add(arr); bs.add(b.getStart().getOffset());
        }

        PrintWriter iw = new PrintWriter(new FileWriter(new File(OUT_DIR, "_inputassign_natives2.txt")));
        Map<Long, String> execs = new LinkedHashMap<>();
        for (String anchor : WALK) {
            List<Address> hits = strHits(anchor);
            iw.println("WALK '" + anchor + "': " + hits.size() + " string hits");
            Set<Long> seen = new LinkedHashSet<>();
            for (Address sa : hits) {
                long target = sa.getOffset();
                for (int bi = 0; bi < bl.size(); bi++) {
                    byte[] arr = bl.get(bi); long base = bs.get(bi);
                    for (int i = 0; i + 16 <= arr.length; i += 8) {
                        if (u64(arr, i) != target) continue;
                        long exec = u64(arr, i + 8);
                        if (!inText(exec)) continue;
                        if (!seen.add(base + i)) continue;
                        iw.printf("  --- class table containing entry @ 0x%x ---%n", base + i);
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
                            iw.printf("    %-46s exec 0x%x%n", nm, ex);
                            execs.putIfAbsent(ex, anchor + "::" + nm);
                        }
                    }
                }
            }
        }
        iw.close();
        pw.close();
        println("DIG done, " + execs.size() + " option-class execs listed");
    }
}
