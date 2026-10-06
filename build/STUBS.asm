	.286
	ifndef	??version
?debug	macro
	endm
publicdll macro	name
	public	name
	endm
$comm	macro	name,dist,size,count
	comm	dist name:BYTE:count*size
	endm
	else
$comm	macro	name,dist,size,count
	comm	dist name[size]:BYTE:count
	endm
	endif
	?debug	V 300h
	?debug	S "stubs.c"
	?debug	C E9D790455D0773747562732E63
	?debug	C E94019CA1812443A5C494E434C5544455C737464696F2E68
	?debug	C E94019CA1812443A5C494E434C5544455C5F646566732E68
	?debug	C E94019CA1813443A5C494E434C5544455C5F6E66696C652E68
	?debug	C E94019CA1812443A5C494E434C5544455C5F6E756C6C2E68
STUBS_TEXT	segment byte public 'CODE'
STUBS_TEXT	ends
DGROUP	group	_DATA,_BSS
	assume	cs:STUBS_TEXT,ds:DGROUP
_DATA	segment word public 'DATA'
d@	label	byte
d@w	label	word
_DATA	ends
_BSS	segment word public 'BSS'
b@	label	byte
b@w	label	word
_BSS	ends
_DATA	segment word public 'DATA'
booted	label	word
	db	0
	db	0
_DATA	ends
STUBS_TEXT	segment byte public 'CODE'
   ;	
   ;	void emit(char *line)
   ;	
	assume	cs:STUBS_TEXT
_emit	proc	far
	push	bp
	mov	bp,sp
	sub	sp,4
   ;	
   ;	{
   ;	    FILE *f;
   ;	
   ;	    printf("%s\n", line);
   ;	
	push	word ptr [bp+8]
	push	word ptr [bp+6]
	push	ds
	push	offset DGROUP:s@
	call	far ptr _printf
	add	sp,8
   ;	
   ;	
   ;	    f = fopen(GUEST_LOG, "a");
   ;	
	push	ds
	push	offset DGROUP:s@+21
	push	ds
	push	offset DGROUP:s@+4
	call	far ptr _fopen
	add	sp,8
	mov	word ptr [bp-2],dx
	mov	word ptr [bp-4],ax
   ;	
   ;	    if (!f)
   ;	
	or	ax,word ptr [bp-2]
	je	short @1@86
   ;	
   ;	        return;               /* logging must never take the game down */
   ;	    fprintf(f, "%s\n", line);
   ;	
	push	word ptr [bp+8]
	push	word ptr [bp+6]
	push	ds
	push	offset DGROUP:s@+23
	push	word ptr [bp-2]
	push	word ptr [bp-4]
	call	far ptr _fprintf
	add	sp,12
   ;	
   ;	    fclose(f);
   ;	
	push	word ptr [bp-2]
	push	word ptr [bp-4]
	call	far ptr _fclose
	add	sp,4
@1@86:
   ;	
   ;	}
   ;	
	leave	
	ret	
_emit	endp
   ;	
   ;	void stub_boot(void)
   ;	
	assume	cs:STUBS_TEXT
