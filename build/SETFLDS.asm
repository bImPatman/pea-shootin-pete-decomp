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
	?debug	S "setflds.c"
	?debug	C E9AB83445D09736574666C64732E63
SETFLDS_TEXT	segment byte public 'CODE'
SETFLDS_TEXT	ends
DGROUP	group	_DATA,_BSS
	assume	cs:SETFLDS_TEXT,ds:DGROUP
_DATA	segment word public 'DATA'
d@	label	byte
d@w	label	word
_DATA	ends
_BSS	segment word public 'BSS'
b@	label	byte
b@w	label	word
_BSS	ends
SETFLDS_TEXT	segment byte public 'CODE'
   ;	
   ;	void setflds(struct set_s far *rec, unsigned char x, unsigned char y)
   ;	
	assume	cs:SETFLDS_TEXT
_setflds	proc	far
	push	bp
	mov	bp,sp
   ;	
   ;	{
   ;	    rec->a = x;
   ;	
	les	bx,dword ptr [bp+6]
	mov	al,byte ptr [bp+10]
	mov	byte ptr es:[bx+43],al
   ;	
   ;	    rec->b = y;
   ;	
	mov	al,byte ptr [bp+12]
	mov	byte ptr es:[bx+44],al
   ;	
   ;	    rec->d = 0;
   ;	
	mov	byte ptr es:[bx+55],0
   ;	
   ;	    if (rec->a == 4) {
   ;	
	cmp	byte ptr es:[bx+43],4
	jne	short @1@86
   ;	
   ;	        rec->c = 0xFF;
   ;	
	mov	byte ptr es:[bx+45],255
   ;	
   ;	        return;
   ;	
	pop	bp
	ret	
@1@86:
   ;	
   ;	    }
   ;	    rec->c = 0;
   ;	
	les	bx,dword ptr [bp+6]
	mov	byte ptr es:[bx+45],0
   ;	
   ;	}
   ;	
	pop	bp
	ret	
_setflds	endp
	?debug	C E9
	?debug	C FA00000000
SETFLDS_TEXT	ends
_DATA	segment word public 'DATA'
s@	label	byte
_DATA	ends
SETFLDS_TEXT	segment byte public 'CODE'
SETFLDS_TEXT	ends
	public	_setflds
_s@	equ	s@
	end
