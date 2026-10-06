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
	?debug	S "x1a40.c"
	?debug	C E99B92445D0778316134302E63
X1A40_TEXT	segment byte public 'CODE'
X1A40_TEXT	ends
DGROUP	group	_DATA,_BSS
	assume	cs:X1A40_TEXT,ds:DGROUP
_DATA	segment word public 'DATA'
d@	label	byte
d@w	label	word
_DATA	ends
_BSS	segment word public 'BSS'
b@	label	byte
b@w	label	word
_BSS	ends
X1A40_TEXT	segment byte public 'CODE'
   ;	
   ;	void m_1a40(void)
   ;	
	assume	cs:X1A40_TEXT
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
X1A40_TEXT	ends
_BSS	segment word public 'BSS'
_g_3858	label	dword
	db	4 dup (?)
_g_27aa	label	dword
	db	4 dup (?)
_g_34a9	label	dword
	db	4 dup (?)
	?debug	C E9
	?debug	C FA00000000
_BSS	ends
_DATA	segment word public 'DATA'
s@	label	byte
_DATA	ends
X1A40_TEXT	segment byte public 'CODE'
X1A40_TEXT	ends
	public	_m_1a40
	public	_g_3858
	public	_g_27aa
	public	_g_34a9
	extrn	_clrbit3:far
	extrn	_statebak:far
	extrn	_buf_to_vga:far
	extrn	_node_free:far
_s@	equ	s@
	end
