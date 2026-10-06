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
	?debug	S "i1.c"
	?debug	C E93185445D0469312E63
I1_TEXT	segment byte public 'CODE'
I1_TEXT	ends
DGROUP	group	_DATA,_BSS
	assume	cs:I1_TEXT,ds:DGROUP
_DATA	segment word public 'DATA'
d@	label	byte
d@w	label	word
_DATA	ends
_BSS	segment word public 'BSS'
b@	label	byte
b@w	label	word
_BSS	ends
I1_TEXT	segment byte public 'CODE'
   ;	
   ;	void i1(struct rec_s far *a, int far *o1, int far *o2)
   ;	
	assume	cs:I1_TEXT
_i1	proc	far
	push	bp
	mov	bp,sp
	sub	sp,2
   ;	
   ;	{
   ;	    int t = a->b2;
   ;	
	les	bx,dword ptr [bp+6]
	mov	al,byte ptr es:[bx+101]
	shr	ax,2
	and	ax,1
	mov	ah,0
	mov	word ptr [bp-2],ax
   ;	
   ;	    if (t) {
   ;	
	cmp	word ptr [bp-2],0
	je	short @1@86
   ;	
   ;	        a->b2 = 0;
   ;	
	and	byte ptr es:[bx+101],251
   ;	
   ;	        g->b2 = 1;
   ;	
	les	bx,dword ptr DGROUP:_g
	or	byte ptr es:[bx+101],4
@1@86:
   ;	
   ;	    }
   ;	    *o1 = g->g02 + a->g49;
   ;	
	les	bx,dword ptr DGROUP:_g
	mov	ax,word ptr es:[bx+2]
	les	bx,dword ptr [bp+6]
	add	ax,word ptr es:[bx+73]
	les	bx,dword ptr [bp+10]
	mov	word ptr es:[bx],ax
   ;	
   ;	    *o2 = g->g04 + a->g4b;
   ;	
	les	bx,dword ptr DGROUP:_g
	mov	ax,word ptr es:[bx+4]
	les	bx,dword ptr [bp+6]
	add	ax,word ptr es:[bx+75]
	les	bx,dword ptr [bp+14]
	mov	word ptr es:[bx],ax
   ;	
   ;	}
   ;	
	leave	
	ret	
_i1	endp
I1_TEXT	ends
_BSS	segment word public 'BSS'
_g	label	dword
	db	4 dup (?)
	?debug	C E9
	?debug	C FA00000000
_BSS	ends
_DATA	segment word public 'DATA'
s@	label	byte
_DATA	ends
I1_TEXT	segment byte public 'CODE'
I1_TEXT	ends
	public	_i1
	public	_g
_s@	equ	s@
	end
