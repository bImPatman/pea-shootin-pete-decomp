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
	?debug	S "main.c"
	?debug	C E96A9B455D066D61696E2E63
	?debug	C E94019CA1813443A5C494E434C5544455C737472696E672E68
	?debug	C E94019CA1812443A5C494E434C5544455C5F646566732E68
	?debug	C E94019CA1812443A5C494E434C5544455C5F6E756C6C2E68
MAIN_TEXT	segment byte public 'CODE'
MAIN_TEXT	ends
DGROUP	group	_DATA,_BSS
	assume	cs:MAIN_TEXT,ds:DGROUP
_DATA	segment word public 'DATA'
d@	label	byte
d@w	label	word
_DATA	ends
_BSS	segment word public 'BSS'
b@	label	byte
b@w	label	word
_BSS	ends
MAIN_TEXT	segment byte public 'CODE'
   ;	
   ;	int main(int argc, char far *argv[], char far *envp[])
   ;	
	assume	cs:MAIN_TEXT
_main	proc	far
	push	bp
	mov	bp,sp
	push	si
	push	di
   ;	
   ;	{
   ;	    if (strcmp(argv[1], g_cmd) == 0)
   ;	
	les	bx,dword ptr [bp+8]
	mov	ax,word ptr es:[bx+6]
	mov	si,word ptr es:[bx+4]
	push	ds
	pop	es
	mov	di,offset DGROUP:_g_cmd
	push	ax
	xor	ax,ax
	mov	cx,-1
	repnz 	scasb	
	not	cx
	sub	di,cx
	mov	ax,ds
	pop	ds
	push	ax
	xor	ax,ax
	repz 	cmpsb	
	pop	ds
	je	short @1@86
	sbb	ax,ax
	sbb	ax,-1
@1@86:
	or	ax,ax
	jne	short @1@142
   ;	
   ;	        g_2c60 = 1;
   ;	
	mov	byte ptr DGROUP:_g_2c60,1
	jmp	short @1@170
@1@142:
   ;	
   ;	    else
   ;	        g_2c60 = 0;
   ;	
	mov	byte ptr DGROUP:_g_2c60,0
@1@170:
   ;	
   ;	
   ;	    m_1df6();
   ;	
	call	far ptr _m_1df6
@1@198:
   ;	
   ;	
   ;	    for (;;) {
   ;	init_loop:
   ;	    if (g_2c60 == 0)
   ;	
	cmp	byte ptr DGROUP:_g_2c60,0
	jne	short @1@254
   ;	
   ;	        m_38a2();
   ;	
	call	far ptr _m_38a2
@1@254:
   ;	
   ;	    m_2415();
   ;	
	call	far ptr _m_2415
   ;	
   ;	    m_24e7(1);
   ;	
	push	1
	call	far ptr _m_24e7
	add	sp,2
   ;	
   ;	    goto check;
   ;	
	jmp	@1@338
@1@282:
   ;	
   ;	
   ;	frame:
   ;	    g_349f = g_349e ^ 1;
   ;	
	mov	al,byte ptr DGROUP:_g_349e
	xor	al,1
	mov	byte ptr DGROUP:_g_349f,al
   ;	
   ;	    m_0dd6();
   ;	
	call	far ptr _m_0dd6
   ;	
   ;	    m_b11a(g_34a9);
   ;	
	push	word ptr DGROUP:_g_34a9+2
	push	word ptr DGROUP:_g_34a9
	call	far ptr _m_b11a
	add	sp,4
   ;	
   ;	    m_154f(0);
   ;	
	push	0
	call	far ptr _m_154f
	add	sp,2
   ;	
   ;	    m_f44d(g_3858);
   ;	
	push	word ptr DGROUP:_g_3858+2
	push	word ptr DGROUP:_g_3858
	call	far ptr _m_f44d
	add	sp,4
   ;	
   ;	    g_349e = g_349f;
   ;	
	mov	al,byte ptr DGROUP:_g_349f
	mov	byte ptr DGROUP:_g_349e,al
   ;	
   ;	    m_f44b(g_3858, g_349f);
   ;	
	push	ax
	push	word ptr DGROUP:_g_3858+2
	push	word ptr DGROUP:_g_3858
	call	far ptr _m_f44b
	add	sp,6
   ;	
   ;	    m_f44c(g_3858);
   ;	
	push	word ptr DGROUP:_g_3858+2
	push	word ptr DGROUP:_g_3858
	call	far ptr _m_f44c
	add	sp,4
   ;	
   ;	    m_e256(g_27aa);
   ;	
	push	word ptr DGROUP:_g_27aa+2
	push	word ptr DGROUP:_g_27aa
	call	far ptr _m_e256
	add	sp,4
   ;	
   ;	    m_013b();
   ;	
	call	far ptr _m_013b
   ;	
   ;	
   ;	    if (g_27f3) {
   ;	
	cmp	byte ptr DGROUP:_g_27f3,0
	je	short @1@338
   ;	
   ;	        m_1a40();
   ;	
	call	far ptr _m_1a40
   ;	
   ;	        goto tail;
   ;	
	jmp	short @1@366
@1@338:
   ;	
   ;	    }
   ;	
   ;	check:
   ;	    if (!g_27fc)
   ;	
	mov	al,byte ptr DGROUP:_g_27fc
	mov	ah,0
	or	ax,ax
	jne	@@0
	jmp	@1@282
@@0:
@1@366:
   ;	
   ;	        goto frame;
   ;	
   ;	tail:
   ;	    m_b159(g_34ad);
   ;	
	push	word ptr DGROUP:_g_34ad+2
	push	word ptr DGROUP:_g_34ad
	call	far ptr _m_b159
	add	sp,4
   ;	
   ;	    if (g_386d) {
   ;	
	cmp	byte ptr DGROUP:_g_386d,0
	je	short @1@506
	jmp	short @1@450
@1@422:
   ;	
   ;	        while (key_poll())
   ;	            key_read();
   ;	
	call	far ptr _key_read
@1@450:
	call	far ptr _key_poll
	or	ax,ax
	jne	short @1@422
   ;	
   ;	    } else {
   ;	
	jmp	@1@198
@1@506:
   ;	
   ;	m_34e5();
   ;	
	call	far ptr _m_34e5
   ;	
   ;	        m_35dd();
   ;	
	call	far ptr _m_35dd
	jmp	@1@198
   ;	
   ;	    }
   ;	    }
   ;	}
   ;	
	pop	di
	pop	si
	pop	bp
	ret	
