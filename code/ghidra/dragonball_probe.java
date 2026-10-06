// Multi-query probe for the Dragon Ball RE pass. Arg: path to a command file; output path = cmdfile + ".out".
// Commands (one per line, '#' = comment):
//   ufunc NAME            registration-table entries {name, exec} -> decompile exec + its direct call targets
//   dec ADDR              decompile the function containing ADDR
//   callers ADDR          list references to ADDR (from-address + containing function)
//   callersdec ADDR       same, and decompile each distinct calling function
//   calls ADDR            list call targets inside the function containing ADDR
//   str TEXT              ASCII + UTF-16LE occurrences of TEXT and the functions referencing them
//   disp HEX ADDR         instructions in the function at ADDR whose operand uses displacement HEX
//   dispr HEX LO HI       same, over every function with entry in [LO,HI)
//   dref ADDR             8-byte pointers to ADDR in .rdata/.data (vtable slots, tables)
//   qw ADDR N             dump N qwords at ADDR, naming function targets
//   asm ADDR              disassembly of the function containing ADDR
//   tbl ADDR N            N native-registration rows {name, exec} starting at ADDR
//   scal LO HI V...       instructions in [LO,HI) with any scalar operand equal to one of V...
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
import ghidra.program.model.symbol.ReferenceIterator;
import ghidra.util.task.ConsoleTaskMonitor;
import java.io.*;
import java.nio.file.Files;
import java.util.*;

public class dragonball_probe extends GhidraScript {
    Memory mem; FunctionManager fm; AddressSpace space; DecompInterface decomp;
    ConsoleTaskMonitor mon; PrintWriter out;
    byte[][] blocks; long[] blockStart;
    Set<Address> decompiled = new HashSet<>();

    long u64(byte[] b, int i) { long v = 0; for (int k = 0; k < 8; k++) v |= ((long)(b[i+k] & 0xff)) << (8*k); return v; }
    Address A(String s) { return space.getAddress(Long.parseLong(s.replaceFirst("^0x", ""), 16)); }
    Function fn(Address a) {
        Function f = fm.getFunctionContaining(a);
        if (f == null) { try { f = createFunction(a, null); } catch (Exception e) {} }
        return f;
    }

    @Override
    public void run() throws Exception {
        mem = currentProgram.getMemory(); fm = currentProgram.getFunctionManager();
        space = currentProgram.getAddressFactory().getDefaultAddressSpace();
        mon = new ConsoleTaskMonitor(); decomp = new DecompInterface(); decomp.openProgram(currentProgram);
        List<byte[]> bl = new ArrayList<>(); List<Long> bs = new ArrayList<>();
        for (String bn : new String[]{".rdata", ".data"}) {
            MemoryBlock b = mem.getBlock(bn); if (b == null) continue;
            byte[] arr = new byte[(int) b.getSize()]; b.getBytes(b.getStart(), arr);
            bl.add(arr); bs.add(b.getStart().getOffset());
        }
        blocks = bl.toArray(new byte[0][]); blockStart = new long[bs.size()];
        for (int i = 0; i < bs.size(); i++) blockStart[i] = bs.get(i);

        String cmdPath = String.join(" ", getScriptArgs()).trim();
        out = new PrintWriter(new FileWriter(cmdPath + ".out"));
        for (String line : Files.readAllLines(new File(cmdPath).toPath())) {
            line = line.trim(); if (line.isEmpty() || line.startsWith("#")) continue;
            String[] t = line.split("\\s+", 2);
            String arg = t.length > 1 ? t[1] : "";
            out.println("\n######## " + line); out.flush();
            try {
                switch (t[0]) {
                    case "ufunc": ufunc(arg); break;
                    case "dec": dec(A(arg)); break;
                    case "callers": callers(A(arg), false); break;
                    case "callersdec": callers(A(arg), true); break;
                    case "calls": calls(A(arg)); break;
                    case "str": str(arg); break;
                    case "disp": { String[] p = arg.split("\\s+"); disp(Long.parseLong(p[0].replaceFirst("^0x",""),16), fn(A(p[1]))); break; }
                    case "dispr": { String[] p = arg.split("\\s+"); long d = Long.parseLong(p[0].replaceFirst("^0x",""),16);
                        long lo = A(p[1]).getOffset(), hi = A(p[2]).getOffset();
                        for (Function f : fm.getFunctions(A(p[1]), true)) { if (f.getEntryPoint().getOffset() >= hi) break; disp(d, f); } break; }
                    case "dref": dref(A(arg).getOffset()); break;
                    case "qw": { String[] p = arg.split("\\s+"); qw(A(p[0]), Integer.decode(p[1])); break; }
                    case "asm": asm(A(arg)); break;
                    case "scal": { String[] p = arg.split("\\s+"); Set<Long> vs = new HashSet<>();
                        for (int i = 2; i < p.length; i++) vs.add(Long.parseLong(p[i].replaceFirst("^0x",""),16));
                        scal(A(p[0]), A(p[1]), vs); break; }
                    case "tbl": { String[] p = arg.split("\\s+"); tbl(A(p[0]), Integer.decode(p[1])); break; }
                    default: out.println("?? unknown command");
                }
            } catch (Exception e) { out.println("!! error " + e); }
            out.flush();
        }
        out.close(); println("DBPROBE: done");
    }

