// Find candidate "status getter" virtuals: small functions that load [this+0x100]
// (the StatusInstance pointer inside a UATCharacterAttributeComponent) and then
// return an int32 read from [that+disp]. Report those whose entry address appears
// in a vtable-like slot in .rdata, and specifically flag the slot at vtable+0x3E8
// (the slot the aura power-compare code calls). Output -> decompiled/_status_getter_scan.txt
//@category Kakarot
import ghidra.app.script.GhidraScript;
import ghidra.program.model.address.Address;
import ghidra.program.model.listing.Function;
import ghidra.program.model.listing.FunctionManager;
import ghidra.program.model.listing.Instruction;
import ghidra.program.model.listing.InstructionIterator;
import ghidra.program.model.mem.Memory;
import ghidra.program.model.mem.MemoryBlock;
import ghidra.program.model.scalar.Scalar;
import java.io.File;
import java.io.FileWriter;
import java.io.PrintWriter;
import java.util.ArrayList;
import java.util.List;

public class scan_status_getter extends GhidraScript {
    static final String OUT_DIR =
        "D:\\code\\unreal\\dragon ball kakarot access\\code\\decompiled";

    Memory mem;
    long textStart, textEnd;

    boolean inText(long a) { return a >= textStart && a < textEnd; }

    long u64(byte[] b, int i) {
        long v = 0;
        for (int k = 0; k < 8; k++) v |= ((long)(b[i+k] & 0xff)) << (8*k);
        return v;
    }

    @Override
    public void run() throws Exception {
        new File(OUT_DIR).mkdirs();
        mem = currentProgram.getMemory();
        FunctionManager fm = currentProgram.getFunctionManager();
        MemoryBlock text = mem.getBlock(".text");
        textStart = text.getStart().getOffset();
        textEnd = text.getEnd().getOffset();

        // 1) collect candidate getter functions
        // pattern: MOV r64,[r?+0x100]  then within next few insns  MOV r32,[sameDest+disp] (disp<0x400)
        List<long[]> cands = new ArrayList<>(); // {funcAddr, disp}
        List<String> candDesc = new ArrayList<>();
        int fcount = 0;
        for (Function f : fm.getFunctions(true)) {
            fcount++;
            long fa = f.getEntryPoint().getOffset();
            // only scan reasonably small functions to limit noise
            long size = f.getBody().getNumAddresses();
            if (size > 0x120) continue;
            InstructionIterator ii = currentProgram.getListing().getInstructions(f.getBody(), true);
            Instruction[] win = new Instruction[0];
            List<Instruction> insns = new ArrayList<>();
            while (ii.hasNext()) insns.add(ii.next());
            for (int i = 0; i < insns.size(); i++) {
                Instruction ins = insns.get(i);
                if (!ins.getMnemonicString().equalsIgnoreCase("MOV")) continue;
                // want: dest = reg, src = [base + 0x100]
                String reg100 = movLoadReg(ins, 0x100);
                if (reg100 == null) continue;
                // look ahead up to 10 insns for MOV r32,[reg100 + disp]
                for (int j = i+1; j < insns.size() && j <= i+12; j++) {
                    Instruction n = insns.get(j);
                    if (!n.getMnemonicString().equalsIgnoreCase("MOV")) continue;
                    long disp = movLoadFromBase(n, reg100);
                    if (disp >= 0 && disp < 0x400) {
                        cands.add(new long[]{fa, disp});
                        candDesc.add(Long.toHexString(fa) + "  reads [SI+0x" + Long.toHexString(disp) + "]  (" + f.getName() + ")");
                        break;
                    }
                }
            }
        }

        // 2) cache .rdata / .data bytes
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

        PrintWriter pw = new PrintWriter(new FileWriter(new File(OUT_DIR, "_status_getter_scan.txt")));
        pw.println("// scanned " + fcount + " functions; " + cands.size() + " getter candidates");
        for (int c = 0; c < cands.size(); c++) {
            long fa = cands.get(c)[0];
            long disp = cands.get(c)[1];
            // find occurrences of fa in .rdata/.data and test vtable-likeness
            for (int bi = 0; bi < bl.size(); bi++) {
                byte[] arr = bl.get(bi);
                long base = bs.get(bi);
                for (int i = 0; i + 8 <= arr.length; i += 8) {
                    if (u64(arr, i) == fa) {
                        // count preceding & following .text pointers
                        int back = 0;
                        for (int k = i - 8; k >= 0 && back < 40; k -= 8) {
                            if (inText(u64(arr, k))) back++; else break;
                        }
                        int fwd = 0;
                        for (int k = i + 8; k + 8 <= arr.length && fwd < 10; k += 8) {
                            if (inText(u64(arr, k))) fwd++; else break;
                        }
                        if (back + fwd >= 12) {
                            long slotOff = (long)back * 8; // this slot's offset from first counted .text ptr (approx)
                            long pos = base + i;
                            long vbaseApprox = pos - slotOff;
                            long slotFromVbase = pos - vbaseApprox;
                            pw.println(candDesc.get(c)
                                + "  | vtable-slot @rdata 0x" + Long.toHexString(pos)
                                + "  approxVbase 0x" + Long.toHexString(vbaseApprox)
                                + "  slotOff~0x" + Long.toHexString(slotFromVbase)
                                + "  (back=" + back + " fwd=" + fwd + ")");
                        }
                    }
                }
            }
        }
        pw.close();
        println("SCAN: done, " + cands.size() + " candidates");
    }

    // if ins is  MOV destReg, qword ptr [base + dispWanted]  return destReg name; else null
    String movLoadReg(Instruction ins, long dispWanted) {
        try {
            if (ins.getNumOperands() < 2) return null;
            // operand0 = register dest, operand1 = dynamic [base+disp]
            Object[] o0 = ins.getOpObjects(0);
            Object[] o1 = ins.getOpObjects(1);
            if (o0.length != 1 || !(o0[0] instanceof ghidra.program.model.lang.Register)) return null;
            // src must be memory ref with scalar disp == dispWanted
            boolean hasBase = false, hasDisp = false;
            for (Object o : o1) {
                if (o instanceof ghidra.program.model.lang.Register) hasBase = true;
                if (o instanceof Scalar) { if (((Scalar)o).getUnsignedValue() == dispWanted) hasDisp = true; }
            }
            // dispWanted!=0 requires explicit scalar; for 0x100 it's explicit
            if (hasBase && hasDisp) {
                return ((ghidra.program.model.lang.Register)o0[0]).getName();
            }
        } catch (Exception e) {}
        return null;
    }

    // if ins is MOV r32,[baseRegName + disp] return disp else -1
    long movLoadFromBase(Instruction ins, String baseRegName) {
        try {
            if (ins.getNumOperands() < 2) return -1;
            Object[] o0 = ins.getOpObjects(0);
            Object[] o1 = ins.getOpObjects(1);
            if (o0.length != 1 || !(o0[0] instanceof ghidra.program.model.lang.Register)) return -1;
            boolean baseMatch = false; long disp = 0; boolean hasScalar = false;
            for (Object o : o1) {
                if (o instanceof ghidra.program.model.lang.Register) {
                    String rn = ((ghidra.program.model.lang.Register)o).getName();
                    // match 64-bit base register name (baseRegName is 64-bit from load)
                    if (rn.equalsIgnoreCase(baseRegName)) baseMatch = true;
                }
                if (o instanceof Scalar) { disp = ((Scalar)o).getUnsignedValue(); hasScalar = true; }
            }
            if (baseMatch) return hasScalar ? disp : 0;
        } catch (Exception e) {}
        return -1;
    }
}