_main	endp
MAIN_TEXT	ends
_BSS	segment word public 'BSS'
_g_386d	label	byte
	db	1 dup (?)
_g_2c60	label	byte
	db	1 dup (?)
_g_cmd	label	byte
	db	80 dup (?)
	?debug	C E9
	?debug	C FA80000000
_BSS	ends
_DATA	segment word public 'DATA'
s@	label	byte
_DATA	ends
MAIN_TEXT	segment byte public 'CODE'
MAIN_TEXT	ends
	extrn	__setenvp__:far
	extrn	__setargv__:far
	public	_main
	extrn	_g_3858:dword
	extrn	_g_34ad:dword
	extrn	_g_34a9:dword
	extrn	_g_27aa:dword
	extrn	_g_27fc:byte
	extrn	_g_27f3:byte
	extrn	_g_349f:byte
	extrn	_g_349e:byte
	public	_g_386d
	public	_g_2c60
	public	_g_cmd
	extrn	_m_35dd:far
	extrn	_m_34e5:far
	extrn	_m_1a40:far
	extrn	_m_013b:far
	extrn	_m_154f:far
	extrn	_m_0dd6:far
	extrn	_m_24e7:far
	extrn	_m_2415:far
	extrn	_m_38a2:far
	extrn	_m_1df6:far
	extrn	_key_poll:far
	extrn	_key_read:far
	extrn	_m_b159:far
	extrn	_m_e256:far
	extrn	_m_f44c:far
	extrn	_m_f44b:far
	extrn	_m_f44d:far
	extrn	_m_b11a:far
_s@	equ	s@
	end
