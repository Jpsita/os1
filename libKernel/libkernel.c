#include "libkernel.h"
#include "libkernel/video.h"
#include "libkernel/utils.h"
#include "libkernel/keyboard.h"
#include "libkernel/string.h"
#include "kernel/functionIds.h"

#define GET_KERNEL_FUNCTION(fName, fId) if(fName == 0){ fName = util_int49(fId); }

void (*_printString)(char* str) = 0;
void (*_printCharacter)(char c) = 0;
void (*_clearScreen)() = 0;
void (*_deleteCurrentChar)() = 0;

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