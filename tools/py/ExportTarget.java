// Ghidra headless export: dumps segment/blocks, functions, and per-instruction
// detail for a DOS EXE into a line-oriented text format that mzdump.py parses.
//
// Usage (analyzeHeadless): -postScript ExportTarget.java <programName> <outBase>
//
// Format:
//   @META key value
//   @BLOCK name start end rwx init
//   @XREF from to kind                 (memory/data references, whole program)
//   @FUNC entry end name size nbodies
//   @INSTR addr size mnemonic ref0 ref1 ... | bytes
//   @REF  addr kind targetAddr symbolName
//   @STR  addr len text

import java.io.*;
import java.util.*;

import ghidra.app.script.GhidraScript;
import ghidra.program.model.address.Address;
import ghidra.program.model.listing.*;
import ghidra.program.model.mem.MemoryBlock;
import ghidra.program.model.symbol.*;

public class ExportTarget extends GhidraScript {

    private PrintWriter out;

    @Override
    public void run() throws Exception {
        String[] args = getScriptArgs();
        String progName = args.length > 0 ? args[0] : "prog";
        String outBase = args.length > 1 ? args[1] : "target";

        File f = new File(outBase + ".txt");
        out = new PrintWriter(new BufferedWriter(new FileWriter(f)));
        try {
            dumpMeta(progName);
            dumpBlocks();
            dumpStrings();
            dumpFunctions();
        } finally {
            out.close();
        }
        println("ExportTarget: wrote " + f.getAbsolutePath());
    }

    private void p(String s) {
        out.println(s);
    }

    private static String esc(String s) {
        if (s == null)
            return "";
        StringBuilder b = new StringBuilder();
        for (char c : s.toCharArray()) {
            if (c == '\t')
                b.append("\\t");
            else if (c == '\n')
                b.append("\\n");
            else if (c == '\r')
                b.append("\\r");
            else if (c < 32 || c > 126)
                b.append('.');
            else
                b.append(c);
        }
        return b.toString();
    }

    private void dumpMeta(String progName) {
        p("@META program " + esc(progName));
        p("@META language " + esc(currentProgram.getLanguageID().getIdAsString()));
        p("@META compiler " + esc(currentProgram.getCompilerSpec().getCompilerSpecID().getIdAsString()));
        p("@META imageBase " + currentProgram.getImageBase().getOffset());
        p("@META format " + esc(currentProgram.getExecutableFormat()));
        long mn = currentProgram.getMinAddress().getOffset();
        long mx = currentProgram.getMaxAddress().getOffset();
        p("@META minAddr " + mn);
        p("@META maxAddr " + mx);
    }

    private void dumpBlocks() {
        for (MemoryBlock b : currentProgram.getMemory().getBlocks()) {
            p("@BLOCK " + esc(b.getName())
                    + " 0x" + Long.toHexString(b.getStart().getOffset())
                    + " 0x" + Long.toHexString(b.getEnd().getOffset())
                    + " " + b.isRead() + b.isWrite() + b.isExecute()
                    + " " + b.isInitialized());
        }
    }

    private void dumpStrings() {
        Listing l = currentProgram.getListing();
        DataIterator it = l.getDefinedData(true);
        int n = 0;
        while (it.hasNext()) {
            Data d = it.next();
            if (d.hasStringValue()) {
                String v = d.getDefaultValueRepresentation();
                if (v != null) {
                    p("@STR 0x" + Long.toHexString(d.getAddress().getOffset())
                            + " " + d.getLength()
                            + " " + esc(v));
                    n++;
                }
            }
            if (n > 20000)
                break;
        }
    }

    private void dumpFunctions() throws Exception {
        FunctionManager fm = currentProgram.getFunctionManager();
        Listing l = currentProgram.getListing();
        int count = 0;
        for (Function fn : fm.getFunctions(true)) {
            Address entry = fn.getEntryPoint();
            Address bodyMax = fn.getBody().getMaxAddress();
            p("@FUNC 0x" + Long.toHexString(entry.getOffset())
                    + " 0x" + Long.toHexString(bodyMax.getOffset() + 1)
                    + " " + esc(fn.getName())
                    + " " + fn.getBody().getNumAddresses()
                    + " " + (fn.isThunk() ? 1 : 0));

            InstructionIterator ii = l.getInstructions(fn.getBody(), true);
            while (ii.hasNext()) {
                Instruction ins = ii.next();
                StringBuilder sb = new StringBuilder();
                sb.append("@INSTR 0x").append(Long.toHexString(ins.getAddress().getOffset()));
                sb.append(' ').append(ins.getLength());
                sb.append(' ').append(esc(ins.getMnemonicString()));
                for (int i = 0; i < ins.getNumOperands(); i++) {
                    sb.append(' ').append(esc(ins.getDefaultOperandRepresentation(i)));
                }
                sb.append(" | ");
                byte[] bs = ins.getBytes();
                for (byte b : bs)
                    sb.append(String.format("%02x", b & 0xff));
                p(sb.toString());

                for (Reference r : ins.getReferencesFrom()) {
                    Symbol s = currentProgram.getSymbolTable().getPrimarySymbol(r.getToAddress());
                    p("@REF 0x" + Long.toHexString(ins.getAddress().getOffset())
                            + " " + r.getReferenceType()
                            + " 0x" + Long.toHexString(r.getToAddress().getOffset())
                            + " " + esc(s == null ? "" : s.getName()));
                }
                if (!ins.getFlowType().hasFallthrough()) {
                    p("@REF 0x" + Long.toHexString(ins.getAddress().getOffset())
                            + " NOFLOW - -");
                }
                count++;
            }
            p("@ENDFUNC");
        }
        p("@TOTALFUNCS " + fm.getFunctionCount());
        p("@TOTALINSTR " + count);
    }
}