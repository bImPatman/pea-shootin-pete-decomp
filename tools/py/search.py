"""Brute-force search for the C that produces a given target function.

Hand-guessing source shapes is slow and the compiler's idioms are not obvious
(`shl r/m8,cl` with `mov cl,4` is a shape no obvious C produces).  This tries a
library of candidate snippets, compiles each one, and reports any that match
the target under the normal two-tier comparison.

    python tools/py/search.py 0x0F36 --variants shift
"""
import os, sys, copy, itertools, argparse

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

TMPL = ('unsigned char g_mask;\n'
        '%s\n'
        'void fn(unsigned char v)\n'
        '{\n%s\n}\n')

# Each variant is a body for fn().  The global is always g_mask.
VARIANTS = {
 'lit4':        [r"g_mask = (g_mask & 0x8F) | ((v << 4) & 0x7F);"],
 'lit4_twice':  [r"unsigned char t = (unsigned char)(v << 4); g_mask = (g_mask & 0x8F) | ((t | (g_mask >> 4)) & 0x7F);"],
 'lit4_two_sh': [r"g_mask = (g_mask & 0x8F) | ((((v << 4) & 0x0F) | ((g_mask >> 4) & 0x70)) & 0x7F);"],
 'n_local':     [r"char n = 4; g_mask = (g_mask & 0x8F) | ((v << n) & 0x7F);"],
 'n_global':    [r"extern unsigned char g_shift; g_mask = (g_mask & 0x8F) | ((v << g_shift) & 0x7F);"],
 'n_enum':      [r"enum { SH = 4 }; g_mask = (g_mask & 0x8F) | ((v << SH) & 0x7F);"],
 'n_param':     [r"g_mask = (g_mask & 0x8F) | ((v << 4) & 0x7F);"],
 'n_sizeof':    [r"g_mask = (g_mask & 0x8F) | ((v << sizeof(int)) & 0x7F);"],
 'bitfield':    [r"union { unsigned char b; struct { unsigned char lo : 4, hi : 4; } f; } u; u.b = g_mask; u.f.lo = v & 0x0F; u.f.hi = u.f.lo; g_mask = u.b;"],
 'mul16':       [r"g_mask = (g_mask & 0x8F) | ((v * 16) & 0x7F);"],
 'shift_then':  [r"g_mask = (g_mask & 0x8F) | (((v << 4) << 0) & 0x7F);"],
 'two_stmts':   [r"unsigned char t = (unsigned char)(v << 4); t &= 0x7F; g_mask = (g_mask & 0x8F) | t;"],
 'n_local_use2':[r"char n = 4; unsigned char t = (unsigned char)(v << n); g_mask = (g_mask & 0x8F) | (t & 0x7F);"],
 'volatile_n':  [r"volatile char n = 4; g_mask = (g_mask & 0x8F) | ((v << n) & 0x7F);"],
 'n_param_var': [r"char n; n = 4; g_mask = (g_mask & 0x8F) | ((v << n) & 0x7F);"],
 # Shapes that stop the optimiser folding the count into shl r/m8,imm8.
 'reg_uchar':   [r"register unsigned char n = 4; g_mask = (g_mask & 0x8F) | ((v << n) & 0x7F);"],
 'reg_int':     [r"register int n = 4; g_mask = (g_mask & 0x8F) | ((v << n) & 0x7F);"],
 'ternary':     [r"int c = 1; g_mask = (g_mask & 0x8F) | ((v << (c ? 4 : 4)) & 0x7F);"],
 'const_shift': [r"const int S = 4; g_mask = (g_mask & 0x8F) | ((v << S) & 0x7F);"],
 'n_twice':     [r"unsigned char n = 4; unsigned char a = (unsigned char)(v << n), b = (unsigned char)(v >> n); g_mask = (g_mask & 0x8F) | ((a & 0x7F) | (b & 0x00));"],
 'n_ternary2':  [r"unsigned char n = 4; g_mask = (g_mask & 0x8F) | ((v << (n ? n : 4)) & 0x7F);"],
 'shift_in_if': [r"unsigned char n = 4; if (v) g_mask = (g_mask & 0x8F) | ((v << n) & 0x7F); else g_mask = (g_mask & 0x8F);"],
 'param_count': [r"g_mask = (g_mask & 0x8F) | ((v << 4) & 0x7F); g_mask = (unsigned char)(g_mask >> 4);"],
 'xor_shift':   [r"g_mask = (g_mask & 0x8F) | ((((v << 4) & 0x7F)) ^ (((g_mask & 0x8F) & 0x7F) ^ ((g_mask & 0x8F) & 0x7F)));"],
 'n_static_ro': [r"static const unsigned char n = 4; g_mask = (g_mask & 0x8F) | ((v << n) & 0x7F);"],
 'long_imm':    [r"g_mask = (g_mask & 0x8F) | (((unsigned long)v << 4) & 0x7F);"],
 'neg_shift':   [r"g_mask = (g_mask & 0x8F) | ((v << (8 - 4)) & 0x7F);"],
}

