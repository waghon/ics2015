#include "common.h"

typedef struct {
	char *name;
	uint32_t size;
	uint32_t disk_offset;
} file_info;

typedef struct {
	bool opened;
	uint32_t offset;
} Fstate;

enum {SEEK_SET, SEEK_CUR, SEEK_END};

/* This is the information about all files in disk. */
static const file_info file_table[] = {
	{"1.rpg", 188864, 1048576}, {"2.rpg", 188864, 1237440},
	{"3.rpg", 188864, 1426304}, {"4.rpg", 188864, 1615168},
	{"5.rpg", 188864, 1804032}, {"abc.mkf", 1022564, 1992896},
	{"ball.mkf", 134704, 3015460}, {"data.mkf", 66418, 3150164},
	{"desc.dat", 16027, 3216582}, {"fbp.mkf", 1128064, 3232609},
	{"fire.mkf", 834728, 4360673}, {"f.mkf", 186966, 5195401},
	{"gop.mkf", 11530322, 5382367}, {"map.mkf", 1496578, 16912689},
	{"mgo.mkf", 1577442, 18409267}, {"m.msg", 188232, 19986709},
	{"mus.mkf", 331284, 20174941}, {"pat.mkf", 8488, 20506225},
	{"rgm.mkf", 453202, 20514713}, {"rng.mkf", 4546074, 20967915},
	{"sss.mkf", 557004, 25513989}, {"voc.mkf", 1997044, 26070993},
	{"wor16.asc", 5374, 28068037}, {"wor16.fon", 82306, 28073411},
	{"word.dat", 5650, 28155717},
};


#define NR_FILES (sizeof(file_table) / sizeof(file_table[0]))

static Fstate file_state[NR_FILES+3];

void serial_printc(char);
void ide_read(uint8_t *, uint32_t, uint32_t);
void ide_write(uint8_t *, uint32_t, uint32_t);

/* TODO: implement a simplified file system here. */

static int strcmp(const char *dst,char *src){
	while(*dst!='\0'&&*src!='\0'){
		if(*dst!=*src){
			return 1;
		}
		else{
			dst++;
			src++;
		}
	}
	if(*dst=='\0'&&*src=='\0')
		return 0;
	else
		return 1;
}

int fs_open(const char *pathname, int flags){
	int i=0;
	for(;i<NR_FILES;i++){
		if(strcmp(pathname,file_table[i].name)==0){
			file_state[i+3].opened=1;
			file_state[i+3].offset=0;
			return i+3;
		}
	}
	assert(0);
	return -1;		
}

int fs_read(int fd, void *buf, int len){
//	uint32_t disk_off=file_info[fd-3].disk_offset;
	if(file_state[fd].offset<file_table[fd-3].size){
		if(file_state[fd].offset+len>file_table[fd-3].size){
			len=file_table[fd-3].size-file_state[fd].offset;
		}
		ide_read(buf,file_table[fd-3].disk_offset+file_state[fd].offset,len);
		file_state[fd].offset+=len;
		return len;
	}
	else{
		return 0;		
	}
}

int fs_write(int fd, void *buf, int len){
	/* deal with stdout stderr */
	if(fd==1||fd==2){
		int i=0;
		for(;i<len;i++){
			serial_printc(((char *)buf)[i]);
		}
		return len;
	}

	if(file_state[fd].offset<file_table[fd-3].size){
		if(file_state[fd].offset+len>file_table[fd-3].size){
			len=file_table[fd-3].size-file_state[fd].offset;
		}
		ide_write(buf,file_table[fd-3].disk_offset+file_state[fd].offset,len);
		file_state[fd].offset+=len;
		return len;
	}
	else{
		return 0;		
	}	
}

int fs_lseek(int fd, int offset, int whence){
	if(whence==SEEK_SET){
		file_state[fd].offset=offset;
	}
	else if(whence==SEEK_CUR){
		file_state[fd].offset+=offset;
	}
	else{
		file_state[fd].offset=file_table[fd-3].size+offset;
	}
	return file_state[fd].offset;
}

int fs_close(int fd){
	file_state[fd].opened=0;
	return 0;
}
