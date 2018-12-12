/*
 * This file was added by Shanay.
 * It initializes system calls
 */
 
 
#include "system_calls.h"
 

 
 
//control_block * curr_pcb=NULL;
uint8_t h_ret;
 
 /*   
 *   halt
 *   DESCRIPTION: halts specified file
 *   INPUTS: status given from execute (0 to 255)
 *   OUTPUTS: none
 *   RETURN VALUE: returns 256 if it has come from an exception
 					return status otherwise
 *   SIDE EFFECTS: restore current pcb to its parent pcb and decrement total running pcbs
 */

int32_t halt (uint8_t status){

	cli();
	//halt_ret_status = status;
	//printf("halt    ");
	uint8_t shell[6] = "shell";

	if(all_running_pcbs[cur_process_id].parent_process_id < 0){
		//TODO: what should happen when shell is halted, implement that
		execute(shell);
	}


//	occupied_pcbs = occupied_pcbs & ~(1<<cur_process_id);
	int esp = all_running_pcbs[cur_process_id].stack_pointer;
	int ebp = all_running_pcbs[cur_process_id].base_pointer;
	occupied_pcbs[cur_process_id] = 0;
	//all_running_pcbs[cur_process_id] = 0;
	//TODO: vacate this pcbs position in the all running pcbs array by using occupied array
	total_running_pcbs--;
	cur_process_id = all_running_pcbs[cur_process_id].parent_process_id;

	page_for_process(cur_process_id);

	all_running_pcbs[cur_process_id].is_scheduled=1;
	tss.esp0 = all_running_pcbs[cur_process_id].tss_esp0;
	h_ret = status;
	if(get_ef())
		return ERROR_FLAG;
	//restoring esp and ebp

	sti();
	asm volatile(
		"movl %0, %%ebp \n\
		 movl %1, %%esp \n\
		 jmp JMP_FROM_HALT"
		 :
		 :"r"(ebp), "r"(esp)
		 : "eax","memory","cc"
		);
/*
	asm volatile (
		"xorl %%eax,%%eax; \n\
		 movb %0,%%al; \n\
		 jmp JMP_FROM_HALT;"
		:
		:"r"(status)
		:"edi","memory","cc"
	);*/
//will never ever reach this line
	return status;

}



/*   
 *   execute
 *   DESCRIPTION: executes specified file
 *   INPUTS: file to be executed
 *   OUTPUTS: none
 *   RETURN VALUE: return -1 if fails 
 * 				   return value of 256 if program dies by an exception
 * 				   return value of 0-255 if program executes a halt system call
 * 				   in which case the value returned is that given by the program’s call to halt 
 *   SIDE EFFECTS: creates virtual address space for new process, and begins execution
 *				   and outputs to buffer 
 */
//uint8_t * pass = "ghghg\n";
/*
The execute system call attempts to load and execute a new program, handing off the processor to the new program
until it terminates. The command is a space-separated sequence of words. The first word is the file name of the
program to be executed, and the rest of the command—stripped of leading spaces—should be provided to the new
program on request via the getargs system call. The execute call returns -1 if the command cannot be executed,
for example, if the program does not exist or the filename specified is not an executable, 256 if the program dies by an
exception, or a value in the range 0 to 255 if the program executes a halt system call, in which case the value returned
is that given by the program’s call to halt.
*/

//pcb_t* curr_ptr;

