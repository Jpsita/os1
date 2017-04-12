#include "fat.h"
#include "floppy.h"
#include "string.h"

FAT_IMPL initializeFAT(uint8_t isFloppy, uint8_t *buffer, uint16_t bufferSize, uint8_t* dirBuffer, uint16_t dirBufferSize){
	FAT_INFO fatInfo;
	FAT_IMPL fatImpl;
	fatImpl.fat = 0;
	fatImpl.root_directory = 0;
	fatImpl.fat_size = 0;
	fatImpl.root_dir_size = 0;
	floppy_read(0, sizeof(FAT_INFO), (uint8_t*)&fatInfo);

	uint16_t requiredSize = 0;
	requiredSize = fatInfo.bytes_per_sector * fatInfo.sectors_per_cluster * fatInfo.sectors_per_fat * fatInfo.number_of_fats;
	if(requiredSize > bufferSize){
		return fatImpl;
	}

	floppy_read(fatInfo.reserved_sectors, requiredSize, buffer);
	uint16_t rootDirectoryCluster = fatInfo.reserved_sectors + (fatInfo.sectors_per_fat * fatInfo.number_of_fats); 
	uint16_t rootDirectorySize = fatInfo.max_root_dirs * 32;
	floppy_read(rootDirectoryCluster, rootDirectorySize, dirBuffer);

	fatImpl.fat = buffer;
	fatImpl.root_directory = dirBuffer;

	return fatImpl;
}



uint16_t listFilesFAT(FAT_IMPL* fat, uint8_t* path, FAT_DIR_LN_ENTRY* output, uint16_t num_entries){
	uint8_t* ptr = fat->root_directory;
	uint16_t n_entries = 0;
	FAT_DIR_LN_ENTRY* ent = output;
	ent->long_name[0] = 0;
	FAT_LONG_NAME* ln;
	FAT_ENTRY* en;
	if(strcmp(path, "/") == 0){
		while(*ptr != 0){
			switch(ptr[11]){
				case 0x0F:
				ln = (FAT_LONG_NAME*)ptr;
				stradd(ent->long_name, (uint8_t)(ln->name[0] >> 8));
				stradd(ent->long_name, (uint8_t)(ln->name[1] >> 8));
				stradd(ent->long_name, (uint8_t)(ln->name[2] >> 8));
				stradd(ent->long_name, (uint8_t)(ln->name[3] >> 8));
				stradd(ent->long_name, (uint8_t)(ln->name[4] >> 8));
				stradd(ent->long_name, (uint8_t)(ln->name2[0] >> 8));
				stradd(ent->long_name, (uint8_t)(ln->name2[1] >> 8));
				stradd(ent->long_name, (uint8_t)(ln->name2[2] >> 8));
				stradd(ent->long_name, (uint8_t)(ln->name2[3] >> 8));
				stradd(ent->long_name, (uint8_t)(ln->name2[4] >> 8));
				stradd(ent->long_name, (uint8_t)(ln->name2[5] >> 8));
				stradd(ent->long_name, (uint8_t)(ln->name3[0] >> 8));
				stradd(ent->long_name, (uint8_t)(ln->name3[1] >> 8));
				break;
				case 0x00:
				case 0x01:
				case 0x02:
				case 0x04:
				case 0x10:
				case 0x20:
				en = (FAT_ENTRY*)ptr;
				if(strcat_nn(ent->long_name, en->name, 8, ' ') != 0){
					stradd(ent->long_name, '.');
					strcat_n(ent->long_name, en->extension, 3);
				}
				ent->attributes = en->attributes;
				ent->cluster = en->cluster_low;
				ent++;
				n_entries++;
				break;
				case 0x08:
				break;
			}
			ptr += 32;
			if(n_entries >= num_entries){
				return ~0;
			}
		}
	}
	return n_entries;
}


