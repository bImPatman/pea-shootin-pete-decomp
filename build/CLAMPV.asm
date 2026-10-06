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
	?debug	S "clampv.c"
	?debug	C E90E85445D08636C616D70762E63
CLAMPV_TEXT	segment byte public 'CODE'
CLAMPV_TEXT	ends
DGROUP	group	_DATA,_BSS
	assume	cs:CLAMPV_TEXT,ds:DGROUP
_DATA	segment word public 'DATA'
d@	label	byte
d@w	label	word
_DATA	ends
_BSS	segment word public 'BSS'
b@	label	byte
b@w	label	word
_BSS	ends
CLAMPV_TEXT	segment byte public 'CODE'
   ;	
   ;	void clampv(struct cfg_s far *c)
   ;	
	assume	cs:CLAMPV_TEXT
_clampv	proc	far
	push	bp
	mov	bp,sp
   ;	
   ;	{
   ;	    c->f4 = c->f0 = c->f5 = 0;
   ;	
	les	bx,dword ptr [bp+6]
	mov	al,0
	mov	byte ptr es:[bx+5],al
	mov	byte ptr es:[bx],al
	mov	byte ptr es:[bx+4],al
   ;	
   ;	    if (gflag != 0) {
   ;	
	cmp	word ptr DGROUP:_gflag,0
	je	short @1@170
   ;	
   ;	        c->v = c->src1;
   ;	
	mov	ax,word ptr es:[bx+18]
	mov	word ptr es:[bx+12],ax
   ;	
   ;	        if (c->v > 0xD2)
   ;	
	cmp	word ptr es:[bx+12],210
	jle	short @1@114
   ;	
   ;	            c->v = 0xD2;
   ;	
	mov	word ptr es:[bx+12],210
@1@114:
   ;	
   ;	        if (c->v < 0x28)
   ;	
	les	bx,dword ptr [bp+6]
	cmp	word ptr es:[bx+12],40
	jge	short @1@198
   ;	
   ;	            c->v = 0x28;
   ;	
	mov	word ptr es:[bx+12],40
	jmp	short @1@198
@1@170:
   ;	
   ;	    } else {
   ;	        c->v = c->src2;
   ;	
	les	bx,dword ptr [bp+6]
	mov	ax,word ptr es:[bx+20]
	mov	word ptr es:[bx+12],ax
@1@198:
   ;	
   ;	    }
   ;	    c->w = c->v;
   ;	
	les	bx,dword ptr [bp+6]
	mov	ax,word ptr es:[bx+12]
	mov	word ptr es:[bx+14],ax
   ;	
   ;	    c->x = c->v;
   ;	
	mov	word ptr es:[bx+16],ax
   ;	
   ;	}
   ;	
	pop	bp
	ret	
_clampv	endp
CLAMPV_TEXT	ends
_BSS	segment word public 'BSS'
_gflag	label	word
	db	2 dup (?)
	?debug	C E9
	?debug	C FA00000000
_BSS	ends
_DATA	segment word public 'DATA'
s@	label	byte
_DATA	ends
CLAMPV_TEXT	segment byte public 'CODE'
CLAMPV_TEXT	ends
	public	_clampv
	public	_gflag
_s@	equ	s@
	end