int32_t execute(const uint8_t* command){
	

	disable_irq(0);
	cli();
	int i,j;

	

	/*
	The command is a space-separated sequence of words. The first word is the file name of the
program to be executed, and the rest of the command—stripped of leading spaces—should be provided to the new
program on request via the getargs system call. 
*/
	if(total_running_pcbs >= 6)
	{
		return -1;
	}
	
	if(command == NULL)
		return -1;
	set_ef(0);

	if(cur_process_id==-1){
		for(i=0; i<6; i++){
			occupied_pcbs[i] = 0;
		}
	}

	
	int leading_space_hit = 0; //1 if leading space reached in command string, 0 if not
	uint8_t file_name[MAX_NAME_LENGTH];
	uint8_t argument_name[MAX_NAME_LENGTH];
	int cmd_len = strlen((int8_t*) command);
	
	for( i = 0; i < MAX_NAME_LENGTH; i++) {
		file_name[i] = '\0';
	}
	
	for( i = 0; i < cmd_len; i++)
	{
		if (leading_space_hit) {
			break;
		}

		else if((command[i] == ' ' && leading_space_hit == 0))
		{
			leading_space_hit = 1;
			file_name[i] = '\0';
		}
		
		else {
			  if(i >= MAX_NAME_LENGTH && leading_space_hit == 0 )
			  {
			  	  return -1;
		   	  }
		   	  file_name[i] = command[i];
		}
	}
	
	cmd_len = cmd_len - strlen((int8_t*)file_name);
	
	if(leading_space_hit){
		for (j = 0; j < MAX_NAME_LENGTH; j++) {

			argument_name[j] = command[i];
			i++;
			if(command[i] == '\0') break;
			if(j >= cmd_len) break;
		}
		argument_name[j+1] = '\0';
	
	}
	
/*
	const uint8_t file_name[MAX_NAME_LENGTH];
	strcpy(file_name, command);
	*/

	//check if file has magic numbers that designate it an exectuable
	//the magic numbers are 0: 0x7f; 1: 0x45; 2: 0x4c; 3: 0x46.
	//0x7f = DELETE, 0X45 = E, 0X4C = L, 0X46 = F.

	dentry_t file_dentry;
	//int length=strlen((const char*)file_name);
	if(read_dentry_by_name(file_name, &file_dentry) == -1) {
		return -1;
	}
	//printf("%d",file_dentry._type);
	if(file_dentry._type != 2)
		return -1;

	

	uint8_t* buffer = (void*) ADDRESS;
	if(   read_data(file_dentry._inode_number, 0, buffer, 4)   == -1)
		return -1;

	//If the magic number is not present, the execute system call should fail
	if(buffer[0] != ELF1 || buffer[1]!= ELF2 || buffer[2] != ELF3 || buffer[3] != ELF4) {
		return -1;
	}

	/*
	The other important bit of information that you need to execute programs is the entry point into the
program, i.e., the virtual address of the first instruction that should be executed. This information is stored as a 4-byte
unsigned integer in bytes 24-27 of the executable, and the value of it falls somewhere near 0x08048000 for all programs
we have provided to you. When processing the execute system call, your code should make a note of the entry
point, and then copy the entire file to memory starting at virtual address 0x08048000. It then must jump to the entry
point of the program to begin execution. The details of how to jump to this entry point are explained in the next section
	*/
	//finding entry point into file
	if(    read_data(file_dentry._inode_number, 24, buffer, 4) == -1    )
	{
		return -1;
	} //buffer now holds four bytes pointing to file entry location
	//one byte is 8 bits, use bitwise OR not logical OR
	uint32_t file_entry_point = buffer[0] | (buffer[1] << 8) | (buffer[2] << 16) | (buffer [3] << 24);
	//uint32_t file_entry_point = 0X0804800;
	//uint32_t * file_ptr_tmp= VIR_ADDR;
	//uint32_t * tits= ((VIR_ADDR + ((cur_process_id +1) *KERNEL_SIZE)));
	//uint32_t eip = tits[6];

	//COPY ENTIRE FILE INTO MEMORY STARTING AT VIRTUAL ADDRESS 0X0804800
	//			  THEN JUMP TO ENTRY POINT OF PROGRAM TO BEGIN EXECUTION
	
	int parent_id_temp;
	for(i=0; i<MAX_PCBS; i++){
		if(all_running_pcbs[i].is_scheduled){
			if(all_running_pcbs[i].term_id==get_current_viewed_terminal()){
				parent_id_temp = i;
				break;
			}
		}
	}
	
	
	for(i=0; i<MAX_PCBS; i++){
		if(occupied_pcbs[i]==0)
		{
			cur_process_id=i;
			break;
		}
	}


	page_for_process(cur_process_id);
	
	int retval = load_exec(file_name, (VIR_ADDR));
	retval++;

	total_running_pcbs++;


//creating new pcb for this process

	pcb_t new_pcb;
	
	if(no_parent){
		new_pcb.parent_process_id = -1;
		no_parent = 0;
	}
	else{
		new_pcb.parent_process_id = parent_id_temp;
	}
	
	//cur_process_id++;
	new_pcb.process_id = cur_process_id;

	// 3 is the location where the executable is loaded
	new_pcb.total_count = 3;
	new_pcb.file_list[2] = (void*) &file_dentry;



	file_d_t new_fd;
	file_d_t stdin;
	file_d_t stdout;

//setting the jump table to correct values
	new_fd.jump_table.read   = &do_nothing;
	new_fd.jump_table.write  = &fs_write;
	new_fd.jump_table.open   = &fs_open;
	new_fd.jump_table.close  = &fs_close;




	new_fd.inode = file_dentry._inode_number;
	new_fd.file_pos = 0;



	stdout.jump_table.write = &term_write_wrapper;
	stdout.jump_table.open = &do_nothing;
	stdout.jump_table.read = &do_nothing;
	stdout.jump_table.close = &do_nothing;
	stdout.inode = -1;
	stdout.file_pos = -1;

	stdin.jump_table.read = &term_read_wrapper;
	stdin.jump_table.write = &do_nothing;
	stdin.jump_table.open = &do_nothing;
	stdin.jump_table.close = &do_nothing;
	stdin.inode = -1;
	stdin.file_pos = -1;


	new_pcb.fd_list[2] = new_fd;
	new_pcb.fd_list[0] = stdin;
	new_pcb.fd_list[1] = stdout;
	new_pcb.occupied = 0x07; //3 LSB are occupied because STDIN, STDOUT and the executable need to be loaded
	new_pcb.page_pointer =  VIR_USER_MEMORY + _4MB_ - 4;

	//0x083ffffC  ; //VIR_ADDR + _4MB_ -4;

	//TODO double check that STACK_SIZE is correct val of 0x2000 or 8Kb
	//new_pcb.stack_pointer = PHYSICAL_USER_MEMORY - STACK_SIZE * (cur_process_id+1);
	tss.esp0 = PHYSICAL_USER_MEMORY - STACK_SIZE * cur_process_id - 4;
	new_pcb.term_id = get_current_viewed_terminal();
	new_pcb.tss_esp0 = tss.esp0;
	//new_pcb.tss_ss0 = tss.ss0;
	new_pcb.is_scheduled = 1;

	//curr_ptr = (void*) PHYSICAL_USER_MEMORY - STACK_SIZE * (cur_process_id+1);

	new_pcb.command_name = (void*) file_name;

	if(leading_space_hit){
		new_pcb.arg_name = argument_name;
	}
	else{
		new_pcb.arg_name = '\0';
	}

	all_running_pcbs[cur_process_id] = new_pcb;

	if(all_running_pcbs[cur_process_id].parent_process_id > -1)
	{
		all_running_pcbs[all_running_pcbs[cur_process_id].parent_process_id].is_scheduled=0;
	}
	else{
		total_scheduled_pcbs++;
	}
	//saving esp and ebp
	occupied_pcbs[cur_process_id] = 1;



	asm volatile(
		"movl %%esp , %0 \n\
		 movl %%ebp, %1;"
		 :"=r"(all_running_pcbs[cur_process_id].stack_pointer), "=r"(all_running_pcbs[cur_process_id].base_pointer)
		 :
		 : "eax","memory","cc"
	);

	sti();
	enable_irq(0);

	//push user_ds
	//push ESP (new_pcb.page_pointer)
	//push flags
	//push user_cs
	//push eip (eip is file entry point)
	asm volatile(
			"movw $0x2B, %%bx \n\
			movw %%bx, %%ds  \n\
			pushl %0 \n\
			pushl %1  \n\
			pushfl    \n\
			pushl %2  \n\
			pushl %3 \n\
			iret	\n\
			JMP_FROM_HALT:"
			:
			:"r"(USER_DS),"r"(new_pcb.page_pointer),"r"(USER_CS),"r"(file_entry_point)
			: "eax"
		);

	//returen value of 256 if program dies by an exception
	//return 256;

	//return value of 0-255 if program executes a halt system call
	//in which case the value returned is that given by the program’s call to halt.
	return h_ret;
}

