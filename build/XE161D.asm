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
	?debug	S "xe161d.c"
	?debug	C E9FD86455D087865313631642E63
XE161D_TEXT	segment byte public 'CODE'
XE161D_TEXT	ends
DGROUP	group	_DATA,_BSS
	assume	cs:XE161D_TEXT,ds:DGROUP
_DATA	segment word public 'DATA'
d@	label	byte
d@w	label	word
_DATA	ends
_BSS	segment word public 'BSS'
b@	label	byte
b@w	label	word
_BSS	ends
XE161D_TEXT	segment byte public 'CODE'
   ;	
   ;	int FUN_161d_08de(void) { return 1; }
   ;	
	assume	cs:XE161D_TEXT
_FUN_161d_08de	proc	far
	mov	ax,1
	ret	
_FUN_161d_08de	endp
	?debug	C E9
	?debug	C FA00000000
XE161D_TEXT	ends
_DATA	segment word public 'DATA'
s@	label	byte
_DATA	ends
XE161D_TEXT	segment byte public 'CODE'
XE161D_TEXT	ends
	public	_FUN_161d_08de
_s@	equ	s@
	end
