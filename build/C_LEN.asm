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
	?debug	S "c_len.c"
	?debug	C E9C380445D07635F6C656E2E63
C_LEN_TEXT	segment byte public 'CODE'
C_LEN_TEXT	ends
DGROUP	group	_DATA,_BSS
	assume	cs:C_LEN_TEXT,ds:DGROUP
_DATA	segment word public 'DATA'
d@	label	byte
d@w	label	word
_DATA	ends
_BSS	segment word public 'BSS'
b@	label	byte
b@w	label	word
_BSS	ends
C_LEN_TEXT	segment byte public 'CODE'
   ;	
   ;	unsigned slen_c(char far *s)
   ;	
	assume	cs:C_LEN_TEXT
_slen_c	proc	far
	push	bp
	mov	bp,sp
   ;	
   ;	{
   ;	    return strlen(s);
   ;	
	push	word ptr [bp+8]
	push	word ptr [bp+6]
	call	far ptr _strlen
	add	sp,4
   ;	
   ;	}
   ;	
	pop	bp
	ret	
_slen_c	endp
	?debug	C E9
	?debug	C FA00000000
C_LEN_TEXT	ends
_DATA	segment word public 'DATA'
s@	label	byte
_DATA	ends
C_LEN_TEXT	segment byte public 'CODE'
C_LEN_TEXT	ends
	extrn	_strlen:far
	public	_slen_c
_s@	equ	s@
	end
