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
	?debug	S "probe.c"
	?debug	C E95D65445D0770726F62652E63
	?debug	C E94019CA1812443A5C494E434C5544455C737464696F2E68
	?debug	C E94019CA1812443A5C494E434C5544455C5F646566732E68
	?debug	C E94019CA1813443A5C494E434C5544455C5F6E66696C652E68
	?debug	C E94019CA1812443A5C494E434C5544455C5F6E756C6C2E68
PROBE_TEXT	segment byte public 'CODE'
PROBE_TEXT	ends
DGROUP	group	_DATA,_BSS
	assume	cs:PROBE_TEXT,ds:DGROUP
_DATA	segment word public 'DATA'
d@	label	byte
d@w	label	word
_DATA	ends
_BSS	segment word public 'BSS'
b@	label	byte
b@w	label	word
_BSS	ends
PROBE_TEXT	segment byte public 'CODE'
   ;	
   ;	int main(void) { printf("probe\n"); return 0; }
   ;	
	assume	cs:PROBE_TEXT
_main	proc	far
	push	ds
	push	offset DGROUP:s@
	call	far ptr _printf
	add	sp,4
	xor	ax,ax
	ret	
_main	endp
	?debug	C E9
	?debug	C FA00000000
PROBE_TEXT	ends
_DATA	segment word public 'DATA'
s@	label	byte
	db	'probe'
	db	10
	db	0
_DATA	ends
PROBE_TEXT	segment byte public 'CODE'
PROBE_TEXT	ends
	public	_main
	extrn	_printf:far
_s@	equ	s@
	end
