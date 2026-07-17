// UAT_UIXcmnAgreement REAL native impls (2026-07-17, definitive).
// StaticRegisterNatives table @0x14407d8a0 (5 entries), registered by FUN_1417be950.
// Read the {name,exec} pairs, resolve each exec thunk -> impl (the tail call past
// P_FINISH), decompile impl + callees depth 3 => page/doc tail writes + ImageTextureMap
// (0x508) key formula.
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
import ghidra.program.model.symbol.Reference;
import ghidra.util.task.ConsoleTaskMonitor;
import java.io.File;
import java.io.FileWriter;
import java.io.PrintWriter;
import java.util.ArrayDeque;
import java.util.ArrayList;
import java.util.LinkedHashMap;
import java.util.List;
import java.util.Map;

public class agree_impls extends GhidraScript {
    static final String OUT_DIR =
        "D:\\code\\unreal\\dragon ball kakarot access\\code\\decompiled";
    static final long TABLE=0x14407d8a0L; static final int N=5;

    Memory mem; FunctionManager fm; AddressSpace space; DecompInterface decomp; ConsoleTaskMonitor mon;
    long u64(Address a) throws Exception { byte[] b=new byte[8]; mem.getBytes(a,b); long v=0; for(int k=0;k<8;k++) v|=((long)(b[k]&0xff))<<(8*k); return v; }
    String asciiAt(long p){ try{ byte[] b=new byte[48]; mem.getBytes(space.getAddress(p),b); StringBuilder s=new StringBuilder();
        for(byte c:b){ if(c==0)return s.toString(); if(c<0x20||c>0x7e)return null; s.append((char)c);} }catch(Exception e){} return null; }

    @Override public void run() throws Exception {
        new File(OUT_DIR).mkdirs();
        mem=currentProgram.getMemory(); fm=currentProgram.getFunctionManager();
        space=currentProgram.getAddressFactory().getDefaultAddressSpace();
        mon=new ConsoleTaskMonitor(); decomp=new DecompInterface(); decomp.openProgram(currentProgram);
        StringBuilder idx=new StringBuilder();

        Map<Long,Integer> dep=new LinkedHashMap<>(); Map<Long,String> role=new LinkedHashMap<>(); ArrayDeque<Long> q=new ArrayDeque<>();
        // Real agreement impls (from the exec thunk bodies in agree2.c).
        long[][] IMPLS = { {0x1416e4b40L,0},{0x1416f29f0L,1},{0x1416eec40L,2},{0x1416eed50L,3},{0x1416eee20L,4} };
        String[] NM = {"CheckHovered","Loop_Arrow","MouseClickDecide","MouseClickTabLeft","MouseClickTabRight"};
        for(long[] im:IMPLS){ role.put(im[0],"IMPL "+NM[(int)im[1]]); dep.put(im[0],0); q.add(im[0]); }
        while(!q.isEmpty() && role.size()<160){ long cur=q.poll(); int d=dep.get(cur); if(d>=3) continue;
            for(Address c:calls(getOrCreate(space.getAddress(cur)))){ long ca=c.getOffset(); if(role.containsKey(ca))continue; if(role.size()>=160)break;
                role.put(ca,"d"+(d+1)+"<-0x"+Long.toHexString(cur)); dep.put(ca,d+1); q.add(ca); } }

        PrintWriter pw=new PrintWriter(new FileWriter(new File(OUT_DIR,"agree_impls.c")));
        pw.println("// UAT_UIXcmnAgreement (0x5e8) native impls + callees depth3.");
        pw.println("// SubjectImage 0x3a8, HeadImage 0x3b0, LeftArrow 0x3c0, RightArrow 0x3c8, Map 0x508.");
        for(Map.Entry<Long,String> e:role.entrySet()){ Function g=getOrCreate(space.getAddress(e.getKey())); if(g==null)continue;
            pw.println("// ======== "+e.getValue()+"  @ 0x"+Long.toHexString(e.getKey())+" ========");
            try{ DecompileResults r=decomp.decompileFunction(g,120,mon); pw.println((r!=null&&r.decompileCompleted())?r.getDecompiledFunction().getC():"// <fail>"); }
            catch(Exception ex){ pw.println("// <err> "+ex); } pw.println(); }
        pw.close();
        PrintWriter iw=new PrintWriter(new FileWriter(new File(OUT_DIR,"_agree_impls.txt"))); iw.print(idx.toString()); iw.close();
        println("IMPLS: done "+role.size());
    }
    Function getOrCreate(Address a){ if(a==null)return null; Function f=fm.getFunctionAt(a); if(f==null){ try{ f=createFunction(a,null);}catch(Exception e){} } return f; }
    // exec thunks are: P_FINISH (advance stack) then tail-call impl. Return the LAST call target.
    Address tailCall(Function f){ if(f==null)return null; Address last=null; try{ InstructionIterator ii=currentProgram.getListing().getInstructions(f.getBody(),true);
        while(ii.hasNext()){ Instruction ins=ii.next(); if(ins.getMnemonicString().toLowerCase().startsWith("call"))
            for(Reference r:ins.getReferencesFrom()){ Function g=fm.getFunctionAt(r.getToAddress()); if(g!=null&&!g.equals(f)) last=r.getToAddress(); } } }catch(Exception e){} return last; }
    List<Address> calls(Function f){ List<Address> o=new ArrayList<>(); if(f==null)return o; try{ InstructionIterator ii=currentProgram.getListing().getInstructions(f.getBody(),true);
        while(ii.hasNext()){ Instruction ins=ii.next(); if(ins.getMnemonicString().toLowerCase().startsWith("call"))
            for(Reference r:ins.getReferencesFrom()){ Function g=fm.getFunctionAt(r.getToAddress()); if(g!=null&&!g.equals(f)) o.add(r.getToAddress()); } } }catch(Exception e){} return o; }
}
