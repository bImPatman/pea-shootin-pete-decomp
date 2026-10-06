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
	?debug	S "v3.c"
	?debug	C E98F81445D0476332E63
V3_TEXT	segment byte public 'CODE'
V3_TEXT	ends
DGROUP	group	_DATA,_BSS
	assume	cs:V3_TEXT,ds:DGROUP
_DATA	segment word public 'DATA'
d@	label	byte
d@w	label	word
_DATA	ends
_BSS	segment word public 'BSS'
b@	label	byte
b@w	label	word
_BSS	ends
V3_TEXT	segment byte public 'CODE'
   ;	
   ;	void v3(unsigned int off, unsigned int seg)
   ;	
	assume	cs:V3_TEXT
_v3	proc	far
	push	bp
	mov	bp,sp
   ;	
   ;	{
   ;	    g_pair.seg = seg;
   ;	
	mov	ax,word ptr [bp+8]
	mov	word ptr DGROUP:_g_pair+2,ax
   ;	
   ;	    g_pair.off = off;
   ;	
	mov	ax,word ptr [bp+6]
	mov	word ptr DGROUP:_g_pair,ax
   ;	
   ;	}
   ;	
	pop	bp
	ret	
_v3	endp
V3_TEXT	ends
_BSS	segment word public 'BSS'
_g_pair	label	word
	db	4 dup (?)
	?debug	C E9
	?debug	C FA00000000
_BSS	ends
_DATA	segment word public 'DATA'
s@	label	byte
_DATA	ends
V3_TEXT	segment byte public 'CODE'
V3_TEXT	ends
	public	_v3
	public	_g_pair
_s@	equ	s@
	end
