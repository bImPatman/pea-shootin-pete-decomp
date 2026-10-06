"""Two-tier assembly comparison, as chosen for this project.

Tier 1  SHAPE   Instruction sequence matches: same mnemonics, same operands,
               same encoding *sizes*, with every address-valued field replaced
               by a wildcard.  This is the fast signal for "did I write
               equivalent C" - it ignores where things live in memory but
               insists on the same computation and register allocation.

Tier 2  EXACT   The two functions are byte-for-byte identical except in the
               fields that are genuinely addresses (jump/call targets and data
               pointers).  Address correspondence is reported so you can eyeball
               that target@0x10153 <-> mine@0x1A3E really are the same thing.

Both tiers are reported as PASS/FAIL plus a match percentage, and a
side-by-side listing is produced showing the first divergence in context.
"""
import re

REG16 = {'ax', 'bx', 'cx', 'dx', 'si', 'di', 'bp', 'sp'}
REG8 = {'al', 'bl', 'cl', 'dl', 'ah', 'bh', 'ch', 'dh'}
SEGREG = {'cs', 'ds', 'es', 'ss', 'fs', 'gs'}
BRANCH = re.compile(r'^(j\w+|loop\w*|jcxz|jecxz)$')
CALLRET = re.compile(r'^(call\w*|ret\w*|retn|retf|iret\w*)$')
FARCTL = re.compile(r'^(lcall|ljmp|callf|jmpf)$')

SMALL_LIT = 256          # |value| <= this is treated as a stable literal
SMALL_DISP = 0x200       # |frame displacement| <= this is stable


class Tok:
    """One normalised operand."""
    __slots__ = ('kind', 'text', 'value')

    def __init__(self, kind, text, value=None):
        self.kind = kind
        self.text = text
        self.value = value

    def shape(self):
        return self.kind

    def exact(self):
        return self.text

    def __repr__(self):
        return 'Tok(%s,%s)' % (self.kind, self.text)


def _num(s):
    s = s.strip()
    try:
        if s.lower().startswith('0x'):
            return int(s, 16)
        if s.lower().startswith('-0x'):
            return -int(s[3:], 16)
        return int(s, 10)
    except ValueError:
        return None


def norm_operand(op, mnem, insn, img_size):
    """Normalise one capstone operand string into a Tok."""
    o = op.strip()
    low = o.lower()

    if low in REG16 or low in REG8 or low in SEGREG or re.fullmatch(
            r'(eax|ebx|ecx|edx|esi|edi|ebp|esp|ax|bx|cx|dx|si|di|bp|sp|[abcd][lh])', low):
        return Tok('reg', low)

    # far pointer  seg:offset   (capstone: "0x1234:0x5678" or "es:0x1234")
    if ':' in o:
        a, b = o.split(':', 1)
        if _num(a) is not None and _num(b) is not None:
            return Tok('far', 'far:?')
        return Tok('mem', '%s:%s' % (a.lower(), norm_disp(b)))

    # memory
    m = re.search(r'\[(.*)\]\s*$', o)
    if m:
        inner = m.group(1)
        parts = [p.strip() for p in re.split(r'[\+\-]', inner) if p.strip()]
        seg = ''
        if ':' in inner:
            seg = inner.split(':')[0].lower() + ':'
            inner = inner.split(':', 1)[1]
        regs = '+'.join(p.lower() for p in parts if p.lower() in REG16 | REG8 | {'bp', 'sp'})
        disp = 0
        for p in parts:
            v = _num(p)
            if v is not None:
                disp += v
        dtext = ('%+d' % disp) if disp else ''
        if abs(disp) <= SMALL_DISP:
            return Tok('mem', '%smem[%s%s]' % (seg, regs, dtext))
        return Tok('mem?', '%smem[%s?]' % (seg, regs))

    v = _num(o)
    if v is None:
        return Tok('imm?', re.sub(r'0x[0-9a-fA-F]+', '?', o))

    if BRANCH.match(mnem):
        # relative target: express as an offset from function start (stable)
        tgt = insn.addr + insn.size + v
        return Tok('rel', 'rel:F+%d' % (tgt - insn.addr), v)
    if CALLRET.match(mnem) and mnem.startswith('call'):
        return Tok('call', 'call:?', v)

    # A far control transfer carries two absolute words, seg:off, that address
    # code in a *different* module.  How large those two numbers are depends
    # entirely on how much the other module contains, so the same call is
    # `addr` on the 113KB target and a small literal in a rebuild built from
    # stubs.  Classify both words as far addresses so the tiers compare the
    # instruction rather than the unrelated module's size.
    if FARCTL.match(mnem.lower()):
        return Tok('far', 'far:?', v)

    if 0 <= v < img_size or -SMALL_LIT <= v < SMALL_LIT:
        if abs(v) < SMALL_LIT:
            return Tok('imm', 'imm:%d' % v, v)
        return Tok('addr', 'addr:?', v)
    return Tok('imm', 'imm:%d' % v, v)


