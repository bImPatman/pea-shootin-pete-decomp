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
	?debug	S "statebak.c"
	?debug	C E99583445D0A737461746562616B2E63
STATEBAK_TEXT	segment byte public 'CODE'
STATEBAK_TEXT	ends
DGROUP	group	_DATA,_BSS
	assume	cs:STATEBAK_TEXT,ds:DGROUP
_DATA	segment word public 'DATA'
d@	label	byte
d@w	label	word
_DATA	ends
_BSS	segment word public 'BSS'
b@	label	byte
b@w	label	word
_BSS	ends
STATEBAK_TEXT	segment byte public 'CODE'
   ;	
   ;	void statebak(struct blk_s far *blk)
   ;	
	assume	cs:STATEBAK_TEXT
_statebak	proc	far
	push	bp
	mov	bp,sp
	sub	sp,4
   ;	
   ;	{
   ;	    int i;
   ;	    if (blk->state == 1) {
   ;	
	les	bx,dword ptr [bp+6]
	cmp	byte ptr es:[bx+2],1
	jne	short @1@254
   ;	
   ;	        for (i = 0; i < 0x300; i++)
   ;	
	mov	word ptr [bp-2],0
	mov	ax,word ptr [bp+6]
	add	ax,6
	mov	word ptr [bp-4],ax
@1@114:
   ;	
   ;	            tbl[i] = blk->data[i];
   ;	
	mov	es,word ptr [bp+8]
	mov	bx,word ptr [bp-4]
	mov	al,byte ptr es:[bx]
	mov	bx,word ptr [bp-2]
	mov	byte ptr DGROUP:_tbl[bx],al
	inc	word ptr [bp-4]
	inc	word ptr [bp-2]
	cmp	word ptr [bp-2],768
	jl	short @1@114
   ;	
   ;	        blk->state = 0;
   ;	
	les	bx,dword ptr [bp+6]
	mov	byte ptr es:[bx+2],0
@1@254:
   ;	
   ;	    }
   ;	}
   ;	
	leave	
	ret	
_statebak	endp
STATEBAK_TEXT	ends
_BSS	segment word public 'BSS'
_tbl	label	byte
	db	768 dup (?)
	?debug	C E9
	?debug	C FA04000000
_BSS	ends
_DATA	segment word public 'DATA'
s@	label	byte
_DATA	ends
STATEBAK_TEXT	segment byte public 'CODE'
STATEBAK_TEXT	ends
	public	_statebak
	public	_tbl
_s@	equ	s@
	end
