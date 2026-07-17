// For every vtable slot-0x3E8 target, decompile it and keep only those whose body
// references [this+0x100] (the StatusInstance member of an attribute component).
// Those are the attribute-component getters the aura power-compare calls. Dump their
// decompilation (with the forwarded StatusInstance-vtable slot) to one file.
// Output -> decompiled/_3e8_status.txt
//@category Kakarot
import ghidra.app.script.GhidraScript;
import ghidra.app.decompiler.DecompInterface;
import ghidra.app.decompiler.DecompileResults;
import ghidra.program.model.address.Address;
import ghidra.program.model.listing.Function;
import ghidra.program.model.listing.FunctionManager;
import ghidra.program.model.mem.Memory;
import ghidra.program.model.mem.MemoryBlock;
import ghidra.util.task.ConsoleTaskMonitor;
import java.io.File;
import java.io.FileWriter;
import java.io.PrintWriter;
import java.util.LinkedHashSet;
import java.util.Set;

public class scan_3e8_forward extends GhidraScript {
    static final String OUT_DIR =
        "D:\\code\\unreal\\dragon ball kakarot access\\code\\decompiled";
    static final long SLOT = 0x3E8;

    Memory mem;
    long textStart, textEnd;
    boolean inText(long a) { return a >= textStart && a < textEnd; }
    long u64(byte[] b, int i) { long v=0; for(int k=0;k<8;k++) v|=((long)(b[i+k]&0xff))<<(8*k); return v; }

    @Override
    public void run() throws Exception {
        new File(OUT_DIR).mkdirs();
        mem = currentProgram.getMemory();
        FunctionManager fm = currentProgram.getFunctionManager();
        MemoryBlock text = mem.getBlock(".text");
        textStart = text.getStart().getOffset();
        textEnd = text.getEnd().getOffset();

        Set<Long> targets = new LinkedHashSet<>();
        for (String bn : new String[]{".rdata", ".data"}) {
            MemoryBlock b = mem.getBlock(bn);
            if (b == null) continue;
            byte[] arr = new byte[(int) b.getSize()];
            b.getBytes(b.getStart(), arr);
            int i = 0;
            while (i + 8 <= arr.length) {
                boolean prevText = (i >= 8) && inText(u64(arr, i - 8));
                if (inText(u64(arr, i)) && !prevText) {
                    int start = i, j = i;
                    while (j + 8 <= arr.length && inText(u64(arr, j))) j += 8;
                    if (j - start > SLOT) targets.add(u64(arr, start + (int)SLOT));
                    i = j;
                } else i += 8;
            }
        }

        DecompInterface decomp = new DecompInterface();
        decomp.openProgram(currentProgram);
        ConsoleTaskMonitor monT = new ConsoleTaskMonitor();
        PrintWriter pw = new PrintWriter(new FileWriter(new File(OUT_DIR, "_3e8_status.txt")));
        int kept = 0;
        for (Long t : targets) {
            Address a = currentProgram.getAddressFactory().getDefaultAddressSpace().getAddress(t);
            Function f = fm.getFunctionAt(a);
            if (f == null) { try { f = createFunction(a, null); } catch (Exception e) {} }
            if (f == null) continue;
            if (f.getBody().getNumAddresses() > 0xB0) continue; // getters are tiny leaves
            String c;
            try {
                DecompileResults res = decomp.decompileFunction(f, 60, monT);
                c = (res != null && res.decompileCompleted()) ? res.getDecompiledFunction().getC() : "";
            } catch (Exception e) { c = ""; }
            // keep tiny funcs that return a literal 1 default and/or read a member -> level getter
            if (c.contains("return 1;") || c.contains("param_1 +")) {
                pw.println("// ================= slot0x3E8 target " + f.getName() + " @ " + a
                    + " size=0x" + Long.toHexString(f.getBody().getNumAddresses()) + " =================");
                pw.println(c);
                pw.println();
                kept++;
            }
        }
        pw.println("// kept " + kept + " of " + targets.size());
        pw.close();
        println("F3E8: done kept=" + kept + " total=" + targets.size());
    }
}
