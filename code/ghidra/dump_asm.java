// Dump raw instructions (with call reference targets) of given function addresses.
// Usage: -postScript "dump_asm.java 0x141823c00,0x141823430"
// Writes decompiled/asm_<addr>.txt each.
//@category Kakarot
import ghidra.app.script.GhidraScript;
import ghidra.program.model.address.Address;
import ghidra.program.model.listing.Function;
import ghidra.program.model.listing.FunctionManager;
import ghidra.program.model.listing.Instruction;
import ghidra.program.model.listing.InstructionIterator;
import ghidra.program.model.symbol.Reference;
import java.io.File;
import java.io.FileWriter;
import java.io.PrintWriter;

public class dump_asm extends GhidraScript {
    static final String OUT_DIR =
        "D:\\code\\unreal\\dragon ball kakarot access\\code\\decompiled";

    @Override
    public void run() throws Exception {
        new File(OUT_DIR).mkdirs();
        String[] rawArgs = getScriptArgs();
        if (rawArgs.length == 0) { println("DUMPASM: no addresses"); return; }
        String all = String.join(",", rawArgs);
        FunctionManager fm = currentProgram.getFunctionManager();
        for (String tok : all.split(",")) {
            tok = tok.trim();
            if (tok.isEmpty()) continue;
            long off = Long.parseLong(tok.replaceFirst("^0x", ""), 16);
            Address a = currentProgram.getAddressFactory().getDefaultAddressSpace().getAddress(off);
            Function f = fm.getFunctionAt(a);
            if (f == null) { try { f = createFunction(a, null); } catch (Exception e) {} }
            File out = new File(OUT_DIR, "asm_" + tok.replaceFirst("^0x", "") + ".txt");
            PrintWriter pw = new PrintWriter(new FileWriter(out));
            if (f == null) { pw.println("// no function at " + tok); pw.close(); continue; }
            pw.println("// " + f.getName() + " @ " + f.getEntryPoint());
            InstructionIterator ii = currentProgram.getListing().getInstructions(f.getBody(), true);
            while (ii.hasNext()) {
                Instruction ins = ii.next();
                StringBuilder sb = new StringBuilder();
                sb.append(ins.getAddress()).append("  ").append(ins.toString());
                Reference[] refs = ins.getReferencesFrom();
                for (Reference r : refs) {
                    Address t = r.getToAddress();
                    Function g = fm.getFunctionAt(t);
                    sb.append("   -> ").append(t);
                    if (g != null) sb.append(" [").append(g.getName()).append("]");
                }
                pw.println(sb.toString());
            }
            pw.close();
            println("DUMPASM: wrote " + out.getName());
        }
        println("DUMPASM: done");
    }
}
