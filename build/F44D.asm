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
	?debug	S "f44d.c"
	?debug	C E9E78D455D06663434642E63
F44D_TEXT	segment byte public 'CODE'
F44D_TEXT	ends
DGROUP	group	_DATA,_BSS
	assume	cs:F44D_TEXT,ds:DGROUP
_DATA	segment word public 'DATA'
d@	label	byte
d@w	label	word
_DATA	ends
_BSS	segment word public 'BSS'
b@	label	byte
b@w	label	word
_BSS	ends
F44D_TEXT	segment byte public 'CODE'
   ;	
   ;	void m_f44d(char far *p)
   ;	
	assume	cs:F44D_TEXT
_m_f44d	proc	far
	push	bp
	mov	bp,sp
	sub	sp,10
   ;	
   ;	{
   ;	    /* `x` pads the frame out to the ten bytes the target reserves, and `q` is
   ;	       kept in a stack slot rather than folded into an immediate segment:offset
   ;	       pair: the target reloads it with `les` and then addresses es:[bx+n]. */
   ;	    struct { struct node far *q; char x[6]; } v;
   ;	
   ;	    if (p[2]) {
   ;	
	les	bx,dword ptr [bp+6]
	cmp	byte ptr es:[bx+2],0
	je	short @1@114
   ;	
   ;	        if (--g_3853 == 0) {
   ;	
	mov	al,byte ptr DGROUP:_g_3853
	add	al,255
	mov	byte ptr DGROUP:_g_3853,al
	or	al,al
	jne	short @1@114
   ;	
   ;	            v.q = &g_3850;
   ;	
	mov	word ptr [bp-8],ds
	mov	word ptr [bp-10],offset DGROUP:_g_3850
   ;	
   ;	            v.q->b = v.q->a;
   ;	
	mov	es,word ptr [bp-8]
	mov	al,byte ptr es:_g_3850+2
	mov	byte ptr es:_g_3850+3,al
   ;	
   ;	            pal_shift(v.q->w4, v.q->w6, g_3550);
   ;	
	push	ds
	push	offset DGROUP:_g_3550
	mov	bx,word ptr [bp-10]
	push	word ptr es:[bx+6]
	push	word ptr es:[bx+4]
	call	far ptr _pal_shift
	add	sp,8
   ;	
   ;	            return;
   ;	
@1@114:
   ;	
   ;	        }
   ;	    }
   ;	}
   ;	
	leave	
	ret	
_m_f44d	endp
F44D_TEXT	ends
_BSS	segment word public 'BSS'
_g_3853	label	byte
	db	1 dup (?)
_g_3550	label	byte
	db	768 dup (?)
_g_3850	label	word
	db	8 dup (?)
	?debug	C E9
	?debug	C FA00020000
_BSS	ends
_DATA	segment word public 'DATA'
s@	label	byte
_DATA	ends
F44D_TEXT	segment byte public 'CODE'
F44D_TEXT	ends
	public	_m_f44d
	extrn	_pal_shift:far
	public	_g_3853
	public	_g_3550
	public	_g_3850
_s@	equ	s@
	end
