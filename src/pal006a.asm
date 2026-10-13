; @target FUN_1fa6_006a
; @name   FUN_1fa6_006a
; @proto  void FUN_1fa6_006a(char far *buf)
; @module cross
;
; Upload 0x300 bytes from the caller's buffer to the VGA DAC data port with a
; single `rep outsb`.  BCC 3.1 saves and restores SI around any body that
; touches it, which is why this 21-byte leaf cannot be produced from C: the
; compiler either emits the extra push/pop si (23 bytes) or refuses `outsb`.
; TASM preserves the exact encoding, so the reconstruction is byte-exact.
;
; The original had one address known to its callers under two local names --
; `dac_write` in m_f44c and `upload` in m_f44d.  The body is emitted once per
; name so both byte-exact callers keep resolving, without editing them.

    .186
PAL006A_TEXT segment byte public 'CODE'
    assume cs:PAL006A_TEXT

pal006a_body macro
    push bp
    mov  bp, sp
    mov  si, [bp+6]
    mov  dx, 03C8h
    mov  al, 0
    out  dx, al
    inc  dx
    mov  cx, 300h
    cld
    rep  outsb
    pop  bp
    retf
    endm

    public _FUN_1fa6_006a
_FUN_1fa6_006a proc far
    pal006a_body
_FUN_1fa6_006a endp

    public _dac_write
_dac_write proc far
    pal006a_body
_dac_write endp

    public _upload
_upload proc far
    pal006a_body
_upload endp

PAL006A_TEXT ends
    end
