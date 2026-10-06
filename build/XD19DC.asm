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
	?debug	S "xd19dc.c"
	?debug	C E95D86455D087864313964632E63
XD19DC_TEXT	segment byte public 'CODE'
XD19DC_TEXT	ends
DGROUP	group	_DATA,_BSS
	assume	cs:XD19DC_TEXT,ds:DGROUP
_DATA	segment word public 'DATA'
d@	label	byte
d@w	label	word
_DATA	ends
_BSS	segment word public 'BSS'
b@	label	byte
b@w	label	word
_BSS	ends
XD19DC_TEXT	segment byte public 'CODE'
   ;	
   ;	void m_d19_dc(char far *p, unsigned int kind) { }
   ;	
	assume	cs:XD19DC_TEXT
_m_d19_dc	proc	far
	push	bp
	mov	bp,sp
	pop	bp
	ret	
_m_d19_dc	endp
	?debug	C E9
	?debug	C FA00000000
XD19DC_TEXT	ends
_DATA	segment word public 'DATA'
s@	label	byte
_DATA	ends
XD19DC_TEXT	segment byte public 'CODE'
XD19DC_TEXT	ends
	public	_m_d19_dc
_s@	equ	s@
	end
