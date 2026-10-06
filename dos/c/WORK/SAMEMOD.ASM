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
	?debug	S "samemod.c"
	?debug	C E9E795455D0973616D656D6F642E63
SAMEMOD_TEXT	segment byte public 'CODE'
SAMEMOD_TEXT	ends
DGROUP	group	_DATA,_BSS
	assume	cs:SAMEMOD_TEXT,ds:DGROUP
_DATA	segment word public 'DATA'
d@	label	byte
d@w	label	word
_DATA	ends
_BSS	segment word public 'BSS'
b@	label	byte
b@w	label	word
_BSS	ends
SAMEMOD_TEXT	segment byte public 'CODE'
   ;	
   ;	int __pascal __near farstreq(char far *t, char far *s)
   ;	
	assume	cs:SAMEMOD_TEXT
FARSTREQ	proc	near
	push	bp
	mov	bp,sp
	jmp	short @1@114
@1@58:
   ;	
   ;	{
   ;	    while (*s) {
   ;	        if (*s++ != *t++) {
   ;	
	les	bx,dword ptr [bp+4]
	inc	word ptr [bp+4]
	mov	al,byte ptr es:[bx]
	les	bx,dword ptr [bp+8]
	inc	word ptr [bp+8]
	cmp	al,byte ptr es:[bx]
	je	short @1@114
   ;	
   ;	            return 0;
   ;	
	xor	ax,ax
	jmp	short @1@170
@1@114:
	les	bx,dword ptr [bp+4]
	cmp	byte ptr es:[bx],0
	jne	short @1@58
   ;	
   ;	        }
   ;	    }
   ;	    return 1;
   ;	
	mov	ax,1
@1@170:
   ;	
   ;	}
   ;	
	pop	bp
	ret	8
FARSTREQ	endp
   ;	
   ;	void main(void) { farstreq((char far *)0, (char far *)0); }
   ;	
	assume	cs:SAMEMOD_TEXT
_main	proc	far
	push	0
	push	0
	push	0
	push	0
	call	near ptr FARSTREQ
	ret	
_main	endp
	?debug	C E9
	?debug	C FA00000000
SAMEMOD_TEXT	ends
_DATA	segment word public 'DATA'
s@	label	byte
_DATA	ends
SAMEMOD_TEXT	segment byte public 'CODE'
SAMEMOD_TEXT	ends
	public	_main
	public	FARSTREQ
_s@	equ	s@
	end
