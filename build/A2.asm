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
	?debug	S "a2.c"
	?debug	C E90A82445D0461322E63
A2_TEXT	segment byte public 'CODE'
A2_TEXT	ends
DGROUP	group	_DATA,_BSS
	assume	cs:A2_TEXT,ds:DGROUP
_DATA	segment word public 'DATA'
d@	label	byte
d@w	label	word
_DATA	ends
_BSS	segment word public 'BSS'
b@	label	byte
b@w	label	word
_BSS	ends
A2_TEXT	segment byte public 'CODE'
   ;	
   ;	void a2(unsigned char far *p)
   ;	
	assume	cs:A2_TEXT
_a2	proc	far
	push	bp
	mov	bp,sp
   ;	
   ;	{
   ;	    p[3] = 0;
   ;	
	les	bx,dword ptr [bp+6]
	mov	byte ptr es:[bx+3],0
   ;	
   ;	}
   ;	
	pop	bp
	ret	
_a2	endp
	?debug	C E9
	?debug	C FA00000000
A2_TEXT	ends
_DATA	segment word public 'DATA'
s@	label	byte
_DATA	ends
A2_TEXT	segment byte public 'CODE'
A2_TEXT	ends
	public	_a2
_s@	equ	s@
	end
