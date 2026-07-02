// Diagnostic: what RTTI/class info did analysis actually produce, and what is
// the exact name of the RTTI analyzer option?
//@category Kakarot
import ghidra.app.script.GhidraScript;
import ghidra.program.model.symbol.Symbol;
import ghidra.program.model.symbol.SymbolIterator;
import ghidra.program.model.symbol.SymbolTable;
import ghidra.program.model.data.DataType;
import ghidra.program.model.data.DataTypeManager;
import java.util.Iterator;
import java.util.Map;

public class diag extends GhidraScript {
    @Override
    public void run() throws Exception {
        // 1) analysis option names of interest
        Map<String, String> opts = getCurrentAnalysisOptionsAndValues(currentProgram);
        println("DIAG: options mentioning rtti/class/vftable/vtable:");
        for (String n : opts.keySet()) {
            String l = n.toLowerCase();
            if (l.contains("rtti") || l.contains("vftable") || l.contains("vtable")
                    || l.contains("class") || l.contains("windows")) {
                println("   '" + n + "' = " + opts.get(n));
            }
        }

        // 2) symbol substring counts + examples
        String[] needles = {"vftable", "vtable", "RTTI", "XCmnPause", "Gameover",
                             "StartTop", "AT_UI", "MenuManager", "??_7", "type_info"};
        int[] counts = new int[needles.length];
        StringBuilder[] ex = new StringBuilder[needles.length];
        for (int i = 0; i < needles.length; i++) ex[i] = new StringBuilder();
        SymbolTable st = currentProgram.getSymbolTable();
        SymbolIterator it = st.getAllSymbols(true);
        int total = 0;
        while (it.hasNext()) {
            Symbol s = it.next();
            String nm = s.getName(true);
            total++;
            for (int i = 0; i < needles.length; i++) {
                if (nm.contains(needles[i])) {
                    counts[i]++;
                    if (counts[i] <= 6) ex[i].append("      ").append(s.getAddress())
                        .append("  ").append(nm).append("\n");
                }
            }
        }
        println("DIAG: total symbols = " + total);
        for (int i = 0; i < needles.length; i++) {
            println("DIAG: symbols containing '" + needles[i] + "' = " + counts[i]);
            if (ex[i].length() > 0) print(ex[i].toString());
        }

        // 3) data types matching key class names
        DataTypeManager dtm = currentProgram.getDataTypeManager();
        Iterator<DataType> dit = dtm.getAllDataTypes();
        int dtHit = 0;
        while (dit.hasNext()) {
            DataType dt = dit.next();
            String nm = dt.getName();
            if (nm.contains("XCmnPause") || nm.contains("Gameover") || nm.contains("StartTop")
                    || nm.contains("MenuManager")) {
                if (dtHit < 30) println("DIAG: datatype '" + nm + "'  (" + dt.getPathName() + ")");
                dtHit++;
            }
        }
        println("DIAG: matching datatypes = " + dtHit);
        println("DIAG: done");
    }
}
