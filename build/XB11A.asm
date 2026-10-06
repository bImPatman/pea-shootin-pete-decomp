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
	?debug	S "xb11a.c"
	?debug	C E9E68B445D0778623131612E63
XB11A_TEXT	segment byte public 'CODE'
XB11A_TEXT	ends
DGROUP	group	_DATA,_BSS
	assume	cs:XB11A_TEXT,ds:DGROUP
_DATA	segment word public 'DATA'
d@	label	byte
d@w	label	word
_DATA	ends
_BSS	segment word public 'BSS'
b@	label	byte
b@w	label	word
_BSS	ends
XB11A_TEXT	segment byte public 'CODE'
   ;	
   ;	int m_b11a(char far *p) { return p == 0; }
   ;	
	assume	cs:XB11A_TEXT
_m_b11a	proc	far
	push	bp
	mov	bp,sp
	mov	ax,word ptr [bp+6]
	or	ax,word ptr [bp+8]
	jne	short @1@86
	mov	ax,1
	jmp	short @1@114
@1@86:
	xor	ax,ax
@1@114:
	pop	bp
	ret	
_m_b11a	endp
	?debug	C E9
	?debug	C FA00000000
XB11A_TEXT	ends
_DATA	segment word public 'DATA'
s@	label	byte
_DATA	ends
XB11A_TEXT	segment byte public 'CODE'
XB11A_TEXT	ends
	public	_m_b11a
_s@	equ	s@
	end
