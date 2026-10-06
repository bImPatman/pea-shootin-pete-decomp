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
	?debug	S "v6slot.c"
	?debug	C E910A8445D087636736C6F742E63
V6SLOT_TEXT	segment byte public 'CODE'
V6SLOT_TEXT	ends
DGROUP	group	_DATA,_BSS
	assume	cs:V6SLOT_TEXT,ds:DGROUP
_DATA	segment word public 'DATA'
d@	label	byte
d@w	label	word
_DATA	ends
_BSS	segment word public 'BSS'
b@	label	byte
b@w	label	word
_BSS	ends
V6SLOT_TEXT	segment byte public 'CODE'
   ;	
   ;	int m_b159(struct table160 far *p)
   ;	
	assume	cs:V6SLOT_TEXT
_m_b159	proc	far
	push	bp
	mov	bp,sp
	sub	sp,2
   ;	
   ;	{
   ;	    int i = 0;
   ;	
	mov	word ptr [bp-2],0
	jmp	short @1@86
@1@58:
   ;	
   ;	    while (p->count > i) {
   ;	        FUN_1b11_1257(p->fn[i]);
   ;	
	mov	ax,word ptr [bp-2]
	shl	ax,2
	les	bx,dword ptr [bp+6]
	add	bx,ax
	push	word ptr es:[bx+2]
	push	word ptr es:[bx]
	call	far ptr _FUN_1b11_1257
	add	sp,4
   ;	
   ;	        i = i + 1;
   ;	
	mov	ax,word ptr [bp-2]
	inc	ax
	mov	word ptr [bp-2],ax
@1@86:
	les	bx,dword ptr [bp+6]
	mov	al,byte ptr es:[bx+400]
	mov	ah,0
	cmp	ax,word ptr [bp-2]
	jg	short @1@58
   ;	
   ;	    }
   ;	}
   ;	
	leave	
	ret	
_m_b159	endp
   ;	
   ;	void FUN_1b11_1257(char far *fn) { fn = fn; }
   ;	
	assume	cs:V6SLOT_TEXT
_FUN_1b11_1257	proc	far
	push	bp
	mov	bp,sp
	pop	bp
	ret	
_FUN_1b11_1257	endp
	?debug	C E9
	?debug	C FA00010000
V6SLOT_TEXT	ends
_DATA	segment word public 'DATA'
s@	label	byte
_DATA	ends
V6SLOT_TEXT	segment byte public 'CODE'
V6SLOT_TEXT	ends
	public	_m_b159
	public	_FUN_1b11_1257
_s@	equ	s@
	end
