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
	?debug	S "e256.c"
	?debug	C E96689455D06653235362E63
E256_TEXT	segment byte public 'CODE'
E256_TEXT	ends
DGROUP	group	_DATA,_BSS
	assume	cs:E256_TEXT,ds:DGROUP
_DATA	segment word public 'DATA'
d@	label	byte
d@w	label	word
_DATA	ends
_BSS	segment word public 'BSS'
b@	label	byte
b@w	label	word
_BSS	ends
E256_TEXT	segment byte public 'CODE'
   ;	
   ;	void m_e256(struct e256 far *p)
   ;	
	assume	cs:E256_TEXT
_m_e256	proc	far
	push	bp
	mov	bp,sp
	sub	sp,14
   ;	
   ;	{
   ;	    int i, j, best;
   ;	
   ;	    if (!g_18f8)
   ;	
	mov	al,byte ptr DGROUP:_g_18f8
	mov	ah,0
	or	ax,ax
	jne	@@0
	jmp	@1@730
@@0:
   ;	
   ;	        return;
   ;	
   ;	    if (g_18f9) {
   ;	
	cmp	byte ptr DGROUP:_g_18f9,0
	je	short @1@142
   ;	
   ;	        if (!FUN_161d_08de())
   ;	
	call	far ptr _FUN_161d_08de
	or	ax,ax
	jne	short @1@142
   ;	
   ;	            g_18f9 = 0;
   ;	
	mov	byte ptr DGROUP:_g_18f9,0
@1@142:
   ;	
   ;	    }
   ;	
   ;	    for (i = 0; i < p->count; i++) {
   ;	
	mov	word ptr [bp-2],0
	mov	ax,word ptr [bp+6]
	add	ax,80
	mov	word ptr [bp-12],ax
	mov	ax,word ptr [bp+6]
	mov	word ptr [bp-14],ax
	jmp	@1@702
@1@198:
   ;	
   ;	        if (p->used[i]) {
   ;	
	mov	es,word ptr [bp+8]
	mov	bx,word ptr [bp-12]
	cmp	byte ptr es:[bx],0
	jne	@@1
	jmp	@1@618
@@1:
   ;	
   ;	            FUN_1e25_105b(p->ent[i]);
   ;	
	mov	bx,word ptr [bp-14]
	push	word ptr es:[bx+2]
	push	word ptr es:[bx]
	call	far ptr _FUN_1e25_105b
	add	sp,4
   ;	
   ;	
   ;	            if (p->ent[i]->live) {
   ;	
	mov	es,word ptr [bp+8]
	mov	bx,word ptr [bp-14]
	les	bx,dword ptr es:[bx]
	cmp	byte ptr es:[bx+4],0
	jne	@@2
	jmp	@1@618
@@2:
   ;	
   ;	                FUN_1e25_0dde(p->ent[i]);
   ;	
	mov	es,word ptr [bp+8]
	mov	bx,word ptr [bp-14]
	push	word ptr es:[bx+2]
	push	word ptr es:[bx]
	call	far ptr _FUN_1e25_0dde
	add	sp,4
   ;	
   ;	                p->used[i] = 0;
   ;	
	mov	es,word ptr [bp+8]
	mov	bx,word ptr [bp-12]
	mov	byte ptr es:[bx],0
   ;	
   ;	
   ;	                if (p->ent[i] == g_cur) {
   ;	
	mov	bx,word ptr [bp-14]
	mov	ax,word ptr es:[bx+2]
	mov	dx,word ptr es:[bx]
	cmp	ax,word ptr DGROUP:_g_cur+2
	je	@@3
	jmp	@1@618
@@3:
	cmp	dx,word ptr DGROUP:_g_cur
	je	@@4
	jmp	@1@618
@@4:
   ;	
   ;	                    best = 0;
   ;	
	mov	word ptr [bp-6],0
   ;	
   ;	                    for (j = 0; j < p->count; j++) {
   ;	
	mov	word ptr [bp-4],0
	mov	ax,word ptr [bp+6]
	add	ax,80
	mov	word ptr [bp-8],ax
	mov	ax,word ptr [bp+6]
	mov	word ptr [bp-10],ax
	jmp	short @1@534
@1@366:
   ;	
   ;	                        if (p->used[j]) {
   ;	
	mov	es,word ptr [bp+8]
	mov	bx,word ptr [bp-8]
	cmp	byte ptr es:[bx],0
	je	short @1@450
   ;	
   ;	                            if (p->ent[j]->score > best) {
   ;	
	mov	bx,word ptr [bp-10]
	les	bx,dword ptr es:[bx]
	mov	al,byte ptr es:[bx+3]
	mov	ah,0
	cmp	ax,word ptr [bp-6]
	jle	short @1@450
   ;	
   ;	                                g_cur = p->ent[j];
   ;	
	mov	es,word ptr [bp+8]
	mov	bx,word ptr [bp-10]
	mov	ax,word ptr es:[bx+2]
	mov	dx,word ptr es:[bx]
	mov	word ptr DGROUP:_g_cur+2,ax
	mov	word ptr DGROUP:_g_cur,dx
   ;	
   ;	                                best = p->ent[j]->score;
   ;	
	les	bx,dword ptr es:[bx]
	mov	al,byte ptr es:[bx+3]
	mov	ah,0
	mov	word ptr [bp-6],ax
@1@450:
	inc	word ptr [bp-8]
	add	word ptr [bp-10],4
	inc	word ptr [bp-4]
@1@534:
	les	bx,dword ptr [bp+6]
	mov	al,byte ptr es:[bx+265]
	mov	ah,0
	cmp	ax,word ptr [bp-4]
	jg	short @1@366
   ;	
   ;	                            }
   ;	                        }
   ;	                    }
   ;	
   ;	                    if (best == 0)
   ;	
	cmp	word ptr [bp-6],0
	jne	short @1@618
   ;	
   ;	                        g_cur = 0;
   ;	
	mov	word ptr DGROUP:_g_cur+2,0
	mov	word ptr DGROUP:_g_cur,0
@1@618:
	inc	word ptr [bp-12]
	add	word ptr [bp-14],4
	inc	word ptr [bp-2]
@1@702:
	les	bx,dword ptr [bp+6]
	mov	al,byte ptr es:[bx+265]
	mov	ah,0
	cmp	ax,word ptr [bp-2]
	jle	@@5
	jmp	@1@198
@@5:
@1@730:
   ;	
   ;	                }
   ;	            }
   ;	        }
   ;	    }
   ;	}
   ;	
	leave	
	ret	
