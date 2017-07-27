#ifndef __JPS_COMMON_FAT_H
#define __JPS_COMMON_FAT_H

#include <common/common_main.h>

typedef struct  PACKED{
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

typedef struct PACKED{
	uint8_t*	fat;
	uint8_t*	root_directory;
	uint16_t	fat_size;
	uint16_t	root_dir_size;
	uint16_t 	data_offset_secs;
} FAT_IMPL;

typedef struct PACKED{
	uint8_t		long_name[64];
	uint8_t		attributes;
	uint16_t	cluster;
	uint16_t	n_entries;
} FAT_DIR_LN_ENTRY;

typedef struct PACKED{
	uint8_t index;
	uint16_t name[5];
	uint8_t	attribute;
	uint8_t type;
	uint8_t checksum;
	uint16_t name2[6];
	uint16_t zero;
	uint16_t name3[2];
} FAT_LONG_NAME;

typedef struct PACKED{
	uint8_t name[8];
	uint8_t extension[3];
	uint8_t attributes;
	uint8_t reserved;
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

#endif