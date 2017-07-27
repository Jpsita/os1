#include "libkernel.h"
#include "libkernel/video.h"
#include "libkernel/utils.h"
#include "libkernel/keyboard.h"
#include "libkernel/string.h"
#include "common/functionIds.h"

#define GET_KERNEL_FUNCTION(fName, fId) if(fName == 0){ fName = util_int49(fId); }

void (*_printString)(char* str) = 0;
void (*_printCharacter)(char c) = 0;
void (*_clearScreen)() = 0;
void (*_deleteCurrentChar)() = 0;
void (*_printUint32)(uint32_t num) = 0;

char (*_getCh)() = 0;
char (*_getCharacter)() = 0;
void (*_buff_clean)() = 0;

void v_printString(char* s){
    if(_printString == 0){
        _printString = util_int49(PRINTSTRING_ID);
    }
    _printString(s);
}

void v_printCharacter(char c){
    if(_printCharacter == 0){
        _printCharacter = util_int49(PRINTCHARACTER_ID);
    }
    _printCharacter(c);
}

void v_clear(){
    if(_clearScreen == 0){
        _clearScreen = util_int49(CLEARSCREEN_ID);
    }

    _clearScreen();
}

void v_bksp(){
    if(_deleteCurrentChar == 0){
        _deleteCurrentChar = util_int49(DELETECURRENTCHAR_ID);
    }

    _deleteCurrentChar();
}

void v_printUint32(uint32_t num){
    GET_KERNEL_FUNCTION(_printUint32, PRINTUINT32_ID);
    _printUint32(num);
}

char k_getCh(){
    if(_getCh == 0){
        _getCh = util_int49(BUFF_GETCH_ID);
    }

    return _getCh();
}

char k_getCharacter(){
    if(_getCharacter == 0){
        _getCharacter = util_int49(BUFF_GETCHARACTER_ID);
    }

    return _getCharacter();
}

void k_cleanBuff(){
    if(_buff_clean == 0){
        _buff_clean = util_int49(BUFF_CLEAN_ID);
    }
    _buff_clean();
}

void s_strcpy(uint8_t* first, uint8_t* second){
    while(*first != 0){
        *second = *first;
        first++;
        second++;
    }
}

uint32_t (*_strlen)(uint8_t* string) = 0;

uint32_t (*_strcmp)(uint8_t* first, uint8_t* second) = 0;


uint32_t (*_strcat)(uint8_t* first, uint8_t* second) = 0;
uint32_t (*_strcat_n)(uint8_t* first, uint8_t* second, uint32_t max_chars) = 0;
uint32_t (*_strcat_nn)(uint8_t* first, uint8_t* second, uint32_t max_chrs, uint8_t myNull) = 0;
uint32_t (*_strpos_s)(uint8_t* string, uint8_t needle, uint32_t start) = 0;
uint32_t (*_strpos)(uint8_t* string, uint8_t needle) = 0;
uint32_t (*_substr)(uint8_t* string, uint8_t* buffer, uint32_t start, uint32_t end) = 0;
void (*_stradd)(uint8_t* string, uint8_t chr) = 0;
uint32_t (*_strpos_rs)(uint8_t* string, uint8_t neelde, uint32_t start) = 0;
uint32_t (*_strpos_r)(uint8_t* string, uint8_t neelde) = 0;

uint32_t s_strlen(uint8_t* string){
    GET_KERNEL_FUNCTION(_strlen, STRLEN_ID)
    return _strlen(string);
}

uint32_t s_strcmp(uint8_t* first, uint8_t* second){
    GET_KERNEL_FUNCTION(_strcmp, STRCMP_ID)
    return _strcmp(first, second);
}


uint32_t s_strcat(uint8_t* first, uint8_t* second){
    GET_KERNEL_FUNCTION(_strcat, STRCAT_ID)
    return _strcat(first, second);
}
uint32_t s_strcat_n(uint8_t* first, uint8_t* second, uint32_t max_chars){
    GET_KERNEL_FUNCTION(_strcat_n, STRCAT_N_ID)
    return _strcat_n(first, second, max_chars);
}
uint32_t s_strcat_nn(uint8_t* first, uint8_t* second, uint32_t max_chrs, uint8_t myNull){
    GET_KERNEL_FUNCTION(_strcat_nn, STRCAT_NN_ID)
    return _strcat_nn(first, second, max_chrs, myNull);
}
uint32_t s_strpos_s(uint8_t* string, uint8_t needle, uint32_t start){
    GET_KERNEL_FUNCTION(_strpos_s, STRPOS_S_ID)
    return _strpos_s(string, needle, start);
}
uint32_t s_strpos(uint8_t* string, uint8_t needle){
    GET_KERNEL_FUNCTION(_strpos, STRPOS_ID)
    return _strpos(string, needle);
}
uint32_t s_substr(uint8_t* string, uint8_t* buffer, uint32_t start, uint32_t end){
    GET_KERNEL_FUNCTION(_substr, SUBSTR_ID)
    return _substr(string, buffer, start, end);
}
void s_stradd(uint8_t* string, uint8_t chr){
    GET_KERNEL_FUNCTION(_stradd, STRADD_ID)
    _stradd(string, chr);
}

