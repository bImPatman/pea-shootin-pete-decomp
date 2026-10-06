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
	?debug	S "fb2.c"
	?debug	C E9956A445D056662322E63
FB2_TEXT	segment byte public 'CODE'
FB2_TEXT	ends
DGROUP	group	_DATA,_BSS
	assume	cs:FB2_TEXT,ds:DGROUP
_DATA	segment word public 'DATA'
d@	label	byte
d@w	label	word
_DATA	ends
_BSS	segment word public 'BSS'
b@	label	byte
b@w	label	word
_BSS	ends
FB2_TEXT	segment byte public 'CODE'
   ;	
   ;	void main(void) { flagbits_set(0); }
   ;	
	assume	cs:FB2_TEXT
_main	proc	far
	push	0
	call	far ptr _flagbits_set
	add	sp,2
	ret	
_main	endp
	?debug	C E9
	?debug	C FA00000000
FB2_TEXT	ends
_DATA	segment word public 'DATA'
s@	label	byte
_DATA	ends
FB2_TEXT	segment byte public 'CODE'
FB2_TEXT	ends
	public	_main
	extrn	_flagbits_set:far
_s@	equ	s@
	end