_stub_boot	proc	far
	push	di
   ;	
   ;	{
   ;	    int i;
   ;	
   ;	    if (booted)
   ;	
	cmp	word ptr DGROUP:booted,0
	je	@@0
	jmp	@2@170
@@0:
   ;	
   ;	        return;
   ;	    booted = 1;
   ;	
	mov	word ptr DGROUP:booted,1
   ;	
   ;	
   ;	    /* main() never returns, so a buffered stdout is never flushed and the
   ;	       console stays blank however much gets printed. */
   ;	    setvbuf(stdout, NULL, _IONBF, 0);
   ;	
	push	0
	push	2
	push	0
	push	0
	push	ds
	push	offset DGROUP:__streams+20
	call	far ptr _setvbuf
	add	sp,12
   ;	
   ;	
   ;	    for (i = 0; i < 0x308; i++)
   ;	
	mov	cx,388
	mov	di,offset DGROUP:rec
	push	ds
	pop	es
	xor	ax,ax
	rep 	stosw	
   ;	
   ;	        rec[i] = 0;
   ;	    g_3858 = (char far *)rec;
   ;	
	mov	word ptr DGROUP:_g_3858+2,ds
	mov	word ptr DGROUP:_g_3858,offset DGROUP:rec
   ;	
   ;	    g_27aa = (char far *)rec;
   ;	
	mov	word ptr DGROUP:_g_27aa+2,ds
	mov	word ptr DGROUP:_g_27aa,offset DGROUP:rec
   ;	
   ;	    g_34a9 = (char far *)rec;
   ;	
	mov	word ptr DGROUP:_g_34a9+2,ds
	mov	word ptr DGROUP:_g_34a9,offset DGROUP:rec
   ;	
   ;	
   ;	    /* The counts below describe the default build.  This file is linked by
   ;	       --with-partial too, and that build swaps in the exact m_f44b, so its real
   ;	       list is one longer and it prints no m_f44b gap line -- that difference is
   ;	       the whole way to tell the two builds apart from the log alone. */
   ;	    emit("[stub] main() has 18 callees; 13 are reconstructed and linked");
   ;	
	push	ds
	push	offset DGROUP:s@+27
	push	cs
	call	near ptr _emit
	add	sp,4
   ;	
   ;	    emit("[stub]   real: key_poll key_read statebak clrbit3 m_1a40");
   ;	
	push	ds
	push	offset DGROUP:s@+89
	push	cs
	call	near ptr _emit
	add	sp,4
   ;	
   ;	    emit("[stub]         m_b159 m_f44c m_e256 node_free buf_to_vga");
   ;	
	push	ds
	push	offset DGROUP:s@+146
	push	cs
	call	near ptr _emit
	add	sp,4
   ;	
   ;	    emit("[stub]   inert no-ops in src/mcallees.c, not yet reconstructed:");
   ;	
	push	ds
	push	offset DGROUP:s@+203
	push	cs
	call	near ptr _emit
	add	sp,4
   ;	
   ;	    emit("[stub]     m_1df6 m_38a2 m_2415 m_24e7 m_0dd6 m_154f");
   ;	
	push	ds
	push	offset DGROUP:s@+267
	push	cs
	call	near ptr _emit
	add	sp,4
   ;	
   ;	    emit("[stub]     m_013b m_34e5 m_35dd");
   ;	
	push	ds
	push	offset DGROUP:s@+320
	push	cs
	call	near ptr _emit
	add	sp,4
   ;	
   ;	    emit("[stub] next gap reached will be reported below");
   ;	
	push	ds
	push	offset DGROUP:s@+352
	push	cs
	call	near ptr _emit
	add	sp,4
@2@170:
   ;	
   ;	}
   ;	
	pop	di
	ret	
_stub_boot	endp
   ;	
   ;	void note(char *name, char *target)
   ;	
	assume	cs:STUBS_TEXT
_note	proc	far
	push	bp
	mov	bp,sp
	sub	sp,160
   ;	
   ;	{
   ;	    char line[160];
   ;	
   ;	    sprintf(line, "[stub] %-11s -> %-16s not reconstructed, ignored",
   ;	
   ;	
   ;	            name, target);
   ;	
	push	word ptr [bp+12]
	push	word ptr [bp+10]
	push	word ptr [bp+8]
	push	word ptr [bp+6]
	push	ds
	push	offset DGROUP:s@+399
	push	ss
	lea	ax,word ptr [bp-160]
	push	ax
	call	far ptr _sprintf
	add	sp,16
   ;	
   ;	    emit(line);
   ;	
	push	ss
	lea	ax,word ptr [bp-160]
	push	ax
	push	cs
	call	near ptr _emit
	add	sp,4
   ;	
   ;	}
   ;	
	leave	
	ret	
_note	endp
STUBS_TEXT	ends
_BSS	segment word public 'BSS'
	db	2 dup (?)
_BSS	ends
STUBS_TEXT	segment byte public 'CODE'
   ;	
   ;	int m_b11a(char far *p)
   ;	
	assume	cs:STUBS_TEXT
