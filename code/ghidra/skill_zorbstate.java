// Find who writes the skill-tree node (Zorb) widget state byte at +0x460, which
// drives the padlock icon. Scans .text for `mov byte ptr [reg+0x460], imm8`
// (C6 8x 60 04 00 00 ii) and for `mov [reg+0x460], r8b/dl/...` (88 8x 60 04 00 00),
// then reports the containing function so it can be decompiled.
//@category Kakarot
import ghidra.app.script.GhidraScript;
import ghidra.program.model.address.Address;
import ghidra.program.model.listing.Function;
import ghidra.program.model.listing.FunctionManager;
import ghidra.program.model.mem.Memory;
import java.io.File;
import java.io.FileWriter;
import java.io.PrintWriter;
import java.util.LinkedHashSet;
import java.util.Set;

public class skill_zorbstate extends GhidraScript {
    static final String OUT_DIR =
        "D:\\code\\unreal\\dragon ball kakarot access\\code\\decompiled";

    @Override
    public void run() throws Exception {
        Memory mem = currentProgram.getMemory();
        FunctionManager fm = currentProgram.getFunctionManager();
        PrintWriter pw = new PrintWriter(new FileWriter(new File(OUT_DIR, "_skill_zorbstate.txt")));
        Set<String> seen = new LinkedHashSet<>();

        // opcode C6 = mov r/m8, imm8 ; 88 = mov r/m8, r8
        for (int opc : new int[]{0xC6, 0x88}) {
            for (int modrm = 0x80; modrm <= 0xBF; modrm++) {
                if ((modrm & 0x07) == 0x04) continue; // SIB form, skip
                byte[] needle = { (byte) opc, (byte) modrm, 0x60, 0x04, 0x00, 0x00 };
                Address from = mem.getMinAddress();
                while (true) {
                    Address hit = find(from, needle);
                    if (hit == null) break;
                    from = hit.add(1);
                    Function f = fm.getFunctionContaining(hit);
                    if (f == null) continue;
                    byte imm = 0;
                    if (opc == 0xC6) { try { imm = mem.getByte(hit.add(6)); } catch (Exception e) {} }
                    String line = String.format("%s  in %s  (%s)  %s",
                        hit, f.getEntryPoint(),
                        opc == 0xC6 ? ("imm=" + (imm & 0xff)) : "reg",
                        String.format("modrm=%02x", modrm));
                    if (seen.add(line)) pw.println(line);
                }
            }
        }
        pw.close();
        println("ZORBSTATE: done, " + seen.size() + " writes to +0x460");
    }
}
