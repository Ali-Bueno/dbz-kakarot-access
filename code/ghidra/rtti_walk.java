// Manual MSVC x64 RTTI walk for the target UE UI classes (Ghidra's RTTI analyzer
// recovered runtime classes but none of the game's UObject UI classes).
// For each class:
//   1) find its Type Descriptor via the mangled name string ".?AVU<Class>@@"
//   2) scan .rdata for Complete Object Locators whose pTypeDescriptor RVA == it
//   3) scan .rdata for the 64-bit pointer to each COL; vftable = that slot + 8
//   4) read vftable function pointers, decompile them -> decompiled/<Class>.c
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
import java.util.LinkedHashSet;
import java.util.List;
import java.util.Set;

public class rtti_walk extends GhidraScript {
    static final String OUT_DIR =
        "D:\\code\\unreal\\dragon ball kakarot access\\code\\decompiled";
    static final String[] TARGETS = {
        "UAT_UIXCmnPause", "UAT_UIGameoverBar", "UAT_UIGameover", "UAT_UIXCmnHeader",
        "UAT_UIStartTop", "UAT_UIStartTopList", "UAT_UIStartTopListZ",
        "UAT_UIMenuListBase00", "UAT_UIMenuListBase01", "UAT_UIMenuListBase03",
        "UAT_UITips", "UMenuManager", "URootMenu", "UBaseMenu",
    };

    long imageBase;
    byte[] rdata;
    long rdataStart;
    AddressSpace space;

    long u32(int i) {
        return ((long)(rdata[i] & 0xff)) | ((long)(rdata[i+1] & 0xff) << 8)
             | ((long)(rdata[i+2] & 0xff) << 16) | ((long)(rdata[i+3] & 0xff) << 24);
    }
    long u64(int i) {
        long lo = u32(i), hi = u32(i+4);
        return lo | (hi << 32);
    }

    @Override
    public void run() throws Exception {
        new File(OUT_DIR).mkdirs();
        imageBase = currentProgram.getImageBase().getOffset();
        space = currentProgram.getAddressFactory().getDefaultAddressSpace();
        Memory mem = currentProgram.getMemory();
        FunctionManager fm = currentProgram.getFunctionManager();
        ConsoleTaskMonitor mon = new ConsoleTaskMonitor();

        MemoryBlock rb = mem.getBlock(".rdata");
        rdataStart = rb.getStart().getOffset();
        int rlen = (int) rb.getSize();
        rdata = new byte[rlen];
        rb.getBytes(rb.getStart(), rdata);
        println("RTTIWALK: .rdata @ 0x" + Long.toHexString(rdataStart) + " size 0x" + Integer.toHexString(rlen));

        DecompInterface decomp = new DecompInterface();
        decomp.openProgram(currentProgram);
        StringBuilder index = new StringBuilder();

        for (String cls : TARGETS) {
            index.append("=== ").append(cls).append(" ===\n");
            // 1) type descriptor via mangled name string
            byte[] needle = (".?AV" + cls + "@@").getBytes("ISO-8859-1");
            Address nameAddr = findBytesAll(mem, needle);
            if (nameAddr == null) {
                index.append("  type-descriptor string NOT found\n");
                continue;
            }
            long typeDesc = nameAddr.getOffset() - 0x10;      // name is at TD+0x10 (x64)
            long typeDescRVA = typeDesc - imageBase;
            index.append("  typeDesc @ 0x").append(Long.toHexString(typeDesc))
                 .append("  RVA 0x").append(Long.toHexString(typeDescRVA)).append("\n");

            // 2) COLs: .rdata dword == typeDescRVA at offset TD field (+0x0C)
            List<Long> cols = new ArrayList<>();
            for (int i = 0; i + 4 <= rdata.length; i += 4) {
                if (u32(i) == typeDescRVA) {
                    long colBase = (rdataStart + i) - 0x0C;
                    // validate: col+0x14 (pSelf RVA) == colBase-imageBase, sig at col+0 == 1
                    int colOff = (int)(colBase - rdataStart);
                    if (colOff >= 0 && colOff + 0x18 <= rdata.length) {
                        long self = u32(colOff + 0x14);
                        long sig = u32(colOff + 0x00);
                        if (self == (colBase - imageBase) && sig == 1) cols.add(colBase);
                    }
                }
            }
            index.append("  COLs: ").append(cols.size()).append("\n");

            // 3) vftables: .rdata qword == colBase  -> vftable = slot+8
            Set<Address> funcs = new LinkedHashSet<>();
            List<Long> vftables = new ArrayList<>();
            for (long col : cols) {
                for (int i = 0; i + 8 <= rdata.length; i += 8) {
                    if (u64(i) == col) {
                        long vft = rdataStart + i + 8;
                        vftables.add(vft);
                        index.append("  vftable @ 0x").append(Long.toHexString(vft)).append("\n");
                        // 4) read function pointers
                        Address a = space.getAddress(vft);
                        for (int k = 0; k < 400; k++) {
                            long v;
                            try { v = mem.getLong(a); } catch (Exception e) { break; }
                            if (v == 0) break;
                            Address tgt;
                            try { tgt = space.getAddress(v); } catch (Exception e) { break; }
                            Function f = fm.getFunctionAt(tgt);
                            if (f == null) {
                                // try to create a function there
                                try { f = createFunction(tgt, null); } catch (Exception e) { f = null; }
                                if (f == null) break;
                            }
                            funcs.add(tgt);
                            a = a.add(8);
                        }
                    }
                }
            }
            index.append("  functions: ").append(funcs.size()).append("\n");
            if (funcs.isEmpty()) continue;

            File out = new File(OUT_DIR, cls + ".c");
            PrintWriter pw = new PrintWriter(new FileWriter(out));
            pw.println("// " + cls + " — vftable methods (manual MSVC RTTI walk)");
            pw.println("// Source: AT-Win64-Shipping.exe.unpacked.exe (Steamless) + Ghidra 12");
            for (long vft : vftables) pw.println("// vftable @ 0x" + Long.toHexString(vft));
            pw.println();
            int count = 0, slot = 0;
            for (Address fa : funcs) {
                Function f = fm.getFunctionAt(fa);
                if (f == null) continue;
                String c;
                try {
                    DecompileResults res = decomp.decompileFunction(f, 90, mon);
                    c = (res != null && res.decompileCompleted())
                        ? res.getDecompiledFunction().getC()
                        : "// <decompile failed> " + f.getName();
                } catch (Exception e) { c = "// <decompile error> " + e; }
                pw.println("// ---- vtable[" + slot + "]  " + f.getName() + " @ " + f.getEntryPoint() + " ----");
                pw.println(c);
                pw.println();
                index.append("    [").append(slot).append("] ").append(f.getEntryPoint())
                     .append("  ").append(f.getName()).append("\n");
                count++; slot++;
            }
            pw.close();
            index.append("  wrote ").append(out.getName()).append(" (").append(count).append(" funcs)\n");
        }
        PrintWriter iw = new PrintWriter(new FileWriter(new File(OUT_DIR, "_index.txt")));
        iw.print(index.toString());
        iw.close();
        println("RTTIWALK: done");
    }

    // find first occurrence of needle across initialized memory
    Address findBytesAll(Memory mem, byte[] needle) {
        Address found = find(mem.getMinAddress(), needle);
        return found;
    }
}
