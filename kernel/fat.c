#include "fat.h"
#include "floppy.h"
#include "string.h"
#include "video.h"

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

	fatImpl.data_offset_secs = fatInfo.reserved_sectors + (fatInfo.sectors_per_fat * fatInfo.number_of_fats) + (((fatInfo.max_root_dirs * 32) + fatInfo.bytes_per_sector - 1) / fatInfo.bytes_per_sector);
	fatImpl.fat = buffer;
	fatImpl.root_directory = dirBuffer;

	return fatImpl;
}

uint16_t readFatentry(FAT_IMPL* fat, uint16_t cluster){
	uint32_t fat_offset = cluster + (cluster / 2);
	uint16_t val =  *((uint16_t*)&fat->fat[fat_offset]);
	if(cluster & 0x0001){
		val = val >> 4;
	}else{
		val = val & 0x0FFF;
	}
	return val;
}

uint32_t loadFileFromCluster(FAT_IMPL* fat, uint16_t cluster, uint8_t* buffer, uint32_t bufferSize){
	uint32_t redBytes = 0;
	uint16_t fat_value;
	//printString("DT: ");
	//printUint32(fat->data_offset_secs);
	//printString("\n");
	do{
		fat_value = readFatentry(fat, cluster);
		if(fat_value != 0xFF7){
			uint16_t sector_id = fat->data_offset_secs + cluster - 2;
			printString("Reading sector: ");
			printUint32(sector_id);
			printString("\n");
			floppy_read(sector_id, 512, buffer + redBytes);
		}else{
			cluster++;
			continue;
		}
		cluster = fat_value;
		redBytes += 512;
		if(redBytes >= bufferSize){
			return redBytes;
		}
	}while(fat_value < 0xFF8);
	return redBytes;
}

uint8_t tempDirBuffer[1024];

uint16_t readDirectoryEntry(uint8_t* ptr, FAT_DIR_LN_ENTRY* output) {
	uint16_t n_entries = 0;
	FAT_LONG_NAME* ln;
	FAT_DIR_LN_ENTRY* ent = output;
	ent->long_name[0] = 0;
	FAT_ENTRY* en;
	while (*ptr != 0) {
		uint8_t attrs = ptr[11];
		if (attrs == 0x0F) {
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
		}
		else if (attrs == 0x08) {

		}else{
			en = (FAT_ENTRY*)ptr;
			if (strcat_nn(ent->long_name, en->name, 8, ' ') != 0) {
				if ((en->attributes & 0x10) == 0) {
					
					stradd(ent->long_name, '.');
					strcat_n(ent->long_name, en->extension, 3);
				}
			}
			ent->attributes = en->attributes;
			ent->cluster = en->cluster_low;
			ent++;
			n_entries++;
		}
		ptr += 32;
	}
	return n_entries;
}


uint16_t listFilesFAT(FAT_IMPL* fat, uint8_t* path, FAT_DIR_LN_ENTRY* output, uint16_t num_entries) {
	uint8_t* ptr = fat->root_directory;
	uint16_t n_entries = 0;
	uint32_t len = strlen(path);
	uint32_t isFolderEmpty = 0;
	if (path[len - 1] != '/') {
		return ~0;
	}
	FAT_DIR_LN_ENTRY* ent = output;
	ent->long_name[0] = 0;
	FAT_LONG_NAME* ln;
	FAT_ENTRY* en;
	n_entries = readDirectoryEntry(ptr, output);
	uint32_t pos = 0;
	uint32_t found = 0;
	FAT_DIR_LN_ENTRY enx;
	do {
		uint32_t pos = strpos_s(path, '/', 1);
		if (pos == ~0 || isFolderEmpty) {
			return n_entries;
		}
		uint8_t pth[64];
		pth[0] = 0;
		substr(path, pth, 1, pos);
		printString(">Looking for ");
		printString(pth);
		printString(": ");
		for (int i = 0; i < n_entries; i++) {
			if (strcmp(output[i].long_name, pth) == 0) {
				found = 1;
				enx = output[i];
			}
		}
		if (found == 0) {
			printString("Not Found\n");
			return ~0;
		}
		else {
			printString("Found!\n");
			n_entries = 0;
			ent = output;
			if ((enx.attributes && 0x10) > 0) {
				ZeroMemory(tempDirBuffer, sizeof(tempDirBuffer));
				uint32_t redBytes = loadFileFromCluster(fat, enx.cluster, tempDirBuffer, sizeof(tempDirBuffer));
				if(redBytes != 0){
					uint8_t* tmp_ptr = tempDirBuffer;
					ZeroMemory((uint8_t*) output, 240 * sizeof(FAT_DIR_LN_ENTRY));
					n_entries = readDirectoryEntry(tmp_ptr, output);
					path += pos;
					isFolderEmpty = 0;
				}else{
					isFolderEmpty = 1;
					path += pos;
				}
			}
		}
	}while (pos != ~0);
	return n_entries;
}

