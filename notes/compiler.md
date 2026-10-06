# Compiler identification and flag set for PETE.EXE

## Conclusion

| Setting | Value | How established |
|---|---|---|
| Compiler | Borland C++ 3.1 | banner strings in the binary, plus 76.6% byte match of the C runtime |
| Memory model | large (`-ml`) | runtime fingerprint 76.6% vs 52.8% huge, 27.3% medium, 25.8% small |
| CPU | 80186 (`-1`) | 80186-only encodings present (`add r/m,imm8`, `leave`, `push imm16`); no 286/386-only forms |
| Code generation | needs a **cross-module call** | see below |
| Optimisation level | `-O2`, but see below | 100 target functions need `-O2`'s frame idiom and 30 need `-O`/`-O1`'s epilogue merging, which no single 3.1 setting does |

Toolchain: `tools\bc31` mounted read-only as `D:` by DOSBox X.
Build driver: `tools\py\bcbuild.py`, flags in `tools\py\flags.json`.

## Evidence for Borland C++ 3.1

Strings in the load module:

```
C:\BORLANDC\INCLUDE\STDLIB.H
Borland C++  -  Copyright 1991
__TURBOCRT
WORX TOOLKIT VERSION 2.1 COPYRIGHT 1993 BY MYSTIC SOFTWARE
```

A probe linked against `CL.LIB` / `C0L.OBJ` shares long identical byte runs with
PETE.EXE's runtime code, and 76.6% of the runtime load module matches.  Nothing
else tried comes close.

## The finding that matters most: `ret` vs `retf`

80% of the target's functions end in `retf`, 14% in `ret`.

Borland emits **`ret`** in the assembly listing even for a `proc far`:

```
_flagbits_set  proc  far
        push  bp
        mov   bp,sp
        ...
        ret                       <-- what bcc -S prints
```

But Turbo Link **patches that `ret` into `retf`** when the function is reached
by a *far* call, i.e. from a different module.  Verified by disassembling the
linked image:

```
1097  55           push bp
...
10AB  cb           retf          <-- c3 in the listing, cb in the EXE
```

Within one module Borland calls near and leaves `ret`.  So:

* a rebuild whose function is only called from its own module will always emit
  `ret` and can never match the majority of target functions;
* `check.py` therefore compiles each source together with a generated
  `CALLER.C` that lives in its own module and makes the far call.

Declaring the prototype `far` explicitly does *not* help - Borland still calls
near and still emits `ret`.

## Gotchas that cost time

* **Turbo Link's third positional argument is the map file**, not a library:
  `tlink ... EXE,MAP,LIB`.  Getting this wrong makes TLINK overwrite the
  library it was handed.
* **Duplicate `main()` makes Turbo Link loop forever** writing map entries
  instead of reporting the error.  We produced a 13 GB `.MAP` before spotting
  it.  `bcbuild._scan_main_defs` now refuses to link such a build, and
  `_guard_runaway` deletes any absurdly large artefact.
* **Turbo Link `.MAP` uses two different units.**  The segment table's
  `Start`/`Stop`/`Length` are *byte* offsets into the load module; the publics
  table is `SSSS:OOOO` = segment:offset, so the flat address is `SSSS*16+OOOO`.
  Getting this wrong silently yields zero functions.
* Output and module names are subject to 8.3, so keep them short.
* `C0L.OBJ` is the large-model startup object (`C0T` warns "No stack").
* A module containing `double` maths makes Turbo Link die before printing its
  banner.  PETE.EXE contains no real FPU code, so this has not been pursued.

## Which flags actually do anything

Measured by object-file hash (`tools\py` + a flag matrix run):

| Flag | Effect on our probes |
|---|---|
| `-O` / `-O1` / `-O2` | yes |
| `-k` standard stack frame | yes |
| `-N` stack check | yes -- **and ruled out**: it makes `FUN_1000_0f21` grow from 21 to 32 bytes, so the target was built without it |
| `-a` word alignment | no effect without initialised data |
| `-Z` suppress reloads | no effect on simple code |
| `-d` merge strings | no duplicate strings to merge |
| `-2` 80286 | no 286-only constructs in the probes |

