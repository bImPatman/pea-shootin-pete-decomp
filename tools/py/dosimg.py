"""DOS EXE loading + 16-bit x86 disassembly, uniform for target and rebuilds.

Both sides of the comparison end up as the same structure:

    Image   flat bytes of the load module + helpers
    Insn    flat offset, size, raw bytes, mnemonic, operand strings
    Func    name, start/end (flat load-module offsets), list[Insn]

Function boundaries:
  * target  - taken from Ghidra's export (authoritative analysis)
  * rebuild - taken from the Turbo Link .MAP public-symbol table

Flat offsets are byte offsets from the start of the load module, so the two
sides are directly comparable even though they were loaded at different
segments.
"""
import re, struct
from capstone import Cs, CS_ARCH_X86, CS_MODE_16

MD = Cs(CS_ARCH_X86, CS_MODE_16)
MD.detail = False


class Image:
    def __init__(self, path):
        self.path = path
        d = open(path, 'rb').read()
        self.raw = d
        cblp, cp, nrel, hpar = struct.unpack('<HHHH', d[2:10])
        self.img_size = (cp - 1) * 512 + cblp
        self.base = hpar * 16
        self.img = d[self.base:self.base + self.img_size]
        (self.maxalloc, self.minalloc, self.ss, self.sp, self.csum,
         self.ip, self.cs, self.lfarlc, self.ovno) = struct.unpack('<HHHHHHHHH', d[10:28])
        self.header = d[:self.base]

    def disasm(self, off, count=None):
        """Disassemble from flat offset `off`; returns list[Insn]."""
        out = []
        blob = self.img[off:]
        for i in MD.disasm(blob, off):
            out.append(Insn(i.address, i.size, i.bytes, i.mnemonic, i.op_str))
            if count and len(out) >= count:
                break
        return out

    def bytes_at(self, off, n):
        return self.img[off:off + n]


class Insn:
    __slots__ = ('addr', 'size', 'raw', 'mnem', 'ops')

    def __init__(self, addr, size, raw, mnem, op_str):
        self.addr = addr
        self.size = size
        self.raw = bytes(raw)
        self.mnem = mnem
        self.ops = split_ops(op_str)

    @property
    def end(self):
        return self.addr + self.size

    @property
    def hexbytes(self):
        return ' '.join('%02x' % b for b in self.raw)

    def text(self):
        return (self.mnem + ' ' + ', '.join(self.ops)).strip()


_SEG_RE = re.compile(r'^(cs|ds|es|ss|fs|gs):', re.I)


def split_ops(op_str):
    """Split a capstone operand string on top-level commas."""
    if not op_str:
        return []
    ops, depth, cur = [], 0, ''
    for ch in op_str:
        if ch in '([':
            depth += 1
        elif ch in ')]':
            depth -= 1
        if ch == ',' and depth == 0:
            ops.append(cur.strip())
            cur = ''
        else:
            cur += ch
    if cur.strip():
        ops.append(cur.strip())
    return ops


class Func:
    def __init__(self, name, start, end, insns, segment=None):
        self.name = name
        self.start = start
        self.end = end
        self.insns = insns
        self.segment = segment

    @property
    def size(self):
        return self.end - self.start


# --------------------------------------------------------------------------
# Turbo Link .MAP
# --------------------------------------------------------------------------

_SEG_ROW = re.compile(
    r'^\s*([0-9A-Fa-f]{4,})H\s+([0-9A-Fa-f]{4,})H\s+([0-9A-Fa-f]+)H\s+(\S+)\s+(\S+)')
# public lines look like " 010A:000E       _P_ADD"  (no H suffix on the segment),
# and absolute symbols add an "Abs" flag between address and name.
_PUB = re.compile(r'^\s*([0-9A-Fa-f]{4})H?:([0-9A-Fa-f]{4})H?\s+(?:Abs\s+)?(\S+)')


def parse_map(path):
    """Return (segments, publics).

    Turbo Link .MAP uses two different units and it is easy to get this wrong:

      segment table   Start / Stop / Length are BYTE offsets into the load module
                      (_TEXT 0..010ADH = the first 0x10AE bytes)
      publics         "SSSS:OOOO" is segment:offset, so the flat byte address is
                      SSSS*16 + OOOO

    Verified against a linked build: _P_ADD is listed as 010A:000E and the
    bytes at 0x010A*16 + 0x000E really are that function.
    """
    segs, pubs = {}, {}
    in_pub = False
    for line in open(path, encoding='latin1', errors='replace'):
        if 'Publics by Name' in line or 'Publics by Value' in line:
            in_pub = True
            continue
        if not in_pub:
            m = _SEG_ROW.match(line)
            if m:
                segs[m.group(4)] = (int(m.group(1), 16), int(m.group(3), 16))
            continue
        m = _PUB.match(line)
        if m:
            flat = int(m.group(1), 16) * 16 + int(m.group(2), 16)
            pubs.setdefault(m.group(3), flat)
    return segs, pubs


def module_code_segments(segs):
    """Borland gives each module its own code segment named <MODULE>_TEXT.
    Those are the only segments holding the functions we wrote; everything in
    plain _TEXT is the C runtime."""
    return {n: v for n, v in segs.items()
            if n.endswith('_TEXT') and not n.startswith('_')}


def funcs_from_map(image, map_path, module_only=True):
    """Build a Func list from the map's public symbols.

    module_only=True keeps just the functions from the modules we compiled,
    which is what makes a rebuild comparable to a single target function.
    """
    segs, pubs = parse_map(map_path)
    mods = module_code_segments(segs)
    spans = mods if module_only else {n: v for n, v in segs.items() if v[1] > 0}

    hits = []
    for name, flat in pubs.items():
        # Borland prefixes cdecl C symbols with `_` but drops the underscore
        # under __pascal, where `int __pascal f()` links as plain `F`.  Every
        # other underscore-less public in these maps ends in '@' (runtime
        # library helpers, DATASEG@, DGROUP@) and is filtered below, so
        # accepting a leading letter costs nothing.
        if not (name.startswith('_') or name[:1].isascii() and name[:1].isalpha()):
            continue
        if name.endswith('@'):
            continue
        seg = next((n for n, (s, l) in spans.items() if s <= flat < s + l), None)
        if seg is None:
            continue
        hits.append((flat, name, seg))
    hits.sort()

    rets = ('ret', 'retf', 'retn', 'retf', 'iret', 'hlt')
    funcs = []
    for i, (flat, name, seg) in enumerate(hits):
        seg_start, seg_len = spans[seg]
        nxt = hits[i + 1][0] if i + 1 < len(hits) and hits[i + 1][2] == seg else seg_start + seg_len
        # Bound the function by the next public in this segment (or the segment
        # end), then trim to the *last* return.  Trimming at the first return
        # silently cuts short every function with more than one `return`, which
        # is most of the interesting ones.
        body = [x for x in image.disasm(flat) if x.addr < nxt]
        last_ret = max((k for k, x in enumerate(body) if x.mnem in rets), default=None)
        if last_ret is None:
            end = nxt
        else:
            body = body[:last_ret + 1]
            end = body[-1].end
        f = Func(name, flat, end, body)
        f.segment = seg
        funcs.append(f)
    return funcs, segs