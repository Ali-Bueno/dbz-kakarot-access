// Dump the UAT_UIXcmnAgreement FClassParams + FClassFunctionLinkInfo array (2026-07-17).
// Property pointer array @0x14407ac80; FClassParams field @0x14407ad18 points to it.
// Dump .rdata [0x14407ab00 .. 0x14407af00] resolving each qword to string / code(func)
// so we read: the function-link array {Z_Construct_UFunction*, "FuncName"} = the class's
// REAL UFunction set, and the ClassNoRegisterFunc = GetPrivateStaticClass (which calls
// StaticRegisterNatives with the {name,exec} table). Then decompile every code pointer
// found in the region + 1-level callees (to reach StaticRegisterNatives / the impls).
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
import ghidra.util.task.ConsoleTaskMonitor;
import java.io.File;
import java.io.FileWriter;
import java.io.PrintWriter;
import java.util.LinkedHashSet;
import java.util.Set;

public class agree_dump extends GhidraScript {
    static final String OUT_DIR =
        "D:\\code\\unreal\\dragon ball kakarot access\\code\\decompiled";
    static final long FROM=0x14407ab00L, TO=0x14407af00L;

    Memory mem; FunctionManager fm; AddressSpace space; DecompInterface decomp; ConsoleTaskMonitor mon;
    long u64(Address a) throws Exception { byte[] b=new byte[8]; mem.getBytes(a,b); long v=0; for(int k=0;k<8;k++) v|=((long)(b[k]&0xff))<<(8*k); return v; }
    String asciiAt(long p){ try{ byte[] b=new byte[80]; mem.getBytes(space.getAddress(p),b); StringBuilder s=new StringBuilder();
        for(byte c:b){ if(c==0)return s.length()>=1?s.toString():null; if(c<0x20||c>0x7e)return null; s.append((char)c);} }catch(Exception e){} return null; }
    boolean inText(long p){ try{ MemoryBlock b=mem.getBlock(space.getAddress(p)); return b!=null&&b.isExecute(); }catch(Exception e){return false;} }

    @Override public void run() throws Exception {
        new File(OUT_DIR).mkdirs();
        mem=currentProgram.getMemory(); fm=currentProgram.getFunctionManager();
        space=currentProgram.getAddressFactory().getDefaultAddressSpace();
        mon=new ConsoleTaskMonitor(); decomp=new DecompInterface(); decomp.openProgram(currentProgram);
        StringBuilder idx=new StringBuilder();
        Set<Long> codeFns=new LinkedHashSet<>();

        idx.append("== .rdata dump [0x"+Long.toHexString(FROM)+"..0x"+Long.toHexString(TO)+"] ==\n");
        for(long a=FROM;a<TO;a+=8){ long q; try{ q=u64(space.getAddress(a)); }catch(Exception e){ continue; }
            String note=""; String s=asciiAt(q); if(s!=null&&s.length()>=2) note=" \""+s+"\"";
            if(inText(q)){ note+=" <CODE"; Function f=fm.getFunctionAt(space.getAddress(q)); if(f==null){ try{ f=createFunction(space.getAddress(q),null);}catch(Exception e){} }
                if(f!=null){ note+=" fn@0x"+Long.toHexString(f.getEntryPoint().getOffset()); codeFns.add(f.getEntryPoint().getOffset()); } note+=">"; }
            idx.append(String.format("  0x%x: 0x%x%s%n", a, q, note));
        }

        // decompile code pointers found + 1-level callees (reach StaticRegisterNatives + impls)
        Set<Long> dec=new LinkedHashSet<>(codeFns);
        for(long z:codeFns){ Function f=fm.getFunctionAt(space.getAddress(z)); if(f==null)continue;
            InstructionIterator ii=currentProgram.getListing().getInstructions(f.getBody(),true);
            while(ii.hasNext()){ Instruction ins=ii.next(); if(ins.getMnemonicString().toLowerCase().startsWith("call"))
                for(Reference r:ins.getReferencesFrom()){ Function g=fm.getFunctionAt(r.getToAddress()); if(g!=null&&dec.size()<80) dec.add(r.getToAddress().getOffset()); } } }

        PrintWriter pw=new PrintWriter(new FileWriter(new File(OUT_DIR,"agree_dump.c")));
        for(long a:dec){ Function f=fm.getFunctionAt(space.getAddress(a)); if(f==null)continue;
            pw.println("// ======== 0x"+Long.toHexString(a)+" ========");
            try{ DecompileResults r=decomp.decompileFunction(f,120,mon); pw.println((r!=null&&r.decompileCompleted())?r.getDecompiledFunction().getC():"// <fail>"); }
            catch(Exception ex){ pw.println("// <err> "+ex); } pw.println(); }
        pw.close();
        PrintWriter iw=new PrintWriter(new FileWriter(new File(OUT_DIR,"_agree_dump.txt"))); iw.print(idx.toString()); iw.close();
        println("DUMP: done code="+codeFns.size());
    }
}
