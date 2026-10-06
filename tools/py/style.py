"""Codegen-style statistics.

Calibrating on "does this probe function match some target function" needs 454
right guesses and gives no signal.  Instead, measure style features that depend
only on the compiler's *settings*, not on what the function does, and compare
those distributions between PETE.EXE and a rebuild.

The features that actually move between Borland settings:

  frame      standard `push bp / mov bp,sp` prologue vs a bare frame vs none.
             -k switches to Borland's "standard" frame; optimisation often
             drops the frame for leaf functions.
  retret     far (retf) vs near (ret) return.  Follows the memory model.
  callfar    far (lcall/call far ptr) vs near (call rel16) calls.
  subsp      distribution of explicit `sub sp,N` frame sizes, i.e. how many
             locals the compiler decided to keep in the frame.
  loads      `mov reg,[bp+N]` count per function: -Z suppresses redundant
             reloads, so this is a direct read-out of the -Z setting.
  align      whether function entry points are word aligned (-a).
  opt        frequency of the tell-tale optimised idioms:
               xor reg,reg / inc / dec / shl vs the naive
               mov reg,imm ... add reg,imm

Usage:
    python tools/py/style.py target          # PETE.EXE statistics
    python tools/py/style.py calib           # every flag set, ranked
"""
import os, sys, copy, collections, json

HERE = os.path.dirname(os.path.abspath(__file__))
ROOT = os.path.dirname(os.path.dirname(HERE))
sys.path.insert(0, HERE)
for _v in ('stdout', 'stderr'):
    try:
        getattr(sys, _v).reconfigure(encoding='utf-8', errors='replace')
    except Exception:
        pass

import bcbuild, target as targetmod, calibrate
from dosimg import Image, funcs_from_map


def features(funcs):
    f = collections.Counter()
    sizes = collections.Counter()
    for fn in funcs:
        if not fn.insns:
            continue
        f['n'] += 1
        mn = [i.mnem for i in fn.insns]
        ops = [i.ops for i in fn.insns]

        # ---- prologue shape
        if mn[0] == 'push' and ops[0] and ops[0][0].lower() == 'bp':
            f['pushbp_first'] += 1
            if len(mn) > 1 and mn[1] == 'mov' and len(ops[1]) >= 2 \
               and ops[1][0].lower() == 'bp' and ops[1][1].lower() == 'sp':
                f['frame_standard'] += 1
        elif mn[0] == 'enter':
            f['enter'] += 1
        if mn and mn[-1] in ('retf', 'retn'):
            f['ret_far'] += 1
        elif mn and mn[-1] == 'ret':
            f['ret_near'] += 1
        if fn.start % 2 == 0:
            f['word_aligned'] += 1

        # ---- frame size
        for i, m in enumerate(mn):
            if m == 'sub' and len(ops[i]) == 2 and ops[i][0].lower() == 'sp':
                try:
                    sizes[int(ops[i][1], 0)] += 1
                except ValueError:
                    pass
            if m == 'and' and len(ops[i]) == 2 and ops[i][0].lower() == 'sp':
                f['and_sp_frame'] += 1

        # ---- calls
        for i, m in enumerate(mn):
            if m.startswith('call') or m == 'lcall':
                f['call_any'] += 1
                f['has_call'] = 1
                o = ' '.join(ops[i]).lower()
                if ':' in o or 'ptr' in o or m == 'lcall':
                    f['call_far'] += 1
                else:
                    f['call_near'] += 1
            if m.startswith('loop') or m in ('jcxz', 'jecxz'):
                f['has_loop'] = 1
            if m in ('inc', 'dec'):
                f['incdec'] += 1
            if m in ('les', 'lds'):
                f[m] += 1
            if m == 'mov' and len(ops[i]) == 2 and '[bp' in ops[i][1].lower():
                f['bp_load'] += 1
            if m in ('shl', 'sal', 'shr', 'sar'):
                f['shift_reg'] += 1
            if m == 'xor' and len(ops[i]) == 2 and ops[i][0].lower() == ops[i][1].lower():
                f['zero_reg'] += 1
            if m in ('div', 'idiv'):
                f['div_' + m] += 1
        f['insns'] += len(fn.insns)
    return f, sizes


# Features that describe a function are divided by the function count;
# features that describe instructions are divided by the instruction count.
PER_FUNC = ['pushbp_first', 'frame_standard', 'ret_far', 'ret_near', 'enter',
            'word_aligned', 'has_call', 'has_loop']
PER_INSN = ['call_far', 'call_near', 'les', 'lds', 'bp_load', 'shift_reg',
            'zero_reg', 'and_sp_frame', 'div_div', 'div_idiv', 'incdec']


def vector(f):
    nf = max(f['n'], 1)
    ni = max(f['insns'], 1)
    v = {k: round(100.0 * f[k] / nf, 2) for k in PER_FUNC}
    v.update({k: round(100.0 * f[k] / ni, 2) for k in PER_INSN})
    return v


def load_rebuild(cfg, exe_dir):
    src = os.path.join(exe_dir, 'probes.c')
    os.makedirs(exe_dir, exist_ok=True)
    open(src, 'w').write(calibrate.PROBE_SRC)
    r = bcbuild.build([src], name='PROBES', cfg=cfg, exe_dir=exe_dir, want_asm=False)
    if not r['ok']:
        return None, None, None
    img = Image(r['exe'])
    fs, segs = funcs_from_map(img, r['map'])
    fs = [x for x in fs if not x.name.startswith('_MAIN')]
    f, sizes = features(fs)
    return f, sizes, segs


def main():
    db = targetmod.TargetDB()
    tf, tsizes = features(db.order)
    tv = vector(tf)
    print('TARGET  %s   %d functions, %d instructions' % (db.meta.get('md5', '')[:0] or 'PETE.EXE', tf['n'], tf['insns']))
    print('  frame sizes (sub sp,N): %s' % dict(sorted(tsizes.items())[:12]))
    for k, v in sorted(tv.items()):
        print('    %-16s %6.2f %%' % (k, v))
    print()

    exe_dir = os.path.join(ROOT, 'build', 'style')
    base = bcbuild.DEFAULT
    rows = []
    seen = set()
    for fs_ in calibrate.FLAG_SETS:
        cfg = copy.deepcopy(base)
        cfg.update(fs_)
        key = calibrate.label(cfg, base)
        if key in seen:
            continue
        seen.add(key)
        f, sizes, segs = load_rebuild(cfg, exe_dir)
        if f is None:
            print('%-12s BUILD FAILED' % key)
            continue
        v = vector(f)
        dist = sum(abs(v.get(k, 0) - tv.get(k, 0)) for k in tv)
        rows.append((dist, key, v, f['n'], sizes))
        print('%-12s n=%-3d  L1 style distance %6.2f' % (key, f['n'], dist))
    rows.sort(key=lambda t: t[0])
    print()
    print('=' * 96)
    print('%-12s %8s  %s' % ('flags', 'distance', 'largest style differences vs PETE.EXE'))
    for dist, key, v, n, sizes in rows[:6]:
        diffs = sorted(((abs(v.get(k, 0) - tv.get(k, 0)), k) for k in tv), reverse=True)[:5]
        print('%-12s %8.2f  %s' % (key, dist,
              ', '.join('%s %+.1f' % (k, d) for d, k in diffs)))
    json.dump({'target': tv, 'ranked': [(round(d, 2), k) for d, k, _, _, _ in rows]},
              open(os.path.join(ROOT, 'out', 'style.json'), 'w'), indent=1)


if __name__ == '__main__':
    main()
