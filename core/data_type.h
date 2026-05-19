#ifndef _TYPE_H_
#define _TYPE_H_

typedef char S8_t;
typedef unsigned char U8_t;
typedef short S16_t;
typedef unsigned short U16_t;
typedef int S32_t;
typedef unsigned int U32_t;
typedef float F32_t;
typedef double F64_t;

typedef volatile char VS8_t;
typedef volatile unsigned char VU8_t;
typedef volatile short VS16_t;
typedef volatile unsigned short VU16_t;
typedef volatile int VS32_t;
typedef volatile unsigned int VU32_t;
typedef volatile float VF32_t;
typedef volatile double VF64_t;

typedef enum _BOOL_t {
	BOOL_FALSE = 0,
	BOOL_TRUE = 1
}BOOL_t;

typedef enum _MUXTEX_t {
	MUX_UNLOCK = 0,
	MUX_LOCK = 1
}MUTEX_t;


#endif
