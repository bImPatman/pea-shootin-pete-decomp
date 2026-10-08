"""Compute public symbols for a proposed link set and report dups/undefined.

More careful than _dupscan: a function definition must have '{' before the next
statement and must not end in ';', and global definitions exclude extern/static.
Also reports which FUN_*/m_* names the callers reference but nobody defines.
"""
import os
import re
import sys

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import bcbuild

SRC = os.path.join(bcbuild.ROOT, 'src')

HEAD = re.compile(r'^[A-Za-z_].*')
EXTERN = re.compile(r'^\s*(extern|static|#|/\*|\*|//|\}|\)|while|else)')


def symbols_of(path):
    """Return dict name -> line number for defs in one .c file."""
    try:
        text = open(path, encoding='latin-1').read()
    except OSError:
        return {}
    lines = text.split('\n')
    out = {}
    for i, s in enumerate(lines, 1):
        st = s.strip()
        if not st or EXTERN.match(s):
            continue
        # function definition: name( ... ) with '{' before the next ';'
        m = re.match(r'^[A-Za-z_][\w\s\*]*?(\w+)\s*\((.*)$', st)
        if m and not st.endswith(';'):
            name = m.group(1)
            if name in ('if', 'for', 'while', 'switch', 'return', 'sizeof'):
                continue
            tail = st.split(')', 1)
            if len(tail) == 2 and tail[1].strip() in ('', '{'):
                out[name] = i
                continue
            # multi-line parameter list: look ahead for the closing
            chunk = '\n'.join(lines[i - 1:i + 4])
            if re.search(r'\)\s*\{', chunk) and ';' not in st.split('(', 1)[0]:
                out[name] = i
                continue
        # global variable definition
        m = re.match(r'^(unsigned\s+)?(int|char|short|long|void|struct\s+\w+)\s+'
                     r'(far\s+)?[*\s]*(\w+)\s*(\[[^\]]*\])?\s*(=.*)?$', st)
        if m and st.endswith((';', '=')) or (m and '=' in st and st.endswith(';')):
            out[m.group(4)] = i
        elif m and re.match(r'^(unsigned\s+)?(int|char|short|long|struct\s+\w+)\s+'
                            r'[*\s]*\w+\s*(\[[^\]]*\])?\s*;', st):
            out[re.search(r'(\w+)\s*(\[[^\]]*\])?\s*;',
                          st).group(1)] = i
    return out


def strip_comments(text):
    text = re.sub(r'/\*.*?\*/', ' ', text, flags=re.S)
    text = re.sub(r'//[^\n]*', ' ', text)
    return text


def refs_of(path):
    """FUN_* and m_* names referenced (called) in a file."""
    text = strip_comments(open(path, encoding='latin-1').read())
    return set(re.findall(r'\b((?:FUN_\w+|m_\w+))\s*\(', text))


def main():
    core = [
        'main.c', 'g13b2.c', 'mcallees.c', 'run/stubs.c', 'run/f44bstub.c',
        'keypoll.c', 'keyread.c', 'statebak.c', 'clrbit3.c', 'mb159.c',
        'dacupd.c', 'dacwrite.c', 'nodefree.c', 'xmod/xd19dc.c', 'e256.c',
        'xmod/xe161d.c', 'buftovg.c', 'xmod/xb2vcb.c',
    ]
    # union of @extra from the four reconstruction files
    extra = [
        'xmod/xe25978.c', 'xmod/xe256fe.c', 'xmod/xe25b0f.c',
        'xmod/x1f4429e.c', 'xmod/xd1914a.c', 'xmod/xd1932f.c',
        'xmod/xd192fe.c', 'xmod/x1e25b0.c', 'xmod/x161d38e.c',
        'xmod/xb1134c.c', 'xmod/xb1100b5.c', 'xmod/xb11462.c',
        'xmod/x1028ef.c', 'xmod/x1e2555.c', 'xmod/x1e254a.c',
        'xmod/xf4404b3.c', 'xmod/x0adb1da.c', 'xmod/xb111597.c',
        'xmod/xb111a47.c', 'xmod/xb111106.c',
        'xmod/x1000ef8.c', 'xmod/x1000c92.c', 'xmod/x1000c91.c',
        'xmod/x1e2500a.c', 'xmod/x0f92084.c', 'xmod/x0f9206.c',
        'xmod/x0df000e.c', 'xmod/xb11195d.c', 'xmod/xb111566.c',
        'xmod/xb1115d6.c', 'xmod/xb110056.c', 'xmod/x0adb324.c',
        'xmod/x1f4406.c', 'xmod/x1e253c6.c', 'xmod/xb1111cc.c',
        'xmod/xf02bf.c', 'xmod/xb111653.c',
        'xmod/x0adb1ff.c', 'xmod/xf440426.c', 'xmod/xe253c6.c',
        'xmod/xf0276.c', 'xmod/x0ffe0e.c', 'xmod/xf4403af.c',
        'xmod/xb111dc1.c', 'xmod/xd1995c.c', 'xmod/xd19104.c',
        'xmod/xb1119d2.c', 'xmod/xf440188.c', 'xmod/xf4400fe.c',
        'xmod/xf4404a2.c', 'xmod/xf4403f1.c',
    ]
    seen = set()
    link = []
    for f in core + extra:
        k = os.path.normcase(f)
        if k in seen:
            print('already in CORE, skipped: %s' % f)
            continue
        seen.add(k)
        link.append(f)

    defs = {}
    for f in link:
        p = os.path.join(SRC, f.replace('/', os.sep))
        if not os.path.exists(p):
            print('MISSING %s' % f)
            continue
        for name, ln in symbols_of(p).items():
            defs.setdefault(name, []).append((f, ln))

    print('\n=== duplicate definitions ===')
    dups = 0
    for name, occ in sorted(defs.items()):
        if len(occ) > 1:
            dups += 1
            print('DUP %-16s %s' % (name, occ))
    if not dups:
        print('none')

    defined = set(defs)
    print('\n=== referenced by callers but undefined ===')
    miss = 0
    for caller in ['main.c', 'mcallees.c']:
        p = os.path.join(SRC, caller)
        for name in sorted(refs_of(p)):
            if name not in defined:
                miss += 1
                print('UNDEF %-16s (from %s)' % (name, caller))
    if not miss:
        print('none')

    print('\nlink set: %d files, %d symbols' % (len(link), len(defined)))
    if dups or miss:
        return 1
    # print the CORE block to paste into build_game.py
    print('\n--- suggested build_game.py additions ---')
    for f in extra:
        print("    os.path.join(SRC, '%s')," % f)
    return 0


if __name__ == '__main__':
    sys.exit(main())
