"""Compiler-flag calibration.

We know the target was built by Borland C++ 3.1 in large model with 80186
instructions enabled (established by byte-fingerprinting the C runtime).  The
remaining unknowns -- optimisation level, register variables, and the various
on/off switches -- are found empirically:

  1. compile a module of codegen-sensitive probe functions under each candidate
     flag set
  2. normalise every probe function to an address-independent shape
  3. count how many target functions have a byte-compatible shape

The flag set with the highest score is the one the original author used, and
it also tells us how forgiving this particular compiler is -- i.e. how much of
the "does my C match?" test is real signal.
"""
import os, sys, copy, json, itertools, collections

HERE = os.path.dirname(os.path.abspath(__file__))
ROOT = os.path.dirname(os.path.dirname(HERE))
sys.path.insert(0, HERE)
for _v in ('stdout', 'stderr'):
    try:
        getattr(sys, _v).reconfigure(encoding='utf-8', errors='replace')
    except Exception:
        pass

import bcbuild, diffasm, target as targetmod
from dosimg import Image, funcs_from_map

PROBE_SRC = r'''
/* Codegen probes.  Deliberately cover the constructs whose lowering changes
   most between Borland's optimisation settings. */
int p_add(int a, int b) { return a + b; }
int p_mix(int a, int b, int c) { return a * b + c - (a >> 2); }
int p_max(int a, int b) { return a > b ? a : b; }
int p_clampb(int v) { if (v > 255) v = 255; if (v < 0) v = 0; return v; }
int p_sign(int v) { return v < 0 ? -1 : (v > 0 ? 1 : 0); }
long p_widen(int a) { return (long)a * 3; }
void p_bump(unsigned char *p, int n) { int i; for (i = 0; i < n; i++) p[i]++; }
void p_fill(unsigned char *p, int n, unsigned char v) { while (n-- > 0) *p++ = v; }
int p_switch(int x) { switch (x) { case 0: return 10; case 1: return 20; case 5: return 30; default: return -1; } }
long p_lmul(int a, int b) { return (long)a * b; }
int p_deref(long *q) { return (int)(*q + 1); }
void p_call(void (*f)(void)) { f(); }
int p_div(int a, int b) { return a / b; }
unsigned p_udiv(unsigned a, unsigned b) { return a / b; }
int p_shift(long v) { return (int)(v << 3) >> 2; }

/* main() must touch every probe, otherwise -O2 dead-code-eliminates them and
   there is nothing left to compare. */
static unsigned char buf[64];
static long slot;
static void sink(void) { }
void main(void)
{
    slot = 0;
    p_add(1, 2); p_mix(3, 4, 5); p_max(1, 2); p_clampb(300); p_sign(-1);
    p_widen(7); p_bump(buf, 8); p_fill(buf, 8, 3); p_switch(5);
    p_lmul(6, 7); p_deref(&slot); p_call(sink); p_div(9, 3); p_udiv(9u, 3u);
    p_shift(16);
}
'''

FLAG_SETS = []
for _opt in ['', '-O', '-O1', '-O2']:
    for _rv in ['-r', '-r-']:
        FLAG_SETS.append({'optimize': _opt, 'codegen': _rv})
_EXTRA = [('cpu', '-1'), ('cpu', '-2'), ('alignment', '-a'),
          ('stackframe', '-k'), ('stackchk', '-N'), ('reload', '-Z'),
          ('merge', '-d'), ('pascal', '-p')]
for _k, _v in _EXTRA:
    c = {'optimize': '-O2', 'codegen': '-r'}
    c[_k] = _v
    FLAG_SETS.append(c)


def label(cfg, base):
    """Human-readable flag set, including flags that were *removed*.

    An empty value must not be dropped: `-O2` turned off and `-O2` left alone
    produce the same join and would be indistinguishable.
    """
    d = []
    for k, v in cfg.items():
        if base.get(k) == v:
            continue
        d.append(v if v else '-' + k)
    return ' '.join(d) or '(defaults)'


def probe_shapes(cfg, exe_dir):
    src = os.path.join(exe_dir, 'probes.c')
    os.makedirs(exe_dir, exist_ok=True)
    open(src, 'w').write(PROBE_SRC)
    r = bcbuild.build([src], name='PROBES', cfg=cfg, exe_dir=exe_dir, want_asm=False)
    if not r['ok']:
        return None, r
    img = Image(r['exe'])
    funcs, segs = funcs_from_map(img, r['map'])
    mods = {n: v for n, v in segs.items() if n.endswith('_TEXT') and not n.startswith('_')}
    out = {}
    for f in funcs:
        if any(s <= f.start < s + l for s, l in mods.values()):
            out[f.name] = [i.shape() for i in diffasm.normalise(f, img.img_size)]
    return out, r


def target_shapes(db):
    out = {}
    for f in db.order:
        out[f.name] = [i.shape() for i in diffasm.normalise(f, db.image.img_size)]
    return out


def score(probes, targets):
    """How many distinct target functions are reproduced by some probe."""
    pset = {tuple(v) for v in probes.values()}
    hits = collections.Counter()
    detail = {}
    for tname, shape in targets.items():
        k = tuple(shape)
        if k in pset:
            hits[tname] = len(shape)
            detail[tname] = next(n for n, v in probes.items() if tuple(v) == k)
    return hits, detail


def main():
    db = targetmod.TargetDB()
    tshapes = target_shapes(db)
    base = bcbuild.DEFAULT
    exe_dir = os.path.join(ROOT, 'build', 'calib')
    print('target functions: %d' % len(tshapes))
    print()
    results = []
    seen = set()
    for fs in FLAG_SETS:
        cfg = copy.deepcopy(base)
        cfg.update(fs)
        key = label(cfg, base)
        if key in seen:
            continue
        seen.add(key)
        probes, r = probe_shapes(cfg, exe_dir)
        if probes is None:
            print('%-12s BUILD FAILED' % key)
            continue
        hits, detail = score(probes, tshapes)
        np = len(probes)
        results.append((len(hits), np, key, hits, detail))
        print('%-12s  %2d probe fns   ->  %3d target fns reproduced   (by probe: %s)'
              % (key, np, len(hits),
                 ', '.join(sorted(set(detail.values()), key=str))[:90]))
    results.sort(key=lambda t: -t[0])
    print()
    print('=' * 100)
    print('best flag sets:')
    for n, np, key, hits, detail in results[:6]:
        print('  %-12s  %3d/%d target functions reproduced' % (key, n, len(tshapes)))
    if results and results[0][3]:
        print()
        print('functions reproduced by the winning flag set (probe -> target):')
        for t, p in sorted(results[0][4].items(), key=lambda kv: kv[1]):
            print('  %-10s -> %s' % (p, t))
    json.dump([{'hits': n, 'flags': k} for n, _, k, _, _ in results],
              open(os.path.join(ROOT, 'out', 'calib.json'), 'w'), indent=1)


if __name__ == '__main__':
    main()