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
	?debug	S "xshift.c"
	?debug	C E9EA8D455D087873686966742E63
XSHIFT_TEXT	segment byte public 'CODE'
XSHIFT_TEXT	ends
DGROUP	group	_DATA,_BSS
	assume	cs:XSHIFT_TEXT,ds:DGROUP
_DATA	segment word public 'DATA'
d@	label	byte
d@w	label	word
_DATA	ends
_BSS	segment word public 'BSS'
b@	label	byte
b@w	label	word
_BSS	ends
XSHIFT_TEXT	segment byte public 'CODE'
   ;	
   ;	void pal_shift(unsigned int to, unsigned int from, char far *buf)
   ;	
	assume	cs:XSHIFT_TEXT
_pal_shift	proc	far
	push	bp
	mov	bp,sp
   ;	
   ;	{
   ;	}
   ;	
	pop	bp
	ret	
_pal_shift	endp
	?debug	C E9
	?debug	C FA00000000
XSHIFT_TEXT	ends
_DATA	segment word public 'DATA'
s@	label	byte
_DATA	ends
XSHIFT_TEXT	segment byte public 'CODE'
XSHIFT_TEXT	ends
	public	_pal_shift
_s@	equ	s@
	end
