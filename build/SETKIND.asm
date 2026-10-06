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
	?debug	S "setkind.c"
	?debug	C E92082445D097365746B696E642E63
SETKIND_TEXT	segment byte public 'CODE'
SETKIND_TEXT	ends
DGROUP	group	_DATA,_BSS
	assume	cs:SETKIND_TEXT,ds:DGROUP
_DATA	segment word public 'DATA'
d@	label	byte
d@w	label	word
_DATA	ends
_BSS	segment word public 'BSS'
b@	label	byte
b@w	label	word
_BSS	ends
SETKIND_TEXT	segment byte public 'CODE'
   ;	
   ;	void setkind(unsigned char far *rec)
   ;	
	assume	cs:SETKIND_TEXT
_setkind	proc	far
	push	bp
	mov	bp,sp
   ;	
   ;	{
   ;	    rec[0x30] = 1;
   ;	
	les	bx,dword ptr [bp+6]
	mov	byte ptr es:[bx+48],1
   ;	
   ;	}
   ;	
	pop	bp
	ret	
_setkind	endp
	?debug	C E9
	?debug	C FA00000000
SETKIND_TEXT	ends
_DATA	segment word public 'DATA'
s@	label	byte
_DATA	ends
SETKIND_TEXT	segment byte public 'CODE'
SETKIND_TEXT	ends
	public	_setkind
_s@	equ	s@
	end
