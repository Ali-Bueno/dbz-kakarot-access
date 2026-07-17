// Reach Z_Construct_UClass_UAT_UIXcmnAgreement via the FClassParams struct (2026-07-17).
// The 6 known FPropertyParams sit in a contiguous block ~[0x1454f0400..0x1454f0900].
// FClassParams.Properties points into that block. So:
//   (1) raw-scan .rdata/.data for a qword V in [0x1454f0300..0x1454f0a00] (candidate
//       FClassParams.Properties or a FPropertyParams* array slot). Report location L, V.
//   (2) getReferencesTo(L) and getReferencesTo(structBase near L) -> code = ZC / register.
//   (3) Also raw-scan for a qword == L (who points to the array) to climb to FClassParams,
//       then getReferencesTo that -> ZC.
//   (4) Decompile every code function that references any address in the block or the
//       FClassParams, plus 1-level callees. From ZC read the function-link array names.
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
import ghidra.util.task.ConsoleTaskMonitor;
import java.io.File;
import java.io.FileWriter;
import java.io.PrintWriter;
import java.util.ArrayList;
import java.util.LinkedHashMap;
import java.util.LinkedHashSet;
import java.util.List;
import java.util.Map;
import java.util.Set;

public class agree_zc2 extends GhidraScript {
    static final String OUT_DIR =
        "D:\\code\\unreal\\dragon ball kakarot access\\code\\decompiled";
    static final long LO=0x1454f0300L, HI=0x1454f0a00L;

    Memory mem; FunctionManager fm; AddressSpace space; DecompInterface decomp; ConsoleTaskMonitor mon;
    List<byte[]> bl=new ArrayList<>(); List<Long> bs=new ArrayList<>();
    long u64(byte[] b,int i){ long v=0; for(int k=0;k<8;k++) v|=((long)(b[i+k]&0xff))<<(8*k); return v; }
    String asciiAt(long p){ try{ byte[] b=new byte[64]; mem.getBytes(space.getAddress(p),b);
        StringBuilder s=new StringBuilder(); for(byte c:b){ if(c==0)return s.length()>=1?s.toString():null; if(c<0x20||c>0x7e)return null; s.append((char)c);} }catch(Exception e){} return null; }
    boolean inText(long p){ try{ MemoryBlock b=mem.getBlock(space.getAddress(p)); return b!=null&&b.isExecute(); }catch(Exception e){return false;} }
    List<Address> refsTo(long a){ List<Address> o=new ArrayList<>(); try{ ReferenceIterator ri=currentProgram.getReferenceManager().getReferencesTo(space.getAddress(a));
        int n=0; while(ri.hasNext()&&n<30){ o.add(ri.next().getFromAddress()); n++; } }catch(Exception e){} return o; }

