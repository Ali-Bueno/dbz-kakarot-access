// Pin UAT_UIXcmnAgreement precisely (2026-07-17).
// (1) Dump raw bytes of the FProperty param structs (ImageTextureMap key+value,
//     arrows, images) at 0x1454f05d8..0x1454f07c8 to read their Offset fields
//     (confirm 0x3A8/0x3B0/0x508 and pin LeftArrow/RightArrow) and the map key type.
// (2) Find the code that references the property-param array (Z_Construct_UClass...).
// (3) Brute-scan .text for functions referencing 0x508 AND 0x3a8 AND 0x3b0 (all three)
//     — the agreement's own methods (refresh reader + map populate) — and decompile them.
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
import ghidra.program.model.scalar.Scalar;
import ghidra.program.model.symbol.ReferenceIterator;
import ghidra.util.task.ConsoleTaskMonitor;
import java.io.File;
import java.io.FileWriter;
import java.io.PrintWriter;
import java.util.ArrayList;
import java.util.LinkedHashMap;
import java.util.List;
import java.util.Map;

public class agree_loc2 extends GhidraScript {
    static final String OUT_DIR =
        "D:\\code\\unreal\\dragon ball kakarot access\\code\\decompiled";
    // property-param struct addresses from _agree_locate.txt (the string ptr sits at +0 of each)
    static final long[][] PARAMS = {
        {0x1454f05d8L, 0}, {0x1454f0648L, 1}, {0x1454f0708L, 2},
        {0x1454f0748L, 3}, {0x1454f0788L, 4}, {0x1454f07c8L, 5},
    };
    static final String[] PN = {"ImageTextureMap.A","ImageTextureMap.B","RightArrow","LeftArrow","HeadImage","SubjectImage"};

    Memory mem; FunctionManager fm; AddressSpace space; DecompInterface decomp; ConsoleTaskMonitor mon;

    long u64(Address a) throws Exception { byte[] b=new byte[8]; mem.getBytes(a,b);
        long v=0; for(int k=0;k<8;k++) v|=((long)(b[k]&0xff))<<(8*k); return v; }
    String asciiAt(long p){ try{ byte[] b=new byte[48]; mem.getBytes(space.getAddress(p),b);
        StringBuilder s=new StringBuilder(); for(byte c:b){ if(c==0)break; if(c<0x20||c>0x7e)return null; s.append((char)c);} return s.toString(); }catch(Exception e){return null;} }

    @Override public void run() throws Exception {
        new File(OUT_DIR).mkdirs();
        mem=currentProgram.getMemory(); fm=currentProgram.getFunctionManager();
        space=currentProgram.getAddressFactory().getDefaultAddressSpace();
        mon=new ConsoleTaskMonitor(); decomp=new DecompInterface(); decomp.openProgram(currentProgram);
        StringBuilder idx=new StringBuilder();

        // (1) raw dump of each param struct (0x60 bytes)
        idx.append("==== FProperty param structs (raw) ====\n");
        for(long[] p:PARAMS){ long addr=p[0]; String nm=PN[(int)p[1]];
            idx.append(String.format("-- %s @0x%x --%n", nm, addr));
            byte[] raw=new byte[0x60]; mem.getBytes(space.getAddress(addr),raw);
            StringBuilder hex=new StringBuilder();
            for(int i=0;i<0x60;i++){ hex.append(String.format("%02x ",raw[i]&0xff)); if((i&0xf)==0xf){ idx.append("   +0x"+Integer.toHexString(i-0xf)+": "+hex+"\n"); hex.setLength(0);} }
            // interpret qwords
            for(int i=0;i<0x60;i+=8){ long q=0; for(int k=0;k<8;k++) q|=((long)(raw[i+k]&0xff))<<(8*k);
                String note=""; String s=asciiAt(q); if(s!=null&&s.length()>=2) note=" -> \""+s+"\"";
                idx.append(String.format("     [+0x%02x] 0x%x%s%n", i, q, note)); }
        }

        // (2) code refs to the param array start + each param
        idx.append("\n==== code xrefs to param structs ====\n");
        for(long[] p:PARAMS){ Address a=space.getAddress(p[0]);
            ReferenceIterator ri=currentProgram.getReferenceManager().getReferencesTo(a); int n=0;
            while(ri.hasNext()&&n<12){ Address fr=ri.next().getFromAddress(); Function fn=fm.getFunctionContaining(fr);
                idx.append(String.format("  %s: xref@0x%x%s%n", PN[(int)p[1]], fr.getOffset(),
                    fn!=null?(" in 0x"+Long.toHexString(fn.getEntryPoint().getOffset())):"")); n++; } }

        // (3) brute scan: 0x508 AND 0x3a8 AND 0x3b0 all present
        idx.append("\n==== funcs referencing 0x508 & 0x3a8 & 0x3b0 ====\n");
        Map<Long,int[]> hit=new LinkedHashMap<>();
        FunctionIterator it=fm.getFunctions(true);
        while(it.hasNext()){ Function f=it.next();
            boolean a508=false,a3a8=false,a3b0=false,a3c0=false,a3c8=false;
            InstructionIterator ii=currentProgram.getListing().getInstructions(f.getBody(),true); int c=0;
            while(ii.hasNext()&&c<6000){ Instruction ins=ii.next(); c++;
                for(int op=0;op<ins.getNumOperands();op++) for(Object o:ins.getOpObjects(op)) if(o instanceof Scalar){ long v=((Scalar)o).getUnsignedValue();
                    if(v==0x508)a508=true; else if(v==0x3a8)a3a8=true; else if(v==0x3b0)a3b0=true; else if(v==0x3c0)a3c0=true; else if(v==0x3c8)a3c8=true; } }
            if(a508&&a3a8&&a3b0) hit.put(f.getEntryPoint().getOffset(), new int[]{a3c0?1:0,a3c8?1:0});
        }
        idx.append(hit.size()+" match\n");
        for(Map.Entry<Long,int[]> e:hit.entrySet())
            idx.append(String.format("  0x%x  arrows(3c0=%d,3c8=%d)%n", e.getKey(), e.getValue()[0], e.getValue()[1]));

        PrintWriter iw=new PrintWriter(new FileWriter(new File(OUT_DIR,"_agree_loc2.txt"))); iw.print(idx.toString()); iw.close();

        PrintWriter pw=new PrintWriter(new FileWriter(new File(OUT_DIR,"agree_loc2.c")));
        pw.println("// Agreement methods: reference 0x508(map)+0x3a8(Subject)+0x3b0(Head).");
        for(Long a:hit.keySet()){ Function f=fm.getFunctionAt(space.getAddress(a)); if(f==null)continue;
            pw.println("// ======== 0x"+Long.toHexString(a)+"  arrows(3c0="+hit.get(a)[0]+",3c8="+hit.get(a)[1]+") ========");
            try{ DecompileResults r=decomp.decompileFunction(f,120,mon);
                pw.println((r!=null&&r.decompileCompleted())?r.getDecompiledFunction().getC():"// <fail>"); }
            catch(Exception ex){ pw.println("// <err> "+ex); }
            pw.println(); }
        pw.close();
        println("LOC2: done, "+hit.size()+" methods");
    }
}
