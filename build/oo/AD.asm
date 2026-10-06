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
	?debug	S "ad.c"
	?debug	C E9CC78445D0461642E63
AD_TEXT	segment byte public 'CODE'
AD_TEXT	ends
DGROUP	group	_DATA,_BSS
	assume	cs:AD_TEXT,ds:DGROUP
_DATA	segment word public 'DATA'
d@	label	byte
d@w	label	word
_DATA	ends
_BSS	segment word public 'BSS'
b@	label	byte
b@w	label	word
	db	2 dup (?)
_BSS	ends
AD_TEXT	segment byte public 'CODE'
   ;	
   ;	int f(int n){ static int a; a=n; g+=a; return a; }
   ;	
	assume	cs:AD_TEXT
_f	proc	far
	push	bp
	mov	bp,sp
	mov	ax,word ptr [bp+6]
	mov	word ptr DGROUP:b@w+0,ax
	add	word ptr DGROUP:_g,ax
	pop	bp
	ret	
_f	endp
AD_TEXT	ends
_BSS	segment word public 'BSS'
_g	label	word
	db	2 dup (?)
	?debug	C E9
	?debug	C FA00020000
_BSS	ends
_DATA	segment word public 'DATA'
s@	label	byte
_DATA	ends
AD_TEXT	segment byte public 'CODE'
AD_TEXT	ends
	public	_f
	public	_g
_s@	equ	s@
	end
