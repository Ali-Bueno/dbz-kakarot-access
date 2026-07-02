// Dig for the StartTop ring-selection and pause-selection offsets.
//  A) decompile every CALLER of SetFontType impl (FUN_1416f7bd0) — one of them is
//     the StartTop "section changed" handler, which reads the ring index member.
//  B) resolve StartTop nav UFunctions (In_Curs/Out_Curs/...) via the native-reg
//     table and decompile their impls (they read/write the cursor index).
// Output: decompiled/dig_callers_<addr>.c and dig_ufunc_<name>.c
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
import ghidra.program.model.symbol.ReferenceIterator;
import ghidra.program.model.symbol.ReferenceManager;
import ghidra.util.task.ConsoleTaskMonitor;
import java.io.File;
import java.io.FileWriter;
import java.io.PrintWriter;
import java.util.ArrayList;
import java.util.LinkedHashSet;
import java.util.List;
import java.util.Set;

public class dig2 extends GhidraScript {
    static final String OUT_DIR =
        "D:\\code\\unreal\\dragon ball kakarot access\\code\\decompiled";
    static final long[] CALLER_TARGETS = { 0x1416f7bd0L };
    static final String[] NAMES = {
        "In_Curs", "Sys_In_Curs", "Com_In_Curs", "Out_Curs", "Sys_Out_Curs",
        "CheckHovered", "OnEnterAnimEnd", "MouseClickDecide",
    };

    Memory mem; FunctionManager fm; AddressSpace space; DecompInterface decomp; ConsoleTaskMonitor mon;
    byte[][] blocks; long[] blockStart;

    long u64(byte[] b, int i) { long v = 0; for (int k=0;k<8;k++) v |= ((long)(b[i+k]&0xff))<<(8*k); return v; }

    String dec(Function f) {
        try { DecompileResults r = decomp.decompileFunction(f, 150, mon);
            return (r!=null && r.decompileCompleted()) ? r.getDecompiledFunction().getC() : "// <failed>";
        } catch (Exception e) { return "// <error> "+e; }
    }

    @Override
    public void run() throws Exception {
        new File(OUT_DIR).mkdirs();
        mem = currentProgram.getMemory(); fm = currentProgram.getFunctionManager();
        space = currentProgram.getAddressFactory().getDefaultAddressSpace();
        mon = new ConsoleTaskMonitor(); decomp = new DecompInterface(); decomp.openProgram(currentProgram);
        ReferenceManager rm = currentProgram.getReferenceManager();

        // cache .rdata + .data
        List<byte[]> bl = new ArrayList<>(); List<Long> bs = new ArrayList<>();
        for (String bn : new String[]{".rdata", ".data"}) {
            MemoryBlock b = mem.getBlock(bn); if (b==null) continue;
            byte[] arr = new byte[(int)b.getSize()]; b.getBytes(b.getStart(), arr);
            bl.add(arr); bs.add(b.getStart().getOffset());
        }
        blocks = bl.toArray(new byte[0][]); blockStart = new long[bs.size()];
        for (int i=0;i<bs.size();i++) blockStart[i]=bs.get(i);

        StringBuilder idx = new StringBuilder();

        // A) callers of each target
        for (long t : CALLER_TARGETS) {
            Address ta = space.getAddress(t);
            Set<Address> callers = new LinkedHashSet<>();
            ReferenceIterator ri = rm.getReferencesTo(ta);
            while (ri.hasNext()) {
                Reference r = ri.next();
                Function f = fm.getFunctionContaining(r.getFromAddress());
                if (f != null) callers.add(f.getEntryPoint());
            }
            idx.append("CALLERS of 0x").append(Long.toHexString(t)).append(": ").append(callers.size()).append("\n");
            File out = new File(OUT_DIR, "dig_callers_" + Long.toHexString(t) + ".c");
            PrintWriter pw = new PrintWriter(new FileWriter(out));
            pw.println("// Callers of 0x"+Long.toHexString(t)+" (SetFontType impl)");
            for (Address ca : callers) {
                Function f = fm.getFunctionAt(ca); if (f==null) continue;
                pw.println("// ---- "+f.getName()+" @ "+f.getEntryPoint()+" ----");
                pw.println(dec(f)); pw.println();
                idx.append("   ").append(f.getEntryPoint()).append("\n");
            }
            pw.close();
        }

        // B) StartTop nav UFunctions via native-reg table
        for (String name : NAMES) {
            byte[] needle = (name + "\0").getBytes("ISO-8859-1");
            List<Address> strs = new ArrayList<>();
            Address from = mem.getMinAddress();
            while (true) { Address hit = find(from, needle); if (hit==null) break; strs.add(hit); from = hit.add(1); if (strs.size()>60) break; }
            Set<Address> impls = new LinkedHashSet<>();
            for (Address sa : strs) {
                long target = sa.getOffset();
                for (int bi=0; bi<blocks.length; bi++) {
                    byte[] arr = blocks[bi];
                    for (int i=0; i+16<=arr.length; i+=8) {
                        if (u64(arr,i)==target) {
                            long exec = u64(arr, i+8);
                            Address ea; try { ea = space.getAddress(exec); } catch (Exception e){ continue; }
                            Function ef = fm.getFunctionAt(ea);
                            if (ef==null) { try { ef = createFunction(ea,null);} catch(Exception e){} }
                            if (ef!=null) { impls.add(ea); Address imp = lastCall(ef); if (imp!=null) impls.add(imp); }
                        }
                    }
                }
            }
            idx.append("UFUNC ").append(name).append(": strs=").append(strs.size()).append(" impls=").append(impls.size()).append("\n");
            if (impls.isEmpty()) continue;
            File out = new File(OUT_DIR, "dig_ufunc_" + name + ".c");
            PrintWriter pw = new PrintWriter(new FileWriter(out));
            for (Address a : impls) {
                Function f = fm.getFunctionAt(a); if (f==null) continue;
                pw.println("// ---- "+f.getName()+" @ "+f.getEntryPoint()+" ----");
                pw.println(dec(f)); pw.println();
                idx.append("   impl ").append(a).append("\n");
            }
            pw.close();
        }

        PrintWriter iw = new PrintWriter(new FileWriter(new File(OUT_DIR, "_dig2_index.txt")));
        iw.print(idx.toString()); iw.close();
        println("DIG2: done");
    }

    Address lastCall(Function f) {
        Address last = null;
        try {
            InstructionIterator ii = currentProgram.getListing().getInstructions(f.getBody(), true);
            while (ii.hasNext()) {
                Instruction ins = ii.next();
                if (ins.getMnemonicString().toLowerCase().startsWith("call")) {
                    for (Reference r : ins.getReferencesFrom()) {
                        Function g = fm.getFunctionAt(r.getToAddress());
                        if (g != null && !g.equals(f)) last = r.getToAddress();
                    }
                }
            }
        } catch (Exception e) {}
        return last;
    }
}
