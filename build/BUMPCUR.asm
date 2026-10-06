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
	?debug	S "bumpcur.c"
	?debug	C E93982445D0962756D706375722E63
BUMPCUR_TEXT	segment byte public 'CODE'
BUMPCUR_TEXT	ends
DGROUP	group	_DATA,_BSS
	assume	cs:BUMPCUR_TEXT,ds:DGROUP
_DATA	segment word public 'DATA'
d@	label	byte
d@w	label	word
_DATA	ends
_BSS	segment word public 'BSS'
b@	label	byte
b@w	label	word
_BSS	ends
BUMPCUR_TEXT	segment byte public 'CODE'
   ;	
   ;	void bumpcur(struct rec_s far *rec)
   ;	
	assume	cs:BUMPCUR_TEXT
_bumpcur	proc	far
	push	bp
	mov	bp,sp
   ;	
   ;	{
   ;	    if (rec->cur++ > 0x28)
   ;	
	les	bx,dword ptr [bp+6]
	mov	ax,word ptr es:[bx+73]
	inc	word ptr es:[bx+73]
	cmp	ax,40
	jle	short @1@86
   ;	
   ;	        rec->flags |= 0x20;
   ;	
	or	byte ptr es:[bx+101],32
@1@86:
   ;	
   ;	}
   ;	
	pop	bp
	ret	
_bumpcur	endp
	?debug	C E9
	?debug	C FA00000000
BUMPCUR_TEXT	ends
_DATA	segment word public 'DATA'
s@	label	byte
_DATA	ends
BUMPCUR_TEXT	segment byte public 'CODE'
BUMPCUR_TEXT	ends
	public	_bumpcur
_s@	equ	s@
	end