`fitflags.py` needs the cross-module caller (see the `ret` vs `retf` section) or
every exact comparison fails on that single byte for all ~24 candidate flag sets
- a systematic failure that is easy to misread as "no flag set works".

## Constant shifts routed through CL: a genuine BCC 3.1 blind spot

`FUN_1000_0f36` is byte-identical to the hand-written `FUN_1000_0f21` except for
one idiom:

```
target:   mov  cl, 4        ; b1 04        mine:  shl  dl, 4      ; c2 e0 04
          shl  dl, cl      ; d2 e2
```

`shl r/m8,cl` is the only 8-bit shift x86 encodes, so a *variable* shift count
has to be materialised in CL - but Borland folds a count it can see is 4 straight
into `shl r/m8,imm8`. Getting the long form means the count reached the shift as
a variable whose value Borland had already constant-propagated into CL and then
declined to re-fold.

Measured with `tools\py\shifts.py`:

* **27** `shl/shr r/m8,cl` sites are preceded by `mov cl,imm8`.
* They span **24 of 453** target functions, so it is systematic, not a one-off -
  it is not inline assembly in the original.
* `FUN_1000_07d1` reuses one `cl` for two shifts (`mov cl,4 / shr ax,cl / ... /
  shr bx,cl`), i.e. the count is a local the register allocator wanted to keep.
* `FUN_261d_0989` and `FUN_261d_084a` show the same shape on 16-bit shifts
  (`mov cl,8 / shl ax,cl`) inside `(int)(unsigned char)x << 8` argument pushes.

`tools\py\search.py 0x0F36` sweeps ~30 candidate C shapes (literal, `char`/`int`
local, `register`, `volatile`, enum, `sizeof`, ternary, `static const`, bitfields,
`8-4`, long operands, count used twice, ...) against 13 flag sets and **no
combination matches**. The closest is `static const unsigned char n = 4`, which
reproduces every other instruction exactly and differs only in the count's
storage: `mov cl,byte ptr [n]` vs `mov cl,4` (27 vs 25 bytes).

Conclusion: this is a codegen difference between BCC 3.1 and whatever built
PETE.EXE's game modules, not a wrong flag set. It is a known blind spot covering
~5% of functions; do not spend more time hand-tuning source for these, and do
not "fix" them by rewriting `src\flagshft.c` into something unnatural.

## Frame layout: `__near`, and where the arguments live

Borland reserves a word at `[bp+4]` for the segment half of a **far** return
address, so a `far` function's first argument sits at `[bp+6]`.  Declaring the
function `__near` drops that reservation and moves the first argument to
`[bp+4]`.  The target is split almost perfectly along those lines:

| Kind | First argument at `[bp+4]` | at `[bp+6]` |
|---|---|---|
| ends in `ret N` (near) | 27 | 2 |
| ends in `retf` (far) | 10 | 256 |

So `__pascal __near` plus a `char far *` parameter is what reproduces the ~29
functions that are called from inside their own module; everything else is a
cross-module function whose arguments start at `[bp+6]`.

`__pascal` also supplies the callee cleanup (`ret N`), and pushes arguments right
to left, so the **last** declared parameter lands in the **lowest** slot.  In
`src\farstreq.c` the loop reads `[bp+4]` first and `[bp+8]` second, so `s` is
declared *second*.

A far pointer in an argument slot is stored offset-then-segment, which is why
`inc word ptr [bp+4]` is simply `s++`.

## `enter` versus `sub sp`: proof that this is not the game's compiler

Measured on BCC 3.1 with a matrix of local-variable shapes (plain, array, large
array, `long`, several locals, address-taken, decayed array):

| Flag | Frame setup | Epilogue | Frameless multi-return epilogue |
|---|---|---|---|
| `-O` | `enter N,0` | `leave` | merged |
| `-O1` | `enter N,0` | `leave` | merged |
| `-O2` | `sub sp,N` | `leave` | **duplicated** |
| none | `enter N,0` | `leave` | merged |

`-r` (register variables) removes the frame altogether, so `-r-` is right.

In the target:

