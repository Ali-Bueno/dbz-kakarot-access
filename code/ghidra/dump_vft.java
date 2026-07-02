// Dump vftable symbol names (and any UI-class ones) to see how RTTI named them.
//@category Kakarot
import ghidra.app.script.GhidraScript;
import ghidra.program.model.symbol.Symbol;
import ghidra.program.model.symbol.SymbolIterator;

public class dump_vft extends GhidraScript {
    @Override
    public void run() throws Exception {
        SymbolIterator it = currentProgram.getSymbolTable().getAllSymbols(true);
        int vf = 0, shown = 0, uiHits = 0;
        StringBuilder ui = new StringBuilder();
        StringBuilder sample = new StringBuilder();
        while (it.hasNext()) {
            Symbol s = it.next();
            String n = s.getName(true);
            if (n.contains("vftable")) {
                vf++;
                if (shown < 40) { sample.append("   ").append(s.getAddress()).append("  ").append(n).append("\n"); shown++; }
                if (n.contains("_UI") || n.contains("Pause") || n.contains("Menu")
                        || n.contains("StartTop") || n.contains("Gameover") || n.contains("XCmn")) {
                    uiHits++;
                    if (uiHits <= 60) ui.append("   ").append(s.getAddress()).append("  ").append(n).append("\n");
                }
            }
        }
        println("DUMPVFT: total vftable symbols = " + vf);
        println("DUMPVFT: UI-ish vftables = " + uiHits);
        println("DUMPVFT: --- first 40 vftable names ---\n" + sample.toString());
        println("DUMPVFT: --- UI-ish vftable names ---\n" + ui.toString());
    }
}
