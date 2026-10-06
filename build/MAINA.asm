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
	?debug	S "maina.c"
	?debug	C E9AA8B445D076D61696E612E63
MAINA_TEXT	segment byte public 'CODE'
MAINA_TEXT	ends
DGROUP	group	_DATA,_BSS
	assume	cs:MAINA_TEXT,ds:DGROUP
_DATA	segment word public 'DATA'
d@	label	byte
d@w	label	word
_DATA	ends
_BSS	segment word public 'BSS'
b@	label	byte
b@w	label	word
_BSS	ends
MAINA_TEXT	segment byte public 'CODE'
   ;	
   ;	void maina(void){ subb(1); }
   ;	
	assume	cs:MAINA_TEXT
_maina	proc	far
	push	1
	call	far ptr _subb
	add	sp,2
	ret	
_maina	endp
	?debug	C E9
	?debug	C FA00000000
MAINA_TEXT	ends
_DATA	segment word public 'DATA'
s@	label	byte
_DATA	ends
MAINA_TEXT	segment byte public 'CODE'
MAINA_TEXT	ends
	public	_maina
	extrn	_subb:far
_s@	equ	s@
	end