uint32_t s_strpos_rs(uint8_t* string, uint8_t needle, uint32_t start){
    GET_KERNEL_FUNCTION(_strpos_rs, STRPOS_RS_ID);
    return _strpos_rs(string, needle, start);
}

uint32_t s_strpos_r(uint8_t* string, uint8_t needle){
    GET_KERNEL_FUNCTION(_strpos_r, STRPOS_R_ID);
    return _strpos_r(string, needle);
}

/* FAT */

FAT_IMPL (*_initializeFAT)(uint8_t isFloppy, uint8_t *buffer, uint16_t bufferSize, uint8_t* dirBuffer, uint16_t dirBuffetrSize);

uint16_t (*_listFilesFAT)(FAT_IMPL* fat, uint8_t* path, FAT_DIR_LN_ENTRY* output, uint16_t num_entries);
uint32_t (*_loadFileFromCluster)(FAT_IMPL* fat, uint16_t cluster, uint8_t* buffer, uint32_t bufferSize);

FAT_IMPL* _libk_floppy_fat = 0;

FAT_IMPL f_initializeFAT(uint8_t isFloppy, uint8_t *buffer, uint16_t bufferSize, uint8_t* dirBuffer, uint16_t dirBufferSize){
    GET_KERNEL_FUNCTION(_initializeFAT, INITIALIZEFAT_ID);
    return _initializeFAT(isFloppy, buffer, bufferSize, dirBuffer, dirBufferSize);
}
uint16_t f_listFilesFAT(FAT_IMPL* fat, uint8_t* path, FAT_DIR_LN_ENTRY* output, uint16_t num_entries){
    GET_KERNEL_FUNCTION(_listFilesFAT, LISTFILESFAT_ID);
    return _listFilesFAT(fat, path, output, num_entries);
}

uint32_t f_loadFileFromCluster(FAT_IMPL* fat, uint16_t cluster, uint8_t* buffer, uint32_t bufferSize){
    GET_KERNEL_FUNCTION(_loadFileFromCluster, LOADFILEFROMCLUSTER_ID);
    return _loadFileFromCluster(fat, cluster, buffer, bufferSize);
}

FAT_IMPL* f_getFAT(){
    _libk_floppy_fat = util_int49(RES_DEFAULTFAT_ID);
    return _libk_floppy_fat;
}

uint8_t _loadFile_buff[128];
uint8_t _loadFile_fileName[128];
FAT_DIR_LN_ENTRY _loadFile_dir[64];

uint32_t f_loadFile(FAT_IMPL* fat, char* path, uint8_t* buffer, uint32_t bufferSize){
    uint32_t pos = 0;
    s_strpos_r(path, '/');
    uint32_t len = s_strlen(path);
    s_substr(path, _loadFile_fileName, pos + 1, len);
    s_substr(path, _loadFile_buff, 0, pos + 1);
    uint16_t res = f_listFilesFAT(fat, _loadFile_buff, _loadFile_dir, 64);
    uint32_t cluster = 0;
    for(int i = 0; i < res; i++){
        if(s_strcmp(_loadFile_dir[i].long_name, _loadFile_fileName) == 0){
            cluster = _loadFile_dir[i].cluster;
            break;
        }
    }
    if(cluster == 0){
        for(int i = 0; i < 128; i++){
            _loadFile_buff[i] = 0;
            _loadFile_fileName[i] = 0;
        }
        return ~0;
    }
    for(int i = 0; i < 128; i++){
        _loadFile_buff[i] = 0;
        _loadFile_fileName[i] = 0;
    }
    uint8_t* tmp = (uint8_t*) _loadFile_dir;
    for(int i = 0; i < sizeof(_loadFile_dir); i++){
        tmp[i] = 0;
    }
    return f_loadFileFromCluster(fat, (uint16_t) cluster, buffer, bufferSize);
}