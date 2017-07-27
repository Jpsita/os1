#ifndef __JPS_FAT_H
#define __JPS_FAT_H


#include <common/common_fat.h>


FAT_IMPL initializeFAT(uint8_t isFloppy, uint8_t *buffer, uint16_t bufferSize, uint8_t* dirBuffer, uint16_t dirBuffetrSize);

uint16_t listFilesFAT(FAT_IMPL* fat, uint8_t* path, FAT_DIR_LN_ENTRY* output, uint16_t num_entries);
uint32_t loadFileFromCluster(FAT_IMPL* fat, uint16_t cluster, uint8_t* buffer, uint32_t bufferSize);

#endif
