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
	?debug	S "plain.c"
	?debug	C E9848B445D07706C61696E2E63
PLAIN_TEXT	segment byte public 'CODE'
PLAIN_TEXT	ends
DGROUP	group	_DATA,_BSS
	assume	cs:PLAIN_TEXT,ds:DGROUP
_DATA	segment word public 'DATA'
d@	label	byte
d@w	label	word
_DATA	ends
_BSS	segment word public 'BSS'
b@	label	byte
b@w	label	word
_BSS	ends
PLAIN_TEXT	segment byte public 'CODE'
   ;	
   ;	void caller(void){ f1(); }
   ;	
	assume	cs:PLAIN_TEXT
_caller	proc	far
	call	far ptr _f1
	ret	
_caller	endp
   ;	
   ;	void f1(void){ }
   ;	
	assume	cs:PLAIN_TEXT
_f1	proc	far
	ret	
_f1	endp
	?debug	C E9
	?debug	C FA00000000
PLAIN_TEXT	ends
_DATA	segment word public 'DATA'
s@	label	byte
_DATA	ends
PLAIN_TEXT	segment byte public 'CODE'
PLAIN_TEXT	ends
	public	_caller
	public	_f1
_s@	equ	s@
	end
