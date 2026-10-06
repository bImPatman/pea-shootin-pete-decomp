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
	?debug	S "keypoll.c"
	?debug	C E98A91445D096B6579706F6C6C2E63
KEYPOLL_TEXT	segment byte public 'CODE'
KEYPOLL_TEXT	ends
DGROUP	group	_DATA,_BSS
	assume	cs:KEYPOLL_TEXT,ds:DGROUP
_DATA	segment word public 'DATA'
d@	label	byte
d@w	label	word
_DATA	ends
_BSS	segment word public 'BSS'
b@	label	byte
b@w	label	word
_BSS	ends
KEYPOLL_TEXT	segment byte public 'CODE'
   ;	
   ;	int key_poll(void)
   ;	
	assume	cs:KEYPOLL_TEXT
_key_poll	proc	far
   ;	
   ;	{
   ;	    if (g_2444)
   ;	
	cmp	byte ptr DGROUP:_g_2444,0
	je	short @1@58
   ;	
   ;	        _AX = 1;
   ;	
	mov	ax,1
	jmp	short @1@142
@1@58:
   ;	
   ;	    else
   ;	        asm {
   ;	            mov ah, 0bh
   ;	
	mov	 ah, 0bh
   ;	
   ;	            int 21h
   ;	
	int	 21h
   ;	
   ;	            cbw
   ;	
	cbw	
@1@142:
   ;	
   ;	        }
   ;	    return _AX;
   ;	
   ;	
   ;	}
   ;	
	ret	
_key_poll	endp
KEYPOLL_TEXT	ends
_BSS	segment word public 'BSS'
_g_2444	label	byte
	db	1 dup (?)
	?debug	C E9
	?debug	C FA00000000
_BSS	ends
_DATA	segment word public 'DATA'
s@	label	byte
_DATA	ends
KEYPOLL_TEXT	segment byte public 'CODE'
KEYPOLL_TEXT	ends
	public	_key_poll
	public	_g_2444
_s@	equ	s@
	end
