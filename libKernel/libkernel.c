#include "libkernel.h"
#include "libkernel/video.h"
#include "libkernel/utils.h"
#include "kernel/functionIds.h"

void (*_printString)(char* str) = 0;

void printString(char* s){
    if(_printString == 0){
        _printString = util_int49(PRINTSTRING_ID);
    }
}

