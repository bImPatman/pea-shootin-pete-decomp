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
	?debug	S "gf.c"
	?debug	C E99D79445D0467662E63
GF_TEXT	segment byte public 'CODE'
GF_TEXT	ends
DGROUP	group	_DATA,_BSS
	assume	cs:GF_TEXT,ds:DGROUP
_DATA	segment word public 'DATA'
d@	label	byte
d@w	label	word
_DATA	ends
_BSS	segment word public 'BSS'
b@	label	byte
b@w	label	word
_BSS	ends
GF_TEXT	segment byte public 'CODE'
   ;	
   ;	unsigned char g_find(unsigned int klo, unsigned int khi)
   ;	
	assume	cs:GF_TEXT
_g_find	proc	far
	push	bp
	mov	bp,sp
	sub	sp,4
   ;	
   ;	{
   ;	    int n = 0;
   ;	
	mov	word ptr [bp-2],0
   ;	
   ;	    unsigned int near *p = (unsigned int near *)0x34c5;
   ;	
	mov	word ptr [bp-4],13509
@1@58:
   ;	
   ;	    do {
   ;	        if (p[1] <= khi) {
   ;	
	mov	bx,word ptr [bp-4]
	mov	ax,word ptr [bx+2]
	cmp	ax,word ptr [bp+8]
	ja	short @1@170
   ;	
   ;	            if (p[1] < khi || p[0] < klo)
   ;	
	cmp	ax,word ptr [bp+8]
	jb	short @1@142
	mov	ax,word ptr [bx]
	cmp	ax,word ptr [bp+6]
	jae	short @1@170
@1@142:
   ;	
   ;	                return n + 1;
   ;	
	mov	al,byte ptr [bp-2]
	inc	al
	leave	
	ret	
@1@170:
   ;	
   ;	        }
   ;	        p += 7;
   ;	
	add	word ptr [bp-4],14
   ;	
   ;	        n++;
   ;	
	inc	word ptr [bp-2]
   ;	
   ;	    } while (p != 0x3551);
   ;	
	cmp	word ptr [bp-4],13649
	jne	short @1@58
   ;	
   ;	    return 0;
   ;	
	mov	al,0
   ;	
   ;	}
   ;	
	leave	
	ret	
_g_find	endp
   ;	
   ;	void main(void){ g_find(0,0); }
   ;	
	assume	cs:GF_TEXT
_main	proc	far
	push	0
	push	0
	push	cs
	call	near ptr _g_find
	add	sp,4
	ret	
_main	endp
	?debug	C E9
	?debug	C FA00000000
GF_TEXT	ends
_DATA	segment word public 'DATA'
s@	label	byte
_DATA	ends
GF_TEXT	segment byte public 'CODE'
GF_TEXT	ends
	public	_main
	public	_g_find
_s@	equ	s@
	end
