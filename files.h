/* 
 * file_systems.h
 */
#ifndef _FILES_H
#define _FILES_H

#include "types.h"
#include "lib.h"
#include "keyboard.h"
#include "x86_desc.h"
#include "system_calls.h"

//2 structures
/*
 * dentry stuct 
 * '_' at the start of the names of variables 
 * as defined in 7.1 (apendix A) of the doc
 * 64B struct
 */
 
 
 
 


//address of the first data block
uint32_t data_block_start_addr;
 

#define FILE_PAGE_SIZE	0x1000
#define MAX_NAME_LENGTH 0x20	//32 in decimal
#define RESERVED_BITS 24
typedef struct {
	uint8_t _name[MAX_NAME_LENGTH];
	uint32_t _type;				 // only 4 byte
	uint32_t _inode_number; 		 // only 4 byte
	uint8_t _reversed[RESERVED_BITS];			//24 reserved bytes
	
} dentry_t;

/*
 * holds the stats of the file system in use
 * same for all directories
 */
typedef struct {
	// # of dir. enteries (4b)
	uint8_t _entries;
	//# inodes 👎 (4b)
	uint8_t _nodes;
	//# data blocks (D) (4B)
	uint8_t _blocks;
	
} block_numbers_t;

/*
* strcut inode
*
*/
typedef struct{
	uint32_t _length;
	uint32_t _data_blocks_numbers[1023];// 1023 blocks is the max
} inode_t;



inode_t * nodes;


/*
* the required function as described in documentation
*/
//initalize global variables 
int32_t init_file_systems(int32_t start);

//given functions needed for filesystem
int32_t read_dentry_by_name (const uint8_t* fname, dentry_t* dentry);
int32_t read_dentry_by_index (uint32_t index, dentry_t* dentry);
int32_t read_data (uint32_t inode, uint32_t offset, uint8_t* buf, uint32_t length);

int32_t read_dir(int fd, void* buf, int nbytes);

int32_t load_exec(const uint8_t* fname, uint32_t target_address);



//int32_t halt (uint8 t status);
//int32_t execute (const uint8 t* command);
//int32_t read (int32 t fd, void* buf, int32 t nbytes);
//int32_t write (int32 t fd, const void* buf, int32 t nbytes);
int32_t fs_open (const uint8_t* filename);
int32_t fs_write (int32_t fd, const void* buf, int32_t nbytes);
int32_t fs_close (int32_t fd);
//int32_t getargs (uint8 t* buf, int32 t nbytes);
//int32_t vidmap (uint8 t** screen start);

uint32_t getLen(uint32_t inode);
uint8_t fs_read(uint32_t index);

//ck 2 stuff 
int display_names_of_files(uint8_t * buf);
//str concatenate
uint8_t *
strcat(uint8_t *dest, const uint8_t *src);
//integer to string helper
uint8_t* my_itoa(int n, uint8_t *s);
//reverse string helper
void reverse_str(uint8_t *s, int len);


#endif