* **100** functions use `sub sp,N` + `leave`, and there is **not one `enter`**.
  Only `-O2` ever produces `sub sp` under 3.1, and there is no source-level
  workaround - every framed shape at `-O`/`-O1` produced `enter`.
* **30** frameless functions have two or more `ret`s whose epilogues are
  *merged* behind a `jmp` (e.g. `FUN_1000_10c1`).  3.1's `-O2` duplicates them.
* Conversely the 31 framed multi-return functions each carry their own
  `leave`+`ret` (`rets == leaves`), which is exactly `-O2`'s duplicating
  behaviour.

No single 3.1 flag set satisfies both halves.  `-O2` is kept as the global
default because it wins on function count (100 > 30), and `src\farstreq.c`
carries `@flags -O1` so that reconstruction is still checkable:

```
/* @flags  -O1 */
```

A per-source `@flags` line is honoured by `check.py` (via
`bcbuild.apply_overrides`); recognised values replace the matching config slot,
anything else is passed through.  So the two halves of the evidence are both
recorded rather than one being quietly discarded.  The confirmed settings live
in `tools\py\flags.json`.

Positive confirmation that the `-O2` half is the right one: `FUN_1df0_0222`
(61 bytes, 24 instructions, two locals, no calls) reconstructs to
`sub sp, 4` + the right local slots and the right loop shape.  Declaring the
table pointer `unsigned int near *` rather than plain `unsigned int *` was the
key step — in the large model a plain pointer is **far**, so Borland emits
`les bx,[bp-4]` and `mov ax,es:[bx+2]` where the target has `mov bx,[bp-4]`
and `mov ax,[bx+2]`.  What is left is a single 3-byte difference: the target
loads the record's low word into `dx` up front, before the first comparison,
while 3.1 loads it into `ax` later.  Two `return` statements, a 32-bit `long`
compare and a 14-byte `struct {unsigned lo, hi; char pad[6];}` were all tried
and all reproduce the rest exactly, so this looks like one more small
divergence rather than a misread of the function.

## Where 3.1 stores a 32-bit value: a fourth divergence

`FUN_1f92_0006` (18 bytes / 8 insns) is a two-word global store:

```
push bp / mov bp,sp
mov ax, word ptr [bp + 8]
mov dx, word ptr [bp + 6]
mov word ptr [0x386a], ax
mov word ptr [0x3868], dx
pop bp / retf
```

The target loads both halves before storing either, so the source is a single
32-bit assignment.  With the prototype reversed to `f(hi, lo)` and
`g = ((unsigned long)hi << 16) | lo`, BCC 3.1 reproduces the size (18/18) and
all four loads exactly, but stores the halves the other way round: 3.1 always
puts the `ax` half at the lower offset and the `dx` half at `+2`, whereas the
target puts `dx` at the lower offset.  A `unsigned long`/`long` target, two
separate globals and a `struct` with two word fields were all tried; the
`unsigned long` form is the only one that batches the loads, and no flag set
changes the store order (`fitflags` over 17 combinations: best 50%, 18/18
bytes).  Two separate globals make Borland store immediately after each load
(17 bytes, wrong shape), and a far-pointer destination cannot be right either
because far-pointer variables get segment-qualified stores.

So this is a fourth independent idiom disagreeing with 3.1, and it is the same
*kind* of disagreement as the CL-shift case: a register-allocation choice
rather than a misread of the function.  It is worth remembering when reading
other multi-word stores, because the natural reading of the target (high half
at `+2`) is the one 3.1 will not produce.

## Signedness is visible in the branch opcode

`FUN_18a2_0620` was reconstructed byte-exactly once one declaration was fixed.
The target branches with `jle` (7C) where an unsigned compare emits `jbe` (76),
so the record counter at `+0x49` is a plain `int`.  With `unsigned int` the
rebuild matched 6/10 instructions at exactly the right 26-byte size; changing
only that one field type to `int` took it to 26/26 bytes, 10/10 instructions.
`if (rec->cur++ > 0x28)` also reproduces the read-then-increment pair in the
right order.

Two near misses remain open in the same family, both blocked on 3.1 emitting a
redundant test that the target folds:

* `FUN_1e25_0426` — target does `mov ax,[+0x64] / or ax,[+0x66] / je`, i.e. one
  branch on the combined value.  Written as `if (rec->a || rec->b)` Borland
  short-circuits into two `cmp`/`jne` pairs (28 bytes); written as
  `if ((rec->a | rec->b) == 0)` it batches the loads correctly but then emits a
  redundant `or ax,ax` before the `je` (26 bytes vs 24).
* `FUN_18a2_06e9` — target tests the byte field as a widened int
  (`mov al,[+0x2b] / mov ah,0 / or ax,ax / jne`), which 3.1 folds to a single
  `cmp byte ptr es:[bx+0x2b],0` (and then reloads the pointer).  `unsigned char`
  and `int` locals, and an explicit `(unsigned)` cast, all give the folded form
  or spill to the stack (23 vs 23 and 23 vs 32 bytes).  `fitflags` over 17
  combinations found nothing better than 33%.

## What the record-access vein taught us

Eight of the seventeen exact reconstructions take a `struct ... far *` and touch
two or three byte or word fields.  That shape is unusually forgiving, and the
things that had to be right are worth recording because each one fails
silently.

**A reload of `es:bx` is a statement boundary, not a register decision.**
`FUN_18a2_0154` clamps `y` inside the `f32 == 3` block.  Written as a separate
top-level `if`, Borland emitted an extra `les bx,[bp+6]` before the clamp and the
byte count was 153 against the target's 189; nested inside the existing block,
`bx` stays live from the one load at 0x8b96 through 0x8bd4 and the whole 189
bytes match.  Whenever a far-pointer record is touched by two adjacent
statements, try both nestings before assuming the codegen is at fault.

**A missing block in the listing is not a missing block in the program.**
`FUN_18a2_0154` appeared to contain a jump straight from the middle of one
basic block to a merge point, which reads as a dead `mov ax,[bp-4]`.  The bytes
in between were never exported, so the apparent dead store was the only trace of
a whole clamp.  See the export-hole section in `README.md`; check
`out/target.full.txt` before concluding that target code is unreachable.

**Argument slots after a far pointer.**  The 4-byte pointer occupies
`[bp+6]`..`[bp+9]`, so the next argument is at `[bp+0xa]` and the one after at
`[bp+0xc]`.  Every one of the far-pointer functions confirms this, and getting
it wrong produces a plausible-looking frame that compares badly everywhere.

**Signedness is readable in the opcode.**  `jle` (7C) instead of `jbe` (76)
means a signed compare; `sar` instead of `shr` means a signed field.  This is a
one-declaration fix: `FUN_18a2_0620` sat at 6/10 instructions and exactly the
right 26-byte size until `unsigned int cur` became `int cur`, after which it was
byte-exact.

**Memory-vs-register decides where a comparison reads from.**  `FUN_1d19_02fe`
and `FUN_1d19_032f` both compare a field *after* storing to it
(`cmp word ptr es:[bx + 0xb], 0`), so the conditions must be written against
the struct field.  Writing them against the incoming parameter gets folded to a
register compare and the shape changes.

**`<<= 1` and `*= 2` differ.**  `u *= 2` produces `mov dx,2 / imul dx`; the
target's `shl word ptr [bp + 0xa], 1` requires `u <<= 1`.  This one cost 16
bytes on `FUN_1d19_0104` (86 vs 70) while looking semantically identical.

**Multi-return shape is load-bearing.**  In `FUN_1f44_04c4` and `FUN_1d19_032f`
the early `return` inside a branch arm is what gives that arm its own
`pop bp / retf`, leaving the other arm to fall through to a shared exit.
Flattening to a single exit changes the instruction count.

**Strength reduction reveals the record header size.**  `FUN_1f44_00fe` and
`FUN_1f44_0143` walk `blk->data[i]` as an offset held in one local, initialised
from the pointer's offset word plus a constant: `mov ax,[bp+6] / add ax,6`.
That constant *is* the data array's offset.  With the header at 3 instead of 6
the rebuild still matched 100% on shape and 69/69 bytes and failed only the
exact tier, on `add ax,3` versus `add ax,6` - a good reminder that shape plus
size agreement is not evidence of a correct layout.