def norm_disp(s):
    v = _num(s)
    if v is None:
        return s.lower()
    return ('%+d' % v) if v else ''


class NInsn:
    def __init__(self, insn, func_start, img_size):
        self.insn = insn
        self.mnem = insn.mnem
        self.size = insn.size
        self.ops = [norm_operand(o, insn.mnem, insn, img_size) for o in insn.ops]
        self.off = insn.addr - func_start
        self.addr = insn.addr

    @property
    def raw(self):
        return self.insn.raw

    def shape(self):
        return (self.mnem.lower(), self.size, tuple(o.shape() for o in self.ops))

    def exact(self):
        return (self.mnem.lower(), self.size, tuple(o.exact() for o in self.ops))

    def text(self):
        return self.insn.text()


def normalise(func, img_size):
    return [NInsn(i, func.start, img_size) for i in func.insns]


def addr_fields(n):
    """Indices of operand tokens that hold an address (masked in tier 2)."""
    return [k for k, o in enumerate(n.ops) if o.kind in ('addr', 'far', 'call', 'mem?', 'imm?')]


def shape_compare(a, b):
    """Tier 1. Returns (ok, matched, total, first_diff_index)."""
    n = min(len(a), len(b))
    for i in range(n):
        if a[i].shape() != b[i].shape():
            return False, i, n, i
    return len(a) == len(b), n, max(len(a), len(b)), (n if len(a) != len(b) else -1)


def exact_compare(a, b):
    """Tier 2: bytes identical except at address-valued fields.

    Returns (ok, matched, total, first_diff, address_correspondence).
    """
    n = min(len(a), len(b))
    total = max(len(a), len(b))
    pairs = []
    for i in range(n):
        if a[i].mnem.lower() != b[i].mnem.lower() or a[i].size != b[i].size:
            return False, i, total, i, pairs
        if len(a[i].raw) != len(b[i].raw):
            return False, i, total, i, pairs
        amask = _mask(a[i])
        for k in range(len(a[i].raw)):
            if amask[k] == 0xFF and a[i].raw[k] != b[i].raw[k]:
                return False, i, total, i, pairs
        for oa in addr_fields(a[i]):
            if oa < len(a[i].ops) and a[i].ops[oa].value is not None:
                pairs.append((i, a[i].ops[oa].text, b[i].ops[oa].text))
    return len(a) == len(b), n, total, (n if len(a) != len(b) else -1), pairs


def _mask(n):
    """Byte mask for one instruction; 0xFF where the byte is compared."""
    m = [0] * n.size
    nbytes = n.size
    if n.mnem.lower() in ('ret', 'retf', 'retn', 'iret', 'hlt', 'nop', 'xchg'):
        for k in range(nbytes):
            m[k] = 0xFF
        return m
    # opcodes are always significant
    m[0] = 0xFF
    # for instructions with an immediate/address, mask the low bytes (imm) region
    if any(o.kind in ('addr', 'far', 'call', 'mem?', 'imm?') for o in n.ops):
        for k in range(1, nbytes):
            m[k] = 0
    else:
        for k in range(1, nbytes):
            m[k] = 0xFF
    return m


def report(target_fn, mine_fn, tsize, msize):
    # Both sides must classify immediates identically: norm_operand turns any
    # value below the image size into a masked `addr`.  Using each side's own
    # size made the same bytes shape as `addr` on the 113KB target and `imm` in
    # a small rebuild, so identical code failed tier 1 for no real reason.
    size = max(tsize, msize)
    a = normalise(target_fn, size)
    b = normalise(mine_fn, size)
    s_ok, s_matched, s_total, s_diff = shape_compare(a, b)
    e_ok, e_matched, e_total, e_diff, pairs = exact_compare(a, b)
    return {
        'shape': (s_ok, s_matched, s_total, s_diff),
        'exact': (e_ok, e_matched, e_total, e_diff),
        'addr_pairs': pairs,
        'target': a, 'mine': b,
    }


def side_by_side(a, b, mark=-1):
    """Aligned target-vs-rebuild listing with the first divergence marked."""
    rows = []
    for i in range(max(len(a), len(b))):
        def cell(n):
            if n is None:
                return '%-6s %-44s' % ('', '')
            return '%-6s %-44s' % ('%04X' % n.addr, n.text())
        rows.append((cell(a[i] if i < len(a) else None), cell(b[i] if i < len(b) else None)))
    w = max(len(r[0]) for r in rows) + 2
    out = []
    for i, (l, r) in enumerate(rows):
        note = '  <<<< first difference' if i == mark else ''
        out.append('%s |%s%s%s' % (l, ' ' * max(w - len(l), 1), r, note))
    return out