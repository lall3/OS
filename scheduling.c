#include "scheduling.h"



void pit_init() {

	uint32_t freq = MAX_FREQ_PIT/DIV_25HZ;
	outb(MODE_1, MODE_REG);
	outb(freq & MASK, PIT_PORT_1);
	outb(freq >> 8, PIT_PORT_1);
	enable_irq(PIT_NUM);
	return;
}

void pit_handler() {
	//switching of processes
	send_eoi(PIT_NUM);
	disable_irq(PIT_NUM);
	cli();



	if(get_total_scheduled_pcbs()==0)
	{
		sti();
		enable_irq(PIT_NUM);
		asm volatile("leave;"
					"iret"
		);
	}
	
	//cli();
	

	int pid_= get_curr_pid();
	//asm volatile("pushal;");
	
	//printf("i reached pit handler\n");
	//getting processID

	//get total running pcb and check for edge case of 0/1
	if(get_total_scheduled_pcbs()>=2)
	{

		int nextProcess= next_process();
		
		//context switch
		if(nextProcess!=get_curr_pid())
		{
			
			//paging
			page_for_process(nextProcess);
			pcb_t* next_process = &all_running_pcbs[nextProcess];
			tss.esp0= next_process->tss_esp0;
			//prinint here
			//printf("i reached pit handler\n");
			//printf("esp0:%d", next_process->tss_esp0);
			int temp_esp, temp_ebp;
			asm volatile("\n\
			movl %%esp, %0; \n\
			movl %%ebp, %1; "
			: "=r"(temp_esp), "=r"(temp_ebp)
			:
			: "eax", "memory", "cc"
			);	

	//int pid_= get_curr_pid();

	all_running_pcbs[pid_].base_pointer= temp_ebp;
	all_running_pcbs[pid_].stack_pointer= temp_esp;
			// we need to load these value into the pcb of this process
			// then get te next processes ebp, esp
			// for now I am asuming that temp_esp/ebp have the new values.
			
			set_curr_pid(next_process->process_id);
			set_term_id(all_running_pcbs[next_process->process_id].term_id);

			asm volatile(" \n\
				movl %0, %%esp; \n\
				movl %1, %%ebp; "
				: 
				: "r"(next_process->stack_pointer), "r"(next_process->base_pointer)
				: "eax", "memory", "cc"
				); 	
		}
		//sti();
		//return;
	}
	/*else{
		
		int temp_esp, temp_ebp;
			asm volatile("\n\
			movl %%esp, %0; \n\
			movl %%ebp, %1; "
			: "=r"(temp_esp), "=r"(temp_ebp)
			:
			: "eax", "memory", "cc"
			);	

		//int pid_= get_curr_pid();

		all_running_pcbs[pid_].base_pointer= temp_ebp;
		all_running_pcbs[pid_].stack_pointer= temp_esp;
	}*/
	sti();
	enable_irq(PIT_NUM);
	//asm volatile("popal;");
	asm volatile("leave;"
				"iret;");
		
	return;
}


void PIT_switch_process(pcb_t* next_process)
{
/*
	tss.esp0= next_process->tss_esp0;
	int temp_esp, temp_ebp;
	asm volatile("\n\
		movl %%esp, %0; \n\
		movl %%ebp, %1; "
		: "=r"(temp_esp), "=r"(temp_ebp)
		:
		: "eax", "memory", "cc"
	);

	int pid_= get_curr_pid();

	all_running_pcbs[pid_].base_pointer= temp_ebp;
	all_running_pcbs[pid_].stack_pointer= temp_esp;
	// we need to load these value into the pcb of this process
	// then get te next processes ebp, esp
	// for now I am asuming that temp_esp/ebp have the new values.
	
	set_curr_pid(next_process->process_id);
	set_term_id(all_running_pcbs[next_process->process_id].term_id);

	asm volatile(" \n\
		movl %0, %%esp; \n\
		movl %1, %%ebp; "
		: 
		: "r"(next_process->stack_pointer), "r"(next_process->base_pointer)
		: "eax", "memory", "cc"
		); 
	*/
}


//use this to find next process we need to run
//not exactly sure how we are doing this
int next_process()
{
	int i = 0;
	int cur_id=get_curr_pid();
	for(i=cur_id+1 ; i<MAX_PCBS ; i++)
	{
		if(cur_id != i && occupied_pcbs[i]==1)
		{
			if(all_running_pcbs[i].is_scheduled==1)
			{
				cur_id=i;
				break;
			}
		}
	}
	if(cur_id!=get_curr_pid())
		return cur_id;

	for(i=0 ; i<cur_id ; i++)
	{
		if(cur_id != i && occupied_pcbs[i]==1)
		{
			if(all_running_pcbs[i].is_scheduled==1)
			{
				cur_id=i;
				break;
			}
		}
	}

	return cur_id;
}

