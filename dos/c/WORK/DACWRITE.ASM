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
	?debug	S "dacwrite.c"
	?debug	C E9AD91445D0A64616377726974652E63
DACWRITE_TEXT	segment byte public 'CODE'
DACWRITE_TEXT	ends
DGROUP	group	_DATA,_BSS
	assume	cs:DACWRITE_TEXT,ds:DGROUP
_DATA	segment word public 'DATA'
d@	label	byte
d@w	label	word
_DATA	ends
_BSS	segment word public 'BSS'
b@	label	byte
b@w	label	word
_BSS	ends
DACWRITE_TEXT	segment byte public 'CODE'
   ;	
   ;	void dac_write(char far *p)
   ;	
	assume	cs:DACWRITE_TEXT
_dac_write	proc	far
	push	bp
	mov	bp,sp
	push	si
   ;	
   ;	{
   ;	    asm {
   ;	        mov si, word ptr p
   ;	
	mov	 si, word ptr [bp+6]
   ;	
   ;	        mov dx, 03c8h
   ;	
	mov	 dx, 03c8h
   ;	
   ;	        mov al, 0
   ;	
	mov	 al, 0
   ;	
   ;	        out dx, al
   ;	
	out	 dx, al
   ;	
   ;	        inc dx
   ;	
	inc	 dx
   ;	
   ;	        mov cx, 0300h
   ;	
	mov	 cx, 0300h
   ;	
   ;	        cld
   ;	
	cld	
   ;	
   ;	        rep outsb
   ;	
	rep outsb	
   ;	
   ;	    }
   ;	}
   ;	
	pop	si
	pop	bp
	ret	
_dac_write	endp
	?debug	C E9
	?debug	C FA00000000
DACWRITE_TEXT	ends
_DATA	segment word public 'DATA'
s@	label	byte
_DATA	ends
DACWRITE_TEXT	segment byte public 'CODE'
DACWRITE_TEXT	ends
	public	_dac_write
_s@	equ	s@
	end
