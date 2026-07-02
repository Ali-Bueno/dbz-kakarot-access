// Ghidra headless PRE-script (Java — Ghidra 12 dropped Jython, Python needs PyGhidra).
// Disables analyzers that decode embedded images via native awt.dll (ImageIO),
// which crashes the JVM (EXCEPTION_ACCESS_VIOLATION in awt.dll) on this exe's
// malformed embedded PNG data. Irrelevant to code analysis. Self-verifying.
//@category Kakarot
import ghidra.app.script.GhidraScript;
import java.util.Map;

public class disable_media extends GhidraScript {
    @Override
    public void run() throws Exception {
        Map<String, String> opts = getCurrentAnalysisOptionsAndValues(currentProgram);
        println("PRESCRIPT: " + opts.size() + " analysis options present");
        for (String n : opts.keySet()) {
            String l = n.toLowerCase();
            if (l.contains("media") || l.contains("png") || l.contains("image")) {
                println("PRESCRIPT: media-ish option -> '" + n + "' = " + opts.get(n));
            }
        }
        String[] cands = {"Embedded Media", "PNG resource", "GIF resource", "JPEG resource"};
        for (String name : cands) {
            try {
                setAnalysisOption(currentProgram, name, "false");
            } catch (Exception e) {
                println("PRESCRIPT: error disabling " + name + ": " + e);
            }
        }
        Map<String, String> opts2 = getCurrentAnalysisOptionsAndValues(currentProgram);
        println("PRESCRIPT: 'Embedded Media' now = " + opts2.get("Embedded Media"));
        println("PRESCRIPT: done");
    }
}
