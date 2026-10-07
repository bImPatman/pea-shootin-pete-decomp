"""Iterative decompilation driver for Pea Shootin' Pete.

    python tools/py/check.py show  0x10179          # target disassembly
    python tools/py/check.py diff  0x10179 src/foo.c # build + compare
    python tools/py/check.py all                     # every src/*.c
    python tools/py/check.py list                    # progress table

A source file declares which target function it implements with a marker
comment near the top:

    /* @target 0x10179 */
    /* @name   install_palette */

Borland puts each module's code in its own <MODULE>_TEXT segment, so the
functions this build produced are located unambiguously and cannot be confused
with the C runtime that also lives in the link.
"""
import os, re, sys, glob, json, subprocess, re

try:
    sys.stdout.reconfigure(encoding='utf-8', errors='replace')
    sys.stderr.reconfigure(encoding='utf-8', errors='replace')
except Exception:
    pass

HERE = os.path.dirname(os.path.abspath(__file__))
ROOT = os.path.dirname(os.path.dirname(HERE))
sys.path.insert(0, HERE)

import dosbox, bcbuild, diffasm, target as targetmod
from dosimg import Image, Func, funcs_from_map
import bcbuild

SRC = os.path.join(ROOT, 'src')
BUILD = os.path.join(ROOT, 'build')
OUTDIR = os.path.join(ROOT, 'out')
STATUS = os.path.join(OUTDIR, 'status.json')
# FUN_ has to be tried first: `F` is in [0-9a-fA-F], so the hex branch would
# otherwise match a bare "F" and truncate every FUN_* marker.
TARGET_DECL = re.compile(r'@target\s+(FUN_[0-9A-Fa-f_]+|[0-9a-fA-Fx]+)', re.I)
NAME_DECL = re.compile(r'@name\s+(\w+)')
PROTO_DECL = re.compile(r'@proto\s+(.+?);?\s*$', re.M)
NOMOD_DECL = re.compile(r'@module\s+none\b', re.I)
SAMEMOD_DECL = re.compile(r'@module\s+same\b', re.I)
FLAGS_DECL = re.compile(r'@flags\s+([^\n*/]+)')
EXTRA_DECL = re.compile(r'@extra\s+([^\n]+)')

_bar = '─' * 100


def hr(t=''):
    print('\n' + _bar)
    if t:
        print(' ' + t)
        print(_bar)


def load_status():
    if os.path.exists(STATUS):
        return json.load(open(STATUS))
    return {}


def save_status(st):
    os.makedirs(OUTDIR, exist_ok=True)
    json.dump(st, open(STATUS, 'w'), indent=1, sort_keys=True)


# --------------------------------------------------------------------------
# target side
# --------------------------------------------------------------------------

def resolve_target(db, key):
    f = db.resolve(key)
    if f is None:
        raise SystemExit('no target function matches %r' % key)
    return f


def show_target(db, fn):
    hr('TARGET %s   flat 0x%05X..0x%05X  (%d bytes, %d instrs)  ghidra 0x%05X'
       % (fn.name, fn.start, fn.end, fn.size, len(fn.insns), fn.start + targetmod.GHIDRA_BASE))
    for s in db.strings_used(fn):
        print('   string @0x%05X  %r' % (s[0], s[1]))
    print()
    for i in fn.insns:
        print('  %04X  %-6s %-38s %s' % (i.addr, i.hexbytes, i.text(), ''))


# --------------------------------------------------------------------------
# build side
# --------------------------------------------------------------------------

def module_segments(segs):
    """Borland names each module's code segment <MODULE>_TEXT."""
    return {n: v for n, v in segs.items()
            if n.endswith('_TEXT') and not n.startswith('_')}


def build_module(sources, name='PROBE', cfg=None):
    if cfg is None:
        cfg = bcbuild.load_cfg()
    r = bcbuild.build(sources, name=name, cfg=cfg, exe_dir=BUILD, want_asm=True)
    if not r['ok']:
        print('BUILD FAILED (flags: %s)' % r['flags'])
        print(r['log'])
        return None, r
    return r, r


def source_cfg(text, base=None):
    """Config for one source file: the global default plus its `@flags` line."""
    cfg = dict(base if base is not None else bcbuild.load_cfg())
    m = FLAGS_DECL.search(text)
    if not m:
        return cfg
    return bcbuild.apply_overrides(cfg, m.group(1).split())


def split_params(s):
    out, depth, cur = [], 0, ''
    for ch in s:
        if ch in '([':
            depth += 1
        elif ch in ')]':
            depth -= 1
        if ch == ',' and depth == 0:
            out.append(cur.strip())
            cur = ''
        else:
            cur += ch
    if cur.strip():
        out.append(cur.strip())
    return [p for p in out if p and p != 'void']


