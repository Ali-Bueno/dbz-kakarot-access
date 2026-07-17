// Isolate UAT_UIXcmnAgreement methods by DENSE member signature (2026-07-17).
// Confirmed members: RootPanel 0x398, FadeBlack 0x3a0, SubjectImage 0x3a8, HeadImage
// 0x3b0, LeftArrow 0x3c0, RightArrow 0x3c8, AppearAnim 0x3d0, DisappearAnim 0x3d8,
// ImageTextureMap 0x508. A function referencing >=5 DISTINCT of these is an agreement
// method (ctor / NativeConstruct / refresh). Report the count + which offsets, and
// decompile every match + its callees (depth 2) so we capture the tail state writes
// (page/doc index) and the ImageTextureMap key computation.
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
import java.util.TreeSet;

public class agree_sig extends GhidraScript {
    static final String OUT_DIR =
        "D:\\code\\unreal\\dragon ball kakarot access\\code\\decompiled";
    static final long[] SIG = {0x398,0x3a0,0x3a8,0x3b0,0x3c0,0x3c8,0x3d0,0x3d8,0x508};
    static final int NEED = 5;

    FunctionManager fm; AddressSpace space; DecompInterface decomp; ConsoleTaskMonitor mon;

    @Override public void run() throws Exception {
        new File(OUT_DIR).mkdirs();
        fm=currentProgram.getFunctionManager();
        space=currentProgram.getAddressFactory().getDefaultAddressSpace();
        mon=new ConsoleTaskMonitor(); decomp=new DecompInterface(); decomp.openProgram(currentProgram);
        StringBuilder idx=new StringBuilder();

        Map<Long,TreeSet<Long>> match=new LinkedHashMap<>();
        FunctionIterator it=fm.getFunctions(true);
        while(it.hasNext()){ Function f=it.next(); TreeSet<Long> found=new TreeSet<>(); int c=0;
            InstructionIterator ii=currentProgram.getListing().getInstructions(f.getBody(),true);
            while(ii.hasNext()&&c<5000){ Instruction ins=ii.next(); c++;
                for(int op=0;op<ins.getNumOperands();op++) for(Object o:ins.getOpObjects(op)) if(o instanceof Scalar){ long v=((Scalar)o).getUnsignedValue();
                    for(long s:SIG) if(v==s){ found.add(s); break; } } }
            if(found.size()>=NEED) match.put(f.getEntryPoint().getOffset(),found);
        }
        idx.append(match.size()+" functions with >="+NEED+" signature offsets\n");
        for(Map.Entry<Long,TreeSet<Long>> e:match.entrySet()){
            StringBuilder hx=new StringBuilder(); for(long v:e.getValue()) hx.append(String.format("0x%x ",v));
            idx.append(String.format("  0x%x  [%d] %s%n", e.getKey(), e.getValue().size(), hx.toString())); }

        // depth-2 callees for decompile
        Map<Long,String> role=new LinkedHashMap<>();
        for(Long m:match.keySet()) role.put(m,"SIG");
        for(Long m:match.keySet()){ Function f=fm.getFunctionAt(space.getAddress(m));
            for(Address c1:calls(f)){ role.putIfAbsent(c1.getOffset(),"d1<-"+Long.toHexString(m));
                for(Address c2:calls(fm.getFunctionAt(c1))) if(role.size()<200) role.putIfAbsent(c2.getOffset(),"d2"); } }

        PrintWriter pw=new PrintWriter(new FileWriter(new File(OUT_DIR,"agree_sig.c")));
        pw.println("// UAT_UIXcmnAgreement methods (>= "+NEED+" of member-offset signature) + callees.");
        for(Map.Entry<Long,String> e:role.entrySet()){ Function f=fm.getFunctionAt(space.getAddress(e.getKey())); if(f==null)continue;
            String extra = match.containsKey(e.getKey())?(" "+match.get(e.getKey())):"";
            pw.println("// ======== "+e.getValue()+extra+"  @ 0x"+Long.toHexString(e.getKey())+" ========");
            try{ DecompileResults r=decomp.decompileFunction(f,120,mon); pw.println((r!=null&&r.decompileCompleted())?r.getDecompiledFunction().getC():"// <fail>"); }
            catch(Exception ex){ pw.println("// <err> "+ex); } pw.println(); }
        pw.close();

        PrintWriter iw=new PrintWriter(new FileWriter(new File(OUT_DIR,"_agree_sig.txt"))); iw.print(idx.toString()); iw.close();
        println("SIG: done match="+match.size()+" total="+role.size());
    }
    List<Address> calls(Function f){ List<Address> o=new ArrayList<>(); if(f==null)return o; try{ InstructionIterator ii=currentProgram.getListing().getInstructions(f.getBody(),true);
        while(ii.hasNext()){ Instruction ins=ii.next(); if(ins.getMnemonicString().toLowerCase().startsWith("call"))
            for(Reference r:ins.getReferencesFrom()){ Function g=fm.getFunctionAt(r.getToAddress()); if(g!=null&&!g.equals(f)) o.add(r.getToAddress()); } } }catch(Exception e){} return o; }
}