    void dec(Address a) {
        Function f = fn(a);
        if (f == null) { out.println("no function at " + a); return; }
        if (!decompiled.add(f.getEntryPoint())) { out.println("(already decompiled above: " + f.getName() + ")"); return; }
        String c;
        try { DecompileResults r = decomp.decompileFunction(f, 240, mon);
            c = (r != null && r.decompileCompleted()) ? r.getDecompiledFunction().getC() : "// <decompile failed>"; }
        catch (Exception e) { c = "// <decompile error> " + e; }
        out.println("// ---- " + f.getName() + " @ " + f.getEntryPoint() + " size=0x" + Long.toHexString(f.getBody().getNumAddresses()));
        out.println(c);
    }

    List<Address> callTargets(Function f) {
        List<Address> r = new ArrayList<>();
        InstructionIterator ii = currentProgram.getListing().getInstructions(f.getBody(), true);
        while (ii.hasNext()) { Instruction ins = ii.next();
            if (!ins.getFlowType().isCall()) continue;
            for (Reference ref : ins.getReferencesFrom()) {
                if (ref.getReferenceType().isCall()) r.add(ref.getToAddress()); } }
        return r;
    }

    void ufunc(String name) throws Exception {
        byte[] needle = (name + "\0").getBytes("ISO-8859-1");
        Address from = mem.getMinAddress(); int n = 0;
        while (n < 40) {
            Address hit = mem.findBytes(from, needle, null, true, mon);
            if (hit == null) break; n++; from = hit.add(1);
            // require a NUL (or start) before the name so "XGetDragonball" does not match "GetDragonball"
            try { byte prev = mem.getByte(hit.subtract(1)); if (prev != 0) continue; } catch (Exception e) {}
            long target = hit.getOffset();
            for (int bi = 0; bi < blocks.length; bi++) { byte[] arr = blocks[bi];
                for (int i = 0; i + 16 <= arr.length; i += 8) {
                    if (u64(arr, i) != target) continue;
                    long exec = u64(arr, i + 8); Address ea;
                    try { ea = space.getAddress(exec); } catch (Exception e) { continue; }
                    if (mem.getBlock(ea) == null || !mem.getBlock(ea).isExecute()) continue;
                    out.println("table @0x" + Long.toHexString(blockStart[bi] + i) + " name@" + hit + " exec -> " + ea);
                    Function f = fn(ea); if (f == null) continue;
                    dec(ea);
                    for (Address c : callTargets(f)) { Function g = fm.getFunctionAt(c);
                        if (g != null && g.getBody().getNumAddresses() > 0x30) { out.println("  -> call " + c); dec(c); } }
                } }
        }
    }

    void callers(Address a, boolean decompile) {
        ReferenceIterator it = currentProgram.getReferenceManager().getReferencesTo(a);
        Set<Address> fs = new LinkedHashSet<>(); int n = 0;
        while (it.hasNext() && n < 400) { Reference r = it.next(); n++;
            Function f = fm.getFunctionContaining(r.getFromAddress());
            out.println("  ref from " + r.getFromAddress() + " (" + r.getReferenceType() + ") in " + (f == null ? "<none>" : f.getName() + "@" + f.getEntryPoint()));
            if (f != null) fs.add(f.getEntryPoint()); }
        // data pointers too (vtables)
        dref(a.getOffset());
        if (decompile) for (Address f : fs) dec(f);
    }

    void calls(Address a) {
        Function f = fn(a); if (f == null) return;
        for (Address c : callTargets(f)) { Function g = fm.getFunctionAt(c);
            out.println("  call " + c + " " + (g == null ? "" : g.getName() + " size=0x" + Long.toHexString(g.getBody().getNumAddresses()))); }
    }

