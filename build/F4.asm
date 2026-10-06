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
	?debug	S "f4.c"
	?debug	C E94D84455D0466342E63
F4_TEXT	segment byte public 'CODE'
F4_TEXT	ends
DGROUP	group	_DATA,_BSS
	assume	cs:F4_TEXT,ds:DGROUP
_DATA	segment word public 'DATA'
d@	label	byte
d@w	label	word
_DATA	ends
_BSS	segment word public 'BSS'
b@	label	byte
b@w	label	word
_BSS	ends
F4_TEXT	segment byte public 'CODE'
   ;	
   ;	static unsigned char in_stat(void)
   ;	
	assume	cs:F4_TEXT
in_stat	proc	far
   ;	
   ;	{
   ;	    _DX = 0x3da;
   ;	
	mov	dx,986
   ;	
   ;	    asm { in al, dx }
   ;	
 	in	 al, dx 
   ;	
   ;	    return _AL;
   ;	
   ;	
   ;	}
   ;	
	ret	
in_stat	endp
   ;	
   ;	void setcrtc(unsigned short x)
   ;	
	assume	cs:F4_TEXT
_setcrtc	proc	far
	push	bp
	mov	bp,sp
	sub	sp,2
   ;	
   ;	{
   ;	    unsigned int packed;
   ;	
   ;	    packed = (unsigned int)(((x & 0xff) << 8) | 0x0d);
   ;	
	mov	ax,word ptr [bp+6]
	and	ax,255
	shl	ax,8
	or	ax,13
	mov	word ptr [bp-2],ax
   ;	
   ;	    _BX = (unsigned int)packed;
   ;	
	mov	bx,word ptr [bp-2]
   ;	
   ;	    packed = (unsigned int)((x & 0xff00) | 0x0c);
   ;	
	mov	ax,word ptr [bp+6]
	and	ax,-256
	or	ax,12
	mov	word ptr [bp-2],ax
   ;	
   ;	    _CX = (unsigned int)packed;
   ;	
	mov	cx,word ptr [bp-2]
@2@58:
   ;	
   ;	    while (in_stat() & 1)
   ;	
	push	cs
	call	near ptr in_stat
	mov	ah,0
	test	ax,1
	jne	short @2@58
   ;	
   ;	        ;
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
@2@198:
   ;	
   ;	    while (!(in_stat() & 8))
   ;	
	push	cs
	call	near ptr in_stat
	mov	ah,0
	test	ax,8
	je	short @2@198
   ;	
   ;	        ;
   ;	}
   ;	
	leave	
	ret	
_setcrtc	endp
	?debug	C E9
	?debug	C FA00000000
F4_TEXT	ends
_DATA	segment word public 'DATA'
s@	label	byte
_DATA	ends
F4_TEXT	segment byte public 'CODE'
F4_TEXT	ends
	public	_setcrtc
_in_stat	equ	in_stat
_s@	equ	s@
	end
