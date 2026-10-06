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
	?debug	S "v2.c"
	?debug	C E98E81445D0476322E63
V2_TEXT	segment byte public 'CODE'
V2_TEXT	ends
DGROUP	group	_DATA,_BSS
	assume	cs:V2_TEXT,ds:DGROUP
_DATA	segment word public 'DATA'
d@	label	byte
d@w	label	word
_DATA	ends
_BSS	segment word public 'BSS'
b@	label	byte
b@w	label	word
_BSS	ends
V2_TEXT	segment byte public 'CODE'
   ;	
   ;	void v2(unsigned int off, unsigned int seg)
   ;	
	assume	cs:V2_TEXT
_v2	proc	far
	push	bp
	mov	bp,sp
   ;	
   ;	{
   ;	    g_val = ((unsigned long)seg << 16) | off;
   ;	
	mov	ax,word ptr [bp+6]
	mov	dx,word ptr [bp+8]
	mov	word ptr DGROUP:_g_val+2,dx
	mov	word ptr DGROUP:_g_val,ax
   ;	
   ;	}
   ;	
	pop	bp
	ret	
_v2	endp
V2_TEXT	ends
_BSS	segment word public 'BSS'
_g_val	label	word
	db	4 dup (?)
	?debug	C E9
	?debug	C FA00000000
_BSS	ends
_DATA	segment word public 'DATA'
s@	label	byte
_DATA	ends
V2_TEXT	segment byte public 'CODE'
V2_TEXT	ends
	public	_v2
	public	_g_val
_s@	equ	s@
	end
