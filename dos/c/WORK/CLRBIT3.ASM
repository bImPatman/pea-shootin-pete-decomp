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
	?debug	S "clrbit3.c"
	?debug	C E92482445D09636C72626974332E63
CLRBIT3_TEXT	segment byte public 'CODE'
CLRBIT3_TEXT	ends
DGROUP	group	_DATA,_BSS
	assume	cs:CLRBIT3_TEXT,ds:DGROUP
_DATA	segment word public 'DATA'
d@	label	byte
d@w	label	word
_DATA	ends
_BSS	segment word public 'BSS'
b@	label	byte
b@w	label	word
_BSS	ends
CLRBIT3_TEXT	segment byte public 'CODE'
   ;	
   ;	void clrbit3(unsigned char far *rec)
   ;	
	assume	cs:CLRBIT3_TEXT
_clrbit3	proc	far
	push	bp
	mov	bp,sp
   ;	
   ;	{
   ;	    rec[3] = 0;
   ;	
	les	bx,dword ptr [bp+6]
	mov	byte ptr es:[bx+3],0
   ;	
   ;	}
   ;	
	pop	bp
	ret	
_clrbit3	endp
	?debug	C E9
	?debug	C FA00000000
CLRBIT3_TEXT	ends
_DATA	segment word public 'DATA'
s@	label	byte
_DATA	ends
CLRBIT3_TEXT	segment byte public 'CODE'
CLRBIT3_TEXT	ends
	public	_clrbit3
_s@	equ	s@
	end
