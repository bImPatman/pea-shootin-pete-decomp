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
	?debug	S "n3.c"
	?debug	C E9AB85445D046E332E63
N3_TEXT	segment byte public 'CODE'
N3_TEXT	ends
DGROUP	group	_DATA,_BSS
	assume	cs:N3_TEXT,ds:DGROUP
_DATA	segment word public 'DATA'
d@	label	byte
d@w	label	word
_DATA	ends
_BSS	segment word public 'BSS'
b@	label	byte
b@w	label	word
_BSS	ends
N3_TEXT	segment byte public 'CODE'
   ;	
   ;	void j1(struct rec_s far *a, int far *b, int far *c)
   ;	
	assume	cs:N3_TEXT
_j1	proc	far
	push	bp
	mov	bp,sp
	sub	sp,2
   ;	
   ;	{
   ;	    int t = *c;
   ;	
	les	bx,dword ptr [bp+14]
	mov	ax,word ptr es:[bx]
	mov	word ptr [bp-2],ax
   ;	
   ;	    if (a->f06 + t >= 0x136) {
   ;	
	les	bx,dword ptr [bp+6]
	mov	ax,word ptr es:[bx+6]
	add	ax,word ptr [bp-2]
	cmp	ax,310
	jl	short @1@86
   ;	
   ;	        a->f66 |= 1;
   ;	
	or	byte ptr es:[bx+102],1
   ;	
   ;	    } else {
   ;	
	leave	
	ret	
@1@86:
   ;	
   ;	        *c = a->f0d + t;
   ;	
	les	bx,dword ptr [bp+6]
	mov	ax,word ptr es:[bx+13]
	add	ax,word ptr [bp-2]
	les	bx,dword ptr [bp+14]
	mov	word ptr es:[bx],ax
   ;	
   ;	        *b = *b;
   ;	
	les	bx,dword ptr [bp+10]
	mov	ax,word ptr es:[bx]
	mov	word ptr es:[bx],ax
   ;	
   ;	    }
   ;	}
   ;	
	leave	
	ret	
_j1	endp
	?debug	C E9
	?debug	C FA00000000
N3_TEXT	ends
_DATA	segment word public 'DATA'
s@	label	byte
_DATA	ends
N3_TEXT	segment byte public 'CODE'
N3_TEXT	ends
	public	_j1
_s@	equ	s@
	end
