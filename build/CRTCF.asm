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
	?debug	S "crtcf.c"
	?debug	C E9EB84455D0763727463662E63
CRTCF_TEXT	segment byte public 'CODE'
CRTCF_TEXT	ends
DGROUP	group	_DATA,_BSS
	assume	cs:CRTCF_TEXT,ds:DGROUP
_DATA	segment word public 'DATA'
d@	label	byte
d@w	label	word
_DATA	ends
_BSS	segment word public 'BSS'
b@	label	byte
b@w	label	word
_BSS	ends
CRTCF_TEXT	segment byte public 'CODE'
   ;	
   ;	void setcrtc(unsigned short x)
   ;	
	assume	cs:CRTCF_TEXT
_setcrtc	proc	far
	push	bp
	mov	bp,sp
   ;	
   ;	{
   ;	    asm {
   ;	        mov     bl, 0x0d
   ;	
	mov	     bl, 00dH
   ;	
   ;	        mov     bh, byte ptr [bp+6]
   ;	
	mov	     bh, byte ptr [bp+6]
   ;	
   ;	        mov     cl, 0x0c
   ;	
	mov	     cl, 00cH
   ;	
   ;	        mov     ch, byte ptr [bp+7]
   ;	
	mov	     ch, byte ptr [bp+7]
   ;	
   ;	        mov     dx, 03dah
   ;	
	mov	     dx, 03dah
@1@170:
   ;	
   ;	    }
   ;	lw_h:
   ;	    asm {
   ;	        in      al, dx
   ;	
	in	      al, dx
   ;	
   ;	        test    al, 1
   ;	
	test	    al, 1
   ;	
   ;	        jne     lw_h
   ;	
	jne	short @1@170
   ;	
   ;	        mov     dx, 03d4h
   ;	
	mov	     dx, 03d4h
   ;	
   ;	    }
   ;	    _AX = _BX;
   ;	
	mov	ax,bx
   ;	
   ;	    asm {
   ;	        out     dx, ax
   ;	
	out	     dx, ax
   ;	
   ;	    }
   ;	    _AX = _CX;
   ;	
	mov	ax,cx
   ;	
   ;	    asm {
   ;	        out     dx, ax
   ;	
	out	     dx, ax
   ;	
   ;	    }
   ;	    _DX = 0x3da;
   ;	
	mov	dx,986
@1@422:
   ;	
   ;	lw_v:
   ;	    asm {
   ;	        in      al, dx
   ;	
	in	      al, dx
   ;	
   ;	        test    al, 8
   ;	
	test	    al, 8
   ;	
   ;	        je      lw_v
   ;	
	je	short @1@422
   ;	
   ;	    }
   ;	}
   ;	
	pop	bp
	ret	
_setcrtc	endp
	?debug	C E9
	?debug	C FA00000000
CRTCF_TEXT	ends
_DATA	segment word public 'DATA'
s@	label	byte
_DATA	ends
CRTCF_TEXT	segment byte public 'CODE'
CRTCF_TEXT	ends
	public	_setcrtc
_s@	equ	s@
	end
