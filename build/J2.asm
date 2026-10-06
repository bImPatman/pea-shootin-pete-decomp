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
	?debug	S "j2.c"
	?debug	C E9CF84455D046A322E63
J2_TEXT	segment byte public 'CODE'
J2_TEXT	ends
DGROUP	group	_DATA,_BSS
	assume	cs:J2_TEXT,ds:DGROUP
_DATA	segment word public 'DATA'
d@	label	byte
d@w	label	word
_DATA	ends
_BSS	segment word public 'BSS'
b@	label	byte
b@w	label	word
_BSS	ends
J2_TEXT	segment byte public 'CODE'
   ;	
   ;	int m_f44b(struct f44brec far *p, unsigned char v)
   ;	
	assume	cs:J2_TEXT
_m_f44b	proc	far
	push	bp
	mov	bp,sp
   ;	
   ;	{
   ;	    if (p->live) {
   ;	
	les	bx,dword ptr [bp+6]
	cmp	byte ptr es:[bx],0
	je	short @1@142
   ;	
   ;	        p->jitter = (unsigned)(rand() % 5) * 0x50;
   ;	
	call	far ptr _rand
	mov	bx,5
	cwd	
	idiv	bx
	mov	bx,80
	mov	ax,dx
	imul	bx
	les	bx,dword ptr [bp+6]
	mov	word ptr es:[bx+4],ax
   ;	
   ;	        if (p->countdown-- == 0)
   ;	
	mov	al,byte ptr es:[bx+1]
	dec	byte ptr es:[bx+1]
	or	al,al
	jne	short @1@114
   ;	
   ;	            p->live = 0;
   ;	
	mov	byte ptr es:[bx],0
@1@114:
   ;	
   ;	        setcrtc((unsigned short)(g_crtc[v] + p->jitter));
   ;	
	mov	al,byte ptr [bp+10]
	mov	ah,0
	shl	ax,1
	mov	bx,ax
	mov	ax,word ptr DGROUP:_g_crtc[bx]
	les	bx,dword ptr [bp+6]
	add	ax,word ptr es:[bx+4]
	push	ax
	call	far ptr _setcrtc
	add	sp,2
   ;	
   ;	    } else {
   ;	
	pop	bp
	ret	
@1@142:
   ;	
   ;	        setcrtc(g_crtc[v]);
   ;	
	mov	al,byte ptr [bp+10]
	mov	ah,0
	shl	ax,1
	mov	bx,ax
	push	word ptr DGROUP:_g_crtc[bx]
	call	far ptr _setcrtc
	add	sp,2
   ;	
   ;	    }
   ;	}
   ;	
	pop	bp
	ret	
_m_f44b	endp
J2_TEXT	ends
_BSS	segment word public 'BSS'
_g_crtc	label	word
	db	4 dup (?)
	?debug	C E9
	?debug	C FA00000000
_BSS	ends
_DATA	segment word public 'DATA'
s@	label	byte
_DATA	ends
J2_TEXT	segment byte public 'CODE'
J2_TEXT	ends
	public	_m_f44b
	extrn	_setcrtc:far
	public	_g_crtc
	extrn	_rand:far
_s@	equ	s@
	end
