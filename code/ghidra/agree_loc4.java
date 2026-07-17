// Identify the agreement class among all Loop_Arrow classes (2026-07-17).
// Loop_Arrow has only ~9 name hits. For each {name="Loop_Arrow", exec} registration
// entry: resolve exec -> impl (single tail call in the P_FINISH thunk), decompile the
// impl, and report which struct offsets it touches. The AGREEMENT impl is the one that
// drives BOTH arrows at 0x3c0 (LeftArrow) AND 0x3c8 (RightArrow) — cluster B used
// 0x3b8/0x3c0. Also decompile each impl's callees (depth 2) so we catch the page-flip
// + texture refresh + ImageTextureMap(0x508) key. Same for MouseClickTabRight/Left and
// MouseClickDecide entries whose impl references 0x3c8 or 0x508.
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
import ghidra.util.task.ConsoleTaskMonitor;
import java.io.File;
import java.io.FileWriter;
import java.io.PrintWriter;
import java.util.ArrayDeque;
import java.util.ArrayList;
import java.util.LinkedHashMap;
import java.util.LinkedHashSet;
import java.util.List;
import java.util.Map;
import java.util.Set;

public class agree_loc4 extends GhidraScript {
    static final String OUT_DIR =
        "D:\\code\\unreal\\dragon ball kakarot access\\code\\decompiled";
    static final String[] ANCHORS = {"Loop_Arrow","MouseClickTabRight","MouseClickTabLeft","MouseClickDecide"};

    Memory mem; FunctionManager fm; AddressSpace space; DecompInterface decomp; ConsoleTaskMonitor mon;
    List<byte[]> bl=new ArrayList<>(); List<Long> bs=new ArrayList<>();

    long u64(byte[] b,int i){ long v=0; for(int k=0;k<8;k++) v|=((long)(b[i+k]&0xff))<<(8*k); return v; }
    boolean inText(long p){ try{ MemoryBlock b=mem.getBlock(space.getAddress(p)); return b!=null&&b.isExecute(); }catch(Exception e){return false;} }
    List<Address> strHits(String s) throws Exception { byte[] n=(s+"\0").getBytes("ISO-8859-1"); List<Address> o=new ArrayList<>();
        Address f=mem.getMinAddress(); while(true){ Address h=find(f,n); if(h==null)break; o.add(h); f=h.add(1); if(o.size()>60)break; } return o; }

    Function getOrCreate(Address a){ if(a==null)return null; Function f=fm.getFunctionAt(a); if(f==null){ try{ f=createFunction(a,null);}catch(Exception e){} } return f; }
    Address firstCall(Function f){ if(f==null)return null; try{ InstructionIterator ii=currentProgram.getListing().getInstructions(f.getBody(),true);
        while(ii.hasNext()){ Instruction ins=ii.next(); if(ins.getMnemonicString().toLowerCase().startsWith("call"))
            for(Reference r:ins.getReferencesFrom()){ Function g=fm.getFunctionAt(r.getToAddress()); if(g!=null&&!g.equals(f)) return r.getToAddress(); } } }catch(Exception e){} return null; }
    List<Address> calls(Function f){ List<Address> o=new ArrayList<>(); if(f==null)return o; try{ InstructionIterator ii=currentProgram.getListing().getInstructions(f.getBody(),true);
        while(ii.hasNext()){ Instruction ins=ii.next(); if(ins.getMnemonicString().toLowerCase().startsWith("call"))
            for(Reference r:ins.getReferencesFrom()){ Function g=fm.getFunctionAt(r.getToAddress()); if(g!=null&&!g.equals(f)) o.add(r.getToAddress()); } } }catch(Exception e){} return o; }
    Set<Long> offsetsOf(Function f){ Set<Long> o=new LinkedHashSet<>(); if(f==null)return o; try{ InstructionIterator ii=currentProgram.getListing().getInstructions(f.getBody(),true); int c=0;
        while(ii.hasNext()&&c<3000){ Instruction ins=ii.next(); c++; for(int op=0;op<ins.getNumOperands();op++) for(Object x:ins.getOpObjects(op)) if(x instanceof Scalar){ long v=((Scalar)x).getUnsignedValue(); if(v>=0x300&&v<0x600) o.add(v); } } }catch(Exception e){} return o; }

