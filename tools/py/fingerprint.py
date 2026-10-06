"""Fingerprint: find long byte-identical runs between a locally-built EXE's
Borland runtime code and PETE.EXE, to identify which compiler/runtime version
produced the target.

usage: fingerprint.py <candidate.exe> [target.exe]
"""
import sys, struct

def load_mz(path):
    d = open(path, 'rb').read()
    cblp, cp, nrel, hpar = struct.unpack('<HHHH', d[2:10])
    img = (cp - 1) * 512 + cblp
    base = hpar * 16
    return d, d[base:base + img], base

def runs(probe, hay, k=24):
    """Return list of (probe_off, hay_off, length) for maximal matches >= k."""
    idx = {}
    for i in range(len(hay) - k + 1):
        idx.setdefault(hay[i:i + k], []).append(i)
    out, seen = [], set()
    i = 0
    while i < len(probe) - k + 1:
        key = probe[i:i + k]
        hits = idx.get(key)
        if not hits:
            i += 1
            continue
        h = hits[0]
        if (i, h) in seen:
            i += 1
            continue
        L = k
        while i + L < len(probe) and h + L < len(hay) and probe[i + L] == hay[h + L]:
            L += 1
        seen.add((i, h))
        out.append((i, h, L))
        i += L if L > k else k
    return out

def main():
    cand = sys.argv[1]
    targ = sys.argv[2] if len(sys.argv) > 2 else 'PETE.EXE'
    pd, pimg, _ = load_mz(cand)
    td, timg, _ = load_mz(targ)
    print("candidate %s  load=%d bytes" % (cand, len(pimg)))
    print("target    %s  load=%d bytes" % (targ, len(timg)))
    rs = runs(pimg, timg)
    rs.sort(key=lambda r: -r[2])
    rs = [r for r in rs if r[2] >= 16]
    print("matches >=16 bytes: %d   total matched bytes: %d (%.1f%% of candidate)"
          % (len(rs), sum(r[2] for r in rs), 100.0 * sum(r[2] for r in rs) / len(pimg)))
    for po, ho, L in rs[:25]:
        print("  cand+0x%05X == targ+0x%05X   len=%d" % (po, ho, L))
    if not rs:
        print("  NO shared 16-byte runs -> different runtime/compiler")

main()