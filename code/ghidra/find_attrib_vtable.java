// Find the UATCharacterAttributeComponent(Base/subclass) vtable(s): a constructor writes
// its vtable to [RCX] (LEA r,[vtable]; MOV [RCX],r) AND initialises StatusInstance at
// [RCX+0x100]. Report each such vtable and the function pointer at vtable+0x3E8 (the
// getter the aura power-compare calls). Then decompile that getter.
// Output -> decompiled/_attrib_vtable.txt  and  decompiled/_attrib_getter.c
//@category Kakarot
import ghidra.app.script.GhidraScript;
import ghidra.app.decompiler.DecompInterface;
import ghidra.app.decompiler.DecompileResults;
import ghidra.program.model.address.Address;
import ghidra.program.model.listing.Function;
import ghidra.program.model.listing.FunctionManager;
import ghidra.program.model.listing.Instruction;
import ghidra.program.model.listing.InstructionIterator;
import ghidra.program.model.mem.Memory;
import ghidra.program.model.mem.MemoryBlock;
import ghidra.program.model.scalar.Scalar;
import ghidra.program.model.symbol.Reference;
import ghidra.util.task.ConsoleTaskMonitor;
import java.io.File;
import java.io.FileWriter;
import java.io.PrintWriter;
import java.util.ArrayList;
import java.util.LinkedHashSet;
import java.util.List;
import java.util.Set;

public class find_attrib_vtable extends GhidraScript {
    static final String OUT_DIR =
        "D:\\code\\unreal\\dragon ball kakarot access\\code\\decompiled";
    Memory mem;
    long rdataStart, rdataEnd, textStart, textEnd;
    boolean inRdata(long a){ return a>=rdataStart && a<rdataEnd; }
    boolean inText(long a){ return a>=textStart && a<textEnd; }
    long u64(byte[] b,int i){ long v=0; for(int k=0;k<8;k++) v|=((long)(b[i+k]&0xff))<<(8*k); return v; }

