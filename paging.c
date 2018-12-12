/*
*	Paging.c
*	header file for paging
*	@auth : Saurav Lall
*/
#include "paging.h"
#include "types.h"
//page directory
static uint32_t page_directory[PAGE_DR_LENGTH] __attribute__((aligned(PAGE_SIZE)));

//video memory, page table
static uint32_t video_memory[PAGE_DR_LENGTH] __attribute__((aligned(PAGE_SIZE)));
uint8_t _flags=0;
static uint32_t page_table_user_vid[PAGE_DR_LENGTH] __attribute__((aligned(PAGE_SIZE)));
//static uint32_t page_table_user_vid1 [PAGE_DR_LENGTH] __attribute__((aligned(PAGE_SIZE)));

//values for last 3 bits to signify if mmory is wrtable or exists
#define WRITE 			 0x2
#define WRITE_AND_EXISTS 0x3
#define USER_WRITE_AND_EXISTS 0x7
#define KWRITE			 0x83

//ensures last 12 bits of vid memory are 0 when initalized
#define MASK_12_BITS	0xffff000	


#define VID_MEM_LOCATION 0xB8000

/*
*	init_paging
*	initializes paging for kernel and video memory
*	returns 0 for success
*/
void init_paging()
{
	
	int i=0;
	for(; i<PAGE_DR_LENGTH ; i++)
	{
		page_directory[i]= WRITE;
	}
	
	for(i=0; i< PAGE_DR_LENGTH ; i++)
	video_memory[i]= i*PAGE_SIZE & MASK_12_BITS;
	
	int tmp= (int) (VID_MEM_LOCATION/PAGE_SIZE);
	// b8000 is physical location of video memory
		
		
	video_memory[tmp] |= WRITE_AND_EXISTS;
	
	
	
	page_directory[0]= ((uint32_t)video_memory | WRITE_AND_EXISTS);
	
	/*
	* now put the value of kernel memory into position 1 of page_directory
	* kernel is at 4MB (0x400000)
	*/
	
	uint32_t _KERNEL= KERNEL_SIZE;
	//_KERNEL = _KERNEL << 8;
	page_directory[1]= _KERNEL| KWRITE;	//sets the size of memory and also signifies that its writable and exists
	 
	 
	/*
	* now we use asm code to help us adjust the CR values to set up paging
	* firt we load paging.
	* the code is from osdev and gcc.gnu.org/onlinedocs/gcc/Extended-Asm.html
	*/
	
	
	//loading paging 
	// by working with cr3
	asm volatile("	\n\
		mov %0, %%eax	\n\
		mov  %%eax, %%cr3 " 
		:
		: "r" (page_directory)
		: "eax", "memory", "cc"
	);
	
	//enabling pse (adjusting cr4)
	asm volatile(" 				\n\
				mov %%cr4,  %%eax \n\
				or $0x10, %%eax		\n\
				mov  %%eax, %%cr4"
				:
				:
				: "eax","memory","cc"	
	);

	//enabling paging
	//ORing cr0 with $0x80000000
	asm volatile(" \n\
				mov  %%cr0, %%eax \n\
				or $0x80000000, %%eax		\n\
				mov %%eax , %%cr0"
				:
				:
				: "eax","memory","cc"	
	);//last
	
}



	
/*
* allocates a page of 4mb for each user program
* input: the index of the process that has been started
* returns -1 for failure and 0 for success 
*/
int page_for_process(int index)
{
	//checking valididty of index
	if(index <0 || index >15)
	{
		return -1;
	}
	
	//generating physical address 
	uint32_t page_location= KERNEL_SIZE*(index)+ PHYSICAL_USER_MEMORY;

	//setting bits that define writable memory, exists and the user privalage level
	page_directory[VIR_USER_MEMORY/_4MB_] = page_location | UWRITE;
	
	//page_directory[2] = 0x800000 | KWRITE;

	//loading paging 
	// by working with cr3
	asm volatile("	\n\
		mov %0, %%eax	\n\
		mov  %%eax, %%cr3 " 
		:
		: "r" (page_directory)
		: "eax", "memory", "cc"
	);
	
	return 0;
}

/*
*removes page occupied by user code once done terminating
*/
int free_paging(int index)
{
	if(index ==0)
		return 0;
	uint32_t page_location= KERNEL_SIZE*(index-1)+ PHYSICAL_USER_MEMORY;

	page_directory[VIR_USER_MEMORY/_4MB_ ] = page_location | UWRITE;
	asm volatile("	\n\
		mov %0, %%eax	\n\
		mov  %%eax, %%cr3 " 
		:
		: "r" (page_directory)
		: "eax", "memory", "cc"
	);
	return 0;



}

/*
* maps given virtual address to video memory 
*/
void page_video_mem(uint32_t pos)
{
	
	page_directory[pos/_4MB_]= (uint32_t )page_table_user_vid | USER_WRITE_AND_EXISTS;
	page_table_user_vid[0]= VID_MEM_LOCATION | USER_WRITE_AND_EXISTS;
	
	//flushing tlb
	asm volatile("	\n\
		mov %0, %%eax	\n\
		mov  %%eax, %%cr3 " 
		:
		: "r" (page_directory)
		: "eax", "memory", "cc"
	);
	
}


void unpage_vid(uint32_t pos)
{
	/*
	page_directory[pos/_4MB_]= (uint32_t )page_table_user_vid1 | USER_WRITE_AND_EXISTS;
	page_table_user_vid1[0]= NOT_VID_MEM_LOCATION | USER_WRITE_AND_EXISTS;
	
	//flushing tlb
	asm volatile("	\n\
		mov %0, %%eax	\n\
		mov  %%eax, %%cr3 " 
		:
		: "r" (page_directory)
		: "eax", "memory", "cc"
	);
	*/
}
