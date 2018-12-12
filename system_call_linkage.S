
/* This file was added by Shanay. 
 It initializes system calls */

#define ASM     1
 
#include "system_call_linkage.h"


.global system_call_handler
system_call_handler:
					pushl	%ebp
					movl 	%esp, %ebp
					push	%ds
					push	%ss
					push	%es
					push	%fs
					push	%gs
					pushl	%esi
					pushl	%edi
					pushl	%edx
					pushl	%ecx
					pushl	%ebx
					/* checking that eax is within 1-10 */
					cmpl	$1, %eax
					jl		wrong_call
					cmpl	$10, %eax
					jg		wrong_call
					
					call	*jump_table(, %eax, 4)
					
					done:
						popl	%ebx
						popl	%ecx
						popl	%edx
						popl	%edi
						popl	%esi
						pop		%gs
						pop		%fs
						pop		%es
						pop		%ss
						pop		%ds
						leave
						iret
					
					
					
					wrong_call:
						xorl	%eax, %eax
						subl	$1, %eax
						jmp		done

					
/* jump table for all 10 system calls */
jump_table:
	.long wrong_call, halt, execute, read, write, open, close, getargs, vidmap, set_handler, sigreturn
	

