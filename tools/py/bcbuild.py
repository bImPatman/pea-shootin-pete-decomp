"""Build C sources with Borland C++ 3.1 inside DOSBox X.

Produces, for each source file:
  <name>.asm   compiler assembly listing  (-S)  -- what the compiler actually emitted
  <name>.obj   object file                  (-c)
and for the whole program:
  <name>.exe    linked executable (+ map)

Flag handling:
  FLAGS holds the full, explicit Borland flag set.  Never rely on TURBOC.CFG /
  TLINK.CFG defaults: they are machine specific.  See FLAGS_NOTES for what each
  flag controls, because getting these wrong is the usual reason a rebuild does
  not match the original.
"""
import os, re, sys, json, shutil
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import dosbox

ROOT = dosbox.ROOT
CFG_PATH = os.path.join(ROOT, 'tools', 'py', 'flags.json')

# Flag set established by fingerprinting PETE.EXE (see notes/compiler.md).
DEFAULT = {
    "model":       "-ml",   # large: far data pointers (LES), per-module _TEXT segments
    "cpu":         "-1",    # 80186+ instructions (PETE.EXE uses ADD r/m,imm8, LEAVE, PUSH imm16)
    "optimize":    "-O2",   # only -O2 emits sub sp/leave: 100 target funcs use it, 0 use enter
    "codegen":     "-r-",   # -r = register variables (default on in BCC)
    "stackframe":  "",      # -k = standard stack frame (Borland "K" style)
    "alignment":   "",      # -a = word-align all functions
    "pascal":      "",      # -p = Pascal calling convention
    "stackchk":    "",      # -N = stack overflow checking
    "merge":       "",      # -d = merge duplicate strings
    "reload":      "",      # -Z = suppress register reloads
    "debug":       "",      # -v / -y = debug / line info (affects codegen, keep OFF)
    "asm":         "",      # -S is added separately when we want the listing
}

BANNER = "@echo off\r\ncd \\WORK\r\n"


def load_cfg():
    if os.path.exists(CFG_PATH):
        return json.load(open(CFG_PATH))
    return DEFAULT


def bcc_flags(cfg, want_asm=False):
    f = [cfg["model"], cfg["cpu"], cfg["optimize"], cfg["codegen"],
         cfg["stackframe"], cfg["alignment"], cfg["pascal"], cfg["stackchk"],
         cfg["merge"], cfg["reload"], cfg["debug"], cfg.get("extra", "")]
    if want_asm:
        f.append("-S")
    return " ".join(x for x in f if x)


# Recognised values per config slot, used by check.py's per-source `@flags`.
SLOT_VALUES = {
    "model":       ("-ms", "-mc", "-mm", "-ml", "-mh"),
    "cpu":         ("-1", "-2"),
    "optimize":    ("-O", "-O1", "-O2", "-Oz"),
    "codegen":     ("-r", "-r-"),
    "stackframe":  ("-s", "-s-"),
    "alignment":   ("-a", "-a-"),
    "pascal":      ("-p", "-p-"),
    "stackchk":    ("-C", "-C-"),
    "merge":       ("-M", "-M-"),
    "reload":      ("-R", "-R-"),
    "debug":       ("-d", "-d-"),
}


def apply_overrides(cfg, tokens):
    """Return a copy of cfg with recognised `@flags` tokens applied.

    A token that names a value of some slot replaces that slot; anything else is
    passed through in `extra`.  This lets one source file opt out of the global
    optimisation level, which is occasionally necessary because PETE.EXE was not
    built with this exact compiler (see notes/compiler.md).
    """
    cfg = dict(cfg)
    extra = []
    for tok in tokens:
        t = tok.strip()
        if not t:
            continue
        for slot, vals in SLOT_VALUES.items():
            if t in vals:
                cfg[slot] = t
                break
        else:
            extra.append(t)
    cfg["extra"] = " ".join(extra)
    return cfg


def startup_obj(model):
    """C0x.OBJ matching the memory model (C0S/C0M/C0L/C0H/C0C)."""
    return {"-ms": "C0S", "-mm": "C0M", "-ml": "C0L", "-mh": "C0H", "-mc": "C0C"}.get(model, "C0L")


MAX_ARTEFACT = 4 << 20   # a sane .MAP/.EXE is well under this
MAX_NAME = 8             # output names are subject to 8.3


def _check_name(name):
    if len(name) > MAX_NAME or '.' in name:
        return ('link output name %r is not 8.3-safe (max %d chars, no dots); '
                'DOS truncates it silently and the artefacts are then not found'
                % (name, MAX_NAME))
    return None


def _scan_main_defs(sources):
    """Turbo Link does not report a duplicate _MAIN: it loops forever emitting
    map entries until the disk fills (we produced a 13 GB .MAP).  Catch it here."""
    hits = []
    for s in sources:
        try:
            txt = open(s, encoding='latin1', errors='replace').read()
        except OSError:
            continue
        if re.search(r'\b(?:int|void)\s+main\s*\(', re.sub(r'/\*.*?\*/', '', txt, flags=re.S)):
            hits.append(os.path.basename(s))
    return hits


