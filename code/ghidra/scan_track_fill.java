// Find the FEnemyTrackInfo fill: a function that writes, to the SAME base register B,
// a dword at [B+0x8] (int Lv) and MOVSS floats at [B+0xC] (HpMax) and [B+0x10] (HpNow).
// That function reads the live enemy level/HP. Report + decompile candidates.
// Output -> decompiled/_track_fill.txt
//@category Kakarot
import ghidra.app.script.GhidraScript;
import ghidra.app.decompiler.DecompInterface;
import ghidra.app.decompiler.DecompileResults;
import ghidra.program.model.address.Address;
import ghidra.program.model.listing.Function;
import ghidra.program.model.listing.FunctionManager;
import ghidra.program.model.listing.Instruction;
import ghidra.program.model.listing.InstructionIterator;
import ghidra.program.model.scalar.Scalar;
import ghidra.util.task.ConsoleTaskMonitor;
import java.io.File;
import java.io.FileWriter;
import java.io.PrintWriter;
import java.util.ArrayList;
import java.util.List;

public class scan_track_fill extends GhidraScript {
    static final String OUT_DIR =
        "D:\\code\\unreal\\dragon ball kakarot access\\code\\decompiled";

    // returns [baseRegName-hash unused]; we just detect write to [base+disp] and return base reg name or null
    String writeBase(Instruction n, long disp, boolean movss) {
        try {
            String m = n.getMnemonicString();
            if (movss && !m.equalsIgnoreCase("MOVSS")) return null;
            if (!movss && !m.equalsIgnoreCase("MOV")) return null;
            Object[] o0 = n.getOpObjects(0); // dest memory
            // dest must be [reg + disp]
            String base=null; boolean d=false;
            for (Object o : o0) {
                if (o instanceof ghidra.program.model.lang.Register) base = ((ghidra.program.model.lang.Register)o).getName();
                if (o instanceof Scalar && ((Scalar)o).getUnsignedValue()==disp) d=true;
            }
            // ensure operand0 is memory (has a scalar disp or is dynamic) and operand1 is the source
            if (base!=null && d) {
                // crude: make sure it's a store (op0 is memory). getOpType check:
                if (n.getOperandType(0) != 0 && (n.getOperandType(0) & 0x8/*dynamic/scalar addr*/) != 0) return base;
                return base; // fallback accept
            }
        } catch (Exception e) {}
        return null;
    }

    @Override
    public void run() throws Exception {
        new File(OUT_DIR).mkdirs();
        FunctionManager fm = currentProgram.getFunctionManager();
        DecompInterface decomp = new DecompInterface();
        decomp.openProgram(currentProgram);
        ConsoleTaskMonitor mon = new ConsoleTaskMonitor();
        PrintWriter pw = new PrintWriter(new FileWriter(new File(OUT_DIR, "_track_fill.txt")));
        int hits = 0;
        for (Function f : fm.getFunctions(true)) {
            long size = f.getBody().getNumAddresses();
            if (size < 0x20 || size > 0x600) continue;
            List<Instruction> ins = new ArrayList<>();
            InstructionIterator ii = currentProgram.getListing().getInstructions(f.getBody(), true);
            while (ii.hasNext()) ins.add(ii.next());
            // collect base regs with each write kind
            java.util.Set<String> lv = new java.util.HashSet<>();
            java.util.Set<String> hpMax = new java.util.HashSet<>();
            java.util.Set<String> hpNow = new java.util.HashSet<>();
            for (Instruction n : ins) {
                String b;
                b = writeBase(n, 0x8, false);  if (b!=null) lv.add(b);
                b = writeBase(n, 0xC, true);   if (b!=null) hpMax.add(b);
                b = writeBase(n, 0x10, true);  if (b!=null) hpNow.add(b);
            }
            // require same base register present in all three sets
            for (String b : lv) {
                if (hpMax.contains(b) && hpNow.contains(b)) {
                    hits++;
                    pw.println("// ===== FEnemyTrackInfo fill candidate " + f.getName() + " @ " + f.getEntryPoint()
                        + "  base=" + b + " =====");
                    String c;
                    try { DecompileResults r = decomp.decompileFunction(f, 120, mon);
                        c = (r!=null && r.decompileCompleted()) ? r.getDecompiledFunction().getC() : "// <fail>"; }
                    catch (Exception e){ c = "// <err>"; }
                    pw.println(c); pw.println();
                    break;
                }
            }
        }
        pw.println("// hits=" + hits);
        pw.close();
        println("TRACKFILL: done hits=" + hits);
    }
}