def _arg_list(proto):
    """Turn a prototype's parameter list into a matching list of zero arguments.

    The stub only has to provoke a *far call* so that Turbo Link patches the
    callee's `ret` into `retf`; the argument values themselves are never used.
    """
    i = proto.find('(')
    if i < 0:
        return ''
    j = proto.rfind(')')
    if j <= i:
        return ''
    args = []
    for p in split_params(proto[i + 1:j]):
        toks = p.replace('*', ' * ').split()
        if toks and toks[-1].startswith('*'):
            args.append('(%s)0' % ' '.join(toks[:-1]))
        elif len(toks) >= 2:
            args.append('(%s)0' % ' '.join(toks[:-1]))
        else:
            args.append('(%s)0' % p)
    return ', '.join(args)


def caller_stub(text, fname, srcpath=None):
    """Build the caller that makes the callee's return instruction correct.

    Two cases, and getting this wrong is what makes the whole exercise look
    impossible:

    * cross-module (default).  Borland calls and returns *near* between
      functions of the same module, emitting `ret`; across modules it emits a far
      call and Turbo Link patches the callee's `ret` into `retf`.  About 80% of
      the functions in PETE.EXE end in `retf`, so those need a separate CALLER.C
      module.

    * `@module same`.  For the ~29 functions the target calls within its own
      module: `__near` + `__pascal`, ending in `ret N`, with arguments at
      [bp+4] rather than [bp+6].  The large model gives *every* .c file its own
      code segment, so a near call needs the caller in the same file -- this
      returns a combined copy with a `main` appended.  A separate file would
      work for the call but tlink would then patch the callee to `retf`.
    """
    if not fname or NOMOD_DECL.search(text):
        return None
    m = PROTO_DECL.search(text)
    proto = (m.group(1).strip() if m else 'void %s(void)' % fname).rstrip('*/ ').strip()
    if not proto.endswith(')'):
        proto = 'void %s(void)' % fname
    call = '%s(%s);' % (fname, _arg_list(proto))
    os.makedirs(BUILD, exist_ok=True)
    if SAMEMOD_DECL.search(text):
        if not srcpath or not os.path.exists(srcpath):
            return None
        body = open(srcpath, encoding='latin1', errors='replace').read()
        p = os.path.join(BUILD, 'SAMEMOD.C')
        open(p, 'w').write('%s\n\nvoid main(void) { %s }\n' % (body, call))
        return p
    p = os.path.join(BUILD, 'CALLER.C')
    open(p, 'w').write('%s;\nvoid main(void) { %s }\n' % (proto, call))
    return p


def extra_sources(text, srcpath):
    """Auxiliary modules named by `@extra`, one per far call in the target.

    In the large model Borland gives every .c file its own code segment, so a
    call to a function in a *different* file is a genuine far call: Borland
    emits `lcall`, and Turbo Link fills in the segment:offset.  A call within one
    file is the cheap `nop / push cs / call rel32` form instead.  A function that
    mixes both -- main() does, with ten same-module calls and six cross-module
    ones -- therefore needs the far callees supplied as separate sources.
    """
    base = os.path.dirname(os.path.abspath(srcpath))
    out = []
    for m in EXTRA_DECL.finditer(text):
        raw = m.group(1).replace('*/', ' ')
        for tok in raw.split():
            p = tok if os.path.isabs(tok) else os.path.join(base, tok)
            if not os.path.isfile(p):
                print('  @extra: missing source %s' % p)
                continue
            out.append(p)
    return out


def my_functions(r):
    """Functions from the freshly built modules (not the runtime)."""
    img = Image(r['exe'])
    funcs, segs = funcs_from_map(img, r['map'])
    return img, funcs, segs, module_segments(segs)


def asm_listing(r):
    p = os.path.join(BUILD, os.path.splitext(os.path.basename(r['name'] + '.asm'))[0] + '.asm')
    return open(p, encoding='latin1', errors='replace').read() if os.path.exists(p) else ''


def show_asm(r):
    txt = asm_listing(r)
    if not txt:
        return
    hr('COMPILER ASSEMBLY (bcc -S)  -- Borland\'s own listing for this source')
    for line in txt.splitlines():
        print('  ' + line)


# --------------------------------------------------------------------------
# comparison
# --------------------------------------------------------------------------

