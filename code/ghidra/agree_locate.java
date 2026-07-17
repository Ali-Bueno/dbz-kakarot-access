// Locate the REAL UAT_UIXcmnAgreement (2026-07-17). Cluster B was a look-alike paged
// viewer. Anchor instead on the class's own strings:
//   - "UIXcmnAgreement" (class name, used by GetPrivateStaticClass registration)
//   - "SubjectImage" / "ImageTextureMap" (UPROPERTY names in the Z_Construct property list)
// For each string: find .rdata/.data pointers to it and dump a window of the surrounding
// pointer table + the code xrefs, so we can find (a) the class's native-function
// registration table (its true UFunction set) and (b) functions that touch 0x3A8/0x508.
// Also brute-scans .text for functions that reference BOTH constant offsets 0x3a8 and
// 0x508 (SubjectImage + ImageTextureMap) — those are the agreement's own methods.
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
import ghidra.program.model.mem.Memory;
import ghidra.program.model.mem.MemoryBlock;
import ghidra.program.model.scalar.Scalar;
import ghidra.util.task.ConsoleTaskMonitor;
import java.io.File;
import java.io.FileWriter;
import java.io.PrintWriter;
import java.util.ArrayList;
import java.util.LinkedHashMap;
import java.util.List;
import java.util.Map;

public class agree_locate extends GhidraScript {
    static final String OUT_DIR =
        "D:\\code\\unreal\\dragon ball kakarot access\\code\\decompiled";
    static final String[] STR = {"UIXcmnAgreement", "SubjectImage", "ImageTextureMap",
                                 "HeadImage", "LeftArrow", "RightArrow"};

    Memory mem;
    FunctionManager fm;
    AddressSpace space;
    DecompInterface decomp;
    ConsoleTaskMonitor mon;
    List<byte[]> bl = new ArrayList<>();
    List<Long> bs = new ArrayList<>();

    long u64(byte[] b, int i) { long v=0; for(int k=0;k<8;k++) v|=((long)(b[i+k]&0xff))<<(8*k); return v; }

    String asciiAt(long ptr){ try{ Address a=space.getAddress(ptr); byte[] buf=new byte[64]; mem.getBytes(a,buf);
        StringBuilder sb=new StringBuilder(); for(byte c:buf){ if(c==0) return sb.length()>=2?sb.toString():null;
        if(c<0x20||c>0x7e) return null; sb.append((char)c);} }catch(Exception e){} return null; }

    boolean inText(long p){ try{ MemoryBlock b=mem.getBlock(space.getAddress(p)); return b!=null&&b.isExecute(); }catch(Exception e){return false;} }

    List<Address> strHits(String s) throws Exception {
        byte[] n=(s+"\0").getBytes("ISO-8859-1"); List<Address> o=new ArrayList<>();
        Address f=mem.getMinAddress();
        while(true){ Address h=find(f,n); if(h==null)break; o.add(h); f=h.add(1); if(o.size()>50)break; }
        return o;
    }

