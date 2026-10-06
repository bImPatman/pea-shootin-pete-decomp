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
	?debug	S "b1.c"
	?debug	C E94282445D0462312E63
B1_TEXT	segment byte public 'CODE'
B1_TEXT	ends
DGROUP	group	_DATA,_BSS
	assume	cs:B1_TEXT,ds:DGROUP
_DATA	segment word public 'DATA'
d@	label	byte
d@w	label	word
_DATA	ends
_BSS	segment word public 'BSS'
b@	label	byte
b@w	label	word
_BSS	ends
B1_TEXT	segment byte public 'CODE'
   ;	
   ;	void b1(struct flg_s far *rec)
   ;	
	assume	cs:B1_TEXT
_b1	proc	far
	push	bp
	mov	bp,sp
   ;	
   ;	{
   ;	    if (rec->kind)
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
_b1	endp
	?debug	C E9
	?debug	C FA00000000
B1_TEXT	ends
_DATA	segment word public 'DATA'
s@	label	byte
_DATA	ends
B1_TEXT	segment byte public 'CODE'
B1_TEXT	ends
	public	_b1
_s@	equ	s@
	end
