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
	?debug	S "clippos.c"
	?debug	C E9D185445D09636C6970706F732E63
CLIPPOS_TEXT	segment byte public 'CODE'
CLIPPOS_TEXT	ends
DGROUP	group	_DATA,_BSS
	assume	cs:CLIPPOS_TEXT,ds:DGROUP
_DATA	segment word public 'DATA'
d@	label	byte
d@w	label	word
_DATA	ends
_BSS	segment word public 'BSS'
b@	label	byte
b@w	label	word
_BSS	ends
CLIPPOS_TEXT	segment byte public 'CODE'
   ;	
   ;	void clippos(struct rec_s far *a, int far *p, int far *q)
   ;	
	assume	cs:CLIPPOS_TEXT
_clippos	proc	far
	push	bp
	mov	bp,sp
	push	si
   ;	
   ;	{
   ;	    *p = g->f02 - 1;
   ;	
	les	bx,dword ptr DGROUP:_g
	mov	ax,word ptr es:[bx+2]
	dec	ax
	les	bx,dword ptr [bp+10]
	mov	word ptr es:[bx],ax
   ;	
   ;	    *q = a->f04 + a->f0d;
   ;	
	les	bx,dword ptr [bp+6]
	mov	ax,word ptr es:[bx+4]
	add	ax,word ptr es:[bx+13]
	les	bx,dword ptr [bp+14]
	mov	word ptr es:[bx],ax
   ;	
   ;	    if (*p < 0)
   ;	
	les	bx,dword ptr [bp+10]
	cmp	word ptr es:[bx],0
	jge	short @1@86
   ;	
   ;	        *p = 0;
   ;	
	mov	word ptr es:[bx],0
@1@86:
   ;	
   ;	    if (*p + a->f08 > 0x280)
   ;	
	les	bx,dword ptr [bp+6]
	push	es
	les	si,dword ptr [bp+10]
	mov	ax,word ptr es:[si]
	pop	es
	add	ax,word ptr es:[bx+8]
	cmp	ax,640
	jle	short @1@142
   ;	
   ;	        *p = 0x280 - a->f08;
   ;	
	mov	es,word ptr [bp+8]
	mov	ax,640
	sub	ax,word ptr es:[bx+8]
	les	bx,dword ptr [bp+10]
	mov	word ptr es:[bx],ax
@1@142:
   ;	
   ;	}
   ;	
	pop	si
	pop	bp
	ret	
_clippos	endp
CLIPPOS_TEXT	ends
_BSS	segment word public 'BSS'
_g	label	dword
	db	4 dup (?)
	?debug	C E9
	?debug	C FA00000000
_BSS	ends
_DATA	segment word public 'DATA'
s@	label	byte
_DATA	ends
CLIPPOS_TEXT	segment byte public 'CODE'
CLIPPOS_TEXT	ends
	public	_clippos
	public	_g
_s@	equ	s@
	end
