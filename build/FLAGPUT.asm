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
	?debug	S "flagput.c"
	?debug	C E9B26E445D09666C61677075742E63
FLAGPUT_TEXT	segment byte public 'CODE'
FLAGPUT_TEXT	ends
DGROUP	group	_DATA,_BSS
	assume	cs:FLAGPUT_TEXT,ds:DGROUP
_DATA	segment word public 'DATA'
d@	label	byte
d@w	label	word
_DATA	ends
_BSS	segment word public 'BSS'
b@	label	byte
b@w	label	word
_BSS	ends
FLAGPUT_TEXT	segment byte public 'CODE'
   ;	
   ;	void flagbits_store(unsigned char v)
   ;	
	assume	cs:FLAGPUT_TEXT
_flagbits_store	proc	far
	push	bp
	mov	bp,sp
   ;	
   ;	{
   ;	    g_mask = v;
   ;	
	mov	al,byte ptr [bp+6]
	mov	byte ptr DGROUP:_g_mask,al
   ;	
   ;	}
   ;	
	pop	bp
	ret	
_flagbits_store	endp
FLAGPUT_TEXT	ends
_BSS	segment word public 'BSS'
_g_mask	label	byte
	db	1 dup (?)
	?debug	C E9
	?debug	C FA00000000
_BSS	ends
_DATA	segment word public 'DATA'
s@	label	byte
_DATA	ends
FLAGPUT_TEXT	segment byte public 'CODE'
FLAGPUT_TEXT	ends
	public	_flagbits_store
	public	_g_mask
_s@	equ	s@
	end
