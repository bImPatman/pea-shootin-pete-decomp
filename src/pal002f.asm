; @target FUN_1fa6_002f
; @name   FUN_1fa6_002f
; @proto  void FUN_1fa6_002f(char far *dst, char far *src, int n)
; @module cross
;
; Slide a run of DAC colour triples one position with a backward `rep movsb`
; (`std`), then patch the three colour bytes back at the source end.  SI and DI
; both carry meaningful state across the loop, so BCC cannot emit this body
; without wrappers; TASM reproduces it exactly.
;
; m_f44d knows this address as `pal_shift`, so the body is emitted under both
; names to keep that byte-exact caller resolving.

    .186
PAL002F_TEXT segment byte public 'CODE'
    assume cs:PAL002F_TEXT

pal002f_body macro
    local pal002f_loop
    push bp
    mov  bp, sp
    mov  ax, [bp+8]
    mov  bx, [bp+6]
    sub  ax, bx
    mov  dl, al
    mov  ax, [bp+8]
    mov  di, [bp+0ah]
    add  di, ax
    mov  si, di
    dec  si
    mov  bh, [di]
    inc  di
    mov  bl, [di]
    inc  di
    mov  dh, [di]
    mov  ax, ds
    mov  es, ax
    std
pal002f_loop:
    mov  cx, 1
    rep  movsb
    dec  dl
    jne  pal002f_loop
    add  si, 3
    mov  [si], dh
    dec  si
    mov  [si], bl
    dec  si
    mov  [si], bh
    pop  bp
    retf
    endm

    public _FUN_1fa6_002f
_FUN_1fa6_002f proc far
    pal002f_body
_FUN_1fa6_002f endp

    public _pal_shift
_pal_shift proc far
    pal002f_body
_pal_shift endp

PAL002F_TEXT ends
    end