_m_b11a	proc	far
	push	bp
	mov	bp,sp
   ;	
   ;	{
   ;	    static int logged;
   ;	    stub_boot();
   ;	
	push	cs
	call	near ptr _stub_boot
   ;	
   ;	    if (!logged) { logged = 1; note("m_b11a", "FUN_1b11_1a94"); }
   ;	
	cmp	word ptr DGROUP:b@w+0,0
	jne	short @4@86
	mov	word ptr DGROUP:b@w+0,1
	push	ds
	push	offset DGROUP:s@+455
	push	ds
	push	offset DGROUP:s@+448
	push	cs
	call	near ptr _note
	add	sp,8
@4@86:
   ;	
   ;	    return 0;
   ;	
	xor	ax,ax
   ;	
   ;	}
   ;	
	pop	bp
	ret	
_m_b11a	endp
STUBS_TEXT	ends
_BSS	segment word public 'BSS'
	db	2 dup (?)
_BSS	ends
STUBS_TEXT	segment byte public 'CODE'
   ;	
   ;	int m_f44d(char far *p)
   ;	
	assume	cs:STUBS_TEXT
_m_f44d	proc	far
	push	bp
	mov	bp,sp
   ;	
   ;	{
   ;	    static int logged;
   ;	    stub_boot();
   ;	
	push	cs
	call	near ptr _stub_boot
   ;	
   ;	    if (!logged) { logged = 1; note("m_f44d", "FUN_1f44_01de"); }
   ;	
	cmp	word ptr DGROUP:b@w+2,0
	jne	short @5@86
	mov	word ptr DGROUP:b@w+2,1
	push	ds
	push	offset DGROUP:s@+476
	push	ds
	push	offset DGROUP:s@+469
	push	cs
	call	near ptr _note
	add	sp,8
@5@86:
   ;	
   ;	    return 0;
   ;	
	xor	ax,ax
   ;	
   ;	}
   ;	
	pop	bp
	ret	
_m_f44d	endp
STUBS_TEXT	ends
_BSS	segment word public 'BSS'
rec	label	byte
	db	776 dup (?)
	?debug	C E9
	?debug	C FA40010000
_BSS	ends
_DATA	segment word public 'DATA'
s@	label	byte
	db	'%s'
	db	10
	db	0
	db	'C:\OUT\GUEST.LOG'
	db	0
	db	'a'
	db	0
	db	'%s'
	db	10
	db	0
	db	'[stub] main() has 18 callees; 13 are reconstructed and linked'
	db	0
	db	'[stub]   real: key_poll key_read statebak clrbit3 m_1a40'
	db	0
	db	'[stub]         m_b159 m_f44c m_e256 node_free buf_to_vga'
	db	0
	db	'[stub]   inert no-ops in src/mcallees.c, not yet reconstructe'
	db	'd:'
	db	0
	db	'[stub]     m_1df6 m_38a2 m_2415 m_24e7 m_0dd6 m_154f'
	db	0
	db	'[stub]     m_013b m_34e5 m_35dd'
	db	0
	db	'[stub] next gap reached will be reported below'
	db	0
	db	'[stub] %-11s -> %-16s not reconstructed, ignored'
	db	0
	db	'm_b11a'
	db	0
	db	'FUN_1b11_1a94'
	db	0
	db	'm_f44d'
	db	0
	db	'FUN_1f44_01de'
	db	0
_DATA	ends
STUBS_TEXT	segment byte public 'CODE'
STUBS_TEXT	ends
	public	_m_f44d
	public	_m_b11a
	public	_note
	public	_stub_boot
	public	_emit
_booted	equ	booted
_rec	equ	rec
	extrn	_g_34a9:dword
	extrn	_g_27aa:dword
	extrn	_g_3858:dword
	extrn	_sprintf:far
	extrn	_setvbuf:far
	extrn	_printf:far
	extrn	_fprintf:far
	extrn	_fopen:far
	extrn	_fclose:far
	extrn	__streams:word
_s@	equ	s@
	end
