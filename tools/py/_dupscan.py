import re, os, sys
CORE = ["main.c","g13b2.c","mcallees.c","run/stubs.c","run/f44bstub.c","keypoll.c","keyread.c",
"statebak.c","clrbit3.c","mb159.c","dacupd.c","dacwrite.c","nodefree.c","xmod/xd19dc.c",
"e256.c","xmod/xe161d.c","buftovg.c","xmod/xb2vcb.c"]
src = os.path.join(os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__)))), "src")
vardef = re.compile(r'^(?:unsigned\s+int|unsigned\s+char|unsigned\s+short|unsigned\s+long|unsigned|int|char|short|long|char\s+far\s*\*|void\s+far\s*\*|struct\s+\w+)\s+(?:far\s*\*\s*)?(\*?\s*\w+)\s*(\[[^\]]*\])?\s*[=;]')
# function definition: type ... name(args) where line ends with '{' (or next non-empty is '{')
fundef = re.compile(r'^(?:[\w\*]+\s+)+\*?\s*(\w+)\s*\(')
found = {}
funcs = {}
for f in CORE:
    p = os.path.join(src, f.replace("/", os.sep))
    if not os.path.exists(p):
        print("MISSING", f); continue
    lines = open(p, encoding="latin-1").read().split("\n")
    for i, s in enumerate(lines, 1):
        st = s.lstrip()
        if st.startswith(("extern","static","#","/*","*","//")):
            continue
        m = vardef.match(s)
        if m:
            name = m.group(1).strip().lstrip("*").strip()
            if name:
                found.setdefault(name, []).append((f, i))
        # function def: opening line containing '(' and not ending in ';'
        if "(" in s and not s.rstrip().endswith(";") and st and not st.startswith(("if","for","while","switch","else","return")):
            mm = fundef.match(s)
            if mm:
                name = mm.group(1)
                # require a following '{' within a couple lines to confirm definition
                nxt = "\n".join(lines[i:i+3])
                if "{" in nxt and ";" not in s.split("(")[0]:
                    funcs.setdefault(name, []).append((f, i))
print("=== duplicate global variables ===")
for n, occ in sorted(found.items()):
    if len(occ) > 1:
        print("DUP VAR", n, occ)
print("=== duplicate function definitions ===")
for n, occ in sorted(funcs.items()):
    if len(occ) > 1:
        print("DUP FN ", n, occ)
