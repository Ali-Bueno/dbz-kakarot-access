// Locate the attribute-component vtable(s) by finding a known attribute-component method
// (FUN_140f3c120, a stat forwarder) inside a vtable; report the vtable base and the
// function at base+0x3E8 (the level getter that the aura power-compare calls). Decompile
// that getter AND, if it forwards to a StatusInstance vtable slot [[this+0x100]+S], find
// the StatusInstance vtable (one that also holds the stat forwarders' leaves) and decompile
// its slot S leaf to reveal the raw level offset.
// Output -> decompiled/_lvl_getter.txt
//@category Kakarot
import ghidra.app.script.GhidraScript;
import ghidra.app.decompiler.DecompInterface;
import ghidra.app.decompiler.DecompileResults;
import ghidra.program.model.address.Address;
import ghidra.program.model.listing.Function;
import ghidra.program.model.listing.FunctionManager;
import ghidra.program.model.mem.Memory;
import ghidra.program.model.mem.MemoryBlock;
import ghidra.util.task.ConsoleTaskMonitor;
import java.io.File;
import java.io.FileWriter;
import java.io.PrintWriter;
import java.util.ArrayList;
import java.util.List;

public class find_lvl_getter extends GhidraScript {
    static final String OUT_DIR =
        "D:\\code\\unreal\\dragon ball kakarot access\\code\\decompiled";
    // a known attribute-component vtable method (forwards to StatusInstance)
    static final long KNOWN_ATTR_METHOD = 0x140f3c120L;

    Memory mem; long textStart, textEnd, rdataStart; byte[] rd;
    boolean inText(long a){ return a>=textStart && a<textEnd; }
    long u64(byte[] b,int i){ long v=0; for(int k=0;k<8;k++) v|=((long)(b[i+k]&0xff))<<(8*k); return v; }

    FunctionManager fm; DecompInterface decomp; ConsoleTaskMonitor mon; PrintWriter pw;

    String dec(long addr){
        Address a = currentProgram.getAddressFactory().getDefaultAddressSpace().getAddress(addr);
        Function f = fm.getFunctionAt(a);
        if (f==null){ try { f=createFunction(a,null); } catch(Exception e){} }
        if (f==null) return "// no func @ 0x"+Long.toHexString(addr);
        try { DecompileResults r=decomp.decompileFunction(f,120,mon);
            return (r!=null&&r.decompileCompleted())?r.getDecompiledFunction().getC():"// <fail>"; }
        catch(Exception e){ return "// <err>"; }
    }

    @Override
    public void run() throws Exception {
        new File(OUT_DIR).mkdirs();
        mem=currentProgram.getMemory(); fm=currentProgram.getFunctionManager();
        MemoryBlock tx=mem.getBlock(".text"); textStart=tx.getStart().getOffset(); textEnd=tx.getEnd().getOffset();
        MemoryBlock r=mem.getBlock(".rdata"); rdataStart=r.getStart().getOffset();
        rd=new byte[(int)r.getSize()]; r.getBytes(r.getStart(),rd);
        decomp=new DecompInterface(); decomp.openProgram(currentProgram); mon=new ConsoleTaskMonitor();
        pw=new PrintWriter(new FileWriter(new File(OUT_DIR,"_lvl_getter.txt")));

        // find vtables containing KNOWN_ATTR_METHOD
        List<Long> vbases=new ArrayList<>();
        for (int i=0;i+8<=rd.length;i+=8){
            if (u64(rd,i)==KNOWN_ATTR_METHOD){
                // scan back to vtable start (first .text ptr with non-.text before it)
                int start=i;
                while (start-8>=0 && inText(u64(rd,start-8))) start-=8;
                long vbase=rdataStart+start;
                vbases.add(vbase);
                long slotOfKnown=(long)(i-start);
                pw.println("// vtable base 0x"+Long.toHexString(vbase)+"  (known method at slot 0x"+Long.toHexString(slotOfKnown)+")");
                // read slot 0x3E8
                if (start+0x3E8+8<=rd.length){
                    long lvlGetter=u64(rd,start+0x3E8);
                    pw.println("//   slot0x3E8 -> 0x"+Long.toHexString(lvlGetter)+(inText(lvlGetter)?"":" (not text)"));
                    if (inText(lvlGetter)){
                        pw.println("// ===== LEVEL GETTER (attrib vtable+0x3E8) 0x"+Long.toHexString(lvlGetter)+" =====");
                        String c=dec(lvlGetter);
                        pw.println(c);
                        // if it forwards to [[this+0x100]+S], find S and resolve on this same vtable's StatusInstance?
                        // the StatusInstance vtable = *(StatusInstance); we can't know statically here, but print the forward slot.
                        java.util.regex.Matcher m=java.util.regex.Pattern.compile("\\+ 0x100\\) \\+ (0x[0-9a-f]+)\\)|\\+ (0x[0-9a-f]+)\\)\\(\\)").matcher(c);
                        pw.println();
                    }
                }
                // also print a handful of neighbor slots around 0x3E8 for context
            }
        }
        pw.println("// vtables containing known attr method: "+vbases.size());
        pw.close();
        println("LVLGET: done vtables="+vbases.size());
    }
}
