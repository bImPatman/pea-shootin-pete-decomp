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
	?debug	S "b4.c"
	?debug	C E95A82445D0462342E63
B4_TEXT	segment byte public 'CODE'
B4_TEXT	ends
DGROUP	group	_DATA,_BSS
	assume	cs:B4_TEXT,ds:DGROUP
_DATA	segment word public 'DATA'
d@	label	byte
d@w	label	word
_DATA	ends
_BSS	segment word public 'BSS'
b@	label	byte
b@w	label	word
_BSS	ends
B4_TEXT	segment byte public 'CODE'
   ;	
   ;	void b4(struct flg_s far *rec)
   ;	
	assume	cs:B4_TEXT
_b4	proc	far
	push	bp
	mov	bp,sp
   ;	
   ;	{
   ;	    if ((unsigned)rec->kind != 0)
   ;	
	les	bx,dword ptr [bp+6]
	cmp	byte ptr es:[bx+43],0
	jne	short @1@86
   ;	
   ;	        return;
   ;	    rec->flags |= 0x20;
   ;	
	les	bx,dword ptr [bp+6]
	or	byte ptr es:[bx+101],32
@1@86:
   ;	
   ;	}
   ;	
	pop	bp
	ret	
_b4	endp
	?debug	C E9
	?debug	C FA00000000
B4_TEXT	ends
_DATA	segment word public 'DATA'
s@	label	byte
_DATA	ends
B4_TEXT	segment byte public 'CODE'
B4_TEXT	ends
	public	_b4
_s@	equ	s@
	end