**Loops over a far-pointer array reload `es` every iteration** but keep only the
offset in a local, so the local count is one per walking pointer, not per far
pointer.  `sub sp,4` for a 0x300-iteration copy loop is two words, not four.

**Sibling functions in a module share one record.**  `FUN_1f44_00fe` and
`FUN_1f44_0143` are 0x45 bytes apart, guard on the same `+0x02` flag and write
the same global at 0x3550 in opposite directions - they are two halves of one
save/restore pair in a single source file.  Cross-checking the second against
the first is what confirms the layout rather than merely fitting it.

## Chain assignment versus three statements

`FUN_1e25_0dde` clears three flag bytes and the target opens with

```
mov al, 0
mov byte ptr es:[bx + 5], al
mov byte ptr es:[bx], al
mov byte ptr es:[bx + 4], al
```

Three separate `c->f5 = 0; c->f0 = 0; c->f4 = 0;` statements give three
immediate stores instead - 95 bytes rather than 94.  Two obvious workarounds
both fail:

* a local `unsigned char z = 0;` is spilled, adding `sub sp,2` and a
  `mov byte ptr [bp-1],0` (103 bytes), and
* `register unsigned char z = 0;` spills identically.

The chain `c->f4 = c->f0 = c->f5 = 0;` is byte-exact.  C evaluates an
assignment's right-hand side once and then stores right-to-left, so the value
lives in `al` for all three stores - and the *store order* (5, 0, 4) is exactly
the right-to-left order of the chain.  This is a reliable fingerprint: whenever
a target loads a constant into a register once and then stores it to several
disjoint fields, look for a chain rather than a local.

## Bitfield tests, and where Borland won't match them

Thirty functions in the target test a single flag with the same four-instruction
sequence, e.g. `FUN_18a2_1d2c`:

```
mov al, byte ptr es:[bx + 0x65]
shr ax, 2
and ax, 1
or ax, ax
jz  <skip>
```

The `mov al` / `shr ax` pair is the signature of a **bitfield** read, and it is
reproducible: declare the flag as a one-bit field and Borland emits exactly
`shr ax,2 / and ax,1`.  Even the bit numbering is recoverable - bit 0 fields
emit no shift at all (`mov al, byte / and ax,1`), bit 1 emits `shr ax,1`.

What Borland will not reproduce is the final `or ax, ax`.  It narrows to
`or al, al` on the grounds that `and ax,1` cleared the upper bits, so the two
are equivalent.  Attempts that all failed to widen it: `int t = a->bitfield`
tested through the local (which spilled, adding `sub sp,2`), the same as
`unsigned int`, `== 1`, and `(bitfield & 3) != 0`.

A *signed* one-bit field is not the answer either - its range is -1..0, so
Borland emits `shl ax,0xf` to sign-extend instead of a logical shift.

This is therefore the fifth independent place where the game's compiler
disagrees with BCC 3.1, and unlike the others it is systematic: it will block
any function whose only bitfield use is a test.  Functions that merely *set* a
bit are fine, because `or byte ptr es:[bx + 0x66], 1` needs no test at all -
`src/clippos.c` was matched on the first attempt with a plain `unsigned char`
field and `|= 1`.

## Spilled locals versus register-allocated ones

`FUN_18a2_0700` (flat 0x9120, 29 insns) is a clean 27/29 but for five bytes:
the target allocates a stack slot (`sub sp,2 / push si / ... / pop si`) and
caches `*c` in it, where Borland keeps the value in a register and skips both
prologue and epilogue pairs.  `volatile int t = *c;` does not force the spill
either.  Recorded as a near-miss rather than chased further.

## Turbo C runtime code is in the target's function list

`FUN_1000_097c` looked like a frameless `strlen`:

```
push di / mov cx,0xffff / xor al,al / repne scasb / not cx / dec cx / pop di / ret
```

