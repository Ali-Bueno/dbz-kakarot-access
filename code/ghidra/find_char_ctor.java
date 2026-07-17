// Find the function(s) that store a qword to BOTH [reg+0x8E0] (StatusCmp) and
// [reg+0x8E8] (AttributeComponent) -> AAT_Character component setup. Decompile them.
// Output -> decompiled/_char_ctor.txt
//@category Kakarot
import ghidra.app.script.GhidraScript;
import ghidra.app.decompiler.DecompInterface;
import ghidra.app.decompiler.DecompileResults;
import ghidra.program.model.listing.Function;
import ghidra.program.model.listing.FunctionManager;
import ghidra.program.model.listing.Instruction;
import ghidra.program.model.listing.InstructionIterator;
import ghidra.program.model.scalar.Scalar;
import ghidra.util.task.ConsoleTaskMonitor;
import java.io.File;
import java.io.FileWriter;
import java.io.PrintWriter;

public class find_char_ctor extends GhidraScript {
    static final String OUT_DIR =
        "D:\\code\\unreal\\dragon ball kakarot access\\code\\decompiled";

    boolean storeQword(Instruction n, long disp) {
        try {
            if (!n.getMnemonicString().equalsIgnoreCase("MOV")) return false;
            // dest memory [reg+disp], source register (qword) -> operand sizes: check it's 8-byte via reg name r*/R*
            Object[] o0 = n.getOpObjects(0);
            Object[] o1 = n.getOpObjects(1);
            if (o1.length!=1 || !(o1[0] instanceof ghidra.program.model.lang.Register)) return false;
            String sr = ((ghidra.program.model.lang.Register)o1[0]).getName();
            if (!(sr.startsWith("R") && sr.length()<=3)) { /* allow R8..R15 too */ }
            boolean base=false, d=false;
            for (Object o : o0) {
                if (o instanceof ghidra.program.model.lang.Register) base=true;
                if (o instanceof Scalar && ((Scalar)o).getUnsignedValue()==disp) d=true;
            }
            return base && d;
        } catch (Exception e) { return false; }
    }

    @Override
    public void run() throws Exception {
        new File(OUT_DIR).mkdirs();
        FunctionManager fm = currentProgram.getFunctionManager();
        DecompInterface decomp = new DecompInterface();
        decomp.openProgram(currentProgram);
        ConsoleTaskMonitor mon = new ConsoleTaskMonitor();
        PrintWriter pw = new PrintWriter(new FileWriter(new File(OUT_DIR, "_char_ctor.txt")));
        int hits=0;
        for (Function f : fm.getFunctions(true)) {
            long size = f.getBody().getNumAddresses();
            if (size < 0x40 || size > 0x2000) continue;
            boolean s8e0=false, s8e8=false;
            InstructionIterator ii = currentProgram.getListing().getInstructions(f.getBody(), true);
            while (ii.hasNext()) {
                Instruction n = ii.next();
                if (storeQword(n, 0x8E0)) s8e0=true;
                if (storeQword(n, 0x8E8)) s8e8=true;
            }
            if (s8e0 && s8e8) {
                hits++;
                pw.println("// ===== char-ctor candidate " + f.getName() + " @ " + f.getEntryPoint() + " =====");
                String c;
                try { DecompileResults r = decomp.decompileFunction(f, 180, mon);
                    c = (r!=null && r.decompileCompleted()) ? r.getDecompiledFunction().getC() : "// <fail>"; }
                catch (Exception e){ c = "// <err>"; }
                pw.println(c); pw.println();
            }
        }
        pw.println("// hits="+hits);
        pw.close();
        println("CHARCTOR: done hits="+hits);
    }
}
