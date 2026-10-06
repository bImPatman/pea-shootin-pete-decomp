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
	?debug	S "r3.c"
	?debug	C E93987445D0472332E63
R3_TEXT	segment byte public 'CODE'
R3_TEXT	ends
DGROUP	group	_DATA,_BSS
	assume	cs:R3_TEXT,ds:DGROUP
_DATA	segment word public 'DATA'
d@	label	byte
d@w	label	word
_DATA	ends
_BSS	segment word public 'BSS'
b@	label	byte
b@w	label	word
_BSS	ends
R3_TEXT	segment byte public 'CODE'
   ;	
   ;	void r1(struct rec_s far *a, int far *p, int far *q)
   ;	
	assume	cs:R3_TEXT
_r1	proc	far
	push	bp
	mov	bp,sp
	sub	sp,4
	push	si
   ;	
   ;	{
   ;	    int x = *p + a->f0b;
   ;	
	les	bx,dword ptr [bp+6]
	push	es
	les	si,dword ptr [bp+10]
	mov	ax,word ptr es:[si]
	pop	es
	add	ax,word ptr es:[bx+11]
	mov	word ptr [bp-2],ax
   ;	
   ;	    int y = *q;
   ;	
	les	bx,dword ptr [bp+14]
	mov	ax,word ptr es:[bx]
	mov	word ptr [bp-4],ax
   ;	
   ;	    if (a->f32 == 3) {
   ;	
	les	bx,dword ptr [bp+6]
	cmp	byte ptr es:[bx+50],3
	jne	short @1@86
   ;	
   ;	        y += a->f0d / 2 + 4;
   ;	
	mov	ax,word ptr es:[bx+13]
	cwd	
	sub	ax,dx
	sar	ax,1
	add	ax,4
	add	word ptr [bp-4],ax
   ;	
   ;	        a->f0d++;
   ;	
	inc	word ptr es:[bx+13]
@1@86:
   ;	
   ;	    }
   ;	    if (y + a->f6 > 0x136) {
   ;	
	les	bx,dword ptr [bp+6]
	mov	ax,word ptr [bp-4]
	add	ax,word ptr es:[bx+6]
	cmp	ax,310
	jle	short @1@142
   ;	
   ;	        a->f66 |= 1;
   ;	
	or	byte ptr es:[bx+102],1
   ;	
   ;	        a->f0d = 0;
   ;	
	mov	word ptr es:[bx+13],0
   ;	
   ;	        y = 0x136 - a->f6 - 7;
   ;	
	mov	ax,310
	sub	ax,word ptr es:[bx+6]
	add	ax,-7
	mov	word ptr [bp-4],ax
@1@142:
   ;	
   ;	    }
   ;	    if (x + a->f08 >= 0x280)
   ;	
	les	bx,dword ptr [bp+6]
	mov	ax,word ptr [bp-2]
	add	ax,word ptr es:[bx+8]
	cmp	ax,640
	jl	short @1@198
   ;	
   ;	        x = 0x280 - a->f08 - 1;
   ;	
	mov	ax,640
	sub	ax,word ptr es:[bx+8]
	dec	ax
	mov	word ptr [bp-2],ax
@1@198:
   ;	
   ;	    if (y < 0) {
   ;	
	cmp	word ptr [bp-4],0
	jge	short @1@254
   ;	
   ;	        y = *q;
   ;	
	les	bx,dword ptr [bp+14]
	mov	ax,word ptr es:[bx]
	mov	word ptr [bp-4],ax
   ;	
   ;	        a->f0d = -a->f0d;
   ;	
	les	bx,dword ptr [bp+6]
	mov	ax,word ptr es:[bx+13]
	neg	ax
	mov	word ptr es:[bx+13],ax
@1@254:
   ;	
   ;	    }
   ;	    if (x < 0)
   ;	
	cmp	word ptr [bp-2],0
	jge	short @1@310
   ;	
   ;	        x = *p;
   ;	
	les	bx,dword ptr [bp+10]
	mov	ax,word ptr es:[bx]
	mov	word ptr [bp-2],ax
@1@310:
   ;	
   ;	    *p = x;
   ;	
	les	bx,dword ptr [bp+10]
	mov	ax,word ptr [bp-2]
	mov	word ptr es:[bx],ax
   ;	
   ;	    *q = y;
   ;	
	les	bx,dword ptr [bp+14]
	mov	ax,word ptr [bp-4]
	mov	word ptr es:[bx],ax
   ;	
   ;	}
   ;	
	pop	si
	leave	
	ret	
_r1	endp
	?debug	C E9
	?debug	C FA00000000
R3_TEXT	ends
_DATA	segment word public 'DATA'
s@	label	byte
_DATA	ends
R3_TEXT	segment byte public 'CODE'
R3_TEXT	ends
	public	_r1
_s@	equ	s@
	end
