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
	?debug	S "xf1ae.c"
	?debug	C E9E68B445D0778663161652E63
XF1AE_TEXT	segment byte public 'CODE'
XF1AE_TEXT	ends
DGROUP	group	_DATA,_BSS
	assume	cs:XF1AE_TEXT,ds:DGROUP
_DATA	segment word public 'DATA'
d@	label	byte
d@w	label	word
_DATA	ends
_BSS	segment word public 'BSS'
b@	label	byte
b@w	label	word
_BSS	ends
XF1AE_TEXT	segment byte public 'CODE'
   ;	
   ;	int m_f1ae(void) { return 0; }
   ;	
	assume	cs:XF1AE_TEXT
_m_f1ae	proc	far
	xor	ax,ax
	ret	
_m_f1ae	endp
	?debug	C E9
	?debug	C FA00000000
XF1AE_TEXT	ends
_DATA	segment word public 'DATA'
s@	label	byte
_DATA	ends
XF1AE_TEXT	segment byte public 'CODE'
XF1AE_TEXT	ends
	public	_m_f1ae
_s@	equ	s@
	end
