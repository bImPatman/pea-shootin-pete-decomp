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
	?debug	S "g13b2.c"
	?debug	C E932A8465D0767313362322E63
G13B2_TEXT	segment byte public 'CODE'
G13B2_TEXT	ends
DGROUP	group	_DATA,_BSS
	assume	cs:G13B2_TEXT,ds:DGROUP
_DATA	segment word public 'DATA'
d@	label	byte
d@w	label	word
_DATA	ends
_BSS	segment word public 'BSS'
b@	label	byte
b@w	label	word
_g_19e6	label	dword
	db	4 dup (?)
_g_2c56	label	byte
	db	32 dup (?)
_g_2c33	label	byte
	db	7 dup (?)
_g_dispatch	label	word
	db	416 dup (?)
_g_34a1	label	dword
	db	4 dup (?)
_g_2bfd	label	word
	db	2 dup (?)
_g_2bfb	label	word
	db	2 dup (?)
_g_2c2f	label	dword
	db	4 dup (?)
_g_2c60	label	byte
	db	1 dup (?)
_g_18fa	label	byte
	db	1 dup (?)
_g_18f8	label	byte
	db	1 dup (?)
_g_1690	label	byte
	db	1 dup (?)
_g_2807	label	byte
	db	1 dup (?)
_g_2804	label	byte
	db	1 dup (?)
_g_2805	label	byte
	db	1 dup (?)
_g_2803	label	byte
	db	1 dup (?)
_g_27fb	label	byte
	db	1 dup (?)
_g_27ef	label	byte
	db	1 dup (?)
_g_27e7	label	byte
	db	1 dup (?)
_g_27e6	label	byte
	db	1 dup (?)
_g_27fe	label	byte
	db	1 dup (?)
_g_27f7	label	byte
	db	1 dup (?)
_g_27f4	label	byte
	db	1 dup (?)
_g_27f1	label	byte
	db	1 dup (?)
_g_27f0	label	byte
	db	1 dup (?)
_g_27ee	label	byte
	db	1 dup (?)
_g_2bf0	label	byte
	db	1 dup (?)
_g_27ae	label	dword
	db	4 dup (?)
_g_2bff	label	word
	db	2 dup (?)
_g_2c01	label	word
	db	2 dup (?)
_g_168c	label	word
	db	2 dup (?)
_g_3858	label	dword
	db	4 dup (?)
_g_34a9	label	dword
	db	4 dup (?)
_g_27aa	label	dword
	db	4 dup (?)
_g_349f	label	byte
	db	1 dup (?)
_g_349e	label	byte
	db	1 dup (?)
_g_34ad	label	dword
	db	4 dup (?)
_g_19e4	label	word
	db	2 dup (?)
_g_2bf5	label	word
	db	2 dup (?)
_g_27fa	label	byte
	db	1 dup (?)
_g_27f9	label	byte
	db	1 dup (?)
_g_27ed	label	byte
	db	1 dup (?)
_g_27ec	label	byte
	db	1 dup (?)
_g_2c61	label	byte
	db	1 dup (?)
_g_27f6	label	byte
	db	1 dup (?)
_g_27f2	label	byte
	db	1 dup (?)
_g_2800	label	byte
	db	1 dup (?)
_g_27f3	label	byte
	db	1 dup (?)
_g_27ff	label	byte
	db	1 dup (?)
_g_27fd	label	byte
	db	1 dup (?)
_g_27fc	label	byte
	db	1 dup (?)
	?debug	C E9
	?debug	C FA00000000
_BSS	ends
_DATA	segment word public 'DATA'
s@	label	byte
_DATA	ends
G13B2_TEXT	segment byte public 'CODE'
G13B2_TEXT	ends
	public	_g_19e6
	public	_g_2c56
	public	_g_2c33
	public	_g_dispatch
	public	_g_34a1
	public	_g_2bfd
	public	_g_2bfb
	public	_g_2c2f
	public	_g_2c60
	public	_g_18fa
	public	_g_18f8
	public	_g_1690
	public	_g_2807
	public	_g_2804
	public	_g_2805
	public	_g_2803
	public	_g_27fb
	public	_g_27ef
	public	_g_27e7
	public	_g_27e6
	public	_g_27fe
	public	_g_27f7
	public	_g_27f4
	public	_g_27f1
	public	_g_27f0
	public	_g_27ee
	public	_g_2bf0
	public	_g_27ae
	public	_g_2bff
	public	_g_2c01
	public	_g_168c
	public	_g_3858
	public	_g_34a9
	public	_g_27aa
	public	_g_349f
	public	_g_349e
	public	_g_34ad
	public	_g_19e4
	public	_g_2bf5
	public	_g_27fa
	public	_g_27f9
	public	_g_27ed
	public	_g_27ec
	public	_g_2c61
	public	_g_27f6
	public	_g_27f2
	public	_g_2800
	public	_g_27f3
	public	_g_27ff
	public	_g_27fd
	public	_g_27fc
_s@	equ	s@
	end
