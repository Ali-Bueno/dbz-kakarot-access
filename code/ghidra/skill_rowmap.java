// Resolve UDataTable::GetRowMap() -> the byte offset of RowMap inside UDataTable.
// The game never touches RowMap directly: it calls a virtual at vtable+0x258 that
// returns a TSet*. Such a getter compiles to a tiny stub `lea rax,[rcx+imm]; ret`.
// So: find every `lea rax,[rcx+imm]; ret` stub, find .rdata pointers to it, and
// keep the ones that sit exactly 0x258 bytes into a run of code pointers (= slot 75
// of a vtable). The imm of the winner IS the RowMap offset.
//@category Kakarot
import ghidra.app.script.GhidraScript;
import ghidra.program.model.address.Address;
import ghidra.program.model.address.AddressSpace;
import ghidra.program.model.mem.Memory;
import ghidra.program.model.mem.MemoryBlock;
import java.io.File;
import java.io.FileWriter;
import java.io.PrintWriter;
import java.util.ArrayList;
import java.util.List;

public class skill_rowmap extends GhidraScript {
    static final String OUT_DIR =
        "D:\\code\\unreal\\dragon ball kakarot access\\code\\decompiled";
    static final long VSLOT = 0x258;

    Memory mem;
    AddressSpace space;

    long u64(byte[] b, int i) {
        long v = 0;
        for (int k = 0; k < 8; k++) v |= ((long)(b[i+k] & 0xff)) << (8*k);
        return v;
    }
    boolean inText(long p) {
        try { MemoryBlock b = mem.getBlock(space.getAddress(p)); return b != null && b.isExecute(); }
        catch (Exception e) { return false; }
    }

    @Override
    public void run() throws Exception {
        mem = currentProgram.getMemory();
        space = currentProgram.getAddressFactory().getDefaultAddressSpace();
        PrintWriter pw = new PrintWriter(new FileWriter(new File(OUT_DIR, "_skill_rowmap.txt")));

        // 1) collect `lea rax,[rcx+imm8]; ret`  = 48 8D 41 imm C3
        //    and `lea rax,[rcx+imm32]; ret`     = 48 8D 81 i i i i C3
        List<long[]> stubs = new ArrayList<>();   // {addr, imm}
        Address from = mem.getMinAddress();
        while (true) {
            Address hit = find(from, new byte[]{0x48, (byte)0x8D, 0x41});
            if (hit == null) break;
            from = hit.add(1);
            if (!inText(hit.getOffset())) continue;
            byte[] b = new byte[5];
            try { mem.getBytes(hit, b); } catch (Exception e) { continue; }
            if (b[4] != (byte)0xC3) continue;
            stubs.add(new long[]{hit.getOffset(), b[3] & 0xff});
        }
        from = mem.getMinAddress();
        while (true) {
            Address hit = find(from, new byte[]{0x48, (byte)0x8D, (byte)0x81});
            if (hit == null) break;
            from = hit.add(1);
            if (!inText(hit.getOffset())) continue;
            byte[] b = new byte[8];
            try { mem.getBytes(hit, b); } catch (Exception e) { continue; }
            if (b[7] != (byte)0xC3) continue;
            long imm = (b[3]&0xffL) | ((b[4]&0xffL)<<8) | ((b[5]&0xffL)<<16) | ((b[6]&0xffL)<<24);
            stubs.add(new long[]{hit.getOffset(), imm});
        }
        pw.println("lea rax,[rcx+imm]; ret stubs: " + stubs.size());

        // 2) cache .rdata / .data
        List<byte[]> bl = new ArrayList<>();
        List<Long> bs = new ArrayList<>();
        for (String bn : new String[]{".rdata", ".data"}) {
            MemoryBlock b = mem.getBlock(bn);
            if (b == null) continue;
            byte[] arr = new byte[(int) b.getSize()];
            b.getBytes(b.getStart(), arr);
            bl.add(arr); bs.add(b.getStart().getOffset());
        }

        // 3) for each stub, find qword pointers to it; test "is slot 0x258 of a vtable"
        for (long[] st : stubs) {
            for (int bi = 0; bi < bl.size(); bi++) {
                byte[] arr = bl.get(bi);
                long base = bs.get(bi);
                for (int i = 0; i + 8 <= arr.length; i += 8) {
                    if (u64(arr, i) != st[0]) continue;
                    int slotIdx = (int) (VSLOT / 8);          // 75
                    int vt = i - (int) VSLOT;
                    if (vt < 0) continue;
                    // every qword from vt..i must be a code pointer (a plausible vtable run)
                    boolean ok = true;
                    for (int j = vt; j <= i; j += 8) {
                        if (!inText(u64(arr, j))) { ok = false; break; }
                    }
                    if (!ok) continue;
                    // and the qword just BEFORE the vtable start must NOT be code
                    // (so vt really is the vtable's slot 0)
                    boolean startsHere = (vt - 8 < 0) || !inText(u64(arr, vt - 8));
                    pw.printf("CANDIDATE vtable 0x%x  slot%d(+0x%x) -> stub 0x%x  => returns this+0x%x  %s%n",
                        base + vt, slotIdx, VSLOT, st[0], st[1],
                        startsHere ? "[vtable STARTS here]" : "[run continues before]");
                }
            }
        }
        pw.close();
        println("ROWMAP: done");
    }
}
