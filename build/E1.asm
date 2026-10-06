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
	?debug	S "e1.c"
	?debug	C E93483445D0465312E63
E1_TEXT	segment byte public 'CODE'
E1_TEXT	ends
DGROUP	group	_DATA,_BSS
	assume	cs:E1_TEXT,ds:DGROUP
_DATA	segment word public 'DATA'
d@	label	byte
d@w	label	word
_DATA	ends
_BSS	segment word public 'BSS'
b@	label	byte
b@w	label	word
_BSS	ends
E1_TEXT	segment byte public 'CODE'
   ;	
   ;	void e1(struct cls_s far *rec, int u, int v, unsigned char w)
   ;	
	assume	cs:E1_TEXT
_e1	proc	far
	push	bp
	mov	bp,sp
   ;	
   ;	{
   ;	    rec->a = u;
   ;	
	les	bx,dword ptr [bp+6]
	mov	ax,word ptr [bp+10]
	mov	word ptr es:[bx+11],ax
   ;	
   ;	    rec->b = v;
   ;	
	mov	ax,word ptr [bp+12]
	mov	word ptr es:[bx+13],ax
   ;	
   ;	    rec->c = w;
   ;	
	mov	al,byte ptr [bp+14]
	mov	byte ptr es:[bx+47],al
   ;	
   ;	    if (rec->a != 0) {
   ;	
	cmp	word ptr es:[bx+11],0
	je	short @1@142
   ;	
   ;	        if (rec->a < 0) {
   ;	
	cmp	word ptr es:[bx+11],0
	jge	short @1@114
   ;	
   ;	            rec->d = 1;
   ;	
	mov	byte ptr es:[bx+51],1
   ;	
   ;	            return;
   ;	
	pop	bp
	ret	
@1@114:
   ;	
   ;	        }
   ;	        rec->d = 2;
   ;	
	les	bx,dword ptr [bp+6]
	mov	byte ptr es:[bx+51],2
@1@142:
   ;	
   ;	    }
   ;	}
   ;	
	pop	bp
	ret	
_e1	endp
	?debug	C E9
	?debug	C FA00000000
E1_TEXT	ends
_DATA	segment word public 'DATA'
s@	label	byte
_DATA	ends
E1_TEXT	segment byte public 'CODE'
E1_TEXT	ends
	public	_e1
_s@	equ	s@
	end
