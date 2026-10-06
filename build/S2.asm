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
	?debug	S "s2.c"
	?debug	C E90381445D0473322E63
S2_TEXT	segment byte public 'CODE'
S2_TEXT	ends
DGROUP	group	_DATA,_BSS
	assume	cs:S2_TEXT,ds:DGROUP
_DATA	segment word public 'DATA'
d@	label	byte
d@w	label	word
_DATA	ends
_BSS	segment word public 'BSS'
b@	label	byte
b@w	label	word
_BSS	ends
S2_TEXT	segment byte public 'CODE'
   ;	
   ;	void sp2(unsigned int lo, unsigned int hi)
   ;	
	assume	cs:S2_TEXT
_sp2	proc	far
	push	bp
	mov	bp,sp
   ;	
   ;	{
   ;	    g_pair = ((unsigned long)hi << 16) | lo;
   ;	
	mov	ax,word ptr [bp+6]
	mov	dx,word ptr [bp+8]
	mov	word ptr DGROUP:_g_pair+2,dx
	mov	word ptr DGROUP:_g_pair,ax
   ;	
   ;	}
   ;	
	pop	bp
	ret	
_sp2	endp
S2_TEXT	ends
_BSS	segment word public 'BSS'
_g_pair	label	word
	db	4 dup (?)
	?debug	C E9
	?debug	C FA00000000
_BSS	ends
_DATA	segment word public 'DATA'
s@	label	byte
_DATA	ends
S2_TEXT	segment byte public 'CODE'
S2_TEXT	ends
	public	_sp2
	public	_g_pair
_s@	equ	s@
	end
