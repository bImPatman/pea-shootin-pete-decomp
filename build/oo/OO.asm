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
	?debug	S "oo.c"
	?debug	C E9C774445D046F6F2E63
OO_TEXT	segment byte public 'CODE'
OO_TEXT	ends
DGROUP	group	_DATA,_BSS
	assume	cs:OO_TEXT,ds:DGROUP
_DATA	segment word public 'DATA'
d@	label	byte
d@w	label	word
_DATA	ends
_BSS	segment word public 'BSS'
b@	label	byte
b@w	label	word
_BSS	ends
OO_TEXT	segment byte public 'CODE'
   ;	
   ;	int loop1(int n) { int i, s = 0; for (i = 0; i < n; i++) s += i; return s; }
   ;	
	assume	cs:OO_TEXT
_loop1	proc	far
	push	bp
	mov	bp,sp
	sub	sp,4
	mov	word ptr [bp-4],0
	mov	word ptr [bp-2],0
	mov	ax,word ptr [bp-2]
	cmp	ax,word ptr [bp+6]
	jge	short @1@170
@1@86:
	mov	ax,word ptr [bp-2]
	add	word ptr [bp-4],ax
	inc	word ptr [bp-2]
	mov	ax,word ptr [bp-2]
	cmp	ax,word ptr [bp+6]
	jl	short @1@86
@1@170:
	mov	ax,word ptr [bp-4]
	leave	
	ret	
_loop1	endp
   ;	
   ;	int loop2(unsigned char *p, int n) { int i, s = 0; for (i = 0; i < n; i++) s += p[i]; return s; }
   ;	
	assume	cs:OO_TEXT
_loop2	proc	far
	push	bp
	mov	bp,sp
	sub	sp,6
	mov	word ptr [bp-4],0
	mov	word ptr [bp-2],0
	mov	ax,word ptr [bp+6]
	mov	word ptr [bp-6],ax
	mov	ax,word ptr [bp-2]
	cmp	ax,word ptr [bp+10]
	jge	short @2@226
@2@114:
	mov	es,word ptr [bp+8]
	mov	bx,word ptr [bp-6]
	mov	al,byte ptr es:[bx]
	mov	ah,0
	add	word ptr [bp-4],ax
	inc	word ptr [bp-6]
	inc	word ptr [bp-2]
	mov	ax,word ptr [bp-2]
	cmp	ax,word ptr [bp+10]
	jl	short @2@114
@2@226:
	mov	ax,word ptr [bp-4]
	leave	
	ret	
_loop2	endp
   ;	
   ;	int sw(int x) { switch (x) { case 1: return 10; case 2: return 20; case 3: return 30;
   ;	
	assume	cs:OO_TEXT
_sw	proc	far
	push	bp
	mov	bp,sp
	mov	bx,word ptr [bp+6]
	dec	bx
	cmp	bx,4
	ja	short @3@254
	shl	bx,1
	jmp	word ptr cs:@3@C162[bx]
@3@114:
	mov	ax,10
	pop	bp
	ret	
@3@142:
	mov	ax,20
	pop	bp
	ret	
@3@170:
	mov	ax,30
	pop	bp
	ret	
@3@198:
   ;	
   ;	                            case 4: return 40; case 5: return 50; default: return -1; } }
   ;	
	mov	ax,40
	pop	bp
	ret	
@3@226:
	mov	ax,50
	pop	bp
	ret	
@3@254:
	mov	ax,-1
	pop	bp
	ret	
_sw	endp
@3@C162	label	word
	dw	@3@114
	dw	@3@142
	dw	@3@170
	dw	@3@198
	dw	@3@226
   ;	
   ;	int nest(int n) { int i, j, s = 0; for (i = 0; i < n; i++) for (j = 0; j < n; j++) s += i*j; return s; }
   ;	
	assume	cs:OO_TEXT
_nest	proc	far
	push	bp
	mov	bp,sp
	sub	sp,6
	mov	word ptr [bp-6],0
	mov	word ptr [bp-2],0
	mov	ax,word ptr [bp-2]
	cmp	ax,word ptr [bp+6]
	jge	short @4@282
@4@86:
	mov	word ptr [bp-4],0
	mov	ax,word ptr [bp-4]
	cmp	ax,word ptr [bp+6]
	jge	short @4@226
@4@142:
	mov	ax,word ptr [bp-2]
	imul	word ptr [bp-4]
	add	word ptr [bp-6],ax
	inc	word ptr [bp-4]
	mov	ax,word ptr [bp-4]
	cmp	ax,word ptr [bp+6]
	jl	short @4@142
@4@226:
	inc	word ptr [bp-2]
	mov	ax,word ptr [bp-2]
	cmp	ax,word ptr [bp+6]
	jl	short @4@86
@4@282:
	mov	ax,word ptr [bp-6]
	leave	
	ret	
_nest	endp
   ;	
   ;	void many(char a, int b, unsigned c, long d, char far *e) { g = (char)(a + b + c + d + (int)e); }
   ;	
	assume	cs:OO_TEXT
_many	proc	far
	push	bp
	mov	bp,sp
	mov	al,byte ptr [bp+6]
	add	al,byte ptr [bp+8]
	add	al,byte ptr [bp+10]
	add	al,byte ptr [bp+12]
	add	al,byte ptr [bp+16]
	mov	byte ptr DGROUP:_g,al
	pop	bp
	ret	
_many	endp
   ;	
   ;	int wh(int x) { int s = 0; while (x) { s += x; x--; } return s; }
   ;	
	assume	cs:OO_TEXT
_wh	proc	far
	push	bp
	mov	bp,sp
	sub	sp,2
	mov	word ptr [bp-2],0
	jmp	short @6@86
@6@58:
	mov	ax,word ptr [bp+6]
	add	word ptr [bp-2],ax
	dec	word ptr [bp+6]
@6@86:
	cmp	word ptr [bp+6],0
	jne	short @6@58
	mov	ax,word ptr [bp-2]
	leave	
	ret	
_wh	endp
OO_TEXT	ends
_BSS	segment word public 'BSS'
_g	label	byte
	db	1 dup (?)
	?debug	C E9
	?debug	C FA04000000
_BSS	ends
_DATA	segment word public 'DATA'
s@	label	byte
_DATA	ends
OO_TEXT	segment byte public 'CODE'
OO_TEXT	ends
	public	_wh
	public	_many
	public	_nest
	public	_sw
	public	_loop2
	public	_loop1
	public	_g
_s@	equ	s@
	end
