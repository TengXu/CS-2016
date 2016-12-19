        .section .data
src:
	.string "Jump and Link worked!\n" /* src[80] = "Jump and Link worked!\n" */ 
	.zero 57 		   /* zero remain portion of 80 bytes */

dst:
	.zero 80                   /* dst[80] zero entire contents */ 


	.section .text
	.globl _start

_start:
	jal memcpy
	
	la  a1,dst
	li  a3,0x4c
	sb  a3,0(a1)
	
	sbreak
	
        .zero 16                /* put some zero filled memory to ensure we really jumped */
	
memcpy:	
	la  a5,src              /* char *a5 = &src */
	la  a1,dst              /* char *a1 = &dest */
                                /* char a3          */
loop:	
	lbu a3,0(a5)            /* a3 = *src        */
	sb  a3,0(a1)            /* *dst = a3        */

	add a5,a5,1             /* src++            */
	add a1,a1,1             /* dst++            */
	
	bnez a3,loop            /* if (a3!=0) goto loop */

	ret
	
