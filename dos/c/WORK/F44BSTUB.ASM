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
	?debug	S "f44bstub.c"
	?debug	C E9708A455D0A66343462737475622E63
F44BSTUB_TEXT	segment byte public 'CODE'
F44BSTUB_TEXT	ends
DGROUP	group	_DATA,_BSS
	assume	cs:F44BSTUB_TEXT,ds:DGROUP
_DATA	segment word public 'DATA'
d@	label	byte
d@w	label	word
_DATA	ends
_BSS	segment word public 'BSS'
b@	label	byte
b@w	label	word
	db	2 dup (?)
_BSS	ends
F44BSTUB_TEXT	segment byte public 'CODE'
   ;	
   ;	int m_f44b(char far *p, unsigned char v)
   ;	
	assume	cs:F44BSTUB_TEXT
_m_f44b	proc	far
	push	bp
	mov	bp,sp
   ;	
   ;	{
   ;	    static int logged;
   ;	    stub_boot();
   ;	
	call	far ptr _stub_boot
   ;	
   ;	    if (!logged) { logged = 1; note("m_f44b", "FUN_1f44_029e"); }
   ;	
	cmp	word ptr DGROUP:b@w+0,0
	jne	short @1@86
	mov	word ptr DGROUP:b@w+0,1
	push	ds
	push	offset DGROUP:s@+7
	push	ds
	push	offset DGROUP:s@
	call	far ptr _note
	add	sp,8
@1@86:
   ;	
   ;	    return 0;
   ;	
	xor	ax,ax
   ;	
   ;	}
   ;	
	pop	bp
	ret	
_m_f44b	endp
	?debug	C E9
	?debug	C FA00000000
F44BSTUB_TEXT	ends
_DATA	segment word public 'DATA'
s@	label	byte
	db	'm_f44b'
	db	0
	db	'FUN_1f44_029e'
	db	0
_DATA	ends
F44BSTUB_TEXT	segment byte public 'CODE'
F44BSTUB_TEXT	ends
	public	_m_f44b
	extrn	_note:far
	extrn	_stub_boot:far
_s@	equ	s@
	end
