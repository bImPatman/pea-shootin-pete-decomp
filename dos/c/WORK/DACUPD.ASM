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
	?debug	S "dacupd.c"
	?debug	C E9F792445D086461637570642E63
DACUPD_TEXT	segment byte public 'CODE'
DACUPD_TEXT	ends
DGROUP	group	_DATA,_BSS
	assume	cs:DACUPD_TEXT,ds:DGROUP
_DATA	segment word public 'DATA'
d@	label	byte
d@w	label	word
_DATA	ends
_BSS	segment word public 'BSS'
b@	label	byte
b@w	label	word
_BSS	ends
DACUPD_TEXT	segment byte public 'CODE'
   ;	
   ;	void m_f44c(char far *p)
   ;	
	assume	cs:DACUPD_TEXT
_m_f44c	proc	far
	push	bp
	mov	bp,sp
   ;	
   ;	{
   ;	    if (p[2])
   ;	
	les	bx,dword ptr [bp+6]
	cmp	byte ptr es:[bx+2],0
	je	short @1@86
   ;	
   ;	        dac_write(g_3550);
   ;	
	push	ds
	push	offset DGROUP:_g_3550
	call	far ptr _dac_write
	add	sp,4
@1@86:
   ;	
   ;	}
   ;	
	pop	bp
	ret	
_m_f44c	endp
DACUPD_TEXT	ends
_BSS	segment word public 'BSS'
_g_3550	label	byte
	db	768 dup (?)
	?debug	C E9
	?debug	C FA00000000
_BSS	ends
_DATA	segment word public 'DATA'
s@	label	byte
_DATA	ends
DACUPD_TEXT	segment byte public 'CODE'
DACUPD_TEXT	ends
	public	_m_f44c
	extrn	_dac_write:far
	public	_g_3550
_s@	equ	s@
	end
