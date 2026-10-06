"""Find the compiler settings that reproduce a specific target function.

style.py compares aggregate statistics, which needs a large sample and so is
blunt.  This tool inverts the problem: take one target function that has already
been written as C, build it under every candidate flag set, and report which
settings make the compiler emit the original bytes.

    python tools/py/fitflags.py src/flagbits.c

Reads the @target / @name markers from the source, exactly like check.py.
"""
import os, sys, copy, json, argparse

HERE = os.path.dirname(os.path.abspath(__file__))
ROOT = os.path.dirname(os.path.dirname(HERE))
sys.path.insert(0, HERE)
for _v in ('stdout', 'stderr'):
    try:
        getattr(sys, _v).reconfigure(encoding='utf-8', errors='replace')
    except Exception:
        pass

import bcbuild, diffasm, target as targetmod, calibrate, check
from dosimg import Image, funcs_from_map

# Broader sweep than calibrate.FLAG_SETS: now that we have a real function to
# match, small differences between settings become visible.
def candidate_sets():
    out = []
    for opt in ['', '-O', '-O1', '-O2']:
        for rv in ['-r', '-r-']:
            out.append({'optimize': opt, 'codegen': rv})
    for k, v in [('stackframe', '-k'), ('stackchk', '-N'), ('reload', '-Z'),
                 ('merge', '-d'), ('cpu', '-2')]:
        for opt in ['-O2', '-O1']:
            out.append({'optimize': opt, 'codegen': '-r', k: v})
    return out


def score_set(db, tfn, src, name, cfg, exe_dir, decl=None, stub=None, verbose=False):
    # The caller stub must be built too: without a second module Borland emits
    # `ret`, and every exact comparison would fail on that one byte for all
    # ~24 candidate flag sets -- which is exactly the kind of systematic
    # failure that gets mistaken for "none of these flags are right".
    # For `@module same` the stub is a copy of the source with main appended,
    # so it must *replace* the source, not accompany it: building both
    # defines the function twice and Turbo Link never finishes.
    srcs = [stub] if check.SAMEMOD_DECL.search(open(src, encoding='latin1',
                                                   errors='replace').read()) else [src]
    if stub and srcs[0] is not stub:
        srcs.append(stub)
    r = bcbuild.build(srcs, name=name, cfg=cfg, exe_dir=exe_dir, want_asm=False)
    if not r['ok']:
        return None
    img, funcs, segs, mods = check.my_functions(r)
    mfn = check.pick_mine(funcs, decl)
    if mfn is None:
        return None
    rep = diffasm.report(tfn, mfn, db.image.img_size, img.img_size)
    s_ok, s_m, s_t, s_d = rep['shape']
    e_ok, e_m, e_t, e_d = rep['exact']
    return {'shape': s_ok, 'exact': e_ok, 'shape_pct': round(100.0 * s_m / max(s_t, 1), 1),
            'exact_pct': round(100.0 * e_m / max(e_t, 1), 1),
            'tsize': tfn.size, 'msize': mfn.size,
            'report': rep if verbose else None}


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument('source')
    ap.add_argument('--verbose', action='store_true')
    a = ap.parse_args()

    src = a.source if os.path.isabs(a.source) else os.path.join(ROOT, a.source)
    import re
    text = open(src, encoding='latin1', errors='replace').read()
    mt = re.search(r'@target\s+([0-9A-Fa-fx]+|FUN_\w+)', text)
    mn = re.search(r'@name\s+(\w+)', text)
    if not mt:
        raise SystemExit('%s has no @target marker' % a.source)

    db = targetmod.TargetDB()
    tfn = db.resolve(mt.group(1))
    # The link output name is subject to 8.3, so derive it from the file name;
    # @name is only used to pick the function out of the built module.
    name = os.path.splitext(os.path.basename(src))[0].upper()
    exe_dir = os.path.join(ROOT, 'build', 'fit')
    decl = mn.group(1) if mn else None
    stub = check.caller_stub(text, decl, src) if decl else None

    print('target %s  flat 0x%05X  %d bytes / %d insns' % (tfn.name, tfn.start, tfn.size, len(tfn.insns)))
    print('mine   %s%s' % (name, '  (+ cross-module caller)' if stub else ''))
    print()

    base = bcbuild.DEFAULT
    rows = []
    seen = set()
    for fs in candidate_sets():
        cfg = copy.deepcopy(base)
        cfg.update(fs)
        key = calibrate.label(cfg, base)
        if key in seen:
            continue
        seen.add(key)
        res = score_set(db, tfn, src, name, cfg, exe_dir, decl, stub, a.verbose)
        if res is None:
            print('%-12s  no match / build problem' % key)
            continue
        rows.append((key, fs, res))
        print('%-12s  shape %-4s %5.1f%%   exact %-4s %5.1f%%   %d/%d bytes'
              % (key, 'PASS' if res['shape'] else 'fail', res['shape_pct'],
                 'PASS' if res['exact'] else 'fail', res['exact_pct'],
                 res['tsize'], res['msize']))

    rows.sort(key=lambda t: (-(t[2]['shape_pct'] + t[2]['exact_pct']), t[0]))
    print()
    print('=' * 96)
    print('best flag sets for %s:' % os.path.basename(src))
    for key, fs, res in rows[:5]:
        print('  %-12s shape %-4s exact %-4s  (%d/%d bytes)'
              % (key, 'PASS' if res['shape'] else 'fail',
                 'PASS' if res['exact'] else 'fail', res['tsize'], res['msize']))
    if a.verbose and rows:
        # Rebuild the winning config verbatim -- the label is a display string,
        # not something to parse back into flags.
        key, fs, _ = rows[0]
        rep = score_set(db, tfn, src, name, dict(copy.deepcopy(base), **fs),
                        exe_dir, decl, stub, True)
        if rep and rep.get('report'):
            print()
            print('side-by-side for %s:' % key)
            for line in diffasm.side_by_side(rep['report']['target'], rep['report']['mine']):
                print('  ' + line)
    json.dump({'source': a.source, 'target': tfn.name,
               'ranked': [(k, r['shape'], r['exact'], r['shape_pct'], r['exact_pct'])
                          for k, _fs, r in rows]},
              open(os.path.join(ROOT, 'out', 'fitflags.json'), 'w'), indent=1)


if __name__ == '__main__':
    main()