def _guard_runaway(res):
    """Delete any absurdly large artefact and flag the link as failed."""
    big = []
    for f in sorted(os.listdir(dosbox.OUT)):
        p = os.path.join(dosbox.OUT, f)
        if os.path.isfile(p) and os.path.getsize(p) > MAX_ARTEFACT:
            big.append((f, os.path.getsize(p)))
            os.remove(p)
    if big:
        res['ok'] = False
        res['runaway'] = big
        res['log'] += ('\nLINK RUNAWAY: Turbo Link emitted %s -- almost always a '
                       'duplicate symbol such as a second main().\n'
                       % ', '.join('%s (%.1f MB)' % (f, n / 1048576.0) for f, n in big))
    return res


def build(sources, name="prog", cfg=None, exe_dir=None, want_asm=True,
          extra_bcc="", timeout=300, keep=True):
    """sources: list of host paths to .c files.  Returns a result dict."""
    cfg = cfg or load_cfg()
    exe_dir = exe_dir or os.path.join(ROOT, 'build')
    os.makedirs(exe_dir, exist_ok=True)

    mains = _scan_main_defs(sources)
    if len(mains) > 1:
        return {"rc": -1, "log": 'refusing to link: main() defined in %s\n'
                                 % ', '.join(mains),
                "ok": False, "name": name, "flags": bcc_flags(cfg),
                "model": cfg["model"], "duplicate_main": mains}

    bad = _check_name(name)
    if bad:
        return {"rc": -1, "log": bad, "ok": False, "name": name,
                "flags": bcc_flags(cfg), "model": cfg["model"], "bad_name": True}

    dosbox.clear_out()
    for d in (dosbox.WORK,):
        os.makedirs(d, exist_ok=True)

    bat = []          # DOS-side build script
    objs = []
    for src in sources:
        base = os.path.splitext(os.path.basename(src))[0]
        dosbox.stage(src)
        flags = bcc_flags(cfg, want_asm=False)
        bat.append('bcc %s -c %s.C >> ..\\OUT\\LOG.TXT' %
                   (flags, base.upper()))
        bat.append('if not exist %s.OBJ goto failed' % base.upper())
        if want_asm:
            bat.append('bcc %s -S %s.C >> ..\\OUT\\LOG.TXT' %
                       (flags, base.upper()))
            bat.append('if not exist %s.ASM goto failed' % base.upper())
        objs.append(base.upper() + '.OBJ')

    lib = {'-ms': 'CS', '-mm': 'CM', '-ml': 'CL', '-mh': 'CH', '-mc': 'CC'}.get(cfg["model"], "CL")
    # The argument list goes in a TLINK response file rather than on the batch
    # line.  DOS caps the command tail at 127 characters, and once the object
    # list pushes past that the tail is silently truncated: TLINK then reports
    # only "Fatal: DOS error, ax = 2" (file not found) and leaves a 0-byte .EXE.
    # Anything past seven objects -- which main() needs, since its six far
    # callees each need their own module -- trips it.
    rsp = '%s.RSP' % name.upper()
    rsp_args = '%s D:\\LIB\\%s.OBJ %s, ..\\OUT\\%s.EXE, ..\\OUT\\%s.MAP, D:\\LIB\\%s.LIB' % (
        cfg["model"], startup_obj(cfg["model"]), " ".join(objs),
        name.upper(), name.upper(), lib)
    open(os.path.join(dosbox.WORK, rsp), 'wb').write(
        (rsp_args + "\r\n").encode('latin1'))
    bat.append('tlink @%s >> ..\\OUT\\LOG.TXT' % rsp)
    bat.append('if not exist ..\\OUT\\%s.EXE goto failed' % name.upper())
    bat.append('echo BUILD_OK >> ..\\OUT\\LOG.TXT')
    bat.append('exit')
    bat.append(':failed')
    bat.append('echo BUILD_FAILED >> ..\\OUT\\LOG.TXT')

    batpath = os.path.join(dosbox.WORK, 'B.BAT')
    open(batpath, 'wb').write((BANNER + "\r\n".join(bat) + "\r\nexit\r\n").encode('latin1'))

    rc, log = dosbox.run_batch('B.BAT', timeout=timeout)

    res = {"rc": rc, "log": log, "ok": 'BUILD_OK' in log,
           "name": name, "flags": bcc_flags(cfg), "model": cfg["model"]}
    _guard_runaway(res)
    if res["ok"]:
        for kind, ext in (("asm", ".asm"), ("obj", ".obj"), ("map", ".MAP"), ("exe", ".EXE")):
            got = dosbox.unstage(name.upper() + ext)
            if got is None and kind == "asm":
                for s in sources:
                    b = os.path.splitext(os.path.basename(s))[0].upper() + ext
                    got = dosbox.unstage(b)
            if got:
                dst = os.path.join(exe_dir, os.path.basename(got))
                shutil.copy2(got, dst)
                res[kind] = dst
        for s in sources:
            b = os.path.splitext(os.path.basename(s))[0].upper()
            for ext in ('.asm', '.OBJ'):
                g = dosbox.unstage(b + ext)
                if g:
                    shutil.copy2(g, os.path.join(exe_dir, os.path.basename(g)))
    if not keep:
        for f in os.listdir(dosbox.WORK):
            os.remove(os.path.join(dosbox.WORK, f))
    return res


if __name__ == '__main__':
    cfg = load_cfg()
    srcs = sys.argv[1:] or [os.path.join(ROOT, 'src', 'hello.c')]
    r = build(srcs, name='probe', cfg=cfg)
    print('flags:', r['flags'])
    print('ok:', r['ok'], 'rc:', r['rc'])
    print(r['log'])
