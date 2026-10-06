"""Scan the target for shift idioms and report how systematic they are.

The reconstruction of FUN_1000_0f36 stalled on `mov cl,4 / shl dl,cl`: no BCC
3.1 setting produces it, while `mov cl,[mem] / shl dl,cl` reproduces the rest of
the function exactly.  Before blaming the compiler we need to know whether the
idiom appears once or everywhere -- a single occurrence is probably inline asm
or hand assembly in the original, while dozens mean a different BC++ build.

    python tools/py/shifts.py            # target only
    python tools/py/shifts.py --mine     # also scan the current rebuild
"""
import os, sys, argparse

HERE = os.path.dirname(os.path.abspath(__file__))
sys.path.insert(0, HERE)
try:
    sys.stdout.reconfigure(encoding='utf-8', errors='replace')
except Exception:
    pass

import target as targetmod

SHIFT8 = ('shl', 'shr', 'sal', 'sar')
REGS = ('al', 'cl', 'dl', 'bl', 'ah', 'ch', 'dh', 'bh', 'ax', 'cx', 'dx', 'bx',
        'si', 'di', 'bp', 'sp', 'es', 'ds', 'cs', 'ss')


def classify(op):
    """Kind of one operand, as written in Ghidra/BCC text.

    Target instructions come from Ghidra as text, so there is no capstone
    operand array to inspect -- classify the string instead.
    """
    o = op.strip().lower()
    if '[' in o:
        return 'mem'
    if o in REGS:
        return o
    try:
        int(o, 0)
        return 'imm'
    except ValueError:
        return o


def norm(i):
    return i.mnem.lower(), tuple(classify(o) for o in i.ops)


def scan(insns):
    """Yield (kind, index, text) for interesting shift setups."""
    out = []
    for k, i in enumerate(insns):
        m, ops = norm(i)
        if m in SHIFT8 and len(ops) == 2 and ops[1] == 'cl':
            # where did cl come from?
            if k == 0:
                src = 'entry (cl from caller)'
            else:
                pm, pops = norm(insns[k - 1])
                if pm == 'mov' and pops and pops[0] == 'cl':
                    src = 'mov cl,imm' if len(pops) > 1 and pops[1] == 'imm' else \
                          'mov cl,mem' if len(pops) > 1 and pops[1] == 'mem' else 'mov cl,?'
                elif pm == 'xor' and pops and pops[0] == 'cl':
                    src = 'xor cl,cl'
                elif pm in ('lodsb', 'movsb'):
                    src = 'string op'
                else:
                    src = 'other (%s)' % i.text()
            out.append(('shift8-cl', k, src, '%s ; %s' % (insns[k - 1].text(), i.text())))
    return out


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument('--mine', action='store_true', help='also scan the rebuild in build/shift')
    a = ap.parse_args()

    db = targetmod.TargetDB()
    tally = {}
    total = 0
    examples = {}
    for f in db.funcs.values():
        if not f.insns:
            continue
        total += 1
        for kind, k, src, text in scan(f.insns):
            tally[src] = tally.get(src, 0) + 1
            examples.setdefault(src, []).append('%s @ %s' % (f.name, text))
    print('target: %d functions' % total)
    print('shl/shr r/m8,cl occurrences by CL source:')
    for src, n in sorted(tally.items(), key=lambda kv: -kv[1]):
        print('  %5d  %s' % (n, src))
        for e in examples[src][:3]:
            print('         e.g. %s' % e)
    print()

    if a.mine:
        from dosimg import Image, funcs_from_map
        import bcbuild
        r = bcbuild.build([os.path.join(os.path.dirname(HERE), 'src', 'flagshft.c')],
                          name='SHFT', exe_dir=os.path.join(os.path.dirname(HERE), 'build', 'shift'),
                          want_asm=False)
        if not r['ok']:
            print('rebuild failed')
            return
        img = Image(r['exe'])
        funcs, _ = funcs_from_map(img, r['map'])
        t2 = {}
        for f in funcs:
            if not f.insns:
                continue
            for kind, k, src, text in scan(f.insns):
                t2[src] = t2.get(src, 0) + 1
                print('  mine %s @ %s' % (f.name, text))
        print('rebuild tally:', t2)


if __name__ == '__main__':
    main()