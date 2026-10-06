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
	?debug	S "xf44b.c"
	?debug	C E90F8F445D0778663434622E63
XF44B_TEXT	segment byte public 'CODE'
XF44B_TEXT	ends
DGROUP	group	_DATA,_BSS
	assume	cs:XF44B_TEXT,ds:DGROUP
_DATA	segment word public 'DATA'
d@	label	byte
d@w	label	word
_DATA	ends
_BSS	segment word public 'BSS'
b@	label	byte
b@w	label	word
_BSS	ends
XF44B_TEXT	segment byte public 'CODE'
   ;	
   ;	int m_f44b(char far *p, unsigned char v) { return v + (p == 0); }
   ;	
	assume	cs:XF44B_TEXT
_m_f44b	proc	far
	push	bp
	mov	bp,sp
	mov	al,byte ptr [bp+10]
	mov	ah,0
	push	ax
	mov	ax,word ptr [bp+6]
	or	ax,word ptr [bp+8]
	jne	short @1@86
	mov	ax,1
	jmp	short @1@114
@1@86:
	xor	ax,ax
@1@114:
	mov	dx,ax
	pop	ax
	add	ax,dx
	pop	bp
	ret	
_m_f44b	endp
	?debug	C E9
	?debug	C FA00000000
XF44B_TEXT	ends
_DATA	segment word public 'DATA'
s@	label	byte
_DATA	ends
XF44B_TEXT	segment byte public 'CODE'
XF44B_TEXT	ends
	public	_m_f44b
_s@	equ	s@
	end
