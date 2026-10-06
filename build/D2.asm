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
	?debug	S "d2.c"
	?debug	C E91383445D0464322E63
D2_TEXT	segment byte public 'CODE'
D2_TEXT	ends
DGROUP	group	_DATA,_BSS
	assume	cs:D2_TEXT,ds:DGROUP
_DATA	segment word public 'DATA'
d@	label	byte
d@w	label	word
_DATA	ends
_BSS	segment word public 'BSS'
b@	label	byte
b@w	label	word
_BSS	ends
D2_TEXT	segment byte public 'CODE'
   ;	
   ;	void d2(struct set_s far *rec, unsigned char x, unsigned char y)
   ;	
	assume	cs:D2_TEXT
_d2	proc	far
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
_d2	endp
	?debug	C E9
	?debug	C FA00000000
D2_TEXT	ends
_DATA	segment word public 'DATA'
s@	label	byte
_DATA	ends
D2_TEXT	segment byte public 'CODE'
D2_TEXT	ends
	public	_d2
_s@	equ	s@
	end
