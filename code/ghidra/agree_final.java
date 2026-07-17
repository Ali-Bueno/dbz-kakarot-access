// FINAL: UAT_UIXcmnAgreement ctor + native impls (2026-07-17).
// GetPrivateStaticClass @0x1417bd0d0 proved the class (name "AT_UIXcmnAgreement",
// size 0x5e8). Its ClassConstructor = FUN_1417be950 (inits the unreflected tail
// state). StaticRegisterNatives / the exec table live in the 0x1417be region.
// Decompile: the ctor, everything in [0x1417bd900..0x1417bf000] that is a function
// (register natives + the 5 exec thunks + their impls), and follow impl callees
// (depth 2) to reach the page/doc tail writes + ImageTextureMap(0x508) key.
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

public class agree_final extends GhidraScript {
    static final String OUT_DIR =
        "D:\\code\\unreal\\dragon ball kakarot access\\code\\decompiled";
    // seeds: ctor + GetPrivateStaticClass args (register natives, ctor helpers).
    static final long[] SEEDS = {0x1417be950L, 0x1417bd0d0L, 0x1417be1d0L, 0x1407e4520L};
    // and: enumerate all defined functions whose entry is in this .text window
    static final long WIN_LO=0x1417bd900L, WIN_HI=0x1417bf000L;

    FunctionManager fm; AddressSpace space; DecompInterface decomp; ConsoleTaskMonitor mon;

    @Override public void run() throws Exception {
        new File(OUT_DIR).mkdirs();
        fm=currentProgram.getFunctionManager();
        space=currentProgram.getAddressFactory().getDefaultAddressSpace();
        mon=new ConsoleTaskMonitor(); decomp=new DecompInterface(); decomp.openProgram(currentProgram);

        Map<Long,Integer> dep=new LinkedHashMap<>(); Map<Long,String> role=new LinkedHashMap<>(); ArrayDeque<Long> q=new ArrayDeque<>();
        for(long s:SEEDS){ getOrCreate(space.getAddress(s)); role.put(s,"SEED"); dep.put(s,0); q.add(s); }
        // add all funcs in window
        Function f=fm.getFunctionAt(space.getAddress(WIN_LO));
        java.util.Iterator<Function> it=fm.getFunctions(space.getAddress(WIN_LO),true);
        while(it.hasNext()){ Function g=it.next(); long a=g.getEntryPoint().getOffset(); if(a>=WIN_HI) break;
            if(!role.containsKey(a)){ role.put(a,"WIN"); dep.put(a,0); q.add(a); } }

        while(!q.isEmpty() && role.size()<200){ long cur=q.poll(); int d=dep.get(cur); if(d>=2) continue;
            for(Address c:calls(getOrCreate(space.getAddress(cur)))){ long ca=c.getOffset(); if(role.containsKey(ca))continue; if(role.size()>=200)break;
                role.put(ca,"d"+(d+1)+"<-0x"+Long.toHexString(cur)); dep.put(ca,d+1); q.add(ca); } }

        PrintWriter pw=new PrintWriter(new FileWriter(new File(OUT_DIR,"agree_final.c")));
        pw.println("// UAT_UIXcmnAgreement (size 0x5e8) ctor + register-natives + exec impls + callees.");
        pw.println("// Members: RootPanel 0x398, FadeBlack 0x3a0, SubjectImage 0x3a8, HeadImage 0x3b0,");
        pw.println("//   LeftArrow 0x3c0, RightArrow 0x3c8, AppearAnim 0x3d0, DisappearAnim 0x3d8, Map 0x508.");
        for(Map.Entry<Long,String> e:role.entrySet()){ Function g=getOrCreate(space.getAddress(e.getKey())); if(g==null)continue;
            pw.println("// ======== "+e.getValue()+"  @ 0x"+Long.toHexString(e.getKey())+" ========");
            try{ DecompileResults r=decomp.decompileFunction(g,120,mon); pw.println((r!=null&&r.decompileCompleted())?r.getDecompiledFunction().getC():"// <fail>"); }
            catch(Exception ex){ pw.println("// <err> "+ex); } pw.println(); }
        pw.close();
        println("FINAL: done "+role.size());
    }
    Function getOrCreate(Address a){ if(a==null)return null; Function f=fm.getFunctionAt(a); if(f==null){ try{ f=createFunction(a,null);}catch(Exception e){} } return f; }
    List<Address> calls(Function f){ List<Address> o=new ArrayList<>(); if(f==null)return o; try{ InstructionIterator ii=currentProgram.getListing().getInstructions(f.getBody(),true);
        while(ii.hasNext()){ Instruction ins=ii.next(); if(ins.getMnemonicString().toLowerCase().startsWith("call"))
            for(Reference r:ins.getReferencesFrom()){ Function g=fm.getFunctionAt(r.getToAddress()); if(g!=null&&!g.equals(f)) o.add(r.getToAddress()); } } }catch(Exception e){} return o; }
}
