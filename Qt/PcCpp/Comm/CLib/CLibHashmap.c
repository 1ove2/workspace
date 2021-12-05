/******************************************************************************
 *
 * 文件名  ： CLibHashmap.c
 * 创建日期： 20190628
 * 版本号  ： v1.0
 * 文件描述： hash表,用于实现协议重传
 * 其    他： 1. 地址hash: 除留余数法
 *            2. 冲突处理: 线性探测再散列
 * 修改日志： 无
 *
  *******************************************************************************/


/*---------------------------------- 预处理区 ---------------------------------*/

/************************************ 头文件 ***********************************/
/************************************ 头文件 ***********************************/
#include "CLibHashmap.h"

/*----------------------------------- 声明区 ----------------------------------*/

/********************************** 变量声明区 *********************************/


/********************************** 函数声明区 *********************************/
static bool IsIndexRight(const _CLIB_HASHMAP_T *hashmap, i32 index, bool in, i32 key);
static i32  Key2Index(const _CLIB_HASHMAP_T *hashmap, i32 key, bool in);
static void ClearItem(_CLIB_HASHMAP_ITEM_T_ *item);

/********************************** 变量实现区 *********************************/

/********************************** 函数实现区 *********************************/
void CLibHashmapInit(_CLIB_HASHMAP_T *hashmap)
{
    i32 i = 0;
    _CLIB_HASHMAP_ITEM_T_ *ptrData = hashmap->data;
    for(i = 0; i < _CLIB_HASHMAP_MAX_SIZE_; i++)
    {
        ClearItem(&ptrData[i]);
    }
    hashmap->usedSize = 0;
}

void CLibHashmapPut(_CLIB_HASHMAP_T *hashmap, i32 key, void *ptrObj)
{
    _CLIB_HASHMAP_ITEM_T_ *ptrData = hashmap->data;

    /* step1: 计算地址 */
    i32 index = Key2Index(hashmap, key, TRUE);
    if(-1 == index) /* 未找到 有效索引 */
    {
        while(1);
    }
    
    /* step2: 存入 */
    ptrData[index].key = key;
    ptrData[index].ptrObj = ptrObj;
    ptrData[index].isUsed = TRUE;
    
    /* step3: 变化尺寸 */
    hashmap->usedSize++;
}

/* 获取值 */
void *CLibHashmapGet(const _CLIB_HASHMAP_T *hashmap, i32 key)
{
    const _CLIB_HASHMAP_ITEM_T_ *ptrData = hashmap->data;

    /* step1: 计算地址 */
    i32 index = Key2Index(hashmap, key, FALSE);
    if(-1 == index) /* 未找到 */
    {
        return NULL;
    }
    
    /* step2: 返回值 指针 */
    return ptrData[index].ptrObj;
}

void CLibHashmapDel(_CLIB_HASHMAP_T *hashmap, i32 key)
{
    /* step1: 计算地址 */
    _CLIB_HASHMAP_ITEM_T_ *ptrData = hashmap->data;
    i32 index = 0;

    /* 空hashmap不能删除 */
    if(CLibHashmapIsEmpty(hashmap))
    {
        return;
    }
    
    /* step2: 删除 */
    index = Key2Index(hashmap, key, FALSE);
    ClearItem(&ptrData[index]);
    
    /* step3: 变化尺寸 */
    hashmap->usedSize--;
}

void CLibHashmapIterate(const _CLIB_HASHMAP_T *hashmap, FuncPtr func)
{
    i32 i = 0;
    const _CLIB_HASHMAP_ITEM_T_ *ptrData = hashmap->data;
    for(i = 0; i < _CLIB_HASHMAP_MAX_SIZE_; i++)
    {
        if(ptrData[i].isUsed)
        {
            func(ptrData[i].ptrObj);
        }
    }
}

i32  Key2Index(const _CLIB_HASHMAP_T *hashmap, i32 key, bool in)
{
    i32 tryTimes = 0;

    i32 index = key;
    do {
        /* 除留余数法 */
        index %= _CLIB_HASHMAP_MAX_SIZE_;
        if(IsIndexRight(hashmap, index, in, key)) /* 找到 */
        {
            break;
        }
        else
        {
            index++;              /* 冲突处理: 线性探测再散列 */
        }
    
        /* 避免陷入死循环 */
        tryTimes++;
        if(tryTimes >= _CLIB_HASHMAP_MAX_SIZE_)
        {
            return -1;
        }
    }while(1);
    
    return index;
}

/* 判断index是否正确
 *
 * in:
 * true  需要当前为空的索引                  入hashmap操作 使用
 * flase 需要当前非空且key与当前元素key相同  出hashmap操作 使用
 *
 * */
bool IsIndexRight(const _CLIB_HASHMAP_T *hashmap, i32 index, bool in, i32 key)
{
    const _CLIB_HASHMAP_ITEM_T_ *ptrData = hashmap->data;
    if(in)
    {
        return !ptrData[index].isUsed;
    }
    else
    {
        return (ptrData[index].isUsed) && (key == ptrData[index].key);
    }
}

void ClearItem(_CLIB_HASHMAP_ITEM_T_ *item)
{
    item->key = -1;
    item->ptrObj = NULL;
    item->isUsed = FALSE;
}

bool CLibHashmapIsFull(const _CLIB_HASHMAP_T *hashmap)
{
    return (_CLIB_HASHMAP_MAX_SIZE_ == CLibHashmapSize(hashmap));
}

bool CLibHashmapIsEmpty(const _CLIB_HASHMAP_T *hashmap)
{
    return (0 == CLibHashmapSize(hashmap));
}

i32 CLibHashmapSize(const _CLIB_HASHMAP_T *hashmap)
{
    return hashmap->usedSize;
}
