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
	?debug	S "p_struct.c"
	?debug	C E9FA80445D0A705F7374727563742E63
P_STRUCT_TEXT	segment byte public 'CODE'
P_STRUCT_TEXT	ends
DGROUP	group	_DATA,_BSS
	assume	cs:P_STRUCT_TEXT,ds:DGROUP
_DATA	segment word public 'DATA'
d@	label	byte
d@w	label	word
_DATA	ends
_BSS	segment word public 'BSS'
b@	label	byte
b@w	label	word
_BSS	ends
P_STRUCT_TEXT	segment byte public 'CODE'
   ;	
   ;	void setpair_a(unsigned int lo, unsigned int hi)
   ;	
	assume	cs:P_STRUCT_TEXT
_setpair_a	proc	far
	push	bp
	mov	bp,sp
   ;	
   ;	{
   ;	    g_pair.lo = lo;
   ;	
	mov	ax,word ptr [bp+6]
	mov	word ptr DGROUP:_g_pair,ax
   ;	
   ;	    g_pair.hi = hi;
   ;	
	mov	ax,word ptr [bp+8]
	mov	word ptr DGROUP:_g_pair+2,ax
   ;	
   ;	}
   ;	
	pop	bp
	ret	
_setpair_a	endp
P_STRUCT_TEXT	ends
_BSS	segment word public 'BSS'
_g_pair	label	word
	db	4 dup (?)
	?debug	C E9
	?debug	C FA00000000
_BSS	ends
_DATA	segment word public 'DATA'
s@	label	byte
_DATA	ends
P_STRUCT_TEXT	segment byte public 'CODE'
P_STRUCT_TEXT	ends
	public	_setpair_a
	public	_g_pair
_s@	equ	s@
	end
