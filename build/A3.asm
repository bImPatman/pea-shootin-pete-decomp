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
	?debug	S "a3.c"
	?debug	C E936A8445D0461332E63
A3_TEXT	segment byte public 'CODE'
A3_TEXT	ends
DGROUP	group	_DATA,_BSS
	assume	cs:A3_TEXT,ds:DGROUP
_DATA	segment word public 'DATA'
d@	label	byte
d@w	label	word
_DATA	ends
_BSS	segment word public 'BSS'
b@	label	byte
b@w	label	word
_BSS	ends
A3_TEXT	segment byte public 'CODE'
   ;	
   ;	int m_b159(struct table160 far *p)
   ;	
	assume	cs:A3_TEXT
_m_b159	proc	far
	push	bp
	mov	bp,sp
	sub	sp,4
   ;	
   ;	{
   ;	    int i;
   ;	    i = 0;
   ;	
	mov	word ptr [bp-2],0
	mov	ax,word ptr [bp+6]
	mov	word ptr [bp-4],ax
	jmp	short @1@170
@1@86:
   ;	
   ;	    while (p->count > i) { FUN_1b11_1257(p->fn[i]); i++; }
   ;	
	mov	es,word ptr [bp+8]
	mov	bx,word ptr [bp-4]
	push	word ptr es:[bx+2]
	push	word ptr es:[bx]
	call	far ptr _FUN_1b11_1257
	add	sp,4
	add	word ptr [bp-4],4
	inc	word ptr [bp-2]
@1@170:
	les	bx,dword ptr [bp+6]
	mov	al,byte ptr es:[bx+400]
	mov	ah,0
	cmp	ax,word ptr [bp-2]
	jg	short @1@86
   ;	
   ;	}
   ;	
	leave	
	ret	
_m_b159	endp
   ;	
   ;	void far FUN_1b11_1257(char far *fn) { fn = fn; }
   ;	
	assume	cs:A3_TEXT
_FUN_1b11_1257	proc	far
	push	bp
	mov	bp,sp
	pop	bp
	ret	
_FUN_1b11_1257	endp
	?debug	C E9
	?debug	C FA04010000
A3_TEXT	ends
_DATA	segment word public 'DATA'
s@	label	byte
_DATA	ends
A3_TEXT	segment byte public 'CODE'
A3_TEXT	ends
	public	_m_b159
	public	_FUN_1b11_1257
_s@	equ	s@
	end