def pick_mine(mine, decl_name):
    """Find the rebuilt function matching the source's @name.

    Borland only prefixes C symbols with `_`; under __pascal the leading
    underscore is dropped, so a __pascal reconstruction shows up as
    `FARSTREQ` where a cdecl one shows up as `_FARSTREQ`.  Try both rather than
    silently comparing against _MAIN instead.
    """
    if decl_name:
        up = decl_name.upper()
        cands = ['_' + up, up]
        for want in cands:
            for f in mine:
                if f.name.upper() == want:
                    return f
        for want in cands:
            for f in mine:
                if want in f.name.upper():
                    return f
    # Only safe when there is genuinely one candidate: _MAIN is never the
    # function under test.
    rest = [f for f in mine if f.name.upper() not in ('_MAIN', 'MAIN')]
    if len(rest) == 1:
        return rest[0]
    return None


def compare(db, tfn, mfn, tsize, msize, verbose=True):
    rep = diffasm.report(tfn, mfn, tsize, msize)
    s_ok, s_m, s_t, s_d = rep['shape']
    e_ok, e_m, e_t, e_d = rep['exact']
    pairs = rep['addr_pairs']

    if verbose:
        hr('COMPARISON   target %s  vs  mine %s' % (tfn.name, mfn.name))
        print('  shape : %s   %d/%d instructions   (%.1f%%)'
              % ('PASS' if s_ok else 'FAIL', s_m, s_t, 100.0 * s_m / max(s_t, 1)))
        print('  exact : %s   %d/%d instructions   (%.1f%%)'
              % ('PASS' if e_ok else 'FAIL', e_m, e_t, 100.0 * e_m / max(e_t, 1)))
        print('  size  : target %d bytes / mine %d bytes   %s'
              % (tfn.size, mfn.size, 'ok' if tfn.size == mfn.size else 'MISMATCH'))
        mark = s_d if s_d >= 0 else e_d
        print()
        print('  %-9s %-42s | %-9s %s' % ('target', 'instruction', 'mine', 'instruction'))
        print('  ' + '-' * 104)
        for line in diffasm.side_by_side(rep['target'], rep['mine'], mark):
            print('  ' + line)
        if pairs:
            print()
            print('  address correspondence (masked in the exact tier -- verify these):')
            for i, ta, mb in pairs[:40]:
                print('    insn %-3d  %-14s -> %s' % (i, ta, mb))
            if len(pairs) > 40:
                print('    ... %d more' % (len(pairs) - 40))
    return {'shape': bool(s_ok), 'exact': bool(e_ok),
            'shape_pct': round(100.0 * s_m / max(s_t, 1), 1),
            'exact_pct': round(100.0 * e_m / max(e_t, 1), 1),
            'target_size': tfn.size, 'mine_size': mfn.size}


# --------------------------------------------------------------------------
# commands
# --------------------------------------------------------------------------

def cmd_show(db, key):
    show_target(db, resolve_target(db, key))


def cmd_diff(db, key, srcpath, verbose=True):
    text = open(srcpath, encoding='latin1', errors='replace').read()
    m = NAME_DECL.search(text)
    decl_name = m.group(1) if m else None
    name = os.path.splitext(os.path.basename(srcpath))[0].upper()
    srcs = [srcpath]
    same_mod = bool(SAMEMOD_DECL.search(text))
    stub = caller_stub(text, decl_name, srcpath) if decl_name else None
    # For @module same the stub already contains the source, main and all.
    srcs = [stub] if same_mod else [srcpath]
    if stub and not same_mod:
        srcs.append(stub)
    srcs.extend(extra_sources(text, srcpath))
    r, _ = build_module(srcs, name=name, cfg=source_cfg(text))
    if r is None:
        return None
    img, mine, segs, mods = my_functions(r)
    if verbose:
        show_asm(r)
    mfn = pick_mine(mine, decl_name)
    if mfn is None:
        print('could not identify which built function to compare;')
        print('candidates: %s' % ', '.join(f.name for f in mine))
        return None
    tfn = resolve_target(db, key)
    res = compare(db, tfn, mfn, db.image.img_size, img.img_size, verbose)
    res['mine_name'] = mfn.name
    res['target_name'] = tfn.name
    return res