It is not game code.  Its only caller (`FUN_1000_09c6`, 1071 bytes, clearly
Turbo C's `_printf` family) sets the pointer in `DI` with `mov di,0x238a` or
`les di,[...]` immediately before each `call 0x97c`, and reads the length back
out of `CX`.  There is no stack frame, so no argument slot can be read - this is
Borland's register-convention RTL `strlen`, and Ghidra has simply labelled
library code as a function.

The practical rule: the 0x900-0x9ff cluster is RTL, as are the `int 0x21` and
`int 0x10` wrappers and the far-call thunks.  Excluding them matters, because
they are otherwise very attractive targets - short, frameless and call-free -
and none of them are reconstructible as ordinary C.

## Open questions

* **Which Borland built the game modules.** This is now the main open question.
  Four independent idioms say it was not this compiler:
  1. the `enter` / `sub sp` split above (100 functions vs 30),
  2. epilogue merging in frameless multi-return functions, which 3.1's `-O2`
     does not do,
  3. the constant-shift-through-CL blind spot,
  4. 32-bit stores putting the `dx` half at the lower offset (above).
  All four point the same way: the game's optimiser allocates frames like 3.1's
  `-O2` but merges epilogues like 3.1's `-O`/`-O1`, and folds `shl r/m8,imm8`
  more aggressively.  The 3.1 runtime matched 76.6% of the load module, so the
  *runtime* is 3.1; only the game modules disagree.  Worth checking a 3.0 or 3.2
  archive if one becomes available.
* **`-O` versus `-O1`.** Indistinguishable so far: on every probe tried they emit
  identical code.  `FUN_1000_10c1` matches at both.
* **`-k`.** The target's frame-size distribution (`sub sp,N` with N = 2, 4, 6,
  8 ...) and its `push bp / mov bp,sp` prologue in 80.6% of functions are
  consistent with the default frame, but not yet proven against `-k`.
* **`-Z`.** Still untested against a discriminating function.

## Tools

```
python tools/py/check.py show 0x0F21          # disassembly of a target function
python tools/py/check.py diff 0x0F21 src/x.c # build + two-tier comparison
python tools/py/check.py all                  # every src/*.c, progress table
python tools/py/check.py list                 # summary
python tools/py/check.py list --markdown      # summary as a markdown table
python tools/py/fitflags.py src/x.c           # which flag set reproduces it
python tools/py/search.py 0x0F36 --all        # brute-force candidate C shapes
python tools/py/shifts.py                     # which idioms the target uses
python tools/py/style.py                      # codegen style statistics
python tools/py/fingerprint.py                # runtime byte fingerprint
```

Comparison tiers, as chosen for this project:

* **shape** - mnemonics, operand *kinds* and instruction *sizes* match; every
  address-valued field is a wildcard.
* **exact** - bytes identical except inside address-valued fields, with the
  address correspondence reported so it can be eyeballed.

## Large-model calls, and where a module boundary shows up

The Borland large model gives every `.c` file its own code segment, and that
single fact explains the two call encodings `src/main.c` has to mix:

* a callee in the **same** `.c` file is the cheap 5-byte
  `nop / push cs / call rel32`, because both halves share a segment and the
  callee's `retf` pops the pushed `CS`;
* a callee in a **different** `.c` file gets its own segment, so the same source
  call becomes `lcall seg:off` with a relocated pair.

`FUN_13b2_000f` calls ten functions in its own module and six in other modules,
so it needs stub modules to build at all.  Verified by compiling probe pairs
under `-ml -1 -O2 -r-`; the boundary is the file, not the `far` keyword -
writing an explicit `far` on a same-file callee does not change the encoding.

A corollary, learned the hard way: **prototypes are load-bearing at the call
site.** `m_f44b` was declared `unsigned` in `src/xmod/xf44b.c` and `unsigned
char` in `src/main.c`; Borland used the prototype visible at the call and emitted
a `mov ah,0` widening that the target does not have.  Both declarations have to
agree.

## Two build-harness limits worth recording

* **DOS caps the command tail at 127 characters.** Past seven objects the
  Turbo Link argument list is silently truncated, so TLINK reports only
  `Fatal: DOS error, ax = 2` and leaves a 0-byte `.EXE` - which reads exactly
  like a duplicate-symbol error.  `bcbuild.py` now always passes the link
  arguments in a TLINK `@response` file, which has no such limit.
* **A `@extra` list must stay on one line.** The marker captures only the rest
  of its line, so a wrapped list silently drops every continuation - the build
  then fails on undefined symbols rather than complaining about the marker.

