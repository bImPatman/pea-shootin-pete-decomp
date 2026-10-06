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
	?debug	S "stsave.c"
	?debug	C E98C8C445D087374736176652E63
STSAVE_TEXT	segment byte public 'CODE'
STSAVE_TEXT	ends
DGROUP	group	_DATA,_BSS
	assume	cs:STSAVE_TEXT,ds:DGROUP
_DATA	segment word public 'DATA'
d@	label	byte
d@w	label	word
_DATA	ends
_BSS	segment word public 'BSS'
b@	label	byte
b@w	label	word
_BSS	ends
STSAVE_TEXT	segment byte public 'CODE'
   ;	
   ;	void stsave(struct blk_s far *blk)
   ;	
	assume	cs:STSAVE_TEXT
_stsave	proc	far
	push	bp
	mov	bp,sp
	sub	sp,4
   ;	
   ;	{
   ;	    int i;
   ;	    if (blk->state == 0) {
   ;	
	les	bx,dword ptr [bp+6]
	cmp	byte ptr es:[bx+2],0
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
   ;	            blk->data[i] = tbl[i];
   ;	
	mov	bx,word ptr [bp-2]
	mov	al,byte ptr DGROUP:_tbl[bx]
	mov	es,word ptr [bp+8]
	mov	bx,word ptr [bp-4]
	mov	byte ptr es:[bx],al
	inc	word ptr [bp-4]
	inc	word ptr [bp-2]
	cmp	word ptr [bp-2],768
	jl	short @1@114
   ;	
   ;	        blk->state = 1;
   ;	
	les	bx,dword ptr [bp+6]
	mov	byte ptr es:[bx+2],1
@1@254:
   ;	
   ;	    }
   ;	}
   ;	
	leave	
	ret	
_stsave	endp
STSAVE_TEXT	ends
_BSS	segment word public 'BSS'
_tbl	label	byte
	db	768 dup (?)
	?debug	C E9
	?debug	C FA04000000
_BSS	ends
_DATA	segment word public 'DATA'
s@	label	byte
_DATA	ends
STSAVE_TEXT	segment byte public 'CODE'
STSAVE_TEXT	ends
	public	_stsave
	public	_tbl
_s@	equ	s@
	end
