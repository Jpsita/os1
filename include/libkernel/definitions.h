#ifndef __LIBK_DEFINITIONS_H
#define __LIBK_DEFINITIONS_H

#define NULL 0

#ifdef _MSC_VER
#define PACKED
#else
#define PACKED __attribute__ ((packed))
#endif

typedef	signed char int8_t;
typedef unsigned char uint8_t;
typedef signed short int int16_t;
typedef unsigned short int uint16_t;
typedef signed int int32_t;
typedef unsigned int uint32_t;
typedef signed long long int64_t;
typedef unsigned long long uint64_t;



#endif