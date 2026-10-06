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
	?debug	S "xf18e.c"
	?debug	C E9E68B445D0778663138652E63
XF18E_TEXT	segment byte public 'CODE'
XF18E_TEXT	ends
DGROUP	group	_DATA,_BSS
	assume	cs:XF18E_TEXT,ds:DGROUP
_DATA	segment word public 'DATA'
d@	label	byte
d@w	label	word
_DATA	ends
_BSS	segment word public 'BSS'
b@	label	byte
b@w	label	word
_BSS	ends
XF18E_TEXT	segment byte public 'CODE'
   ;	
   ;	void m_f18e(void) { }
   ;	
	assume	cs:XF18E_TEXT
_m_f18e	proc	far
	ret	
_m_f18e	endp
	?debug	C E9
	?debug	C FA00000000
XF18E_TEXT	ends
_DATA	segment word public 'DATA'
s@	label	byte
_DATA	ends
XF18E_TEXT	segment byte public 'CODE'
XF18E_TEXT	ends
	public	_m_f18e
_s@	equ	s@
	end
