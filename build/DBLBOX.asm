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
	?debug	S "dblbox.c"
	?debug	C E96F83445D0864626C626F782E63
DBLBOX_TEXT	segment byte public 'CODE'
DBLBOX_TEXT	ends
DGROUP	group	_DATA,_BSS
	assume	cs:DBLBOX_TEXT,ds:DGROUP
_DATA	segment word public 'DATA'
d@	label	byte
d@w	label	word
_DATA	ends
_BSS	segment word public 'BSS'
b@	label	byte
b@w	label	word
_BSS	ends
DBLBOX_TEXT	segment byte public 'CODE'
   ;	
   ;	void dblbox(struct rect_s far *r, int u, int v, unsigned char w)
   ;	
	assume	cs:DBLBOX_TEXT
_dblbox	proc	far
	push	bp
	mov	bp,sp
   ;	
   ;	{
   ;	    if (w) {
   ;	
	cmp	byte ptr [bp+14],0
	je	short @1@86
   ;	
   ;	        u <<= 1;
   ;	
	shl	word ptr [bp+10],1
   ;	
   ;	        v <<= 1;
   ;	
	shl	word ptr [bp+12],1
@1@86:
   ;	
   ;	    }
   ;	    r->a1 = u;
   ;	
	les	bx,dword ptr [bp+6]
	mov	ax,word ptr [bp+10]
	mov	word ptr es:[bx+15],ax
   ;	
   ;	    r->a2 = u;
   ;	
	mov	word ptr es:[bx+19],ax
   ;	
   ;	    r->x1 = u;
   ;	
	mov	word ptr es:[bx+2],ax
   ;	
   ;	    r->b1 = v;
   ;	
	mov	ax,word ptr [bp+12]
	mov	word ptr es:[bx+17],ax
   ;	
   ;	    r->b2 = v;
   ;	
	mov	word ptr es:[bx+21],ax
   ;	
   ;	    r->y1 = v;
   ;	
	mov	word ptr es:[bx+4],ax
   ;	
   ;	    r->ha = r->x1 >> 1;
   ;	
	mov	ax,word ptr es:[bx+2]
	sar	ax,1
	mov	word ptr es:[bx+27],ax
   ;	
   ;	    r->hb = r->y1 >> 1;
   ;	
	mov	ax,word ptr es:[bx+4]
	sar	ax,1
	mov	word ptr es:[bx+29],ax
   ;	
   ;	}
   ;	
	pop	bp
	ret	
_dblbox	endp
	?debug	C E9
	?debug	C FA00000000
DBLBOX_TEXT	ends
_DATA	segment word public 'DATA'
s@	label	byte
_DATA	ends
DBLBOX_TEXT	segment byte public 'CODE'
DBLBOX_TEXT	ends
	public	_dblbox
_s@	equ	s@
	end
