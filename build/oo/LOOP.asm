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
	?debug	S "loop.c"
	?debug	C E90175445D066C6F6F702E63
LOOP_TEXT	segment byte public 'CODE'
LOOP_TEXT	ends
DGROUP	group	_DATA,_BSS
	assume	cs:LOOP_TEXT,ds:DGROUP
_DATA	segment word public 'DATA'
d@	label	byte
d@w	label	word
_DATA	ends
_BSS	segment word public 'BSS'
b@	label	byte
b@w	label	word
_BSS	ends
LOOP_TEXT	segment byte public 'CODE'
   ;	
   ;	int f(int n) { int i, s = 0; for (i = 0; i < n; i++) s += i; return s; }
   ;	
	assume	cs:LOOP_TEXT
_f	proc	far
	push	bp
	mov	bp,sp
	sub	sp,4
	mov	word ptr [bp-4],0
	mov	word ptr [bp-2],0
	mov	ax,word ptr [bp-2]
	cmp	ax,word ptr [bp+6]
	jge	short @1@170
@1@86:
	mov	ax,word ptr [bp-2]
	add	word ptr [bp-4],ax
	inc	word ptr [bp-2]
	mov	ax,word ptr [bp-2]
	cmp	ax,word ptr [bp+6]
	jl	short @1@86
@1@170:
	mov	ax,word ptr [bp-4]
	leave	
	ret	
_f	endp
LOOP_TEXT	ends
_BSS	segment word public 'BSS'
_g	label	word
	db	2 dup (?)
	?debug	C E9
	?debug	C FA00000000
_BSS	ends
_DATA	segment word public 'DATA'
s@	label	byte
_DATA	ends
LOOP_TEXT	segment byte public 'CODE'
LOOP_TEXT	ends
	public	_f
	public	_g
_s@	equ	s@
	end
