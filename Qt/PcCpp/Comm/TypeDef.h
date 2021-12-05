#ifndef _TYPE_DEF_H_
#define _TYPE_DEF_H_

/************************************ 头文件 ***********************************/
#include <stddef.h>
#include <stdint.h>

/************************************ 宏定义 ***********************************/
/* bool 宏 */
#ifndef FALSE
#define FALSE (0)
#endif

#ifndef TRUE
#define TRUE (1)
#endif

/* 空字符 */
#ifndef NUL
#define NUL ('\0')
#endif

#ifndef NULL
#define NULL ((void*) 0)
#endif

/*********************************** 类型定义 **********************************/
typedef uint8_t             u8;
typedef uint16_t            u16;
typedef uint32_t            u32;
typedef uint64_t            u64;
typedef int8_t              i8;
typedef int16_t             i16;
typedef int32_t             i32;
typedef int64_t             i64;
typedef float               f32;
typedef double              f64;

#ifdef __cplusplus
extern "C"
{
    /* 回调函数 */
    typedef void(*FuncPtr)  (void *argv);

    /* 序列的生成函数 y = func(x) */
    typedef double(*X2YFuncPtr) (double x);
}
#else
/* 回调函数 */
typedef void(*FuncPtr)  (void *argv);
/* 序列的生成函数 y = func(x) */
typedef double(*X2YFuncPtr) (double x);
/* 避免Qt编译报错 */
typedef unsigned char       bool;
#endif


#endif // _TYPE_DEF_H_
