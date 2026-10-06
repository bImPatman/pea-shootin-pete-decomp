"""Target-side database: parses Ghidra's export of PETE.EXE.

Ghidra gives authoritative function boundaries; the raw bytes come straight out
of the MZ image.  Instruction semantics come from capstone (same disassembler
used for rebuilds) so both sides of a comparison are normalised identically.
"""
import os, re, sys

HERE = os.path.dirname(os.path.abspath(__file__))
ROOT = os.path.dirname(os.path.dirname(HERE))
sys.path.insert(0, HERE)
from dosimg import Image, Insn, Func, MD

ROOT = os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
TARGET_EXE = os.path.join(ROOT, 'PETE.EXE')
# out/target.txt is the raw Ghidra export and out/target.full.txt is that same
# export with its missing basic blocks refilled (tools/py/repairholes.py), which
# is what comparisons run against.  PETE_EXPORT overrides it.
GHI_DRA_EXPORT = os.path.join(ROOT, 'out', 'target.txt')
EXPORT = os.environ.get('PETE_EXPORT') or os.path.join(ROOT, 'out', 'target.full.txt')
if not os.path.exists(EXPORT):
    EXPORT = GHI_DRA_EXPORT

GHIDRA_BASE = 0x10000        # Ghidra maps the DOS load module at 0x10000


def to_flat(gaddr):
    return gaddr - GHIDRA_BASE


class TargetDB:
    def __init__(self, exe=TARGET_EXE, export=EXPORT):
        self.image = Image(exe)
        self.meta = {}
        self.blocks = []
        self.strings = {}
        self.funcs = {}
        self.order = []
        self._load(export)

    def _load(self, path):
        cur = None
        pending = {}
        for line in open(path, encoding='latin1', errors='replace'):
            line = line.rstrip('\n')
            if line.startswith('@META'):
                _, k, v = line.split(' ', 2)
                self.meta[k] = v
            elif line.startswith('@BLOCK'):
                p = line.split()
                self.blocks.append((p[1], int(p[2], 16), int(p[3], 16)))
            elif line.startswith('@STR'):
                p = line.split(' ', 3)
                self.strings[int(p[1], 16)] = p[3] if len(p) > 3 else ''
            elif line.startswith('@FUNC'):
                p = line.split()
                gstart, gend, name = int(p[1], 16), int(p[2], 16), p[3]
                cur = Func(name, to_flat(gstart), to_flat(gend), [])
                pending['g'] = (gstart, gend)
                self.funcs[cur.name] = cur
                self.order.append(cur)
            elif line.startswith('@INSTR'):
                p = line.split(' ', 4)
                gaddr, size = int(p[1], 16), int(p[2])
                pending.setdefault('ins', []).append(gaddr)
                if cur is not None:
                    raw = self.image.bytes_at(to_flat(gaddr), size)
                    cur.insns.append(_decode(to_flat(gaddr), raw))
            elif line.startswith('@ENDFUNC'):
                cur = None
                pending = {}

    # -- lookups ----------------------------------------------------------
    def by_addr(self, flat):
        for f in self.order:
            if f.start == flat:
                return f
        return None

    def by_gaddr(self, gaddr):
        return self.by_addr(to_flat(gaddr))

    def resolve(self, key):
        """Accept a flat offset, a Ghidra address, a name, or a partial name."""
        k = key.strip()
        if k in self.funcs:
            return self.funcs[k]
        try:
            v = int(k, 0)
        except ValueError:
            v = None
        if v is not None:
            f = self.by_gaddr(v) if v >= GHIDRA_BASE else self.by_addr(v)
            if f:
                return f
        low = k.lower()
        hits = [f for f in self.order if low in f.name.lower()]
        if len(hits) == 1:
            return hits[0]
        if hits:
            raise KeyError('%d functions match %r; be more specific' % (len(hits), key))
        return None

    def text_at(self, flat):
        return self.strings.get(flat + GHIDRA_BASE, '')

    def strings_used(self, fn):
        """Strings referenced from inside fn, by Ghidra address."""
        lo, hi = fn.start + GHIDRA_BASE, fn.end + GHIDRA_BASE
        return [(a, s) for a, s in sorted(self.strings.items()) if lo <= a < hi]


def _decode(flat, raw):
    for i in MD.disasm(raw, flat):
        return Insn(i.address, i.size, i.bytes, i.mnemonic, i.op_str)
    return Insn(flat, len(raw), raw, 'db', '')


if __name__ == '__main__':
    db = TargetDB()
    print('image base file offset 0x%X  load %d bytes  entry %04X:%04X'
          % (db.image.base, db.image.img_size, db.image.cs, db.image.ip))
    print('functions: %d' % len(db.order))
    tot = sum(len(f.insns) for f in db.order)
    print('instructions: %d' % tot)
    for f in db.order[:10]:
        print('  %-18s 0x%05X..0x%05X  %3d insns' % (f.name, f.start, f.end, len(f.insns)))
