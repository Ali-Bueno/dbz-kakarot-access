// The pending/live key assignment is the global TMap<FName,FName> at
// 0x14569C3B0 (returned by FUN_141a663e0, consumed by FUN_1419e48a0 to fill
// UATSaveSystem+0x720 on "save changes"). Find every code reference to that
// global and its immediate neighbours, so we get the writers (per-key edit),
// the reloaders (cancel/entry) and the readers (icon resolver).
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
import ghidra.program.model.listing.Listing;
import ghidra.program.model.mem.Memory;
import ghidra.program.model.mem.MemoryBlock;
import ghidra.program.model.scalar.Scalar;
import ghidra.util.task.ConsoleTaskMonitor;
import java.io.File;
import java.io.FileWriter;
import java.io.PrintWriter;
import java.util.LinkedHashMap;
import java.util.LinkedHashSet;
import java.util.Map;
import java.util.Set;

public class inputassign_global extends GhidraScript {
    static final String OUT_DIR =
        "D:\\code\\unreal\\dragon ball kakarot access\\code\\decompiled";

    static final long LO = 0x14569c3a0L, HI = 0x14569c400L;  // the map + a little slack
    static final int DECOMP_CAP = 26;

    @Override
    public void run() throws Exception {
        new File(OUT_DIR).mkdirs();
        Memory mem = currentProgram.getMemory();
        Listing lst = currentProgram.getListing();
        FunctionManager fm = currentProgram.getFunctionManager();
        AddressSpace space = currentProgram.getAddressFactory().getDefaultAddressSpace();
        ConsoleTaskMonitor mon = new ConsoleTaskMonitor();
        DecompInterface decomp = new DecompInterface(); decomp.openProgram(currentProgram);

        Map<Long, Set<String>> byFunc = new LinkedHashMap<>();
        PrintWriter iw = new PrintWriter(new FileWriter(new File(OUT_DIR, "_inputassign_global.txt")));

        long n = 0;
        for (MemoryBlock b : mem.getBlocks()) {
            if (!b.isExecute()) continue;
            InstructionIterator it = lst.getInstructions(b.getStart(), true);
            while (it.hasNext()) {
                Instruction ins = it.next();
                if (ins.getAddress().compareTo(b.getEnd()) > 0) break;
                if ((++n % 4000000) == 0) println("  ..." + n);
                boolean hit = false;
                // RIP-relative operands resolve to an address reference
                for (ghidra.program.model.symbol.Reference r : ins.getReferencesFrom()) {
                    long t = r.getToAddress().getOffset();
                    if (t >= LO && t < HI) { hit = true; break; }
                }
                if (!hit) {
                    for (int oi = 0; oi < ins.getNumOperands() && !hit; oi++)
                        for (Object o : ins.getOpObjects(oi))
                            if (o instanceof Scalar) {
                                long v = ((Scalar) o).getUnsignedValue();
                                if (v >= LO && v < HI) { hit = true; break; }
                            }
                }
                if (!hit) continue;
                Function f = fm.getFunctionContaining(ins.getAddress());
                long key = f == null ? 0 : f.getEntryPoint().getOffset();
                byFunc.computeIfAbsent(key, k -> new LinkedHashSet<>())
                      .add(ins.getAddress() + "  " + ins);
            }
        }
        println("GLOBAL scan done, " + byFunc.size() + " referencing functions");

        for (Map.Entry<Long, Set<String>> e : byFunc.entrySet()) {
            Function f = e.getKey() == 0 ? null : fm.getFunctionAt(space.getAddress(e.getKey()));
            iw.printf("FUNC 0x%x  %s  size=0x%s%n", e.getKey(), f == null ? "<none>" : f.getName(),
                f == null ? "?" : Long.toHexString(f.getBody().getNumAddresses()));
            for (String s : e.getValue()) iw.println("    " + s);
            if (f != null) {
                StringBuilder cs = new StringBuilder();
                for (Function c : f.getCallingFunctions(mon)) {
                    cs.append(c.getName()).append("@").append(c.getEntryPoint()).append(" ");
                    if (cs.length() > 300) break;
                }
                iw.println("    callers: " + cs);
            }
        }
        iw.close();

        PrintWriter pw = new PrintWriter(new FileWriter(new File(OUT_DIR, "_inputassign_global.c")));
        int cap = 0;
        for (Map.Entry<Long, Set<String>> e : byFunc.entrySet()) {
            if (e.getKey() == 0 || cap++ >= DECOMP_CAP) continue;
            Function f = fm.getFunctionAt(space.getAddress(e.getKey()));
            if (f == null) continue;
            pw.println("// ============ " + f.getName() + " @ " + f.getEntryPoint()
                       + "  size=0x" + Long.toHexString(f.getBody().getNumAddresses()) + " ============");
            for (String s : e.getValue()) pw.println("//   " + s);
            try {
                DecompileResults res = decomp.decompileFunction(f, 180, mon);
                pw.println(res != null && res.decompileCompleted()
                    ? res.getDecompiledFunction().getC() : "// <decompile failed>");
            } catch (Exception ex) { pw.println("// <err> " + ex); }
            pw.flush();
        }
        pw.close();
        println("WROTE _inputassign_global.{txt,c}");
    }
}
