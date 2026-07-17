// Find UE UFunction native implementations WITHOUT RTTI, via the native-function
// registration table. UE4 registers each UFunction as { const char* Name,
// FNativeFuncPtr exec } 16-byte pairs. So:
//   1) find the ASCII name string "<Name>\0"
//   2) scan .rdata/.data for an 8-byte pointer == that string  (the table's Name field)
//   3) exec thunk = qword at (that location + 8)
//   4) decompile the exec thunk AND the first game function it calls (the real impl)
// -> decompiled/_ufunc_<Name>.c
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
import java.util.LinkedHashSet;
import java.util.List;
import java.util.Set;

public class find_ufunc_level extends GhidraScript {
    static final String OUT_DIR =
        "D:\\code\\unreal\\dragon ball kakarot access\\code\\decompiled";
    static final String[] NAMES = {
        "GetPowerCompareRank",
    };

    Memory mem;
    FunctionManager fm;
    AddressSpace space;
    DecompInterface decomp;
    ConsoleTaskMonitor mon;
    byte[][] blocks;
    long[] blockStart;

    long u64(byte[] b, int i) {
        long v = 0;
        for (int k = 0; k < 8; k++) v |= ((long)(b[i+k] & 0xff)) << (8*k);
        return v;
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

        // cache .rdata + .data bytes for pointer scans
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
        blocks = bl.toArray(new byte[0][]);
        blockStart = new long[bs.size()];
        for (int i = 0; i < bs.size(); i++) blockStart[i] = bs.get(i);

        StringBuilder idx = new StringBuilder();
        for (String name : NAMES) {
            idx.append("=== ").append(name).append(" ===\n");
            Set<Address> toDecompile = new LinkedHashSet<>();

            // 1) find every ASCII "<name>\0" occurrence
            byte[] needle = (name + "\0").getBytes("ISO-8859-1");
            List<Address> strAddrs = new ArrayList<>();
            Address from = mem.getMinAddress();
            while (true) {
                Address hit = find(from, needle);
                if (hit == null) break;
                strAddrs.add(hit);
                from = hit.add(1);
                if (strAddrs.size() > 40) break;
            }
            idx.append("  name-string occurrences: ").append(strAddrs.size()).append("\n");

            // 2) for each string, scan blocks for a pointer == strAddr; exec = +8
            for (Address sa : strAddrs) {
                long target = sa.getOffset();
                for (int bi = 0; bi < blocks.length; bi++) {
                    byte[] arr = blocks[bi];
                    for (int i = 0; i + 16 <= arr.length; i += 8) {
                        if (u64(arr, i) == target) {
                            long exec = u64(arr, i + 8);
                            Address execAddr;
                            try { execAddr = space.getAddress(exec); } catch (Exception e) { continue; }
                            Function f = fm.getFunctionAt(execAddr);
                            if (f == null) {
                                try { f = createFunction(execAddr, null); } catch (Exception e) { f = null; }
                            }
                            if (f != null) {
                                toDecompile.add(execAddr);
                                idx.append("  table @ 0x").append(Long.toHexString(blockStart[bi] + i))
                                   .append("  exec -> 0x").append(Long.toHexString(exec)).append("\n");
                                // also add first game-code CALL target inside exec (the real impl)
                                Address impl = firstCallTarget(f);
                                if (impl != null) { toDecompile.add(impl);
                                    idx.append("     impl -> 0x").append(Long.toHexString(impl.getOffset())).append("\n"); }
                            }
                        }
                    }
                }
            }

            if (toDecompile.isEmpty()) { idx.append("  (no exec thunk found)\n"); continue; }
            File out = new File(OUT_DIR, "_ufunc_" + name + ".c");
            PrintWriter pw = new PrintWriter(new FileWriter(out));
            pw.println("// UFunction '" + name + "' — exec thunk(s) + native impl (via UE native-registration table)");
            for (Address a : toDecompile) {
                Function f = fm.getFunctionAt(a);
                if (f == null) continue;
                String c;
                try {
                    DecompileResults res = decomp.decompileFunction(f, 120, mon);
                    c = (res != null && res.decompileCompleted())
                        ? res.getDecompiledFunction().getC() : "// <decompile failed>";
                } catch (Exception e) { c = "// <decompile error> " + e; }
                pw.println("// ---- " + f.getName() + " @ " + f.getEntryPoint() + " ----");
                pw.println(c);
                pw.println();
            }
            pw.close();
            idx.append("  wrote _ufunc_").append(name).append(".c (").append(toDecompile.size()).append(" funcs)\n");
        }
        PrintWriter iw = new PrintWriter(new FileWriter(new File(OUT_DIR, "_ufunc_index.txt")));
        iw.print(idx.toString());
        iw.close();
        println("FINDUFUNC: done");
    }

    // first CALL target that lands in .text, from the exec thunk
    Address firstCallTarget(Function f) {
        try {
            InstructionIterator ii = currentProgram.getListing().getInstructions(f.getBody(), true);
            while (ii.hasNext()) {
                Instruction ins = ii.next();
                if (ins.getMnemonicString().toLowerCase().startsWith("call")) {
                    Reference[] refs = ins.getReferencesFrom();
                    for (Reference r : refs) {
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