/*   
 *   read
 *   DESCRIPTION: reads specified file
 *   INPUTS: file to be executed
 *   OUTPUTS: none
 *   RETURN VALUE: return -1 if fails 
 * 				   return value of 256 if program dies by an exception
 * 				   return value of 0-255 if program executes a halt system call
 * 				   in which case the value returned is that given by the program’s call to halt 
 *   SIDE EFFECTS: creates virtual address space for new process, and begins execution
 *				   and outputs to buffer 
 */

int32_t read (int32_t fd, void* buf, int32_t nbytes){

	//disable_irq(0);

	if (fd > FD_UPPER_LIMIT || fd < 0) return -1;
	if ((all_running_pcbs[cur_process_id].occupied & (1 << fd)) == 0) return -1;

	//enable_irq(0);
	
	return all_running_pcbs[cur_process_id].fd_list[fd].jump_table.read(fd, buf, nbytes);

}

/*   
 *   write
 *   DESCRIPTION: writes specified file
 *   INPUTS: file to be executed
 *   OUTPUTS: none
 *   RETURN VALUE: return -1 if fails 
 * 				   return value of 256 if program dies by an exception
 * 				   return value of 0-255 if program executes a halt system call
 * 				   in which case the value returned is that given by the program’s call to halt 
 *   SIDE EFFECTS: creates virtual address space for new process, and begins execution
 *				   and outputs to buffer 
 */

