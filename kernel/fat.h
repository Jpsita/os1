#ifndef __JPS_FAT_H
#define __JPS_FAT_H

#include "main.h"

typedef struct  __attribute__ ((packed)){
	uint8_t		jump[3];
	uint8_t		OEM[8];
	uint16_t 	bytes_per_sector;
	uint8_t 	sectors_per_cluster;
	uint16_t	reserved_sectors;
	uint8_t		number_of_fats;
	uint16_t	max_root_dirs;
	uint16_t	numer_of_sectors;
	uint8_t		media_descriptor;
	uint16_t	sectors_per_fat;
	uint16_t	sectors_per_track;
	uint16_t	number_of_heads;
	uint32_t	number_of_hidden_sectors;
	uint32_t	extra_sectors;
	uint8_t		drive_number;
	uint8_t		reserved;
	uint8_t		extended_boot;
	uint32_t	drive_id;
	uint8_t		volume_label[11];
	uint8_t		fat_type[8];
}FAT_INFO;

typedef struct __attribute__ ((packed)){
	uint8_t*	fat;
	uint8_t*	root_directory;
	uint16_t	fat_size;
	uint16_t	root_dir_size;
} FAT_IMPL;

typedef struct __attribute__ ((packed)){
	uint8_t*	long_name;
	uint8_t*	file_info;
} FAT_DIR_LN_ENTRY;

typedef struct __attribute__ ((packed)){
	uint8_t index;
	uint16_t name[5];
	uint8_t	attribute;
	uint8_t type;
	uint8_t checksum;
	uint16_t name2[12];
	uint16_t zero;
	uint16_t name3[2];
} FAT_LONG_NAME;

typedef struct __attribute__ ((packed)){
	uint8_t name[8];
	uint8_t extension[3];
	uint8_t attributes;
	uint8_t creation_time_ts;
	uint16_t creation_time;
	uint16_t creation_date;
	uint16_t last_access_date;
	uint16_t cluster_high;
	uint16_t last_mod_time;
	uint16_t last_mod_date;
	uint16_t cluster_low;
	uint32_t size;
} FAT_ENTRY;

FAT_IMPL initializeFAT(uint8_t isFloppy, uint8_t *buffer, uint16_t bufferSize, uint8_t* dirBuffer, uint16_t dirBuffetrSize);




#endif