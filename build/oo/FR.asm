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
	?debug	S "fr.c"
	?debug	C E9EB74445D0466722E63
FR_TEXT	segment byte public 'CODE'
FR_TEXT	ends
DGROUP	group	_DATA,_BSS
	assume	cs:FR_TEXT,ds:DGROUP
_DATA	segment word public 'DATA'
d@	label	byte
d@w	label	word
_DATA	ends
_BSS	segment word public 'BSS'
b@	label	byte
b@w	label	word
_BSS	ends
FR_TEXT	segment byte public 'CODE'
   ;	
   ;	int f(int n) { int i, s = 0; for (i = 0; i < n; i++) s += i; return s; }
   ;	
	assume	cs:FR_TEXT
_f	proc	far
	enter	4,0
	mov	word ptr [bp-4],0
	mov	word ptr [bp-2],0
	jmp	short @1@114
@1@58:
	mov	ax,word ptr [bp-2]
	add	word ptr [bp-4],ax
	inc	word ptr [bp-2]
@1@114:
	mov	ax,word ptr [bp-2]
	cmp	ax,word ptr [bp+6]
	jl	short @1@58
	mov	ax,word ptr [bp-4]
	jmp	short @1@170
@1@170:
	leave	
	ret	
_f	endp
	?debug	C E9
	?debug	C FA00000000
FR_TEXT	ends
_DATA	segment word public 'DATA'
s@	label	byte
_DATA	ends
FR_TEXT	segment byte public 'CODE'
FR_TEXT	ends
	public	_f
_s@	equ	s@
	end