    void str(String text) throws Exception {
        for (int enc = 0; enc < 2; enc++) {
            byte[] needle = enc == 0 ? (text + "\0").getBytes("ISO-8859-1") : (text + "\0").getBytes("UTF-16LE");
            Address from = mem.getMinAddress(); int n = 0;
            while (n < 20) {
                Address hit = mem.findBytes(from, needle, null, true, mon);
                if (hit == null) break; n++; from = hit.add(1);
                out.println((enc == 0 ? "ascii " : "utf16 ") + hit);
                ReferenceIterator it = currentProgram.getReferenceManager().getReferencesTo(hit);
                while (it.hasNext()) { Reference r = it.next(); Function f = fm.getFunctionContaining(r.getFromAddress());
                    out.println("   xref " + r.getFromAddress() + " in " + (f == null ? "<none>" : f.getName() + "@" + f.getEntryPoint())); }
                dref(hit.getOffset());
            }
        }
    }

    void disp(long d, Function f) {
        if (f == null) return;
        InstructionIterator ii = currentProgram.getListing().getInstructions(f.getBody(), true);
        while (ii.hasNext()) { Instruction ins = ii.next();
            boolean hit = false;
            for (int op = 0; op < ins.getNumOperands() && !hit; op++) {
                for (Object o : ins.getOpObjects(op)) {
                    if (o instanceof Scalar && ((Scalar) o).getUnsignedValue() == d && ins.toString().contains("[")) { hit = true; break; } } }
            if (hit) out.println("  " + ins.getAddress() + "  " + ins + "    ; in " + f.getName() + "@" + f.getEntryPoint());
        }
    }

    // any scalar operand (immediate or displacement) equal to one of vs, over instructions in [lo,hi)
    void scal(Address lo, Address hi, Set<Long> vs) {
        InstructionIterator ii = currentProgram.getListing().getInstructions(lo, true);
        int n = 0;
        while (ii.hasNext() && n < 2000) { Instruction ins = ii.next();
            if (ins.getAddress().compareTo(hi) >= 0) break;
            boolean hit = false;
            for (int op = 0; op < ins.getNumOperands() && !hit; op++)
                for (Object o : ins.getOpObjects(op))
                    if (o instanceof Scalar && vs.contains(((Scalar) o).getUnsignedValue())) { hit = true; break; }
            if (!hit) continue; n++;
            Function f = fm.getFunctionContaining(ins.getAddress());
            out.println("  " + ins.getAddress() + "  " + ins + "    ; in " + (f == null ? "<none>" : f.getName() + "@" + f.getEntryPoint()));
        }
    }

    void dref(long target) {
        for (int bi = 0; bi < blocks.length; bi++) { byte[] arr = blocks[bi];
            for (int i = 0; i + 8 <= arr.length; i += 8)
                if (u64(arr, i) == target) out.println("  dataptr @0x" + Long.toHexString(blockStart[bi] + i)); }
    }

    void qw(Address a, int n) throws Exception {
        for (int i = 0; i < n; i++) { long v = mem.getLong(a.add(8L * i)); String nm = "";
            try { Function g = fm.getFunctionAt(space.getAddress(v)); if (g != null) nm = g.getName(); } catch (Exception e) {}
            out.println(String.format("  +0x%03x  %016x %s", 8 * i, v, nm)); }
    }

    // native-registration table rows {const char* name, exec}
    void tbl(Address a, int n) throws Exception {
        for (int i = 0; i < n; i++) {
            long np = mem.getLong(a.add(16L * i)), ex = mem.getLong(a.add(16L * i + 8));
            String nm = "?";
            try { StringBuilder sb = new StringBuilder(); Address s = space.getAddress(np);
                for (int k = 0; k < 96; k++) { byte c = mem.getByte(s.add(k)); if (c == 0) break; sb.append((char) c); }
                nm = sb.toString(); } catch (Exception e) {}
            out.println(String.format("  @%s  %-48s exec %x", a.add(16L * i), nm, ex));
        }
    }

    void asm(Address a) {
        Function f = fn(a); if (f == null) return;
        InstructionIterator ii = currentProgram.getListing().getInstructions(f.getBody(), true);
        while (ii.hasNext()) { Instruction ins = ii.next(); out.println("  " + ins.getAddress() + "  " + ins); }
    }
}
