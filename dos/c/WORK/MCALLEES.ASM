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
	?debug	S "mcallees.c"
	?debug	C E9AE90455D0A6D63616C6C6565732E63
MCALLEES_TEXT	segment byte public 'CODE'
MCALLEES_TEXT	ends
DGROUP	group	_DATA,_BSS
	assume	cs:MCALLEES_TEXT,ds:DGROUP
_DATA	segment word public 'DATA'
d@	label	byte
d@w	label	word
_DATA	ends
_BSS	segment word public 'BSS'
b@	label	byte
b@w	label	word
_BSS	ends
MCALLEES_TEXT	segment byte public 'CODE'
   ;	
   ;	void m_1df6(void)
   ;	
	assume	cs:MCALLEES_TEXT
_m_1df6	proc	far
   ;	
   ;	{
   ;	}
   ;	
	ret	
_m_1df6	endp
   ;	
   ;	void m_38a2(void)
   ;	
	assume	cs:MCALLEES_TEXT
_m_38a2	proc	far
   ;	
   ;	{
   ;	}
   ;	
	ret	
_m_38a2	endp
   ;	
   ;	void m_2415(void)
   ;	
	assume	cs:MCALLEES_TEXT
_m_2415	proc	far
   ;	
   ;	{
   ;	}
   ;	
	ret	
_m_2415	endp
   ;	
   ;	void m_24e7(int v)
   ;	
	assume	cs:MCALLEES_TEXT
_m_24e7	proc	far
	push	bp
	mov	bp,sp
   ;	
   ;	{
   ;	    v;
   ;	
   ;	
   ;	}
   ;	
	pop	bp
	ret	
_m_24e7	endp
   ;	
   ;	void m_0dd6(void)
   ;	
	assume	cs:MCALLEES_TEXT
_m_0dd6	proc	far
   ;	
   ;	{
   ;	}
   ;	
	ret	
_m_0dd6	endp
   ;	
   ;	void m_154f(int v)
   ;	
	assume	cs:MCALLEES_TEXT
_m_154f	proc	far
	push	bp
	mov	bp,sp
   ;	
   ;	{
   ;	    v;
   ;	
   ;	
   ;	}
   ;	
	pop	bp
	ret	
_m_154f	endp
   ;	
   ;	void m_013b(void)
   ;	
	assume	cs:MCALLEES_TEXT
_m_013b	proc	far
   ;	
   ;	{
   ;	}
   ;	
	ret	
_m_013b	endp
   ;	
   ;	void m_1a40(void)
   ;	
	assume	cs:MCALLEES_TEXT
_m_1a40	proc	far
   ;	
   ;	{
   ;	    node_free(g_34a9);
   ;	
	push	word ptr DGROUP:_g_34a9+2
	push	word ptr DGROUP:_g_34a9
	call	far ptr _node_free
	add	sp,4
   ;	
   ;	    statebak(g_3858);
   ;	
	push	word ptr DGROUP:_g_3858+2
	push	word ptr DGROUP:_g_3858
	call	far ptr _statebak
	add	sp,4
   ;	
   ;	    clrbit3(g_3858);
   ;	
	push	word ptr DGROUP:_g_3858+2
	push	word ptr DGROUP:_g_3858
	call	far ptr _clrbit3
	add	sp,4
   ;	
   ;	    buf_to_vga(g_27aa);
   ;	
	push	word ptr DGROUP:_g_27aa+2
	push	word ptr DGROUP:_g_27aa
	call	far ptr _buf_to_vga
	add	sp,4
   ;	
   ;	}
   ;	
	ret	
_m_1a40	endp
   ;	
   ;	void m_34e5(void)
   ;	
	assume	cs:MCALLEES_TEXT
_m_34e5	proc	far
   ;	
   ;	{
   ;	}
   ;	
	ret	
_m_34e5	endp
   ;	
   ;	void m_35dd(void)
   ;	
	assume	cs:MCALLEES_TEXT
_m_35dd	proc	far
   ;	
   ;	{
   ;	}
   ;	
	ret	
_m_35dd	endp
	?debug	C E9
	?debug	C FA00000000
MCALLEES_TEXT	ends
_DATA	segment word public 'DATA'
s@	label	byte
_DATA	ends
MCALLEES_TEXT	segment byte public 'CODE'
MCALLEES_TEXT	ends
	public	_m_35dd
	public	_m_34e5
	public	_m_1a40
	public	_m_013b
	public	_m_154f
	public	_m_0dd6
	public	_m_24e7
	public	_m_2415
	public	_m_38a2
	public	_m_1df6
	extrn	_g_3858:dword
	extrn	_g_34a9:dword
	extrn	_g_27aa:dword
	extrn	_clrbit3:far
	extrn	_statebak:far
	extrn	_buf_to_vga:far
	extrn	_node_free:far
_s@	equ	s@
	end
