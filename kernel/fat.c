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

void listFilesFAT(uint8_t* fat, uint8_t* path, uint8_t* output){
	if(strcmp(path, "/") == 0){

	}
}