int32_t write (int32_t fd, const void* buf, int32_t nbytes){

//printf("writeh");
	//disable_irq(0);

	if (fd > FD_UPPER_LIMIT || fd < 0) return -1;
	if ((all_running_pcbs[cur_process_id].occupied & (1 << fd)) == 0) return -1;
/*	if(all_running_pcbs[cur_process_id].total_count <= fd)
		return -1;*/

	//enable_irq(0);

	return all_running_pcbs[cur_process_id].fd_list[fd].jump_table.write(fd, buf, nbytes);

}
 
/*   
 *   open
 *   DESCRIPTION: opens specified file
 *   INPUTS: file to be executed
 *   OUTPUTS: none
 *   RETURN VALUE: return -1 if fails 
 * 				   return value of 256 if program dies by an exception
 * 				   return value of 0-255 if program executes a halt system call
 * 				   in which case the value returned is that given by the program’s call to halt 
 *   SIDE EFFECTS: creates virtual address space for new process, and begins execution
 *				   and outputs to buffer 
 */
 
int32_t open (const uint8_t* filename){

//printf("open    ");
//this 8 is the maximum number of proicessess
	if(all_running_pcbs[cur_process_id].total_count > 8) return -1;
	int i;
	uint8_t checker;
	int current_position = 0;
	uint8_t file_name[MAX_NAME_LENGTH];
	strcpy((int8_t*)file_name, (int8_t*)filename);
	dentry_t * temp = (void*) ADDRESS;
	int result = read_dentry_by_name(file_name,temp);
	if (result == -1) return -1;
	file_d_t new_file;
	switch(temp->_type){
		case 0:
			new_file.jump_table.read   = &read_rtc;
			new_file.jump_table.write  = &write_rtc;
			new_file.jump_table.open   = &open_rtc;
			new_file.jump_table.close  = &close_rtc;
			new_file.file_pos = 0;
			break;
		case 1:
			new_file.jump_table.read   = &read_dir;
			new_file.jump_table.write  = &fs_write;
			new_file.jump_table.open   = &fs_open;
			new_file.jump_table.close  = &fs_close;
			new_file.file_pos = 1;
			break;

		case 2:
			new_file.jump_table.read   = &read_file;
			new_file.jump_table.write  = &fs_write;
			new_file.jump_table.open   = &fs_open;
			new_file.jump_table.close  = &fs_close;
			new_file.inode = temp->_inode_number;
			new_file.file_pos = 0;
			break;
	}

	new_file.inode = temp->_inode_number;
	checker = all_running_pcbs[cur_process_id].occupied;
	checker = checker >> 2;
	for(i=2; i<MAX_FILES_; i++){
		current_position = checker%2;
		if(current_position == 0) break;
		else{
			checker = checker >> 1;
		}
	}
	all_running_pcbs[cur_process_id].file_list[i] = (void* )temp;
	all_running_pcbs[cur_process_id].fd_list[i] = new_file;
	all_running_pcbs[cur_process_id].occupied = all_running_pcbs[cur_process_id].occupied | (1<<i);
	all_running_pcbs[cur_process_id].total_count++;

	
	return i;
}

