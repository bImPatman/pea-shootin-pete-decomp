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
	?debug	S "setgpos.c"
	?debug	C E9EB79445D0973657467706F732E63
SETGPOS_TEXT	segment byte public 'CODE'
SETGPOS_TEXT	ends
DGROUP	group	_DATA,_BSS
	assume	cs:SETGPOS_TEXT,ds:DGROUP
_DATA	segment word public 'DATA'
d@	label	byte
d@w	label	word
_DATA	ends
_BSS	segment word public 'BSS'
b@	label	byte
b@w	label	word
_BSS	ends
SETGPOS_TEXT	segment byte public 'CODE'
   ;	
   ;	void setgpos(unsigned int v)
   ;	
	assume	cs:SETGPOS_TEXT
_setgpos	proc	far
	push	bp
	mov	bp,sp
   ;	
   ;	{
   ;	    g_field = v;
   ;	
	mov	ax,word ptr [bp+6]
	mov	word ptr DGROUP:_g_field+2,0
	mov	word ptr DGROUP:_g_field,ax
   ;	
   ;	}
   ;	
	pop	bp
	ret	
_setgpos	endp
SETGPOS_TEXT	ends
_BSS	segment word public 'BSS'
_g_field	label	word
	db	4 dup (?)
	?debug	C E9
	?debug	C FA00000000
_BSS	ends
_DATA	segment word public 'DATA'
s@	label	byte
_DATA	ends
SETGPOS_TEXT	segment byte public 'CODE'
SETGPOS_TEXT	ends
	public	_setgpos
	public	_g_field
_s@	equ	s@
	end
