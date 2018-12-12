/*
 * files.c
 * 	  functions to interact with read-only filesystem. 
 */
#include "files.h"



 
//will hold filesystem stats 
block_numbers_t stats; 
//array of dentry_ts
dentry_t* array;

int32_t file_addr;






/*   Checkpoint 2
 *   init_file_systems
 *   DESCRIPTION: initializes all global variables to be used by 
 * 		  other file system functions below
 *   INPUTS:  start 
 *   OUTPUTS: return 0 on succsess (always succesful)
 *   RETURN VALUE: return 0 on succsess (always succesful)
 *   SIDE EFFECTS: sets all global variables required for completing
 * 		   other file system functions
 */
int32_t init_file_systems(int32_t start)
{
	file_addr= start;
	
	//initializing stats
	uint32_t* tm;
	tm=(uint32_t*)start;
	stats._entries= *tm;
	tm=(uint32_t*)(start+sizeof(int));
	stats._nodes= *tm;
	tm=(uint32_t*)(start+2*sizeof(int));
	stats._blocks=*tm;
	
	array= (dentry_t *)(start + 2*MAX_NAME_LENGTH); // size of stats is 12 and 52 are reserved, replace with sizeof
	
	nodes= (inode_t *)(start + FILE_PAGE_SIZE);// 4kb after start
	//printf(" nodes in init:%d \n",stats._nodes);

	int32_t tmp= stats._nodes;
	data_block_start_addr=start+ (tmp+1)*(FILE_PAGE_SIZE);
	
	return 0;
}




/*
 * open()
 *  does nothing, returns 0 everytime
 */
int32_t fs_open (const uint8_t* filename)
{
	return 0;
}


/*
 * close()
 *  does nothing, returns 0 everytime
 */
int32_t fs_close (int32_t fd)
{
	return 0;
}
/*
 * write()
 *  does nothing, returns -1 everytime
 */
int32_t fs_write (int32_t fd, const void* buf, int32_t nbytes)
{
	return -1;
}
/*
*reads any file that has been opened


 int32_t fs_read (int32_t fd, void* buf, int32_t nbytes)
 {

 }
*/




/*   Checkpoint 2
 *   read_dentry_by_name
 *   DESCRIPTION: fills in dentry_t block passed into funciton 
 * 		  according to the name also passed in
 * 		       
 *   INPUTS:  *fname, *dentry
 *   OUTPUTS: return 0 on success, -1 on failure
 *   RETURN VALUE:  0 on successful read by file name, -1 on failure
 *   SIDE EFFECTS: writes the corresponding index file to *dentry
 */
int32_t read_dentry_by_name (const uint8_t* fname, dentry_t* dentry)
{
	// gets length of file, might need to typecast the filename
	int length=strlen((const char*)fname);
	
	//check if length is valid
	if(length<1 || length> MAX_NAME_LENGTH)
	{
		return -1; // failure
	}
	int i=0;
	//int ret;
	int tlen= strlen((const char*)fname);
//	dentry_t cmp;// for comparing
	
	for(; i<stats._entries ; i++)
	{
		//ret= read_dentry_by_index(i,&cmp);
		//printf("%s \n",array[i]._name);
		//checks to see if name matches
		if(tlen>=32){
			if( ((strncmp((const char*)array[i]._name, (const char*)fname, tlen) )==0)){
				*dentry = array[i];
				//printf("inside redn_edntry by name\n");
				//read_dentry_by_index(i,dentry);
				return 0;
			}
		}
		else{
			if( ((strncmp((const char*)array[i]._name, (const char*)fname, tlen) )==0) && (strlen((int8_t*)array[i]._name) == tlen)){
				*dentry = array[i];
				//printf("inside redn_edntry by name\n");
				//read_dentry_by_index(i,dentry);
				return 0;
			}
		}

	}
	//if it reaches here its a failure
	return -1;
}	



/*   Checkpoint 2
 *   read_dentry_by_index
 *   DESCRIPTION: fills in dentry_t block passed into funciton 
 * 		  according to the index also passed in
 *   INPUTS: index, pointer to dentry
 *   OUTPUTS: returns 0 on success, -1 on failure
 * 	      
 *   RETURN VALUE:  0 on success, -1 on failure
 * 	     
 *   SIDE EFFECTS: writes the corresponding index file into *dentry
 */
int32_t read_dentry_by_index (uint32_t index, dentry_t* dentry)
{
	if(index> stats._entries)
	   	return -1;
	*dentry= array[index];
	return 0;
}







/*   Checkpoint 2
 *   read_data (uint32_t inode, 
 * 		uint32_t offset, uint8_t* buf, uint32_t length)
 *   DESCRIPTION: reads data from given inode and offset 
 * 		  writes to buffer and returns length	     
 *   INPUTS: inode, offset, *buf, length
 *   OUTPUTS:length of number of bytes read and 
 * 	     placed into the buffer
 *   RETURN VALUE: length of number of bytes read and 
 * 	     placed into the buffer 
 *   SIDE EFFECTS: writes into buffer the bytes that are read 
 */