    @Override public void run() throws Exception {
        new File(OUT_DIR).mkdirs();
        mem=currentProgram.getMemory(); fm=currentProgram.getFunctionManager();
        space=currentProgram.getAddressFactory().getDefaultAddressSpace();
        mon=new ConsoleTaskMonitor(); decomp=new DecompInterface(); decomp.openProgram(currentProgram);
        for(String bn:new String[]{".rdata",".data"}){ MemoryBlock b=mem.getBlock(bn); if(b==null)continue;
            byte[] a=new byte[(int)b.getSize()]; b.getBytes(b.getStart(),a); bl.add(a); bs.add(b.getStart().getOffset()); }
        StringBuilder idx=new StringBuilder();

        // Collect candidate impls (dedup) whose exec-thunk tail-calls them; flag those touching 0x3c8 & 0x3c0.
        Map<Long,String> agreementImpls=new LinkedHashMap<>();
        for(String anchor:ANCHORS){
            List<Address> hits=strHits(anchor);
            idx.append("=== ").append(anchor).append(": ").append(hits.size()).append(" hits ===\n");
            Set<Long> seenExec=new LinkedHashSet<>();
            for(Address sa:hits){ long tgt=sa.getOffset();
                for(int bi=0;bi<bl.size();bi++){ byte[] a=bl.get(bi); long base=bs.get(bi);
                    for(int i=0;i+16<=a.length;i+=8){ if(u64(a,i)!=tgt) continue; long exec=u64(a,i+8); if(!inText(exec))continue;
                        if(!seenExec.add(exec)) continue;
                        Function ef=getOrCreate(space.getAddress(exec)); Address impl=firstCall(ef);
                        if(impl==null){ idx.append(String.format("  exec 0x%x -> (no impl)%n",exec)); continue; }
                        Function imf=getOrCreate(impl); Set<Long> offs=offsetsOf(imf);
                        boolean a3c0=offs.contains(0x3c0L), a3c8=offs.contains(0x3c8L), a3a8=offs.contains(0x3a8L), a508=offs.contains(0x508L);
                        idx.append(String.format("  exec 0x%x -> impl 0x%x  offs{3c0=%b,3c8=%b,3a8=%b,508=%b} %s%n",
                            exec, impl.getOffset(), a3c0,a3c8,a3a8,a508, offs.toString()));
                        if(a3c8 && (a3c0||a3a8||a508)) agreementImpls.put(impl.getOffset(), anchor);
                    } } }
        }

        idx.append("\n==== AGREEMENT-candidate impls (touch 0x3c8 + agreement offset) ====\n");
        for(Map.Entry<Long,String> e:agreementImpls.entrySet())
            idx.append(String.format("  0x%x  (%s)%n", e.getKey(), e.getValue()));

        // BFS depth 3 from agreement impls; decompile all.
        Map<Long,String> role=new LinkedHashMap<>(); ArrayDeque<Long> q=new ArrayDeque<>(); Map<Long,Integer> dep=new LinkedHashMap<>();
        for(Long a:agreementImpls.keySet()){ role.put(a,"SEED "+agreementImpls.get(a)); dep.put(a,0); q.add(a); }
        while(!q.isEmpty() && role.size()<160){ long cur=q.poll(); int d=dep.get(cur); if(d>=3) continue;
            for(Address c:calls(getOrCreate(space.getAddress(cur)))){ long ca=c.getOffset(); if(role.containsKey(ca))continue; if(role.size()>=160)break;
                role.put(ca,"d"+(d+1)+" <-0x"+Long.toHexString(cur)); dep.put(ca,d+1); q.add(ca); } }

        PrintWriter pw=new PrintWriter(new FileWriter(new File(OUT_DIR,"agree_real.c")));
        pw.println("// REAL UAT_UIXcmnAgreement impls (arrows 0x3c0/0x3c8) + callees depth3.");
        pw.println("// map@0x508 SubjectImage@0x3a8 HeadImage@0x3b0 LeftArrow@0x3c0 RightArrow@0x3c8.");
        for(Map.Entry<Long,String> e:role.entrySet()){ Function f=getOrCreate(space.getAddress(e.getKey())); if(f==null)continue;
            pw.println("// ======== "+e.getValue()+"  @ 0x"+Long.toHexString(e.getKey())+" ========");
            try{ DecompileResults r=decomp.decompileFunction(f,120,mon); pw.println((r!=null&&r.decompileCompleted())?r.getDecompiledFunction().getC():"// <fail>"); }
            catch(Exception ex){ pw.println("// <err> "+ex); } pw.println(); }
        pw.close();

        PrintWriter iw=new PrintWriter(new FileWriter(new File(OUT_DIR,"_agree_loc4.txt"))); iw.print(idx.toString()); iw.close();
        println("LOC4: done, agreementImpls="+agreementImpls.size()+" decompiled="+role.size());
    }
}
