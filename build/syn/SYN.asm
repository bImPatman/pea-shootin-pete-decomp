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
	?debug	S "syn.c"
	?debug	C E90070445D0573796E2E63
SYN_TEXT	segment byte public 'CODE'
SYN_TEXT	ends
DGROUP	group	_DATA,_BSS
	assume	cs:SYN_TEXT,ds:DGROUP
_DATA	segment word public 'DATA'
d@	label	byte
d@w	label	word
_DATA	ends
_BSS	segment word public 'BSS'
b@	label	byte
b@w	label	word
_BSS	ends
SYN_TEXT	segment byte public 'CODE'
   ;	
   ;	int __pascal __near F(int a, int b) { return a + b; }
   ;	
	assume	cs:SYN_TEXT
F	proc	near
	push	bp
	mov	bp,sp
	mov	ax,word ptr [bp+6]
	add	ax,word ptr [bp+4]
	pop	bp
	ret	4
F	endp
	?debug	C E9
	?debug	C FA00000000
SYN_TEXT	ends
_DATA	segment word public 'DATA'
s@	label	byte
_DATA	ends
SYN_TEXT	segment byte public 'CODE'
SYN_TEXT	ends
	public	F
_s@	equ	s@
	end