_m_e256	endp
   ;	
   ;	void FUN_1e25_105b(char far *p) { p; }
   ;	
	assume	cs:E256_TEXT
_FUN_1e25_105b	proc	far
	push	bp
	mov	bp,sp
	pop	bp
	ret	
_FUN_1e25_105b	endp
   ;	
   ;	void FUN_1e25_0dde(char far *p) { p; }
   ;	
	assume	cs:E256_TEXT
_FUN_1e25_0dde	proc	far
	push	bp
	mov	bp,sp
	pop	bp
	ret	
_FUN_1e25_0dde	endp
E256_TEXT	ends
_BSS	segment word public 'BSS'
_g_cur	label	dword
	db	4 dup (?)
_g_18f9	label	byte
	db	1 dup (?)
_g_18f8	label	byte
	db	1 dup (?)
	?debug	C E9
	?debug	C FA04000000
_BSS	ends
_DATA	segment word public 'DATA'
s@	label	byte
_DATA	ends
E256_TEXT	segment byte public 'CODE'
E256_TEXT	ends
	public	_m_e256
	public	_FUN_1e25_0dde
	public	_FUN_1e25_105b
	extrn	_FUN_161d_08de:far
	public	_g_cur
	public	_g_18f9
	public	_g_18f8
_s@	equ	s@
	end