/*   
 *   close
 *   DESCRIPTION: closes specified file
 *   INPUTS: file to be executed
 *   OUTPUTS: none
 *   RETURN VALUE: return -1 if fails 
 * 				   return value of 256 if program dies by an exception
 * 				   return value of 0-255 if program executes a halt system call
 * 				   in which case the value returned is that given by the program’s call to halt 
 *   SIDE EFFECTS: creates virtual address space for new process, and begins execution
 *				   and outputs to buffer 
 */

int32_t close (int32_t fd){

	//printf("close   ");
	if(all_running_pcbs[cur_process_id].total_count<=3)
		return -1;
	
	if (fd > FD_UPPER_LIMIT || fd < 2) return -1;
	if ((all_running_pcbs[cur_process_id].occupied & (1 << fd)) == 0) return -1;
	else {
		all_running_pcbs[cur_process_id].occupied = all_running_pcbs[cur_process_id].occupied & ~(1<<fd);
		all_running_pcbs[cur_process_id].total_count--;
		all_running_pcbs[cur_process_id].fd_list[fd].jump_table.read = NULL;
		all_running_pcbs[cur_process_id].fd_list[fd].jump_table.write = NULL;
		all_running_pcbs[cur_process_id].fd_list[fd].jump_table.open = NULL;
		all_running_pcbs[cur_process_id].fd_list[fd].jump_table.close = NULL;
		all_running_pcbs[cur_process_id].fd_list[fd].inode = 0;
		all_running_pcbs[cur_process_id].fd_list[fd].file_pos = 0;
		all_running_pcbs[cur_process_id].fd_list[fd].reserved = 0;
	}
	return 0;
}

/*   
 *   getargs
 *   DESCRIPTION: gets the args of specified file
 *   INPUTS: file to be executed
 *   OUTPUTS: none
 *   RETURN VALUE: return -1 if fails 
 * 				   return value of 256 if program dies by an exception
 * 				   return value of 0-255 if program executes a halt system call
 * 				   in which case the value returned is that given by the program’s call to halt 
 *   SIDE EFFECTS: creates virtual address space for new process, and begins execution
 *				   and outputs to buffer 
 */

int32_t getargs (uint8_t* buf, int32_t nbytes){
	if ((nbytes <= 0)) return -1;
	if(buf == 0) return -1;
	if(all_running_pcbs[cur_process_id].arg_name == 0) return -1;
	if(all_running_pcbs[cur_process_id].arg_name[0] == '\0') return -1;
	buf[0] = ' ';
	//strcpy(buf, pass);
	strcpy((int8_t*)buf,(int8_t*)all_running_pcbs[cur_process_id].arg_name);

	return 0;
}

