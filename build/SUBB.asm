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
	?debug	S "subb.c"
	?debug	C E9AA8B445D06737562622E63
SUBB_TEXT	segment byte public 'CODE'
SUBB_TEXT	ends
DGROUP	group	_DATA,_BSS
	assume	cs:SUBB_TEXT,ds:DGROUP
_DATA	segment word public 'DATA'
d@	label	byte
d@w	label	word
_DATA	ends
_BSS	segment word public 'BSS'
b@	label	byte
b@w	label	word
_BSS	ends
SUBB_TEXT	segment byte public 'CODE'
   ;	
   ;	int subb(int a){ return a; }
   ;	
	assume	cs:SUBB_TEXT
_subb	proc	far
	push	bp
	mov	bp,sp
	mov	ax,word ptr [bp+6]
	pop	bp
	ret	
_subb	endp
	?debug	C E9
	?debug	C FA00000000
SUBB_TEXT	ends
_DATA	segment word public 'DATA'
s@	label	byte
_DATA	ends
SUBB_TEXT	segment byte public 'CODE'
SUBB_TEXT	ends
	public	_subb
_s@	equ	s@
	end
