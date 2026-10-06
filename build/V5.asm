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
	?debug	S "v5.c"
	?debug	C E99181445D0476352E63
V5_TEXT	segment byte public 'CODE'
V5_TEXT	ends
DGROUP	group	_DATA,_BSS
	assume	cs:V5_TEXT,ds:DGROUP
_DATA	segment word public 'DATA'
d@	label	byte
d@w	label	word
_DATA	ends
_BSS	segment word public 'BSS'
b@	label	byte
b@w	label	word
_BSS	ends
V5_TEXT	segment byte public 'CODE'
   ;	
   ;	void v5(unsigned int off, unsigned int seg)
   ;	
	assume	cs:V5_TEXT
_v5	proc	far
	push	bp
	mov	bp,sp
   ;	
   ;	{
   ;	    g_ptr = MK_FP(seg, off);
   ;	
	push	word ptr [bp+6]
	push	word ptr [bp+8]
	call	far ptr _MK_FP
	add	sp,4
	cwd	
	mov	word ptr DGROUP:_g_ptr+2,dx
	mov	word ptr DGROUP:_g_ptr,ax
   ;	
   ;	}
   ;	
	pop	bp
	ret	
_v5	endp
V5_TEXT	ends
_BSS	segment word public 'BSS'
_g_ptr	label	dword
	db	4 dup (?)
	?debug	C E9
	?debug	C FA00000000
_BSS	ends
_DATA	segment word public 'DATA'
s@	label	byte
_DATA	ends
V5_TEXT	segment byte public 'CODE'
V5_TEXT	ends
	extrn	_MK_FP:far
	public	_v5
	public	_g_ptr
_s@	equ	s@
	end
