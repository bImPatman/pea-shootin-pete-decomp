; @target FUN_1fba_0002
; @name   FUN_1fba_0002
; @proto  void FUN_1fba_0002(int x, int y, int x2, int y2, int col, int base)
; @module cross
;
; Mode X rectangle fill across the four VGA planes.  Selects the Sequence
; Controller Map Mask register (index 2, port 3C5), computes DI from
; (y*80) + (x>>2) + base, and walks the row with `stosb`/`rep stosb` while
; switching the active plane mask through the two lookup tables at
; DS:0x2028 and DS:0x202c.  SI/DI/BP are all live across the inner loop, so
; the compiler cannot emit this; TASM reproduces the hand-written original.

    .186
FBA0002_TEXT segment byte public 'CODE'
    assume cs:FBA0002_TEXT

    public _FUN_1fba_0002
_FUN_1fba_0002 proc far
    push bp
    mov  bp, sp
    push si
    push di
    cld
    mov  ax, 50h
    mul  word ptr [bp+8]
    mov  di, [bp+6]
    shr  di, 1
    shr  di, 1
    add  di, ax
    add  di, [bp+0eh]
    mov  ax, 0a000h
    mov  es, ax
    mov  dx, 03c4h
    mov  al, 2
    out  dx, al
    inc  dx
    mov  si, [bp+6]
    and  si, 3
    mov  bh, [si+2028h]
    mov  si, [bp+0ah]
    and  si, 3
    mov  bl, [si+202ch]
    mov  cx, [bp+0ah]
    mov  si, [bp+6]
    cmp  cx, si
    jle  fba0002_done
    dec  cx
    and  si, 0fffch
    sub  cx, si
    shr  cx, 1
    shr  cx, 1
    jne  fba0002_mid
    and  bh, bl
fba0002_mid:
    mov  si, [bp+0ch]
    sub  si, [bp+8]
    jle  fba0002_done
    mov  ah, [bp+10h]
    mov  bp, 50h
    sub  bp, cx
    dec  bp
fba0002_col:
    push cx
    mov  al, bh
    out  dx, al
    mov  al, ah
    stosb
    dec  cx
    js   fba0002_tail
    je   fba0002_endmask
    mov  al, 0fh
    out  dx, al
    mov  al, ah
    rep  stosb
fba0002_endmask:
    mov  al, bl
    out  dx, al
    mov  al, ah
    stosb
fba0002_tail:
    add  di, bp
    pop  cx
    dec  si
    jne  fba0002_col
fba0002_done:
    pop  di
    pop  si
    pop  bp
    retf
_FUN_1fba_0002 endp

FBA0002_TEXT ends
    end