## Trailing dead epilogues

Borland emits a function's epilogue even when the function ends in an
unconditional jump and can never fall through; the epilogue is simply dead code.
Ghidra ends the function at the jump and leaves the bytes as an unclaimed gap,
so a faithful rebuild looks a few bytes too long.  `repairholes.py` hands those
bytes back when the gap is exactly `pop di / pop si / pop bp / retf`, the
preceding instruction is a near `jmp`, and the next function starts immediately
after.  Only `FUN_13b2_000f` qualified.
## Global data addresses are masked, so do not chase them

`diffasm.norm_operand` classifies a memory displacement by magnitude.  Anything
at or below `SMALL_DISP` is compared literally; anything larger becomes a `mem?`
token and the bytes are masked.  The same applies to immediates that fall inside
the image size, which arrive as `addr`.

The consequence is worth stating plainly because it is easy to lose an afternoon
to: **the address of a global never has to match.**  A minimal module's
uninitialised data starts at `_DATA` end, so Borland puts the first BSS object at
offset `0x2C6` no matter what you call it or how big it is:

    _DATA  010E:0000  size 0x2C0
    _G_3550 010E:02C6          <- BSS, and 0x2C6 is not 0x3550

`push 0x2C6` and the target's `push 0x3550` compare equal.  So globals are named
`g_2444`, `g_3550` and friends purely to keep the *intent* legible next to the
target listing, not because the number is load-bearing.  It is still worth using
the real addresses in the names: they make the listing readable and they cost
nothing.

What this does *not* excuse is a frame-size or `[bp-N]` difference.  `bp`-relative
displacements are small, so they are compared exactly, and `sub sp, 0xa` against
`sub sp, 4` is a real mismatch even though every data reference in the function
passes.

## Two modules cannot both define the same global

`FUN_1000_1ae1` and `FUN_1000_18e4` are adjacent in the target's module 0 and
share the latch at `0x2444`: `key_poll` tests it, `key_read` clears it.  Rebuilding
them as two separate files - which the large model forces, since main reaches them
by `lcall` - gives two definitions of the same symbol, and Turbo Link says:

    Fatal: DOS error, ax = 8 in module KEYPOLL.C

`ax = 8` here is the duplicate symbol, not the 127-character tail truncation of the
section above.  The fix is to make one module the owner and have the other say
`extern`, with `@extra` pointing back at the owner so each file still builds
standalone for `check.py`.

## Borland folds `&global` but still stores it

`FUN_1f44_01de` is the one function in this batch that resists.  Everything up to
the countdown gate matches, but the target keeps the far pointer in a local:

    mov word ptr [bp - 8], ds
    mov word ptr [bp - 0xa], 0x3850
    les bx, ptr [bp - 0xa]
    mov al, byte ptr es:[bx + 2]

while `&g_node` is a link-time constant, so Borland emits the two stores as dead
code and then folds the uses into `mov es, [bp - 8]` plus a direct `es:[0x5c8]`.
Declaring the local pointer `volatile` does not stop it: the stores are still
dead, the fold still happens.

Two smaller lessons came out of the same attempt.  `sub sp, 0xa` for a single
four-byte local is reproduced by wrapping the pointer in an anonymous struct with
six spare bytes - the aggregate forces the frame, and the spare member costs
nothing because it is never touched.  And `!--g.b` versus `!g.b--` selects
between `add al, 0xff` / `mov [x], al` and `dec byte ptr [x]`: the post-decrement
form loads the old value before touching memory, which is what the target does.

The unresolved part is the first branch.  `&&` compiles to one `je` that skips
both the gate and the body, where the target has `jne` into the gate plus a `jmp`
past it - same semantics, different block placement.  What makes this function
unfinished rather than merely imperfect is its shape: it runs `0xF61E` to
`0xF6B6`, but the code stops at `0xF666` and the shared epilogue sits at
`0xF6B4`, leaving `0xF667..0xF6B3` unreferenced and containing no instructions at
all.  Borland does not emit dead byte ranges, so the boundary is probably wrong
and chasing 152 bytes may be chasing a Ghidra artefact.