int32_t read_data (uint32_t inode, uint32_t offset, uint8_t* buf, uint32_t length)
{

	//invalid inodes
	if(inode>= stats._nodes)
		return -1;
	//invalid offset
	if(offset >= nodes[inode]._length)
		return 0;

	//invalid data block number
	//if()

	//stires index of the data block in the inode data array	
	int index_i= offset / FILE_PAGE_SIZE;
	//index within the data block
	int index_d= offset % FILE_PAGE_SIZE;
	
	uint8_t * curr_addr=(uint8_t *)(data_block_start_addr+(nodes[inode]._data_blocks_numbers[index_i]*FILE_PAGE_SIZE)+index_d);
	int ctr=0;
	for(; ctr<length ; ctr++)
	{
		buf[ctr]= *curr_addr;
		//printf("%c",buf[ctr]);
		index_d++;
		curr_addr++;
		//printf("%c",buf[ctr]);
		//if(ctr %128==0)
		//	printf("\n");
		if(index_d >= FILE_PAGE_SIZE)
		{
			index_i++;
			index_d=0;
			
			if(nodes[inode]._data_blocks_numbers[index_i]>=stats._blocks)
			{
				return -1;
			}
			else
			{
				curr_addr=(uint8_t *)(data_block_start_addr+(nodes[inode]._data_blocks_numbers[index_i]*FILE_PAGE_SIZE)+index_d);
			}
		}
		
		
		if(ctr+offset > nodes[inode]._length)
		{
			break;
		}	
	}
	//printf("   bytes read: %d", ctr);
	return ctr;
}









int32_t read_dir(int fd, void* buf, int nbytes)
{
	
	dentry_t curr_file;
	int i;
	uint8_t * buff=(uint8_t* )buf;
	if(all_running_pcbs[cur_process_id].fd_list[fd].file_pos >= stats._entries) return 0;
	
	
	int counter=0;
	read_dentry_by_index(all_running_pcbs[cur_process_id].fd_list[fd].file_pos, &curr_file);
	all_running_pcbs[cur_process_id].fd_list[fd].file_pos++;
	counter = strlen(( int8_t*) curr_file._name);
	
	for(i=0; i<strlen(( int8_t*)curr_file._name); i++){
		if(i>=32) break;
		buff[i] = curr_file._name[i];
	}

	return i;
}





/*
* loads executable files to given address
* helps with preocesses
* returns -1 for failure and 0 for sucess
*/
int32_t load_exec(const uint8_t* fname, uint32_t target_address)
{
	dentry_t file;
	
	/*
	* check to see if file with given name exists
	* also loads dentry into variable file
	* return -1 for failure
	*/
	if(read_dentry_by_name(fname, &file)==-1)
	{
		return -1;
	}
	
	//printf("file_name: %s", file._name);

	uint32_t inode= file._inode_number;

	/*int val= read_data(file._inode_number, 0, (uint8_t*)target_address, nodes[file._inode_number]._length);
	
	if (val ==0)
		return -1;
	
	return val;
*/

	//printf("      ratget add:      %d\n", target_address);

	uint8_t * buf= (uint8_t *)(target_address);
	//stires index of the data block in the inode data array	
	int index_i= 0;
	//index within the data block
	int index_d= 0;
	
	uint8_t * curr_addr=(uint8_t *)(data_block_start_addr+(nodes[inode]._data_blocks_numbers[index_i]*FILE_PAGE_SIZE)+index_d);
	int ctr=0;
	for(; ctr<nodes[file._inode_number]._length ; ctr++)
	{
		buf[ctr]= *curr_addr;
		//printf("%c",buf[ctr]);
		index_d++;
		curr_addr++;
		//printf("%c",buf[ctr]);
		//if(ctr %128==0)
		//	printf("\n");
		if(index_d >= FILE_PAGE_SIZE)
		{
			index_i++;
			index_d=0;
			
			if(nodes[inode]._data_blocks_numbers[index_i]>=stats._blocks)
			{
				return -1;
			}
			else
			{
				curr_addr=(uint8_t *)(data_block_start_addr+(nodes[inode]._data_blocks_numbers[index_i]*FILE_PAGE_SIZE)+index_d);
			}
		}	
	}

	return 0;





	
}




/*   Checkpoint 2
 *   @auth: lall3 
 *   display_names_of_files
 *   DESCRIPTION: prints all file names, returns printable arrays
 * 		  given by parameter
 *   INPUTS: *buf
 *   OUTPUTS: return 0 on success (always return 0)
 *   RETURN VALUE:  return 0
 *   SIDE EFFECTS: prints file names, file type, file size, to 
 * 		  screen in a manner similar to linux command "ls"
 */
