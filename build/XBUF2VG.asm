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
	?debug	S "xbuf2vg.c"
	?debug	C E90792445D09786275663276672E63
XBUF2VG_TEXT	segment byte public 'CODE'
XBUF2VG_TEXT	ends
DGROUP	group	_DATA,_BSS
	assume	cs:XBUF2VG_TEXT,ds:DGROUP
_DATA	segment word public 'DATA'
d@	label	byte
d@w	label	word
_DATA	ends
_BSS	segment word public 'BSS'
b@	label	byte
b@w	label	word
_BSS	ends
XBUF2VG_TEXT	segment byte public 'CODE'
   ;	
   ;	void buf_to_vga(unsigned char far *p) { }
   ;	
	assume	cs:XBUF2VG_TEXT
_buf_to_vga	proc	far
	push	bp
	mov	bp,sp
	pop	bp
	ret	
_buf_to_vga	endp
	?debug	C E9
	?debug	C FA00000000
XBUF2VG_TEXT	ends
_DATA	segment word public 'DATA'
s@	label	byte
_DATA	ends
XBUF2VG_TEXT	segment byte public 'CODE'
XBUF2VG_TEXT	ends
	public	_buf_to_vga
_s@	equ	s@
	end
