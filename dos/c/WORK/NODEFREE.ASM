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
	?debug	S "nodefree.c"
	?debug	C E96A86455D0A6E6F6465667265652E63
NODEFREE_TEXT	segment byte public 'CODE'
NODEFREE_TEXT	ends
DGROUP	group	_DATA,_BSS
	assume	cs:NODEFREE_TEXT,ds:DGROUP
_DATA	segment word public 'DATA'
d@	label	byte
d@w	label	word
_DATA	ends
_BSS	segment word public 'BSS'
b@	label	byte
b@w	label	word
_BSS	ends
NODEFREE_TEXT	segment byte public 'CODE'
   ;	
   ;	void node_free(struct nodelist far *p)
   ;	
	assume	cs:NODEFREE_TEXT
_node_free	proc	far
	push	bp
	mov	bp,sp
	sub	sp,4
   ;	
   ;	{
   ;	    int i;
   ;	
   ;	    for (i = 0; i < p->count; i++) {
   ;	
	mov	word ptr [bp-2],0
	mov	ax,word ptr [bp+6]
	mov	word ptr [bp-4],ax
	jmp	short @1@170
@1@86:
   ;	
   ;	        m_d19_dc(p->ent[i], 3);
   ;	
	push	3
	mov	es,word ptr [bp+8]
	mov	bx,word ptr [bp-4]
	push	word ptr es:[bx+4]
	push	word ptr es:[bx+2]
	call	far ptr _m_d19_dc
	add	sp,6
   ;	
   ;	        p->ent[i] = 0;
   ;	
	mov	es,word ptr [bp+8]
	mov	bx,word ptr [bp-4]
	mov	word ptr es:[bx+4],0
	mov	word ptr es:[bx+2],0
	add	word ptr [bp-4],4
	inc	word ptr [bp-2]
@1@170:
	les	bx,dword ptr [bp+6]
	mov	al,byte ptr es:[bx]
	mov	ah,0
	cmp	ax,word ptr [bp-2]
	jg	short @1@86
   ;	
   ;	    }
   ;	    p->count = 0;
   ;	
	mov	byte ptr es:[bx],0
   ;	
   ;	}
   ;	
	leave	
	ret	
_node_free	endp
	?debug	C E9
	?debug	C FA04000000
NODEFREE_TEXT	ends
_DATA	segment word public 'DATA'
s@	label	byte
_DATA	ends
NODEFREE_TEXT	segment byte public 'CODE'
NODEFREE_TEXT	ends
	public	_node_free
	extrn	_m_d19_dc:far
_s@	equ	s@
	end