FLAGS = [(o, '-r-') for o in ('-O2', '-O', '-O1', '')] + \
        [('-O2', '-r-', {'stackframe': '-k'}),
         ('-O2', '-r-', {'stackchk': '-N'}),
         ('-O2', '-r-', {'reload': '-Z'}),
         ('-O2', '-r-', {'alignment': '-a'}),
         ('-O2', '-r-', {'merge': '-d'}),
         ('-O2', '-r-', {'stackframe': '-k', 'stackchk': '-N'})] + \
        [('-O2', '-r'), ('', '-r'), ('-O', '-r')]


def try_one(db, tfn, body, opt, rv, workdir, over=None):
    src = os.path.join(workdir, 'CAND.C')
    open(src, 'w').write(TMPL % ('', body))
    cfg = copy.deepcopy(bcbuild.DEFAULT)
    cfg['optimize'] = opt
    cfg['codegen'] = rv
    cfg.update(over or {})
    r = bcbuild.build([src], name='CAND', cfg=cfg, exe_dir=workdir, want_asm=False)
    if not r['ok']:
        return None, r['log'][-200:]
    img = Image(r['exe'])
    funcs, _ = funcs_from_map(img, r['map'])
    mfn = next((f for f in funcs if f.name == '_FN'), None)
    if mfn is None:
        return None, 'no _FN'
    rep = diffasm.report(tfn, mfn, db.image.img_size, img.img_size)
    s = rep['shape']
    e = rep['exact']
    return {'shape': s[0], 'exact': e[0], 'shape_pct': round(100.0 * s[1] / max(s[2], 1), 1),
            'exact_pct': round(100.0 * e[1] / max(e[2], 1), 1),
            'tsize': tfn.size, 'msize': mfn.size,
            'code': ' ; '.join(i.text() for i in mfn.insns)}, None


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument('target')
    ap.add_argument('--only', default=None, help='comma-separated variant names')
    ap.add_argument('--all', action='store_true', help='show every attempt')
    a = ap.parse_args()

    db = targetmod.TargetDB()
    tfn = db.resolve(a.target)
    workdir = os.path.join(ROOT, 'build', 'search')
    os.makedirs(workdir, exist_ok=True)

    print('target %s  %d bytes / %d insns' % (tfn.name, tfn.size, len(tfn.insns)))
    print()
    names = [n.strip() for n in a.only.split(',')] if a.only else list(VARIANTS)
    hits = []
    for name in names:
        for body in VARIANTS[name]:
            for flagspec in FLAGS:
                opt, rv = flagspec[0], flagspec[1]
                over = flagspec[2] if len(flagspec) > 2 else {}
                res, err = try_one(db, tfn, body, opt, rv, workdir, over)
                tag = '%s [%s %s%s]' % (name, opt or 'none', rv,
                                       (' ' + ' '.join('%s=%s' % kv for kv in over.items())) if over else '')
                if res is None:
                    if a.all:
                        print('%-28s ERROR %s' % (tag, err))
                    continue
                ok = 'MATCH' if res['exact'] else ('shape' if res['shape'] else '')
                print('%-28s %-6s shape %5.1f%%  exact %5.1f%%  %d/%d bytes'
                      % (tag, ok, res['shape_pct'], res['exact_pct'], res['tsize'], res['msize']))
                if a.all or ok:
                    print('        %s' % res['code'])
                if ok:
                    hits.append((tag, body, res))
    print()
    if hits:
        print('=' * 96)
        for tag, body, res in hits:
            print('%s\n    %s' % (tag, body.strip()))
    else:
        print('no variant matched')


if __name__ == '__main__':
    main()