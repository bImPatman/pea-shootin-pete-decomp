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
	?debug	S "fa.c"
	?debug	C E9956A445D0466612E63
FA_TEXT	segment byte public 'CODE'
FA_TEXT	ends
DGROUP	group	_DATA,_BSS
	assume	cs:FA_TEXT,ds:DGROUP
_DATA	segment word public 'DATA'
d@	label	byte
d@w	label	word
_DATA	ends
_BSS	segment word public 'BSS'
b@	label	byte
b@w	label	word
_BSS	ends
FA_TEXT	segment byte public 'CODE'
   ;	
   ;	void flagbits_set(unsigned char v)
   ;	
	assume	cs:FA_TEXT
_flagbits_set	proc	far
	push	bp
	mov	bp,sp
   ;	
   ;	{
   ;	    g_mask = (g_mask & 0x70) | (v & 0x8F);
   ;	
	mov	al,byte ptr DGROUP:_g_mask
	and	al,112
	mov	dl,byte ptr [bp+6]
	and	dl,143
	or	al,dl
	mov	byte ptr DGROUP:_g_mask,al
   ;	
   ;	}
   ;	
	pop	bp
	ret	
_flagbits_set	endp
FA_TEXT	ends
_BSS	segment word public 'BSS'
_g_mask	label	byte
	db	1 dup (?)
	?debug	C E9
	?debug	C FA00000000
_BSS	ends
_DATA	segment word public 'DATA'
s@	label	byte
_DATA	ends
FA_TEXT	segment byte public 'CODE'
FA_TEXT	ends
	public	_flagbits_set
	public	_g_mask
_s@	equ	s@
	end
