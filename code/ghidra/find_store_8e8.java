// Find instructions that STORE into [reg+0x8E8] (writing AAT_Character.AttributeComponent),
// and instructions that READ [reg+0x8E8] then CALL [+0x3E8] (the getter callers).
// Report the containing functions so we can decompile them and identify the concrete
// attribute-component class + its level getter.
// Output -> decompiled/_store_8e8.txt
//@category Kakarot
import ghidra.app.script.GhidraScript;
import ghidra.program.model.address.Address;
import ghidra.program.model.listing.Function;
import ghidra.program.model.listing.FunctionManager;
import ghidra.program.model.listing.Instruction;
import ghidra.program.model.listing.InstructionIterator;
import ghidra.program.model.mem.MemoryBlock;
import ghidra.program.model.scalar.Scalar;
import java.io.File;
import java.io.FileWriter;
import java.io.PrintWriter;
import java.util.LinkedHashSet;
import java.util.Set;

public class find_store_8e8 extends GhidraScript {
    static final String OUT_DIR =
        "D:\\code\\unreal\\dragon ball kakarot access\\code\\decompiled";

    @Override
    public void run() throws Exception {
        new File(OUT_DIR).mkdirs();
        FunctionManager fm = currentProgram.getFunctionManager();
        MemoryBlock text = currentProgram.getMemory().getBlock(".text");
        InstructionIterator ii = currentProgram.getListing().getInstructions(text.getStart(), true);
        Set<String> stores = new LinkedHashSet<>();
        while (ii.hasNext()) {
            Instruction ins = ii.next();
            String m = ins.getMnemonicString();
            if (!m.equalsIgnoreCase("MOV")) continue;
            // operand0 must be memory [base + 0x8E8], operand1 a register (the stored ptr)
            try {
                Object[] o0 = ins.getOpObjects(0);
                Object[] o1 = ins.getOpObjects(1);
                if (o1.length != 1 || !(o1[0] instanceof ghidra.program.model.lang.Register)) continue;
                boolean hasBase=false, has8e8=false;
                for (Object o : o0) {
                    if (o instanceof ghidra.program.model.lang.Register) hasBase = true;
                    if (o instanceof Scalar && ((Scalar)o).getUnsignedValue() == 0x8E8) has8e8 = true;
                }
                if (hasBase && has8e8) {
                    Function f = fm.getFunctionContaining(ins.getAddress());
                    stores.add((f==null?"?":f.getName()+" @ "+f.getEntryPoint()) + "   store-site " + ins.getAddress() + "   " + ins.toString());
                }
            } catch (Exception e) {}
        }
        PrintWriter pw = new PrintWriter(new FileWriter(new File(OUT_DIR, "_store_8e8.txt")));
        pw.println("// stores into [reg+0x8E8] (AttributeComponent):");
        for (String s : stores) pw.println(s);
        pw.close();
        println("STORE8E8: done stores=" + stores.size());
    }
}
