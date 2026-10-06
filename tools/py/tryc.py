#!/usr/bin/env python3
"""Try several candidate sources against one target function.

`check.py diff` is the tool of record: it builds one file, prints a full
side-by-side and writes status.  When reconstructing a function you rarely get
it right first time, and the useful loop is "same target, many source shapes,
one line each".  That is what this does.

    python tools/py/tryc.py 0x097C cand/a.c cand/b.c cand/c.c

Each file is a normal reconstruction: it needs `@name` and `@proto`, and may
carry `@module` / `@flags`.  Only the verdict line is printed unless `-v` is
given, in which case the last failing candidate is also dumped side by side.

Exit status is 0 if any candidate passed both tiers, 1 otherwise, so this can
drive a sweep.
"""
import os
import sys

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))

import check


def main(argv):
    args = [a for a in argv[1:] if a != '-v']
    verbose = '-v' in argv[1:]
    if len(args) < 2:
        print(__doc__.strip())
        return 2
    key, paths = args[0], args[1:]
    db = check.targetmod.TargetDB()
    width = max(len(os.path.basename(p)) for p in paths)
    best = None
    winner = None
    print('%-*s  %-6s %-6s %-12s %s' % (width, 'source', 'shape', 'exact', 'bytes', 'flags'))
    for p in paths:
        name = os.path.basename(p)
        if not os.path.exists(p):
            print('%-*s  MISSING' % (width, name))
            continue
        try:
            res = check.cmd_diff(db, key, p, verbose=False)
        except SystemExit as e:
            print('%-*s  %s' % (width, name, e))
            continue
        if res is None:
            print('%-*s  build failed' % (width, name))
            continue
        text = open(p, encoding='latin1', errors='replace').read()
        flags = check.source_cfg(text)
        flagtxt = check.bcbuild.bcc_flags(flags)
        print('%-*s  %-6s %-6s %-12s %s'
              % (width, name,
                 'PASS' if res['shape'] else 'fail',
                 'PASS' if res['exact'] else 'fail',
                 '%d/%d' % (res['target_size'], res['mine_size']),
                 flagtxt))
        score = res['shape_pct'] + res['exact_pct']
        if best is None or score > best:
            best = score
            winner = (p, res)
    if verbose and winner and not winner[1]['exact']:
        print()
        check.cmd_diff(db, key, winner[0], verbose=True)
    return 0 if (winner and winner[1]['exact']) else 1


if __name__ == '__main__':
    sys.exit(main(sys.argv))