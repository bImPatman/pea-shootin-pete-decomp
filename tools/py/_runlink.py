import os, importlib.util
spec = importlib.util.spec_from_file_location("dosbox", os.path.join(os.path.dirname(os.path.abspath(__file__)), "dosbox.py"))
d = importlib.util.module_from_spec(spec); spec.loader.exec_module(d)
rsp = open(os.path.join(d.WORK, "PETE.RSP"), "rb").read().decode("latin1")
# strip the MAP output: "..., EXE, MAP, LIB" -> "..., EXE, NUL, LIB"
rsp_nomap = rsp.replace("PETE.MAP", "NUL")
open(os.path.join(d.WORK, "NOMAP.RSP"), "wb").write(rsp_nomap.encode("latin1"))
bat = "cd \\WORK\r\ntlink @NOMAP.RSP >> C:\\OUT\\LOG.TXT\r\nexit\r\n"
open(os.path.join(d.WORK, "B3.BAT"), "wb").write(bat.encode("latin1"))
rc, log = d.run_batch("B3.BAT", timeout=90, quiet=False)
print("rc", rc)
print("=== tlink output ===")
print(log)
print("=== OUT ===")
for f in sorted(os.listdir(d.OUT)):
    print(f, os.path.getsize(os.path.join(d.OUT, f)))
