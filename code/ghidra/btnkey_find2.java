// Follow-up: the Btn_Key_Up/Dwn/L/R/Ud/Lr strings exist but Ghidra recorded no
// refs. Find them the raw way: (1) locate numbered "Btn_Key_1..6" strings,
// (2) raw-scan .rdata/.data for 64-bit LE pointers to each direction/numbered
// string (pointer tables), (3) dump the bytes around each string cluster and
// each pointer hit, (4) for pointer hits inside code (lea imm), report+decompile
// the containing function. Writes _btnkey2_index.txt and btnkey2_natives.c.
//@category Kakarot
import ghidra.app.script.GhidraScript;
import ghidra.app.decompiler.DecompInterface;
import ghidra.app.decompiler.DecompileResults;
import ghidra.program.model.address.Address;
import ghidra.program.model.address.AddressSpace;
import ghidra.program.model.listing.Function;
import ghidra.program.model.listing.FunctionManager;
import ghidra.program.model.mem.Memory;
import ghidra.program.model.mem.MemoryBlock;
import ghidra.util.task.ConsoleTaskMonitor;
import java.io.File;
import java.io.FileWriter;
import java.io.PrintWriter;
import java.util.ArrayList;
import java.util.LinkedHashMap;
import java.util.List;
import java.util.Map;

public class btnkey_find2 extends GhidraScript {
    static final String OUT_DIR =
        "D:\\code\\unreal\\dragon ball kakarot access\\code\\decompiled";

    static final String[] NAMES = {
        "Btn_Key_Up", "Btn_Key_Dwn", "Btn_Key_L", "Btn_Key_R", "Btn_Key_Ud", "Btn_Key_Lr",
        "Btn_Key_1", "Btn_Key_2", "Btn_Key_3", "Btn_Key_4", "Btn_Key_5", "Btn_Key_6",
        "Btn_Key",
    };

    Memory mem;
    FunctionManager fm;
    AddressSpace space;
    DecompInterface decomp;
    ConsoleTaskMonitor mon;

    Address firstStr(String s) throws Exception {
        // exact (null-terminated) match; but Btn_Key_1 must not match Btn_Key_10 etc.
        return find(mem.getMinAddress(), (s + "\0").getBytes("ISO-8859-1"));
    }

    String hexDump(Address a, int back, int len) {
        StringBuilder sb = new StringBuilder();
        try {
            Address start = a.subtract(back);
            byte[] buf = new byte[len];
            mem.getBytes(start, buf);
            for (int i = 0; i < len; i += 16) {
                sb.append(String.format("      %s: ", start.add(i)));
                StringBuilder asc = new StringBuilder();
                for (int j = 0; j < 16 && i + j < len; j++) {
                    int b = buf[i + j] & 0xff;
                    sb.append(String.format("%02x ", b));
                    asc.append((b >= 0x20 && b < 0x7f) ? (char) b : '.');
                }
                sb.append(" ").append(asc).append("\n");
            }
        } catch (Exception e) { sb.append("      <dump err>\n"); }
        return sb.toString();
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

        StringBuilder idx = new StringBuilder();
        Map<String, Long> strAddr = new LinkedHashMap<>();

        idx.append("### string locations + surrounding bytes ###\n");
        for (String n : NAMES) {
            Address a = firstStr(n);
            if (a == null) { idx.append(n).append(": NOT FOUND\n"); continue; }
            strAddr.put(n, a.getOffset());
            idx.append(String.format("%s @ %s%n", n, a));
            idx.append(hexDump(a, 16, 48));
        }

        // Raw scan every loaded block for 64-bit LE pointers to each string addr.
        idx.append("\n### 64-bit pointer references (pointer tables / lea imm64) ###\n");
        Map<Long, String> funcs = new LinkedHashMap<>();
        for (MemoryBlock b : mem.getBlocks()) {
            if (!b.isInitialized()) continue;
            long size = b.getSize();
            if (size <= 0 || size > 300_000_000L) continue;
            byte[] arr = new byte[(int) size];
            try { b.getBytes(b.getStart(), arr); } catch (Exception e) { continue; }
            long base = b.getStart().getOffset();
            for (int i = 0; i + 8 <= arr.length; i++) {
                long v = 0;
                for (int k = 0; k < 8; k++) v |= ((long)(arr[i + k] & 0xff)) << (8 * k);
                String hitName = null;
                for (Map.Entry<String, Long> e : strAddr.entrySet())
                    if (e.getValue() == v) { hitName = e.getKey(); break; }
                if (hitName == null) continue;
                Address at = space.getAddress(base + i);
                Function f = fm.getFunctionContaining(at);
                idx.append(String.format("  ptr->%s @ %s  (block %s)  func=%s%n",
                    hitName, at, b.getName(),
                    f == null ? "-" : (f.getName() + " @ " + f.getEntryPoint())));
                if (f != null) funcs.putIfAbsent(f.getEntryPoint().getOffset(), "ptr->" + hitName);
                // show 96 bytes around the pointer to reveal an ordered table
                idx.append(hexDump(at, 32, 96));
            }
        }

        File out = new File(OUT_DIR, "btnkey2_natives.c");
        PrintWriter pw = new PrintWriter(new FileWriter(out));
        pw.println("// Functions that load a Btn_Key direction/numbered string pointer.");
        for (Map.Entry<Long, String> e : funcs.entrySet()) {
            Address a = space.getAddress(e.getKey());
            Function f = fm.getFunctionAt(a);
            if (f == null) continue;
            pw.println("// ======== " + f.getName() + " @ " + f.getEntryPoint()
                + "   (" + e.getValue() + ") ========");
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

        PrintWriter iw = new PrintWriter(new FileWriter(new File(OUT_DIR, "_btnkey2_index.txt")));
        iw.print(idx.toString());
        iw.close();
        println("BTNKEY2: done, " + funcs.size() + " funcs load a Btn_Key string pointer");
    }
}