    @Override public void run() throws Exception {
        new File(OUT_DIR).mkdirs();
        mem=currentProgram.getMemory(); fm=currentProgram.getFunctionManager();
        space=currentProgram.getAddressFactory().getDefaultAddressSpace();
        mon=new ConsoleTaskMonitor(); decomp=new DecompInterface(); decomp.openProgram(currentProgram);
        for(String bn:new String[]{".rdata",".data"}){ MemoryBlock b=mem.getBlock(bn); if(b==null)continue;
            byte[] a=new byte[(int)b.getSize()]; b.getBytes(b.getStart(),a); bl.add(a); bs.add(b.getStart().getOffset()); }
        StringBuilder idx=new StringBuilder();
        Set<Long> codeFns=new LinkedHashSet<>();

        // (1) pointers into the param block
        idx.append("== data slots pointing into ["+Long.toHexString(LO)+".."+Long.toHexString(HI)+"] ==\n");
        List<Long> slots=new ArrayList<>();
        for(int bi=0;bi<bl.size();bi++){ byte[] a=bl.get(bi); long base=bs.get(bi);
            for(int i=0;i+8<=a.length;i+=8){ long q=u64(a,i); if(q>=LO&&q<HI){ long loc=base+i; slots.add(loc);
                idx.append(String.format("  slot@0x%x -> 0x%x%n", loc, q)); } } }

        // (2) code xrefs to each slot location AND to the pointed-to target
        idx.append("\n== code/data xrefs to those slot locations & targets ==\n");
        Set<Long> climb=new LinkedHashSet<>();
        for(long loc:slots){ for(Address r:refsTo(loc)){ Function fn=fm.getFunctionContaining(r);
            idx.append(String.format("  ref->slot@0x%x from 0x%x%s%n", loc, r.getOffset(), fn!=null?(" in 0x"+Long.toHexString(fn.getEntryPoint().getOffset())):""));
            if(fn!=null) codeFns.add(fn.getEntryPoint().getOffset()); else climb.add(r.getOffset()); }
            climb.add(loc); }

        // (3) climb: who points to each slot location (find FClassParams base), then its code xref
        idx.append("\n== climb: qword == slotLoc (who references the array), + their code xrefs ==\n");
        for(long target:new ArrayList<>(climb)){
            for(int bi=0;bi<bl.size();bi++){ byte[] a=bl.get(bi); long base=bs.get(bi);
                for(int i=0;i+8<=a.length;i+=8){ if(u64(a,i)!=target) continue; long loc=base+i;
                    idx.append(String.format("  0x%x -> 0x%x%n", loc, target));
                    for(Address r:refsTo(loc)){ Function fn=fm.getFunctionContaining(r);
                        idx.append(String.format("      ref from 0x%x%s%n", r.getOffset(), fn!=null?(" in 0x"+Long.toHexString(fn.getEntryPoint().getOffset())):""));
                        if(fn!=null) codeFns.add(fn.getEntryPoint().getOffset()); }
                    // also climb one more level
                    for(int bj=0;bj<bl.size();bj++){ byte[] a2=bl.get(bj); long base2=bs.get(bj);
                        for(int j=0;j+8<=a2.length;j+=8){ if(u64(a2,j)!=loc) continue; long loc2=base2+j;
                            for(Address r:refsTo(loc2)){ Function fn=fm.getFunctionContaining(r);
                                if(fn!=null){ codeFns.add(fn.getEntryPoint().getOffset());
                                    idx.append(String.format("        L2 ref from 0x%x in 0x%x%n", r.getOffset(), fn.getEntryPoint().getOffset())); } } } }
                } } }

        // (4) decompile the code functions (ZC/register) + 1-level callees
        idx.append("\ncode functions to decompile: "+codeFns+"\n");
        Set<Long> dec=new LinkedHashSet<>(codeFns);
        for(long z:codeFns){ Function f=fm.getFunctionAt(space.getAddress(z)); if(f==null)continue;
            InstructionIterator ii=currentProgram.getListing().getInstructions(f.getBody(),true);
            while(ii.hasNext()){ Instruction ins=ii.next(); if(ins.getMnemonicString().toLowerCase().startsWith("call"))
                for(Reference r:ins.getReferencesFrom()){ Function g=fm.getFunctionAt(r.getToAddress()); if(g!=null&&dec.size()<60) dec.add(r.getToAddress().getOffset()); } } }
        PrintWriter pw=new PrintWriter(new FileWriter(new File(OUT_DIR,"agree_zc2.c")));
        for(long a:dec){ Function f=fm.getFunctionAt(space.getAddress(a)); if(f==null)continue;
            pw.println("// ======== 0x"+Long.toHexString(a)+" ========");
            try{ DecompileResults r=decomp.decompileFunction(f,120,mon); pw.println((r!=null&&r.decompileCompleted())?r.getDecompiledFunction().getC():"// <fail>"); }
            catch(Exception ex){ pw.println("// <err> "+ex); } pw.println(); }
        pw.close();

        PrintWriter iw=new PrintWriter(new FileWriter(new File(OUT_DIR,"_agree_zc2.txt"))); iw.print(idx.toString()); iw.close();
        println("ZC2: done slots="+slots.size()+" codeFns="+codeFns.size());
    }
}