    @Override public void run() throws Exception {
        new File(OUT_DIR).mkdirs();
        mem=currentProgram.getMemory(); fm=currentProgram.getFunctionManager();
        space=currentProgram.getAddressFactory().getDefaultAddressSpace();
        mon=new ConsoleTaskMonitor(); decomp=new DecompInterface(); decomp.openProgram(currentProgram);
        for(String bn:new String[]{".rdata",".data"}){ MemoryBlock b=mem.getBlock(bn); if(b==null)continue;
            byte[] a=new byte[(int)b.getSize()]; b.getBytes(b.getStart(),a); bl.add(a); bs.add(b.getStart().getOffset()); }

        StringBuilder idx=new StringBuilder();
        for(String s:STR){
            List<Address> hits=strHits(s);
            idx.append("STRING '").append(s).append("' : ").append(hits.size()).append(" hits\n");
            for(Address sa:hits){
                long tgt=sa.getOffset();
                idx.append("  @0x").append(Long.toHexString(tgt)).append("\n");
                // pointers to this string
                for(int bi=0;bi<bl.size();bi++){ byte[] a=bl.get(bi); long base=bs.get(bi);
                    for(int i=0;i+8<=a.length;i+=8){ if(u64(a,i)!=tgt) continue;
                        long loc=base+i;
                        idx.append("    ptr@0x").append(Long.toHexString(loc));
                        // neighbours: is next qword an FNativeFuncPtr (code)?  -> registration entry
                        if(i+16<=a.length){ long nx=u64(a,i+8);
                            if(inText(nx)) idx.append("  [+8 exec 0x").append(Long.toHexString(nx)).append("]"); }
                        // code xrefs to this rdata location (lea of the property name / class name)
                        for(Address ref:refsTo(space.getAddress(loc))){
                            Function fn=fm.getFunctionContaining(ref);
                            idx.append("  xref@0x").append(Long.toHexString(ref.getOffset()));
                            if(fn!=null) idx.append("(in 0x").append(Long.toHexString(fn.getEntryPoint().getOffset())).append(")");
                        }
                        idx.append("\n");
                    }
                }
            }
        }

        // Brute scan: functions whose instructions contain displacement 0x508 AND (0x3a8 OR 0x3b0).
        idx.append("\n==== functions referencing 0x508 AND (0x3a8|0x3b0) ====\n");
        Map<Long,String> hitFns=new LinkedHashMap<>();
        FunctionIterator it=fm.getFunctions(true);
        int scanned=0;
        while(it.hasNext()){
            Function f=it.next(); scanned++;
            boolean has508=false, hasImg=false;
            InstructionIterator ii=currentProgram.getListing().getInstructions(f.getBody(),true);
            int cnt=0;
            while(ii.hasNext()&&cnt<4000){ Instruction ins=ii.next(); cnt++;
                for(int op=0; op<ins.getNumOperands(); op++){
                    for(Object o:ins.getOpObjects(op)){
                        if(o instanceof Scalar){ long v=((Scalar)o).getUnsignedValue();
                            if(v==0x508) has508=true;
                            if(v==0x3a8||v==0x3b0) hasImg=true;
                        }
                    }
                }
                if(has508&&hasImg) break;
            }
            if(has508&&hasImg) hitFns.put(f.getEntryPoint().getOffset(), f.getName());
        }
        idx.append("scanned ").append(scanned).append(" funcs; ").append(hitFns.size()).append(" match\n");
        for(Map.Entry<Long,String> e:hitFns.entrySet())
            idx.append("  0x").append(Long.toHexString(e.getKey())).append("  ").append(e.getValue()).append("\n");

        PrintWriter iw=new PrintWriter(new FileWriter(new File(OUT_DIR,"_agree_locate.txt")));
        iw.print(idx.toString()); iw.close();

        // Decompile the matching functions.
        PrintWriter pw=new PrintWriter(new FileWriter(new File(OUT_DIR,"agree_locate.c")));
        pw.println("// Functions touching ImageTextureMap(0x508) + SubjectImage/HeadImage(0x3a8/0x3b0).");
        for(Long a:hitFns.keySet()){
            Function f=fm.getFunctionAt(space.getAddress(a)); if(f==null)continue;
            pw.println("// ======== 0x"+Long.toHexString(a)+" ========");
            try{ DecompileResults r=decomp.decompileFunction(f,120,mon);
                pw.println((r!=null&&r.decompileCompleted())?r.getDecompiledFunction().getC():"// <fail>");
            }catch(Exception ex){ pw.println("// <err> "+ex); }
            pw.println();
        }
        pw.close();
        println("LOCATE: done, "+hitFns.size()+" match funcs");
    }

    List<Address> refsTo(Address a){ List<Address> o=new ArrayList<>();
        try{ ghidra.program.model.symbol.ReferenceIterator ri=currentProgram.getReferenceManager().getReferencesTo(a);
            int n=0; while(ri.hasNext()&&n<20){ o.add(ri.next().getFromAddress()); n++; } }catch(Exception e){} return o; }
}
