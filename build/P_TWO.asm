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
	?debug	S "p_two.c"
	?debug	C E9FA80445D07705F74776F2E63
P_TWO_TEXT	segment byte public 'CODE'
P_TWO_TEXT	ends
DGROUP	group	_DATA,_BSS
	assume	cs:P_TWO_TEXT,ds:DGROUP
_DATA	segment word public 'DATA'
d@	label	byte
d@w	label	word
_DATA	ends
_BSS	segment word public 'BSS'
b@	label	byte
b@w	label	word
_BSS	ends
P_TWO_TEXT	segment byte public 'CODE'
   ;	
   ;	void setpair_c(unsigned int lo, unsigned int hi)
   ;	
	assume	cs:P_TWO_TEXT
_setpair_c	proc	far
	push	bp
	mov	bp,sp
   ;	
   ;	{
   ;	    g_hi = hi;
   ;	
	mov	ax,word ptr [bp+8]
	mov	word ptr DGROUP:_g_hi,ax
   ;	
   ;	    g_lo = lo;
   ;	
	mov	ax,word ptr [bp+6]
	mov	word ptr DGROUP:_g_lo,ax
   ;	
   ;	}
   ;	
	pop	bp
	ret	
_setpair_c	endp
P_TWO_TEXT	ends
_BSS	segment word public 'BSS'
_g_lo	label	word
	db	2 dup (?)
_g_hi	label	word
	db	2 dup (?)
	?debug	C E9
	?debug	C FA00000000
_BSS	ends
_DATA	segment word public 'DATA'
s@	label	byte
_DATA	ends
P_TWO_TEXT	segment byte public 'CODE'
P_TWO_TEXT	ends
	public	_setpair_c
	public	_g_lo
	public	_g_hi
_s@	equ	s@
	end
