// Find UAT_UIXcmnAgreement's class-construct + native function table (2026-07-17).
// The 6 FPropertyParams live at 0x1454f05d8..0x1454f07c8. UE4.21 codegen lists them
// in a FPropertyParams* pointer array, referenced by Z_Construct_UClass_...().
//   (1) locate the pointer array (consecutive qwords == the param addrs)
//   (2) code xref to the array => Z_Construct_UClass (ZC); decompile ZC + 1-level callees
//   (3) the FClassFunctionLinkInfo array (adjacent) pairs {Z_Construct_UFunction*, name};
//       dump the .rdata window around the array resolving every name string, so we read
//       the agreement's REAL UFunction set.
//   (4) find the agreement's StaticRegisterNatives {name,exec} table: for the class name
//       ptr, dump neighbours. Also: for each generic name in the set, list the {name,exec}
//       entry whose exec-impl references 0x508 or 0x3a8 (the agreement's impls).
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
import ghidra.program.model.scalar.Scalar;
import ghidra.program.model.symbol.Reference;
import ghidra.program.model.symbol.ReferenceIterator;
import ghidra.util.task.ConsoleTaskMonitor;
import java.io.File;
import java.io.FileWriter;
import java.io.PrintWriter;
import java.util.ArrayList;
import java.util.LinkedHashMap;
import java.util.List;
import java.util.Map;

public class agree_loc3 extends GhidraScript {
    static final String OUT_DIR =
        "D:\\code\\unreal\\dragon ball kakarot access\\code\\decompiled";
    static final long[] PARAM_ADDRS = {0x1454f05d8L,0x1454f0648L,0x1454f0708L,0x1454f0748L,0x1454f0788L,0x1454f07c8L};

    Memory mem; FunctionManager fm; AddressSpace space; DecompInterface decomp; ConsoleTaskMonitor mon;
    List<byte[]> bl=new ArrayList<>(); List<Long> bs=new ArrayList<>();

    long u64(byte[] b,int i){ long v=0; for(int k=0;k<8;k++) v|=((long)(b[i+k]&0xff))<<(8*k); return v; }
    String asciiAt(long p){ try{ byte[] b=new byte[64]; mem.getBytes(space.getAddress(p),b);
        StringBuilder s=new StringBuilder(); for(byte c:b){ if(c==0)return s.length()>=1?s.toString():null; if(c<0x20||c>0x7e)return null; s.append((char)c);} }catch(Exception e){} return null; }
    boolean inText(long p){ try{ MemoryBlock b=mem.getBlock(space.getAddress(p)); return b!=null&&b.isExecute(); }catch(Exception e){return false;} }

    @Override public void run() throws Exception {
        new File(OUT_DIR).mkdirs();
        mem=currentProgram.getMemory(); fm=currentProgram.getFunctionManager();
        space=currentProgram.getAddressFactory().getDefaultAddressSpace();
        mon=new ConsoleTaskMonitor(); decomp=new DecompInterface(); decomp.openProgram(currentProgram);
        for(String bn:new String[]{".rdata",".data"}){ MemoryBlock b=mem.getBlock(bn); if(b==null)continue;
            byte[] a=new byte[(int)b.getSize()]; b.getBytes(b.getStart(),a); bl.add(a); bs.add(b.getStart().getOffset()); }
        StringBuilder idx=new StringBuilder();

        // (1) find the FPropertyParams* pointer array: a qword == any PARAM_ADDR
        List<Long> arrLocs=new ArrayList<>();
        for(int bi=0;bi<bl.size();bi++){ byte[] a=bl.get(bi); long base=bs.get(bi);
            for(int i=0;i+8<=a.length;i+=8){ long q=u64(a,i);
                for(long pa:PARAM_ADDRS){ if(q==pa){ arrLocs.add(base+i); break; } } } }
        idx.append("FPropertyParams* pointer slots:\n");
        long arrMin=Long.MAX_VALUE, arrMax=0;
        for(long l:arrLocs){ idx.append(String.format("  slot@0x%x -> ", l));
            // read the qword there
            byte[] one=new byte[8]; mem.getBytes(space.getAddress(l),one); long q=u64(one,0);
            idx.append(String.format("0x%x%n", q)); arrMin=Math.min(arrMin,l); arrMax=Math.max(arrMax,l); }

        // (2) xref to the array base -> Z_Construct_UClass
        idx.append("\nxrefs to array region [0x"+Long.toHexString(arrMin)+"..0x"+Long.toHexString(arrMax)+"]:\n");
        java.util.LinkedHashSet<Long> zc=new java.util.LinkedHashSet<>();
        for(long l=arrMin; l<=arrMax; l+=8){ ReferenceIterator ri=currentProgram.getReferenceManager().getReferencesTo(space.getAddress(l));
            while(ri.hasNext()){ Address fr=ri.next().getFromAddress(); Function fn=fm.getFunctionContaining(fr);
                idx.append(String.format("  ref@0x%x%s%n", fr.getOffset(), fn!=null?(" in 0x"+Long.toHexString(fn.getEntryPoint().getOffset())):""));
                if(fn!=null) zc.add(fn.getEntryPoint().getOffset()); } }

        // (3) dump .rdata window around the param pointer array to reveal the func-link array (name ptrs)
        idx.append("\n.rdata window around 0x"+Long.toHexString(arrMin-0x100)+":\n");
        for(long l=arrMin-0x100; l<=arrMax+0x180; l+=8){
            byte[] one=new byte[8]; try{ mem.getBytes(space.getAddress(l),one); }catch(Exception e){ continue; }
            long q=u64(one,0); String s=asciiAt(q);
            String tag = inText(q)?" <code>":"";
            idx.append(String.format("  0x%x: 0x%x%s%s%n", l, q, s!=null?(" \""+s+"\""):"", tag));
        }

        // (4) decompile ZC + its 1-level callees
        PrintWriter pw=new PrintWriter(new FileWriter(new File(OUT_DIR,"agree_zc.c")));
        java.util.LinkedHashSet<Long> toDec=new java.util.LinkedHashSet<>(zc);
        for(long z:zc){ Function f=fm.getFunctionAt(space.getAddress(z)); if(f==null)continue;
            InstructionIterator ii=currentProgram.getListing().getInstructions(f.getBody(),true);
            while(ii.hasNext()){ Instruction ins=ii.next();
                if(ins.getMnemonicString().toLowerCase().startsWith("call"))
                    for(Reference r:ins.getReferencesFrom()){ Function g=fm.getFunctionAt(r.getToAddress()); if(g!=null) toDec.add(r.getToAddress().getOffset()); } } }
        pw.println("// Z_Construct_UClass_UAT_UIXcmnAgreement + callees (find StaticRegisterNatives {name,exec}).");
        for(long a:toDec){ Function f=fm.getFunctionAt(space.getAddress(a)); if(f==null)continue;
            pw.println("// ======== 0x"+Long.toHexString(a)+" ========");
            try{ DecompileResults r=decomp.decompileFunction(f,120,mon);
                pw.println((r!=null&&r.decompileCompleted())?r.getDecompiledFunction().getC():"// <fail>"); }
            catch(Exception ex){ pw.println("// <err> "+ex); }
            pw.println(); }
        pw.close();

        PrintWriter iw=new PrintWriter(new FileWriter(new File(OUT_DIR,"_agree_loc3.txt"))); iw.print(idx.toString()); iw.close();
        println("LOC3: done, ZC="+zc.size()+" funcs="+toDec.size());
    }
}
