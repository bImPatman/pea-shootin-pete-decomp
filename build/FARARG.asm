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
	?debug	S "fararg.c"
	?debug	C E9898B445D086661726172672E63
FARARG_TEXT	segment byte public 'CODE'
FARARG_TEXT	ends
DGROUP	group	_DATA,_BSS
	assume	cs:FARARG_TEXT,ds:DGROUP
_DATA	segment word public 'DATA'
d@	label	byte
d@w	label	word
_DATA	ends
_BSS	segment word public 'BSS'
b@	label	byte
b@w	label	word
_BSS	ends
FARARG_TEXT	segment byte public 'CODE'
   ;	
   ;	void caller(void){ f1(1); }
   ;	
	assume	cs:FARARG_TEXT
_caller	proc	far
	push	1
	call	far ptr _f1
	add	sp,2
	ret	
_caller	endp
   ;	
   ;	void far f1(int a){ }
   ;	
	assume	cs:FARARG_TEXT
_f1	proc	far
	push	bp
	mov	bp,sp
	pop	bp
	ret	
_f1	endp
	?debug	C E9
	?debug	C FA00000000
FARARG_TEXT	ends
_DATA	segment word public 'DATA'
s@	label	byte
_DATA	ends
FARARG_TEXT	segment byte public 'CODE'
FARARG_TEXT	ends
	public	_caller
	public	_f1
_s@	equ	s@
	end
