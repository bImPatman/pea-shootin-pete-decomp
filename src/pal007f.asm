; @target FUN_1fa6_007f
; @name   FUN_1fa6_007f
; @proto  void FUN_1fa6_007f(char far *buf)
; @module cross
;
; Palette fade.  Seeds g_1d28 with the caller's 0x300 bytes and g_1a28 with
; zeroes, then runs 0x40 passes: each pass nudges every channel toward its
; target by `cl`, waits for the display retrace, and pushes the work buffer to
; the DAC with `rep outsb`.
;
; The two near calls inside this body both target FUN_1fa6_0127, which in the
; original lives 0x48 bytes further on, immediately after FUN_1fa6_00df.  A
; near `call` encodes the distance, so the two neighbours are reproduced here
; in their original order -- otherwise the rel16 would not match the target.
; Both neighbours are bijective here and harmless: nothing else in the tree
; references them.

    .186
PAL007F_TEXT segment byte public 'CODE'
    assume cs:PAL007F_TEXT

; --------------------------------------------------------------------------
; FUN_1fa6_007f -- palette fade (public, the function under test)
; --------------------------------------------------------------------------
    public _FUN_1fa6_007f
_FUN_1fa6_007f proc far
    push bp
    mov  bp, sp
    mov  bx, [bp+6]
    mov  si, 0
    mov  ah, 0
    mov  cx, 300h
pal007f_copy:
    mov  al, [bx]
    mov  [si+1d28h], al
    mov  [si+1a28h], ah
    inc  si
    inc  bx
    loop pal007f_copy
    mov  cx, 40h
pal007f_outer:
    mov  bx, 0
pal007f_scan:
    mov  al, [bx+1d28h]
    mov  dl, [bx+1a28h]
    cmp  dl, al
    je   pal007f_next
    sub  al, cl
    cmp  al, 0
    jg   pal007f_store
    mov  byte ptr [bx+1a28h], 0
    jmp  pal007f_next
pal007f_store:
    mov  [bx+1a28h], al
pal007f_next:
    inc  bx
    cmp  bx, 300h
    jl   pal007f_scan
    call _FUN_1fa6_0127
    mov  dx, 03c8h
    mov  al, 0
    out  dx, al
    inc  dx
    mov  si, 1a28h
    push cx
    mov  cx, 300h
    cld
    rep  outsb
    pop  cx
    loop pal007f_outer
    pop  bp
    retf
_FUN_1fa6_007f endp

; --------------------------------------------------------------------------
; FUN_1fa6_00df -- same shape, but only darkens (decrements) channels
; --------------------------------------------------------------------------
    public _FUN_1fa6_00df
_FUN_1fa6_00df proc far
    push bp
    mov  bp, sp
    push si
    mov  bx, [bp+6]
    mov  si, 0
    mov  cx, 300h
pal00df_copy:
    mov  al, [bx]
    mov  [si+1a28h], al
    inc  si
    inc  bx
    loop pal00df_copy
    mov  cx, 40h
pal00df_dec:
    xor  bx, bx
pal00df_scan:
    cmp  byte ptr [bx+1a28h], 0
    je   pal00df_skip
    dec  byte ptr [bx+1a28h]
pal00df_skip:
    inc  bx
    cmp  bx, 300h
    jl   pal00df_scan
    call _FUN_1fa6_0127
    mov  dx, 03c8h
    mov  al, 0
    out  dx, al
    inc  dx
    mov  si, 1a28h
    push cx
    mov  cx, 300h
    cld
    rep  outsb
    pop  cx
    loop pal00df_dec
    pop  si
    pop  bp
    retf
_FUN_1fa6_00df endp

; --------------------------------------------------------------------------
; FUN_1fa6_0127 -- wait for the vertical retrace bit (port 0x3DA, bit 3)
; --------------------------------------------------------------------------
    public _FUN_1fa6_0127
_FUN_1fa6_0127 proc near
    push es
    push ax
    push dx
    push bx
    mov  ax, 40h
    mov  es, ax
    mov  dx, es:[0063h]
    add  dx, 6
pal0127_wait:
    in   al, dx
    test al, 8
    je   pal0127_wait
    pop  bx
    pop  dx
    pop  ax
    pop  es
    ret
_FUN_1fa6_0127 endp

PAL007F_TEXT ends
    end
