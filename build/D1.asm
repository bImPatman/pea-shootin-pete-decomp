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
	?debug	S "d1.c"
	?debug	C E91283445D0464312E63
D1_TEXT	segment byte public 'CODE'
D1_TEXT	ends
DGROUP	group	_DATA,_BSS
	assume	cs:D1_TEXT,ds:DGROUP
_DATA	segment word public 'DATA'
d@	label	byte
d@w	label	word
_DATA	ends
_BSS	segment word public 'BSS'
b@	label	byte
b@w	label	word
_BSS	ends
D1_TEXT	segment byte public 'CODE'
   ;	
   ;	void d1(struct tag_s far *rec, unsigned char kind)
   ;	
	assume	cs:D1_TEXT
_d1	proc	far
	push	bp
	mov	bp,sp
   ;	
   ;	{
   ;	    if (kind == 0) {
   ;	
	cmp	byte ptr [bp+10],0
	jne	short @1@86
   ;	
   ;	        rec->state = 0;
   ;	
	les	bx,dword ptr [bp+6]
	mov	byte ptr es:[bx],0
   ;	
   ;	        return;
   ;	
	pop	bp
	ret	
@1@86:
   ;	
   ;	    }
   ;	    rec->kind = kind;
   ;	
	les	bx,dword ptr [bp+6]
	mov	al,byte ptr [bp+10]
	mov	byte ptr es:[bx+1],al
   ;	
   ;	    rec->state = 1;
   ;	
	mov	byte ptr es:[bx],1
   ;	
   ;	}
   ;	
	pop	bp
	ret	
_d1	endp
	?debug	C E9
	?debug	C FA00000000
D1_TEXT	ends
_DATA	segment word public 'DATA'
s@	label	byte
_DATA	ends
D1_TEXT	segment byte public 'CODE'
D1_TEXT	ends
	public	_d1
_s@	equ	s@
	end
