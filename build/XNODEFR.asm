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
	?debug	S "xnodefr.c"
	?debug	C E90592445D09786E6F646566722E63
XNODEFR_TEXT	segment byte public 'CODE'
XNODEFR_TEXT	ends
DGROUP	group	_DATA,_BSS
	assume	cs:XNODEFR_TEXT,ds:DGROUP
_DATA	segment word public 'DATA'
d@	label	byte
d@w	label	word
_DATA	ends
_BSS	segment word public 'BSS'
b@	label	byte
b@w	label	word
_BSS	ends
XNODEFR_TEXT	segment byte public 'CODE'
   ;	
   ;	void node_free(char far *p) { }
   ;	
	assume	cs:XNODEFR_TEXT
_node_free	proc	far
	push	bp
	mov	bp,sp
	pop	bp
	ret	
_node_free	endp
	?debug	C E9
	?debug	C FA00000000
XNODEFR_TEXT	ends
_DATA	segment word public 'DATA'
s@	label	byte
_DATA	ends
XNODEFR_TEXT	segment byte public 'CODE'
XNODEFR_TEXT	ends
	public	_node_free
_s@	equ	s@
	end
