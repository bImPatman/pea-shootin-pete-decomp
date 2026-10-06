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
	?debug	S "vl.c"
	?debug	C E96576445D04766C2E63
VL_TEXT	segment byte public 'CODE'
VL_TEXT	ends
DGROUP	group	_DATA,_BSS
	assume	cs:VL_TEXT,ds:DGROUP
_DATA	segment word public 'DATA'
d@	label	byte
d@w	label	word
_DATA	ends
_BSS	segment word public 'BSS'
b@	label	byte
b@w	label	word
_BSS	ends
VL_TEXT	segment byte public 'CODE'
   ;	
   ;	int f(int n){ int i,s=0,a=n; for(i=0;i<n;i++){s+=i+a;} g+=s; return s; }
   ;	
	assume	cs:VL_TEXT
_f	proc	far
	push	bp
	mov	bp,sp
	sub	sp,6
	mov	word ptr [bp-4],0
	mov	word ptr [bp-2],0
	mov	ax,word ptr [bp+6]
	mov	word ptr [bp-6],ax
	mov	ax,word ptr [bp-2]
	cmp	ax,word ptr [bp+6]
	jge	short @1@226
@1@114:
	mov	ax,word ptr [bp-6]
	add	word ptr [bp-4],ax
	inc	word ptr [bp-6]
	inc	word ptr [bp-2]
	mov	ax,word ptr [bp-2]
	cmp	ax,word ptr [bp+6]
	jl	short @1@114
@1@226:
	mov	ax,word ptr [bp-4]
	add	word ptr DGROUP:_g,ax
	leave	
	ret	
_f	endp
VL_TEXT	ends
_BSS	segment word public 'BSS'
_g	label	word
	db	2 dup (?)
	?debug	C E9
	?debug	C FA04030000
_BSS	ends
_DATA	segment word public 'DATA'
s@	label	byte
_DATA	ends
VL_TEXT	segment byte public 'CODE'
VL_TEXT	ends
	public	_f
	public	_g
_s@	equ	s@
	end
