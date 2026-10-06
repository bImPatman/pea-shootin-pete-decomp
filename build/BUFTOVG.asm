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
	?debug	S "buftovg.c"
	?debug	C E95889455D09627566746F76672E63
BUFTOVG_TEXT	segment byte public 'CODE'
BUFTOVG_TEXT	ends
DGROUP	group	_DATA,_BSS
	assume	cs:BUFTOVG_TEXT,ds:DGROUP
_DATA	segment word public 'DATA'
d@	label	byte
d@w	label	word
_DATA	ends
_BSS	segment word public 'BSS'
b@	label	byte
b@w	label	word
_BSS	ends
BUFTOVG_TEXT	segment byte public 'CODE'
   ;	
   ;	void FUN_1e25_043e(struct bvg far *p) { p; }
   ;	
	assume	cs:BUFTOVG_TEXT
_FUN_1e25_043e	proc	far
	push	bp
	mov	bp,sp
	pop	bp
	ret	
_FUN_1e25_043e	endp
   ;	
   ;	void buf_to_vga(struct bvg far *p)
   ;	
	assume	cs:BUFTOVG_TEXT
_buf_to_vga	proc	far
	push	bp
	mov	bp,sp
	sub	sp,6
   ;	
   ;	{
   ;	    int i;
   ;	
   ;	    FUN_1000_20cb();
   ;	
	call	far ptr _FUN_1000_20cb
   ;	
   ;	
   ;	    for (i = 0; i < p->count; i++) {
   ;	
	mov	word ptr [bp-2],0
	mov	ax,word ptr [bp+6]
	add	ax,80
	mov	word ptr [bp-4],ax
	mov	ax,word ptr [bp+6]
	mov	word ptr [bp-6],ax
	jmp	short @2@198
@2@86:
   ;	
   ;	        /* Chained assignment, right to left: the shared zero is evaluated once
   ;	           into al and reused by all four byte stores. */
   ;	        p->ent[i]->live = p->ent[i]->b0 = p->ent[i]->b5 = p->used[i] = 0;
   ;	
	mov	es,word ptr [bp+8]
	mov	bx,word ptr [bp-4]
	mov	al,0
	mov	byte ptr es:[bx],al
	mov	bx,word ptr [bp-6]
	les	bx,dword ptr es:[bx]
	mov	byte ptr es:[bx+5],al
	mov	es,word ptr [bp+8]
	mov	bx,word ptr [bp-6]
	les	bx,dword ptr es:[bx]
	mov	byte ptr es:[bx],al
	mov	es,word ptr [bp+8]
	mov	bx,word ptr [bp-6]
	les	bx,dword ptr es:[bx]
	mov	byte ptr es:[bx+4],al
   ;	
   ;	        p->ent[i]->w12 = p->ent[i]->w16;
   ;	
	mov	es,word ptr [bp+8]
	mov	bx,word ptr [bp-6]
	les	bx,dword ptr es:[bx]
	mov	ax,word ptr es:[bx+16]
	mov	es,word ptr [bp+8]
	mov	bx,word ptr [bp-6]
	les	bx,dword ptr es:[bx]
	mov	word ptr es:[bx+12],ax
	inc	word ptr [bp-4]
	add	word ptr [bp-6],4
	inc	word ptr [bp-2]
@2@198:
	les	bx,dword ptr [bp+6]
	mov	al,byte ptr es:[bx+265]
	mov	ah,0
	cmp	ax,word ptr [bp-2]
	jg	short @2@86
   ;	
   ;	    }
   ;	
   ;	    if (g_cur_a)
   ;	
	mov	ax,word ptr DGROUP:_g_cur_a
	or	ax,word ptr DGROUP:_g_cur_a+2
	je	short @2@282
   ;	
   ;	        FUN_1e25_0d95(g_cur_a);
   ;	
	push	word ptr DGROUP:_g_cur_a+2
	push	word ptr DGROUP:_g_cur_a
	call	far ptr _FUN_1e25_0d95
	add	sp,4
@2@282:
   ;	
   ;	    if (g_cur_b)
   ;	
	mov	ax,word ptr DGROUP:_g_cur_b
	or	ax,word ptr DGROUP:_g_cur_b+2
	je	short @2@338
   ;	
   ;	        FUN_1e25_0d95(g_cur_b);
   ;	
	push	word ptr DGROUP:_g_cur_b+2
	push	word ptr DGROUP:_g_cur_b
	call	far ptr _FUN_1e25_0d95
	add	sp,4
@2@338:
   ;	
   ;	
   ;	    g_cur_a = 0;
   ;	
	mov	word ptr DGROUP:_g_cur_a+2,0
	mov	word ptr DGROUP:_g_cur_a,0
   ;	
   ;	    g_cur_b = 0;
   ;	
	mov	word ptr DGROUP:_g_cur_b+2,0
	mov	word ptr DGROUP:_g_cur_b,0
   ;	
   ;	    g_flag_flush = 0;
   ;	
	mov	byte ptr DGROUP:_g_flag_flush,0
   ;	
   ;	    FUN_1e25_043e(p);
   ;	
	push	word ptr [bp+8]
	push	word ptr [bp+6]
	push	cs
	call	near ptr _FUN_1e25_043e
	add	sp,4
   ;	
   ;	}
   ;	
	leave	
	ret	
_buf_to_vga	endp
   ;	
   ;	void FUN_1e25_0d95(char far *p) { p; }
   ;	
	assume	cs:BUFTOVG_TEXT
_FUN_1e25_0d95	proc	far
	push	bp
	mov	bp,sp
	pop	bp
	ret	
_FUN_1e25_0d95	endp
BUFTOVG_TEXT	ends
_BSS	segment word public 'BSS'
_g_flag_flush	label	byte
	db	1 dup (?)
_g_cur_b	label	dword
	db	4 dup (?)
_g_cur_a	label	dword
	db	4 dup (?)
	?debug	C E9
	?debug	C FA04000000
_BSS	ends
_DATA	segment word public 'DATA'
s@	label	byte
_DATA	ends
BUFTOVG_TEXT	segment byte public 'CODE'
BUFTOVG_TEXT	ends
	public	_buf_to_vga
	public	_FUN_1e25_043e
	public	_FUN_1e25_0d95
	public	_g_flag_flush
	public	_g_cur_b
	public	_g_cur_a
	extrn	_FUN_1000_20cb:far
_s@	equ	s@
	end
