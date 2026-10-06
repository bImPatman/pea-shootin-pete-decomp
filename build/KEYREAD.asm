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
	?debug	S "keyread.c"
	?debug	C E98C92445D096B6579726561642E63
KEYREAD_TEXT	segment byte public 'CODE'
KEYREAD_TEXT	ends
DGROUP	group	_DATA,_BSS
	assume	cs:KEYREAD_TEXT,ds:DGROUP
_DATA	segment word public 'DATA'
d@	label	byte
d@w	label	word
_DATA	ends
_BSS	segment word public 'BSS'
b@	label	byte
b@w	label	word
_BSS	ends
KEYREAD_TEXT	segment byte public 'CODE'
   ;	
   ;	unsigned char key_read(void)
   ;	
	assume	cs:KEYREAD_TEXT
_key_read	proc	far
   ;	
   ;	{
   ;	    if (g_2444) {
   ;	
	cmp	byte ptr DGROUP:_g_2444,0
	je	short @1@58
   ;	
   ;	        g_2444 = 0;
   ;	
	mov	byte ptr DGROUP:_g_2444,0
   ;	
   ;	        _AL = g_2445;
   ;	
	mov	al,byte ptr DGROUP:_g_2445
   ;	
   ;	    } else {
   ;	
	jmp	short @1@114
@1@58:
   ;	
   ;	        asm {
   ;	            mov ax, 0700h
   ;	
	mov	 ax, 0700h
   ;	
   ;	            int 21h
   ;	
	int	 21h
@1@114:
   ;	
   ;	        }
   ;	    }
   ;	    _AH = 0;
   ;	
	mov	ah,0
   ;	
   ;	    return _AX;
   ;	
   ;	
   ;	}
   ;	
	ret	
_key_read	endp
KEYREAD_TEXT	ends
_BSS	segment word public 'BSS'
_g_2445	label	byte
	db	1 dup (?)
	?debug	C E9
	?debug	C FA00000000
_BSS	ends
_DATA	segment word public 'DATA'
s@	label	byte
_DATA	ends
KEYREAD_TEXT	segment byte public 'CODE'
KEYREAD_TEXT	ends
	public	_key_read
	public	_g_2445
	extrn	_g_2444:byte
_s@	equ	s@
	end
