// Force the MSVC RTTI analyzer to run. Ghidra 12 did not auto-register it for
// this binary (SteamStub stripped the Rich header, so the PE wasn't flagged as
// MSVC strongly enough). Running it manually recovers vftables named by class,
// which export_ui.java then walks.
//@category Kakarot
import ghidra.app.script.GhidraScript;
import ghidra.app.plugin.prototype.MicrosoftCodeAnalyzerPlugin.RttiAnalyzer;
import ghidra.app.util.importer.MessageLog;
import ghidra.program.model.symbol.Symbol;
import ghidra.program.model.symbol.SymbolIterator;

public class force_rtti extends GhidraScript {
    @Override
    public void run() throws Exception {
        RttiAnalyzer a = new RttiAnalyzer();
        println("FORCE_RTTI: canAnalyze = " + a.canAnalyze(currentProgram));
        MessageLog log = new MessageLog();
        boolean ok;
        try {
            ok = a.added(currentProgram, currentProgram.getMemory(), monitor, log);
        } catch (Exception e) {
            println("FORCE_RTTI: added threw " + e);
            ok = false;
        }
        println("FORCE_RTTI: added returned " + ok);
        String ls = log.toString();
        if (ls != null && ls.length() > 0) {
            println("FORCE_RTTI log: " + ls.substring(0, Math.min(2000, ls.length())));
        }
        int vf = 0, col = 0, cls = 0;
        SymbolIterator it = currentProgram.getSymbolTable().getAllSymbols(true);
        while (it.hasNext()) {
            Symbol s = it.next();
            String n = s.getName();
            if (n.contains("vftable")) vf++;
            if (n.contains("RTTI") || n.contains("_R4") || n.contains("Locator")) col++;
        }
        println("FORCE_RTTI: vftable symbols now = " + vf + ", rtti-ish = " + col);
        println("FORCE_RTTI: done");
    }
}