def cmd_all(db):
    files = sorted(glob.glob(os.path.join(SRC, '*.c')))
    st = load_status()
    hr('BATCH RUN  (%d source files)' % len(files))
    print('  %-24s %-22s %-7s %-7s %s' % ('source', 'target', 'shape', 'exact', 'note'))
    for f in files:
        text = open(f, encoding='latin1', errors='replace').read()
        mt = TARGET_DECL.search(text)
        if not mt:
            print('  %-24s %-22s %-7s %-7s %s'
                  % (os.path.basename(f), '-', '-', '-', 'no @target marker'))
            continue
        try:
            res = cmd_diff(db, mt.group(1), f, verbose=False)
        except SystemExit as e:
            print('  %-24s %-22s %-7s %-7s %s'
                  % (os.path.basename(f), mt.group(1), '-', '-', e))
            continue
        if res is None:
            print('  %-24s %-22s %-7s %-7s build failed' % (os.path.basename(f), mt.group(1), '-', '-'))
            continue
        print('  %-24s %-22s %-7s %-7s %d/%d bytes'
              % (os.path.basename(f), res['target_name'],
                 'PASS' if res['shape'] else 'fail',
                 'PASS' if res['exact'] else 'fail',
                 res['target_size'], res['mine_size']))
        st[res['target_name']] = dict(res, source=os.path.relpath(f, ROOT))
    save_status(st)
    hr('status written to %s' % os.path.relpath(STATUS, ROOT))


def _addr_of(name):
    """Flat address from a Ghidra name like FUN_1000_0f21, for stable ordering."""
    tail = name.rsplit('_', 1)[-1]
    try:
        return int(tail, 16)
    except ValueError:
        return 0


def status_rows(db, st):
    """(total, n_shape, n_exact, rows) shared by the text and markdown views."""
    rows = []
    for k in sorted(st, key=_addr_of):
        v = st[k]
        rows.append((k,
                     'PASS' if v.get('shape') else 'fail',
                     'PASS' if v.get('exact') else 'fail',
                     os.path.basename(v.get('source', '')),
                     v.get('target_size', 0), v.get('mine_size', 0),
                     v.get('exact_pct', 0.0)))
    return (len(db.order),
            sum(1 for v in st.values() if v.get('shape')),
            sum(1 for v in st.values() if v.get('exact')),
            rows)


def cmd_list(db, markdown=False):
    st = load_status()
    total, n_shape, n_exact, rows = status_rows(db, st)
    done = 0
    max_func = 0
    done_func = 0
    for k, sh, ex, src, ts, ms, pct in rows:
        done += pct
    percentage = (100 / total) * (done / 100)
    if markdown:
        replacement_text = ""
        def add_text(text = ""):
            nonlocal replacement_text
            replacement_text = replacement_text + "\n" + text
            print(text)

        # Regenerates the Progress section of README.md; keep the two in sync.
        add_text("Progress: (" + str(len(rows)) + " / " + str(total) + ") attepted, exact match (" + str(n_exact) + " / " + str(total) + ")\n[" + (int(percentage / 1.5) * "█") + (int((100 - percentage) / 1.5) * "░") + "] " + str(round(percentage, 2)) + "%")
        add_text('| Metric | Count |')
        add_text('|---|---|')
        add_text('| Target functions | %d |' % total)
        add_text('| Verified shape | %d |' % n_shape)
        add_text('| Verified exact | %d |' % n_exact)
        add_text('| Attempted | ' + str(len(rows)) + " |")
        add_text()
        add_text('| Target | Exact % | Source | Bytes |')
        add_text('|---|---|---|---|')
        for k, sh, ex, src, ts, ms, pct in rows:
            add_text('| `%s` | `%s` | `src/%s` | %d/%d |' % (k, pct, src, ts, ms))
        
        file = open("README.md", "r", encoding='utf-8')
        out = file.read()
        file.close()
        pattern = rf"({re.escape("<!-- progress report start -->")}).*?({re.escape("<!-- progress report end -->")})"

        updated_content = re.sub(
            pattern, rf"\1\n{replacement_text}\n\2", out, flags=re.DOTALL
        )

        file = open("README.md", "w", encoding='utf-8')
        file.write(updated_content)
        file.close()
        return
    print('target functions : %d' % total)
    print('verified shape   : %d' % n_shape)
    print('verified exact   : %d' % n_exact)
    print('attempted        : %d' % len(rows))
    if rows:
        hr('per-function status')
        print('  %-20s %-8s %-8s %-10s %s' % ('target', 'shape', 'exact', 'src', 'sizes'))
        for k, sh, ex, src, ts, ms, pct in rows:
            print('  %-20s %-8s %-8s %-10s %d/%d  exact %5.1f%%'
                  % (k, sh, ex, src, ts, ms, pct))
    print("" + str(percentage) + "% Completed")


def main(argv):
    db = targetmod.TargetDB()
    if not argv:
        print(__doc__)
        return
    cmd = argv[0]
    if cmd == 'show':
        cmd_show(db, argv[1])
    elif cmd == 'diff':
        cmd_diff(db, argv[1], argv[2])
    elif cmd == 'all':
        cmd_all(db)
    elif cmd == 'list':
        cmd_list(db, markdown='--markdown' in argv[1:])
    else:
        print(__doc__)


if __name__ == '__main__':
    main(sys.argv[1:])