/******************************************************************************
 *
 * 文件名  ： CLibHashmap.h
 * 创建日期： 20190628
 * 版本号  ： v1.0
 * 文件描述： hash表,用于实现协议重传
 * 其    他： 无
 * 修改日志： 无
 *
  *******************************************************************************/

/*---------------------------------- 预处理区 ---------------------------------*/
#ifdef __cplusplus
extern "C"
{
#endif

#ifndef _CLIB_HASHMAP_H_
#define _CLIB_HASHMAP_H_

/************************************ 头文件 ***********************************/
#include "TypeDef.h"

/************************************ 宏定义 ***********************************/
#define _CLIB_HASHMAP_MAX_SIZE_       (32)

/*********************************** 类型定义 **********************************/
typedef struct _MISC_HASHMAP_ITEM_TAG_{
    i32  key;                                                   /* 键 */
    void *ptrObj;                                               /* 值对象 指针 */
    bool isUsed;                                                /* 元素是否有效 */
} _CLIB_HASHMAP_ITEM_T_;                                        /* hashmap元素 键值对 */

typedef struct _MISC_HASHMAP_TAG_{
    i32 usedSize;
    _CLIB_HASHMAP_ITEM_T_ data[_CLIB_HASHMAP_MAX_SIZE_];    /* 元素存储空间 */
} _CLIB_HASHMAP_T;                                           /* hashmap */
/*--------------------------------- 接口声明区 --------------------------------*/

/*********************************** 全局变量 **********************************/

/*********************************** 接口函数 **********************************/
/* 初始化 */
void CLibHashmapInit(_CLIB_HASHMAP_T *hashmap);
/* 设置值 */
void CLibHashmapPut(_CLIB_HASHMAP_T *hashmap, i32 key, void *ptrObj);
/* 获取值 */
void *CLibHashmapGet(const _CLIB_HASHMAP_T *hashmap, i32 key);
/* 删除 */
void CLibHashmapDel(_CLIB_HASHMAP_T *hashmap, i32 key);

/* hashmap空满判断 */
bool CLibHashmapIsFull(const _CLIB_HASHMAP_T *hashmap);
bool CLibHashmapIsEmpty(const _CLIB_HASHMAP_T *hashmap);
i32  CLibHashmapSize(const _CLIB_HASHMAP_T *hashmap);

/* 迭代遍历 */
void CLibHashmapIterate(const _CLIB_HASHMAP_T *hashmap, FuncPtr func);

#endif // #ifndef _CLIB_HASHMAP_H_

#ifdef __cplusplus
}
#endif
