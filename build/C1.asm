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
	?debug	S "c1.c"
	?debug	C E93A82445D0463312E63
C1_TEXT	segment byte public 'CODE'
C1_TEXT	ends
DGROUP	group	_DATA,_BSS
	assume	cs:C1_TEXT,ds:DGROUP
_DATA	segment word public 'DATA'
d@	label	byte
d@w	label	word
_DATA	ends
_BSS	segment word public 'BSS'
b@	label	byte
b@w	label	word
_BSS	ends
C1_TEXT	segment byte public 'CODE'
   ;	
   ;	unsigned char c1(struct chk_s far *rec)
   ;	
	assume	cs:C1_TEXT
_c1	proc	far
	push	bp
	mov	bp,sp
   ;	
   ;	{
   ;	    if ((rec->a | rec->b) == 0)
   ;	
	les	bx,dword ptr [bp+6]
	mov	ax,word ptr es:[bx+100]
	or	ax,word ptr es:[bx+102]
	or	ax,ax
	jne	short @1@86
   ;	
   ;	        return 0;
   ;	
	mov	al,0
	pop	bp
	ret	
@1@86:
   ;	
   ;	    return 1;
   ;	
	mov	al,1
   ;	
   ;	}
   ;	
	pop	bp
	ret	
_c1	endp
	?debug	C E9
	?debug	C FA00000000
C1_TEXT	ends
_DATA	segment word public 'DATA'
s@	label	byte
_DATA	ends
C1_TEXT	segment byte public 'CODE'
C1_TEXT	ends
	public	_c1
_s@	equ	s@
	end
