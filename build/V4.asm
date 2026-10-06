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
	?debug	S "v4.c"
	?debug	C E99081445D0476342E63
V4_TEXT	segment byte public 'CODE'
V4_TEXT	ends
DGROUP	group	_DATA,_BSS
	assume	cs:V4_TEXT,ds:DGROUP
_DATA	segment word public 'DATA'
d@	label	byte
d@w	label	word
_DATA	ends
_BSS	segment word public 'BSS'
b@	label	byte
b@w	label	word
_BSS	ends
V4_TEXT	segment byte public 'CODE'
   ;	
   ;	void v4(unsigned int off, unsigned int seg)
   ;	
	assume	cs:V4_TEXT
_v4	proc	far
	push	bp
	mov	bp,sp
   ;	
   ;	{
   ;	    g_hi = seg;
   ;	
	mov	ax,word ptr [bp+8]
	mov	word ptr DGROUP:_g_hi,ax
   ;	
   ;	    g_lo = off;
   ;	
	mov	ax,word ptr [bp+6]
	mov	word ptr DGROUP:_g_lo,ax
   ;	
   ;	}
   ;	
	pop	bp
	ret	
_v4	endp
V4_TEXT	ends
_BSS	segment word public 'BSS'
_g_hi	label	word
	db	2 dup (?)
_g_lo	label	word
	db	2 dup (?)
	?debug	C E9
	?debug	C FA00000000
_BSS	ends
_DATA	segment word public 'DATA'
s@	label	byte
_DATA	ends
V4_TEXT	segment byte public 'CODE'
V4_TEXT	ends
	public	_v4
	public	_g_hi
	public	_g_lo
_s@	equ	s@
	end