/*   
 *   vidmap
 *   DESCRIPTION: executes specified file
 *   INPUTS: file to be executed
 *   OUTPUTS: none
 *   RETURN VALUE: return -1 if fails 
 * 				   return value of 256 if program dies by an exception
 * 				   return value of 0-255 if program executes a halt system call
 * 				   in which case the value returned is that given by the program’s call to halt 
 *   SIDE EFFECTS: creates virtual address space for new process, and begins execution
 *				   and outputs to buffer 
 */

int32_t vidmap (uint8_t** screen_start){
	//validity check
	disable_irq(0);
	if( (uint32_t) screen_start== NULL || (uint32_t) screen_start == _4MB_  )
		return -1;// failure 
	
	//page_video_mem( USER_VID_VIR);
	/*int t_id= get_term_id();
	int i=0;
	page_video_mem( USER_VID_VIR - t_id *_4MB_ );
	for(i=0 ; i<3 ; i++)
	{
		if(i!=t_id)
			unpage_vid(USER_VID_VIR - i *_4MB_);
	}*/
	page_video_mem( USER_VID_VIR);
	*screen_start= (uint8_t *)USER_VID_VIR;
	

	
	/*
	switch(t_id){
		case 0:
			page_video_mem(USER_VID_VIR);
			unpage_vid(USER_VID_VIR1);
			unpage_vid(USER_VID_VIR2);
			*screen_start= (uint8_t *)USER_VID_VIR;
			return USER_VID_VIR;
			
		case 1:
			page_video_mem(USER_VID_VIR1);
			unpage_vid(USER_VID_VIR);
			unpage_vid(USER_VID_VIR2);
			*screen_start= (uint8_t *)USER_VID_VIR1;
			return USER_VID_VIR1;
			
		case 2:
			page_video_mem(USER_VID_VIR2);
			unpage_vid(USER_VID_VIR);
			unpage_vid(USER_VID_VIR1);
			*screen_start= (uint8_t *)USER_VID_VIR2;
			return USER_VID_VIR2;	
		
	}
	*/
	enable_irq(0);
	return USER_VID_VIR;
}

int32_t set_handler (int32_t signum, void* handler_address){
	return -1;
}

int32_t sigreturn (void){
	return -1;
}
 
void set_curr_pid(int new_pid)
{
	cur_process_id = new_pid;

}

int get_curr_pid(){
	return cur_process_id;
}



/*
*	syscall wrapper to read file
* 	auth:lall3
*/
int read_file(int fd, void* buf, int nbytes)
{
	if(fd<=RERSVED_IO || fd>= MAX_FILES_)
	{
		return -1;
	}
	uint32_t curr_inode=  all_running_pcbs[cur_process_id].fd_list[fd].inode;
	uint32_t offset= all_running_pcbs[cur_process_id].fd_list[fd].file_pos;
	
	//uint8_t* printable= (uint8_t *) ADDRESS ;
	int retval= read_data(curr_inode, offset ,(uint8_t *) buf , nbytes );
	all_running_pcbs[cur_process_id].fd_list[fd].file_pos += retval;
	
	
	
	return retval;
}


/*
//just for testing
uint8_t * get_parent_command(){
	original_cmd[0] = 's';
	original_cmd[1] = 'h';
	original_cmd[2] = 'e';
	original_cmd[3] = 'l';
	original_cmd[4] = 'l';
	//original_cmd[0] = '';
	return original_cmd;
}
*/

//--------------------------------

void set_total_running_pcbs(int pcbs){
	total_running_pcbs = pcbs;
}

int get_total_pcbs(){
	return total_running_pcbs;
}


//put in function in syscalls.c
int do_nothing()
{
	return -1;
}


void set_total_scheduled_pcbs(int new_sch_pcbs){
	total_scheduled_pcbs = new_sch_pcbs;
}


int get_total_scheduled_pcbs(){
	return total_scheduled_pcbs;
}


void set_no_parent(int flag){
	no_parent = flag;
}

int get_no_parent(){
	return no_parent;
}



