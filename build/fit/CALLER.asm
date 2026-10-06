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
	?debug	S "caller.c"
	?debug	C E9A381445D0863616C6C65722E63
CALLER_TEXT	segment byte public 'CODE'
CALLER_TEXT	ends
DGROUP	group	_DATA,_BSS
	assume	cs:CALLER_TEXT,ds:DGROUP
_DATA	segment word public 'DATA'
d@	label	byte
d@w	label	word
_DATA	ends
_BSS	segment word public 'BSS'
b@	label	byte
b@w	label	word
_BSS	ends
CALLER_TEXT	segment byte public 'CODE'
   ;	
   ;	void main(void) { w1((unsigned int)0, (unsigned int)0); }
   ;	
	assume	cs:CALLER_TEXT
_main	proc	far
	push	0
	push	0
	call	far ptr _w1
	add	sp,4
	ret	
_main	endp
	?debug	C E9
	?debug	C FA00000000
CALLER_TEXT	ends
_DATA	segment word public 'DATA'
s@	label	byte
_DATA	ends
CALLER_TEXT	segment byte public 'CODE'
CALLER_TEXT	ends
	public	_main
	extrn	_w1:far
_s@	equ	s@
	end
