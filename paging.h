/*
*	Paging.h
*	header file for paging
*	@auth : Saurav Lall
*/
#ifndef _PAGING_H
#define _PAGING_H

#include "types.h"

#define PAGE_DR_LENGTH 		1024
#define PAGE_TABLE_LENGTH	1024 
//REGULAR PAGE 
#define PAGE_SIZE			0x1000
//KERNEL 4MB
#define KERNEL_SIZE			0X400000

#define PHYSICAL_USER_MEMORY	0x800000

#define STACK_SIZE 0x2000

#define UWRITE	0x87

#define VIR_USER_MEMORY			0x08000000 //128 mb

#define _4MB_			0X400000

#define NOT_VID_MEM_LOCATION	0xB6000
/* initalizes paging memory */
void init_paging();
int page_for_process(int index);
int free_paging(int index);
void page_video_mem(uint32_t pos);
void unpage_vid(uint32_t pos);

#endif /* _PAGING_H */