    @Override
    public void run() throws Exception {
        new File(OUT_DIR).mkdirs();
        mem = currentProgram.getMemory();
        FunctionManager fm = currentProgram.getFunctionManager();
        MemoryBlock rd = mem.getBlock(".rdata");
        MemoryBlock tx = mem.getBlock(".text");
        rdataStart = rd.getStart().getOffset(); rdataEnd = rd.getEnd().getOffset();
        textStart = tx.getStart().getOffset(); textEnd = tx.getEnd().getOffset();
        byte[] rdBytes = new byte[(int) rd.getSize()];
        rd.getBytes(rd.getStart(), rdBytes);

        Set<Long> vtables = new LinkedHashSet<>();
        // scan functions: find LEA reg,[rdata] ; MOV [RCX],reg ; and a reference to [RCX+0x100]
        for (Function f : fm.getFunctions(true)) {
            long size = f.getBody().getNumAddresses();
            if (size > 0x400) continue;
            List<Instruction> ins = new ArrayList<>();
            InstructionIterator ii = currentProgram.getListing().getInstructions(f.getBody(), true);
            while (ii.hasNext()) ins.add(ii.next());
            long vtCand = -1; boolean storedToRcx = false, touches100 = false;
            // track last LEA dest->rdata addr
            String leaReg = null; long leaAddr = -1;
            for (Instruction n : ins) {
                String m = n.getMnemonicString();
                if (m.equalsIgnoreCase("LEA")) {
                    try {
                        Object[] o0 = n.getOpObjects(0);
                        Address ref = null;
                        Reference[] rr = n.getReferencesFrom();
                        for (Reference r : rr) { if (inRdata(r.getToAddress().getOffset())) ref = r.getToAddress(); }
                        if (o0.length==1 && o0[0] instanceof ghidra.program.model.lang.Register && ref!=null) {
                            leaReg = ((ghidra.program.model.lang.Register)o0[0]).getName();
                            leaAddr = ref.getOffset();
                        }
                    } catch (Exception e) {}
                }
                if (m.equalsIgnoreCase("MOV")) {
                    try {
                        Object[] o0 = n.getOpObjects(0);
                        Object[] o1 = n.getOpObjects(1);
                        // MOV [RCX], leaReg  (store vtable to this)
                        boolean dstRcxNoDisp=false; boolean hasDisp=false;
                        for (Object o : o0) {
                            if (o instanceof ghidra.program.model.lang.Register &&
                                ((ghidra.program.model.lang.Register)o).getName().equalsIgnoreCase("RCX")) dstRcxNoDisp=true;
                            if (o instanceof Scalar) hasDisp=true;
                        }
                        if (dstRcxNoDisp && !hasDisp && o1.length==1 && o1[0] instanceof ghidra.program.model.lang.Register) {
                            String sr = ((ghidra.program.model.lang.Register)o1[0]).getName();
                            if (sr.equalsIgnoreCase(leaReg) && leaAddr!=-1) { vtCand = leaAddr; storedToRcx=true; }
                        }
                        // any operand referencing [RCX + 0x100]
                        for (Object[] ops : new Object[][]{o0,o1}) {
                            boolean rcx=false, d100=false;
                            for (Object o : ops) {
                                if (o instanceof ghidra.program.model.lang.Register &&
                                    ((ghidra.program.model.lang.Register)o).getName().equalsIgnoreCase("RCX")) rcx=true;
                                if (o instanceof Scalar && ((Scalar)o).getUnsignedValue()==0x100) d100=true;
                            }
                            if (rcx && d100) touches100=true;
                        }
                    } catch (Exception e) {}
                }
            }
            if (storedToRcx && touches100 && vtCand!=-1) vtables.add(vtCand);
        }

        PrintWriter pw = new PrintWriter(new FileWriter(new File(OUT_DIR, "_attrib_vtable.txt")));
        PrintWriter gc = new PrintWriter(new FileWriter(new File(OUT_DIR, "_attrib_getter.c")));
        DecompInterface decomp = new DecompInterface();
        decomp.openProgram(currentProgram);
        ConsoleTaskMonitor mon = new ConsoleTaskMonitor();
        Set<Long> gettersSeen = new LinkedHashSet<>();
        pw.println("// attribute-component vtable candidates (ctor writes vtable to [RCX] AND inits [RCX+0x100]): " + vtables.size());
        for (Long vb : vtables) {
            // read slot 0x3E8
            int rel = (int)(vb - rdataStart);
            if (rel < 0 || rel + 0x3E8 + 8 > rdBytes.length) { pw.println("vtable 0x"+Long.toHexString(vb)+"  (slot 0x3E8 out of range)"); continue; }
            long getter = u64(rdBytes, rel + 0x3E8);
            pw.println("vtable 0x"+Long.toHexString(vb)+"  slot0x3E8 -> 0x"+Long.toHexString(getter)+ (inText(getter)?"":"  (not text)"));
            if (inText(getter) && gettersSeen.add(getter)) {
                Function gf = fm.getFunctionAt(currentProgram.getAddressFactory().getDefaultAddressSpace().getAddress(getter));
                if (gf == null) { try { gf = createFunction(currentProgram.getAddressFactory().getDefaultAddressSpace().getAddress(getter), null); } catch (Exception e) {} }
                if (gf != null) {
                    String c;
                    try { DecompileResults r = decomp.decompileFunction(gf, 120, mon);
                        c = (r!=null && r.decompileCompleted()) ? r.getDecompiledFunction().getC() : "// <fail>"; }
                    catch (Exception e){ c = "// <err> "+e; }
                    gc.println("// ===== getter 0x"+Long.toHexString(getter)+" ("+gf.getName()+") =====");
                    gc.println(c); gc.println();
                }
            }
        }
        pw.close(); gc.close();
        println("ATTRVT: done vtables="+vtables.size()+" getters="+gettersSeen.size());
    }
}
