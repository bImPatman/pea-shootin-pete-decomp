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
	?debug	S "xb2vcb.c"
	?debug	C E95287455D087862327663622E63
XB2VCB_TEXT	segment byte public 'CODE'
XB2VCB_TEXT	ends
DGROUP	group	_DATA,_BSS
	assume	cs:XB2VCB_TEXT,ds:DGROUP
_DATA	segment word public 'DATA'
d@	label	byte
d@w	label	word
_DATA	ends
_BSS	segment word public 'BSS'
b@	label	byte
b@w	label	word
_BSS	ends
XB2VCB_TEXT	segment byte public 'CODE'
   ;	
   ;	void FUN_1000_20cb(void) { }
   ;	
	assume	cs:XB2VCB_TEXT
_FUN_1000_20cb	proc	far
	ret	
_FUN_1000_20cb	endp
	?debug	C E9
	?debug	C FA00000000
XB2VCB_TEXT	ends
_DATA	segment word public 'DATA'
s@	label	byte
_DATA	ends
XB2VCB_TEXT	segment byte public 'CODE'
XB2VCB_TEXT	ends
	public	_FUN_1000_20cb
_s@	equ	s@
	end
