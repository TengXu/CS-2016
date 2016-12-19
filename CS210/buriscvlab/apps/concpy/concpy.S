#include "../../buriscv_defines.h"
        .section .data

	.section .text
	.globl _start
	
_start:
    	li	a1,BURISCV_PAGE_LEN
	la      a5,_ram_end
	add	a1,a1,-1
	li	a4,-BURISCV_PAGE_LEN
	add	a1,a5,a1
	and	a1,a1,a4
                                /* char a3          */
loop:	
	lbu a3,0(a1)            /* a3 = *src        */
	sb  a3,0(a1)            /* *dst = a3        */

	bnez a3,loop            /* if (a3!=0) goto loop */

	sbreak
	
 
