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
	?debug	S "flagshft.c"
	?debug	C E9156D445D0A666C6167736866742E63
FLAGSHFT_TEXT	segment byte public 'CODE'
FLAGSHFT_TEXT	ends
DGROUP	group	_DATA,_BSS
	assume	cs:FLAGSHFT_TEXT,ds:DGROUP
_DATA	segment word public 'DATA'
d@	label	byte
d@w	label	word
_DATA	ends
_BSS	segment word public 'BSS'
b@	label	byte
b@w	label	word
_BSS	ends
FLAGSHFT_TEXT	segment byte public 'CODE'
   ;	
   ;	void flagbits_shift(unsigned char v)
   ;	
	assume	cs:FLAGSHFT_TEXT
_flagbits_shift	proc	far
	push	bp
	mov	bp,sp
   ;	
   ;	{
   ;	    g_mask = (g_mask & 0x8F) | ((v << 4) & 0x7F);
   ;	
	mov	al,byte ptr DGROUP:_g_mask
	and	al,143
	mov	dl,byte ptr [bp+6]
	shl	dl,4
	and	dl,127
	or	al,dl
	mov	byte ptr DGROUP:_g_mask,al
   ;	
   ;	}
   ;	
	pop	bp
	ret	
_flagbits_shift	endp
FLAGSHFT_TEXT	ends
_BSS	segment word public 'BSS'
_g_mask	label	byte
	db	1 dup (?)
	?debug	C E9
	?debug	C FA00000000
_BSS	ends
_DATA	segment word public 'DATA'
s@	label	byte
_DATA	ends
FLAGSHFT_TEXT	segment byte public 'CODE'
FLAGSHFT_TEXT	ends
	public	_flagbits_shift
	public	_g_mask
_s@	equ	s@
	end
