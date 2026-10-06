"""Fill orphan holes in the Ghidra export with linearly decoded instructions.

Ghidra's export omits some basic blocks: a function's @INSTR lines can leave
gaps inside its own [start, end) extent that no other function claims.  Those
bytes are real code (they decode cleanly in 16-bit mode, and jumps in the
surrounding listing target them), so a faithful reconstruction cannot match the
export byte-for-byte until they are filled in.

Reads out/target.txt and writes out/target.full.txt; the Ghidra export is never
modified.  Regenerate with `python tools/py/repairholes.py`.
"""
import os
import sys

HERE = os.path.dirname(os.path.abspath(__file__))
ROOT = os.path.dirname(os.path.dirname(HERE))
sys.path.insert(0, HERE)

from dosimg import Image
from target import TargetDB, GHI_DRA_EXPORT, TARGET_EXE, GHIDRA_BASE

# Always read the raw Ghidra export, never the repaired copy: TargetDB's default
# EXPORT now points at the repaired file, and regenerating from that would turn
# this tool into a no-op that silently stops filling holes after a re-export.
SRC = GHI_DRA_EXPORT
OUT = os.path.join(ROOT, 'out', 'target.full.txt')


def covered_spans(f):
    return [(i.addr, i.end) for i in f.insns]


# `pop di; pop si; pop bp; retf`
EPILOGUE = b'\x5f\x5e\x5d\xcb'


def trailing_epilogue(f, allf, img):
    """Give a function back the unreachable epilogue the compiler emitted.

    A function whose last instruction is an unconditional near jump never
    falls through, but Borland still emits its epilogue straight after the
    jump, where it is dead code.  Ghidra ends the function at the jump and
    leaves those four bytes as an unclaimed gap, so a faithful rebuild looks
    four bytes too long until they are handed back to the function that
    emitted them.  Only claim them when the gap is exactly that epilogue and
    the next function begins immediately after it.
    """
    if not f.insns:
        return None
    last = f.insns[-1]
    if last.mnem != 'jmp' or len(last.ops) != 1:
        return None
    gap = f.start + f.size
    try:
        if bytes(img.bytes_at(gap, len(EPILOGUE))) != EPILOGUE:
            return None
    except Exception:
        return None
    after = gap + len(EPILOGUE)
    for o in allf:
        if o is not f and o.start < after and o.start + o.size > gap:
            return None                      # some other function claims the gap
    nxt = [o for o in allf if o is not f and o.start == after]
    if not nxt:
        return None
    return [(gap + k, 1) for k in range(len(EPILOGUE))], len(EPILOGUE)


def orphan_holes(f, allf):
    """Bytes inside f's extent that f never decoded and no other function owns."""
    ranges = [(o.start, o.start + o.size) for o in allf if o is not f]
    holes = []
    prev_end = f.start
    for a, b in covered_spans(f):
        if a > prev_end:
            holes.append((prev_end, min(a, f.start + f.size) - prev_end))
        prev_end = max(prev_end, b)
    end = f.start + f.size
    if prev_end < end:
        holes.append((prev_end, end - prev_end))
    out = []
    for a, n in holes:
        if n <= 0:
            continue
        if any(s < a + n and a < e for s, e in ranges):
            continue          # belongs to a neighbouring function
        out.append((a, n))
    return out


def main():
    db = TargetDB(TARGET_EXE, SRC)
    img = Image(TARGET_EXE)
    allf = [f for f in db.order if f.insns]

    filled = {}
    extended = {}
    total_ins = 0
    for f in allf:
        ins = []
        for a, n in orphan_holes(f, allf):
            got = []
            for i in img.disasm(a):
                if i.addr >= a + n:
                    break
                got.append(i)
            if sum(len(x.raw) for x in got) == n and got and got[-1].end == a + n:
                ins.extend(got)
        if ins:
            filled[f.name] = ins
            total_ins += len(ins)

    epilogue_total = 0
    for f in allf:
        got = trailing_epilogue(f, allf, img)
        if got:
            insns, nbytes = got
            filled.setdefault(f.name, []).extend(
                type(f.insns[0])(a, 1, img.bytes_at(a, 1), mn, op)
                for (a, _), mn, op in zip(insns, ('pop', 'pop', 'pop', 'retf'),
                                          ('di', 'si', 'bp', '')))
            extended[f.name] = nbytes
            epilogue_total += 1

    lines = open(SRC, encoding='latin1', errors='replace').read().split('\n')
    out = []
    cur = None
    body = []

    def flush():
        if not cur:
            return
        merged = dict(body)
        for i in filled.get(cur, []):
            merged[i.addr + GHIDRA_BASE] = i.size
        for g in sorted(merged):
            out.append('@INSTR %08X %d' % (g, merged[g]))

    for line in lines:
        if line.startswith('@FUNC'):
            p = line.split()
            cur = p[3]
            body = []
            if cur in extended:
                p[2] = '0x%x' % (int(p[2], 16) + extended[cur])
                p[4] = str(int(p[4]) + extended[cur])
                line = ' '.join(p)
            out.append(line)
        elif line.startswith('@INSTR'):
            p = line.split(' ', 3)
            body.append((int(p[1], 16), int(p[2])))
        elif line.startswith('@ENDFUNC'):
            flush()
            cur, body = None, []
            out.append(line)
        elif cur is None:
            out.append(line)
    flush()

    open(OUT, 'w', encoding='latin1').write('\n'.join(out))
    print('repaired %d functions, inserted %d instructions' % (len(filled), total_ins))
    print('absorbed a trailing dead epilogue for %d function(s)' % epilogue_total)
    print('wrote %s' % OUT)


if __name__ == '__main__':
    main()