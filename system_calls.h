/*
 * This file was added by Shanay.
 * It is the header file to system_calls.c
 * 
 */

#ifndef _SYSTEM_CALLS_H
#define _SYSTEM_CALLS_H
 
#include "types.h"
#include "lib.h"
#include "files.h"
#include "rtc.h"
#include "keyboard.h"
#include "paging.h"
#include "exception_handler.h"
#include "i8259.h"

//7f = DELETE, 0X45 = E, 0X4C = L, 0X46 = F.
#define ELF1 0x7F
#define ELF2 0x45
#define ELF3 0x4C
#define ELF4 0x46
#define MAX_PROCESSES 16
#define VIR_ADDR 0x08048000
#define KERNEL_STACK_ADDR 0x800000
#define ADDRESS 0x600001
#define RERSVED_IO 	0x2
#define MAX_FILES_	0x8
#define FD_UPPER_LIMIT 7
#define ERROR_FLAG	256
#define USER_VID_VIR	0x7000000
#define USER_VID_VIR1	0x7100000
#define USER_VID_VIR2	0x7200000
#define MAX_PCBS	6

int read_file(int fd, void* buf, int nbytes);






void set_curr_pid(int new_pid);
int get_curr_pid();

//static int32_t exception_flag = 0;
//static int32_t num_processes = 0;
int32_t cur_process_id;

//int32_t halt_ret_status;

int32_t halt(uint8_t status);

int32_t execute(const uint8_t* command);

int32_t read(int32_t fd, void * buf, int32_t nbytes);

int32_t write(int32_t fd, const void* buf, int32_t nbytes);

int32_t open(const uint8_t* filename);

int32_t close(int32_t fd);

int32_t getargs (uint8_t* buf, int32_t nbytes);

int32_t vidmap(uint8_t** screen_start);

int32_t set_handler(int32_t signum, void* handler_address);

int32_t sigreturn();

uint8_t * get_parent_command();

int do_nothing();







typedef struct{
	int (*open)(const uint8_t* fname);
	int (*read)(int32_t fd, void* buf, int32_t nbytes);
	int (*write)(int32_t fd, const void* buf, int32_t nbytes);
	int (*close)(int32_t fd);
} file_jump_table_t;


//this is the struct for a file descriptor and it is used in 
typedef struct{
	file_jump_table_t jump_table;
	uint32_t inode;
	uint32_t file_pos;
	uint32_t reserved;
	
}file_d_t;




//Since PCB contains the critical information for the process,
//it must be kept in an area of memory protected from normal user access. 
//In some operating systems the PCB is placed in the beginning of the kernel
//stack of the process since that is a convenient protected location
//todo: do we need to do this^???? from wikipedia
typedef struct  {
//program id or index
	int process_id;

//files that it contains like an array of file descriptors of max 8
	file_d_t fd_list[8];
	
	struct dentry_t * file_list[8];
	

//global count of max 8
	int total_count;

//one byte of bits that will keep track of which count is opened or closed
	//if 1 it is occupied, 0 if free
	uint8_t occupied;

//parent index with highest being -1. if parents is negative one, you are the first process.
	int parent_process_id;
	
	
	//this is the pointer to the address of this process's page
	uint32_t page_pointer;
	
	//esp0
	uint32_t stack_pointer;

	//ebp0
	uint32_t base_pointer;

	
	uint8_t* command_name;
	
	uint8_t * arg_name;

	/*ADDED ON 2nd DEC BY MITRI FOR CP5, Not sure if this is right*/
	uint32_t tss_esp0;

	uint32_t term_id;

	uint32_t is_scheduled;

	uint32_t tss_ss0;

} pcb_t;


pcb_t all_running_pcbs[6];
//might need for scheduling
uint32_t occupied_pcbs[6];
int total_running_pcbs;
int total_scheduled_pcbs;
int no_parent;


void set_total_running_pcbs(int pcbs);
int get_total_pcbs();

void set_total_scheduled_pcbs(int new_sch_pcbs);
int get_total_scheduled_pcbs();

void set_no_parent(int flag);
int get_no_parent();


#endif

