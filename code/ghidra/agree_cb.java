// UAT_UIXcmnAgreement page-commit callback FUN_1416f0030 (2026-07-17).
// The refresh FUN_1416f2640 registers this as a latent action; it commits the target
// page (0x5ac -> 0x5a8), reads ImageTextureMap@0x508 by a uint32 key derived from the
// page (and possibly a document id), and SetBrushFromTexture on SubjectImage@0x3a8.
// Decompile it + callees depth 3. Also decompile the agreement Open/Setup that
// populates the map + count (found via refs to this+0x5b0 write) — seed a few nearby.
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

public class agree_cb extends GhidraScript {
    static final String OUT_DIR =
        "D:\\code\\unreal\\dragon ball kakarot access\\code\\decompiled";
    static final long[] SEEDS = {0x1416f0030L};

    FunctionManager fm; AddressSpace space; DecompInterface decomp; ConsoleTaskMonitor mon;

    @Override public void run() throws Exception {
        new File(OUT_DIR).mkdirs();
        fm=currentProgram.getFunctionManager();
        space=currentProgram.getAddressFactory().getDefaultAddressSpace();
        mon=new ConsoleTaskMonitor(); decomp=new DecompInterface(); decomp.openProgram(currentProgram);
        Map<Long,Integer> dep=new LinkedHashMap<>(); Map<Long,String> role=new LinkedHashMap<>(); ArrayDeque<Long> q=new ArrayDeque<>();
        for(long s:SEEDS){ role.put(s,"SEED"); dep.put(s,0); q.add(s); }
        while(!q.isEmpty() && role.size()<120){ long cur=q.poll(); int d=dep.get(cur); if(d>=3) continue;
            for(Address c:calls(getOrCreate(space.getAddress(cur)))){ long ca=c.getOffset(); if(role.containsKey(ca))continue; if(role.size()>=120)break;
                role.put(ca,"d"+(d+1)+"<-0x"+Long.toHexString(cur)); dep.put(ca,d+1); q.add(ca); } }
        PrintWriter pw=new PrintWriter(new FileWriter(new File(OUT_DIR,"agree_cb.c")));
        pw.println("// FUN_1416f0030 page-commit callback + callees. Map@0x508 Subject@0x3a8 page@0x5a8/0x5ac count@0x5b0.");
        for(Map.Entry<Long,String> e:role.entrySet()){ Function g=getOrCreate(space.getAddress(e.getKey())); if(g==null)continue;
            pw.println("// ======== "+e.getValue()+"  @ 0x"+Long.toHexString(e.getKey())+" ========");
            try{ DecompileResults r=decomp.decompileFunction(g,120,mon); pw.println((r!=null&&r.decompileCompleted())?r.getDecompiledFunction().getC():"// <fail>"); }
            catch(Exception ex){ pw.println("// <err> "+ex); } pw.println(); }
        pw.close();
        println("CB: done "+role.size());
    }
    Function getOrCreate(Address a){ if(a==null)return null; Function f=fm.getFunctionAt(a); if(f==null){ try{ f=createFunction(a,null);}catch(Exception e){} } return f; }
    List<Address> calls(Function f){ List<Address> o=new ArrayList<>(); if(f==null)return o; try{ InstructionIterator ii=currentProgram.getListing().getInstructions(f.getBody(),true);
        while(ii.hasNext()){ Instruction ins=ii.next(); if(ins.getMnemonicString().toLowerCase().startsWith("call"))
            for(Reference r:ins.getReferencesFrom()){ Function g=fm.getFunctionAt(r.getToAddress()); if(g!=null&&!g.equals(f)) o.add(r.getToAddress()); } } }catch(Exception e){} return o; }
}