int display_names_of_files(uint8_t * buf)
{
	dentry_t  curr_file;
	uint32_t i=0;
	uint8_t* s1= (uint8_t*)"file_name: ";
	uint8_t* s2= (uint8_t*)", file_type: ";
	uint8_t* s3= (uint8_t*)", file_size: ";
	//uint8_t* pname=(uint8_t*)"";
	uint8_t* nl=(uint8_t*)"\n";
	uint8_t * space = (uint8_t*)" \0";
	
	//uint8_t * test=(uint8_t*)" ";
	
	int tmp=0;
	
	int j=0;
	
	uint8_t* type=(uint8_t*)" "; 
	
	uint32_t size=0;
	uint8_t* sz=(uint8_t*)" ";
	//int k=0;
	
	for(; i<stats._entries; i++)
	{
		strcat(buf,s1);
		
		//gets file
		read_dentry_by_index(i, &curr_file);
		//printf("%s\n",buff);
		
		//name being added
		tmp=strlen((const char*)curr_file._name);
		for(j=0 ; j < MAX_NAME_LENGTH-tmp ; j++)
		{
			
			strcat(buf, space);
		}
		//strcat(pname, curr_file._name);
		if(tmp>MAX_NAME_LENGTH)
		{	
			curr_file._name[MAX_NAME_LENGTH]='\0';		
		}
		strcat(buf, curr_file._name );
		
		//file type being added
		strcat(buf, s2);
		type=my_itoa(curr_file._type, type);
		strcat(buf, type);
		
		//file size being added
		strcat(buf, s3);
		size=nodes[curr_file._inode_number]._length;
		sz=my_itoa(size, sz);
		
		tmp=strlen((const char*)sz);
		for(j=0 ; j < 7-tmp ; j++)
		{
			
			strcat(buf, space);
		}
		strcat(buf, sz);
		
		strcat(buf, nl);
	}
	return 0;
}





/*   Checkpoint 2 -- helper function
 *   strcat
 *   DESCRIPTION: Appends copy of src string to destination string
 * 		  Similar to strcat from C string library
 *   INPUTS:  *dest, *src
 *   OUTPUTS: returns dest
 *   RETURN VALUE: dest
 *   SIDE EFFECTS: Appends src to end of dest and returns dest
 */
uint8_t *strcat(uint8_t *dest, const uint8_t *src)
{
    int32_t i,j;
    for (i = 0; dest[i] != '\0'; i++)
        ;
    for (j = 0; src[j] != '\0'; j++)
        dest[i+j] = src[j];
    dest[i+j] = '\0';
    return dest;
}



/*   Checkpoint 2 -- helper function
 *   my_itoa
 *   DESCRIPTION: converts an int into a string
 *   INPUTS:  int n (int to be converted to string), *s (pointer to 
 * 	      string)
 *   OUTPUTS: returns the string
 *   RETURN VALUE: s (string)
 *   SIDE EFFECTS: converts the given integer into a string
 */
uint8_t* my_itoa(int n, uint8_t *s) 
{
	int i = 0;

	do {
		s[i++] = (n % 10) + '0'; //taking the last digit of int
	} while ((n /= 10) != 0);

	

	s[i] = '\0';
	reverse_str(s, i);
	return s;
}




/*   Checkpoint 2  -- helper function
 *   reverse_str
 *   DESCRIPTION: reverses a given string
 *   INPUTS: *s (pointer to string), len (length of given string)
 *   OUTPUTS: none
 *   RETURN VALUE: none 
 *   SIDE EFFECTS: reverses the inputted string
 */
void reverse_str(uint8_t *s, int len) 
{
	int i, j;
	char c;

	for (i = 0, j = len-1; i < j; ++i, --j) 
	{
		c = s[i];
		s[i] = s[j];
		s[j] = c;
	}
}
	

	
	/*
*	returns length of file with given inode val
*/
uint32_t getLen(uint32_t inode)
{
	return nodes[inode]._length;
}



/*
*  reads file by name and prints to screen 
*  returns 0 on sucess and -1 on failure
*  @auth: lall3	
*/
uint8_t fs_read(uint32_t index)
{
	//printf("%s", fname);
	dentry_t dentry;
	int val= read_dentry_by_index (index, &dentry);
	if (val ==-1)
		return -1;
	
	uint32_t len= getLen(dentry._inode_number);
	uint8_t read_data2[len];
	//printf("%s", dentry._name);
	
	read_data(dentry._inode_number, 0, read_data2 , len);
	
	//print read_ data plz
	//printf("here : %s", read_data2);
	term_write(read_data2, len);
	
	
	return 0;
}


