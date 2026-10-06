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
	?debug	S "s3.c"
	?debug	C E90381445D0473332E63
S3_TEXT	segment byte public 'CODE'
S3_TEXT	ends
DGROUP	group	_DATA,_BSS
	assume	cs:S3_TEXT,ds:DGROUP
_DATA	segment word public 'DATA'
d@	label	byte
d@w	label	word
_DATA	ends
_BSS	segment word public 'BSS'
b@	label	byte
b@w	label	word
_BSS	ends
S3_TEXT	segment byte public 'CODE'
   ;	
   ;	void sp3(unsigned int lo, unsigned int hi)
   ;	
	assume	cs:S3_TEXT
_sp3	proc	far
	push	bp
	mov	bp,sp
   ;	
   ;	{
   ;	    g_pair.hi = hi;
   ;	
	mov	ax,word ptr [bp+8]
	mov	word ptr DGROUP:_g_pair+2,ax
   ;	
   ;	    g_pair.lo = lo;
   ;	
	mov	ax,word ptr [bp+6]
	mov	word ptr DGROUP:_g_pair,ax
   ;	
   ;	}
   ;	
	pop	bp
	ret	
_sp3	endp
S3_TEXT	ends
_BSS	segment word public 'BSS'
_g_pair	label	word
	db	4 dup (?)
	?debug	C E9
	?debug	C FA00000000
_BSS	ends
_DATA	segment word public 'DATA'
s@	label	byte
_DATA	ends
S3_TEXT	segment byte public 'CODE'
S3_TEXT	ends
	public	_sp3
	public	_g_pair
_s@	equ	s@
	end
