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
	?debug	S "s4.c"
	?debug	C E98AAA445D0473342E63
S4_TEXT	segment byte public 'CODE'
S4_TEXT	ends
DGROUP	group	_DATA,_BSS
	assume	cs:S4_TEXT,ds:DGROUP
_DATA	segment word public 'DATA'
d@	label	byte
d@w	label	word
_DATA	ends
_BSS	segment word public 'BSS'
b@	label	byte
b@w	label	word
_BSS	ends
S4_TEXT	segment byte public 'CODE'
   ;	
   ;	void setcrtc(unsigned short x)
   ;	
	assume	cs:S4_TEXT
_setcrtc	proc	far
	push	bp
	mov	bp,sp
   ;	
   ;	{
   ;	    _DX = 0x3da;
   ;	
	mov	dx,986
@1@58:
   ;	
   ;	    for (;;) {
   ;	        asm { in al, dx }
   ;	
 	in	 al, dx 
   ;	
   ;	        if (!(_AL & 1))
   ;	
	mov	ah,0
	test	ax,1
	jne	short @1@58
   ;	
   ;	            break;
   ;	
   ;	
   ;	    }
   ;	    _BX = ((x & 0xff) << 8) | 0x0d;
   ;	
	mov	bx,word ptr [bp+6]
	and	bx,255
	shl	bx,8
	or	bx,13
   ;	
   ;	    _CX = (x & 0xff00) | 0x0c;
   ;	
	mov	cx,word ptr [bp+6]
	and	cx,-256
	or	cx,12
   ;	
   ;	    _DX = 0x3d4;
   ;	
	mov	dx,980
   ;	
   ;	    _AX = _BX;
   ;	
	mov	ax,bx
   ;	
   ;	    asm { out dx, ax }
   ;	
 	out	 dx, ax 
   ;	
   ;	    _AX = _CX;
   ;	
	mov	ax,cx
   ;	
   ;	    asm { out dx, ax }
   ;	
 	out	 dx, ax 
   ;	
   ;	    _DX = 0x3da;
   ;	
	mov	dx,986
@1@254:
   ;	
   ;	    for (;;) {
   ;	        asm { in al, dx }
   ;	
 	in	 al, dx 
   ;	
   ;	        if (_AL & 8)
   ;	
	test	al,8
	je	short @1@254
   ;	
   ;	            break;
   ;	
   ;	
   ;	    }
   ;	}
   ;	
	pop	bp
	ret	
_setcrtc	endp
	?debug	C E9
	?debug	C FA00000000
S4_TEXT	ends
_DATA	segment word public 'DATA'
s@	label	byte
_DATA	ends
S4_TEXT	segment byte public 'CODE'
S4_TEXT	ends
	public	_setcrtc
_s@	equ	s@
	end
