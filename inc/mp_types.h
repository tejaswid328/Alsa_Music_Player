#ifndef MP_TYPES_H_
#define MP_TYPES_H_

#define SEEK_FORWARD_VAL 10
#define SEEK_BACKWARD_VAL 10
#define MP_TRUE 1
#define MP_FALSE 0
typedef char MP_CHAR;
typedef unsigned char MP_UCHAR;
typedef unsigned char MP_BYTE;
typedef signed char MP_INT8;
typedef unsigned char MP_UINT8;
typedef signed short MP_INT16;
typedef unsigned short MP_UINT16;
typedef signed int MP_INT32;
typedef unsigned int MP_UINT32;
typedef signed long  MP_INT64;
typedef unsigned long MP_UINT64;
//typedef signed long long MP_LONG64;
//typedef unsigned long long MP_UINT64;
typedef signed long MP_LONG;
typedef unsigned char MP_BOOL;
typedef float MP_FLOAT;
typedef double MP_DOUBLE;
typedef void MP_VOID;
//typedef size_t MP_SIZE_T;
//typedef FILE MP_FILE;
typedef MP_INT32 RET_STATUS;

typedef enum {
MP_STATE_NULL = -1,
MP_STATE_INIT = 0,
MP_STATE_PLAY = 1,
MP_STATE_PAUSE = 2,
MP_STATE_STOP = 3,
MP_STATE_EXIT = 4
}MP_STATE;

typedef enum {
MP_KEY_PLAY = 0,
MP_KEY_PAUSE = 1,
MP_KEY_RESUME = 2,
MP_KEY_SEEK = 3,
MP_KEY_VOL_I = 6,
MP_KEY_VOL_D = 7,
MP_KEY_NEXT = 8,
MP_KEY_PREV = 9,
MP_KEY_STOP = 10
}MP_KEY_OPERATION;

#endif /*mp_types.h*/



