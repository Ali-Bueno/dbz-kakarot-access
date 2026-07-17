// AAT_Title agreement flow (2026-07-17). AAT_Title: +0x338 UIXcmnAgreementClass
// (TSubclassOf), +0x340 AgreementDialog (instance). Find functions referencing BOTH
// displacements 0x338 and 0x340 (AAT_Title create/open/close of the dialog); decompile
// them + their callees (depth 2) to recover: the CreateWidget call, the document
// selector passed to the widget's setup method, and the setup/refresh that populates
// ImageTextureMap and sets the page/doc tail members.
//@category Kakarot
import ghidra.app.script.GhidraScript;
import ghidra.app.decompiler.DecompInterface;
import ghidra.app.decompiler.DecompileResults;
import ghidra.program.model.address.Address;
import ghidra.program.model.address.AddressSpace;
import ghidra.program.model.listing.Function;
import ghidra.program.model.listing.FunctionIterator;
import ghidra.program.model.listing.FunctionManager;
import ghidra.program.model.listing.Instruction;
import ghidra.program.model.listing.InstructionIterator;
import ghidra.program.model.scalar.Scalar;
import ghidra.program.model.symbol.Reference;
import ghidra.util.task.ConsoleTaskMonitor;
import java.io.File;
import java.io.FileWriter;
import java.io.PrintWriter;
import java.util.ArrayList;
import java.util.LinkedHashMap;
import java.util.List;
import java.util.Map;

public class agree_title extends GhidraScript {
    static final String OUT_DIR =
        "D:\\code\\unreal\\dragon ball kakarot access\\code\\decompiled";
    FunctionManager fm; AddressSpace space; DecompInterface decomp; ConsoleTaskMonitor mon;

    @Override public void run() throws Exception {
        new File(OUT_DIR).mkdirs();
        fm=currentProgram.getFunctionManager();
        space=currentProgram.getAddressFactory().getDefaultAddressSpace();
        mon=new ConsoleTaskMonitor(); decomp=new DecompInterface(); decomp.openProgram(currentProgram);

        // scan for functions referencing BOTH 0x338 and 0x340
        Map<Long,String> seeds=new LinkedHashMap<>();
        FunctionIterator it=fm.getFunctions(true);
        while(it.hasNext()){ Function f=it.next(); boolean a338=false,a340=false; int c=0;
            InstructionIterator ii=currentProgram.getListing().getInstructions(f.getBody(),true);
            while(ii.hasNext()&&c<4000){ Instruction ins=ii.next(); c++;
                for(int op=0;op<ins.getNumOperands();op++) for(Object o:ins.getOpObjects(op)) if(o instanceof Scalar){ long v=((Scalar)o).getUnsignedValue(); if(v==0x338)a338=true; else if(v==0x340)a340=true; } }
            if(a338&&a340) seeds.put(f.getEntryPoint().getOffset(),"seed"); }

        // depth-2 callees
        Map<Long,String> role=new LinkedHashMap<>(seeds);
        for(Long s:seeds.keySet()){ Function f=fm.getFunctionAt(space.getAddress(s));
            for(Address c1:calls(f)){ role.putIfAbsent(c1.getOffset(),"d1<-"+Long.toHexString(s));
                for(Address c2:calls(fm.getFunctionAt(c1))){ if(role.size()<220) role.putIfAbsent(c2.getOffset(),"d2<-"+Long.toHexString(c1.getOffset())); } } }

        PrintWriter pw=new PrintWriter(new FileWriter(new File(OUT_DIR,"agree_title.c")));
        pw.println("// AAT_Title dialog flow: funcs referencing 0x338(class)+0x340(AgreementDialog) + callees.");
        pw.println("// seeds ("+seeds.size()+"): "+seeds.keySet());
        for(Map.Entry<Long,String> e:role.entrySet()){ Function f=fm.getFunctionAt(space.getAddress(e.getKey())); if(f==null)continue;
            pw.println("// ======== "+e.getValue()+"  @ 0x"+Long.toHexString(e.getKey())+" ========");
            try{ DecompileResults r=decomp.decompileFunction(f,120,mon); pw.println((r!=null&&r.decompileCompleted())?r.getDecompiledFunction().getC():"// <fail>"); }
            catch(Exception ex){ pw.println("// <err> "+ex); } pw.println(); }
        pw.close();
        println("TITLE: done seeds="+seeds.size()+" total="+role.size());
    }
    List<Address> calls(Function f){ List<Address> o=new ArrayList<>(); if(f==null)return o; try{ InstructionIterator ii=currentProgram.getListing().getInstructions(f.getBody(),true);
        while(ii.hasNext()){ Instruction ins=ii.next(); if(ins.getMnemonicString().toLowerCase().startsWith("call"))
            for(Reference r:ins.getReferencesFrom()){ Function g=fm.getFunctionAt(r.getToAddress()); if(g!=null&&!g.equals(f)) o.add(r.getToAddress()); } } }catch(Exception e){} return o; }
}
