        .section .data
src:
	.string "Hello World!!!\n" /* src[80] = "Hello World!!!\n" */ 
	.zero 64 		   /* zero remain portion of 80 bytes */

dst:
	.zero 80                   /* dst[80] zero entire contents */ 


	.section .text
	.globl _start
	
_start:
	la  a5,src              /* char *a5 = &src */
	la  a1,dst              /* char *a1 = &dest */
                                /* char a3          */
loop:	
	lbu a3,0(a5)            /* a3 = *src        */
	sb  a3,0(a1)            /* *dst = a3        */

	add a5,a5,1             /* src++            */
	add a1,a1,1             /* dst++            */
	
	bnez a3,loop            /* if (a3!=0) goto loop */

	sbreak
	
