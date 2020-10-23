/**
 * @file tuya_hal_memory.h
 * @author qinfl@tuya.com
 * @brief 内存操作接口封装
 * @version 0.1
 * @date 2019-08-19
 * 
 * @copyright Copyright (c) tuya.inc 2019
 * 
 */

#ifndef __TUYA_HAL_MEMORY_H__
#define __TUYA_HAL_MEMORY_H__


#include <stddef.h>


#ifdef __cplusplus
extern "C" {
#endif


/**
 * @brief 内存分配操作函数指针类型
 * 
 */
typedef void* (*TUYA_MALLOC_FUNC_T)(const size_t size);

/**
 * @brief 内存释放操作函数指针类型
 * 
 */
typedef void (*TUYA_FREE_FUNC_T)(void* ptr);

/**
 * @brief tuya_hal_system_malloc用于分配内存
 * 
 * @param[in]       size        需要分配的内存带下
 * @return  分配得到的内存指针
 */
void *tuya_hal_system_malloc(const size_t size);

/**
 * @brief tuya_hal_system_free用于释放内存
 * 
 * @param[in]       ptr         需要释放的内存指针
 */
void tuya_hal_system_free(void* ptr);


#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif // __TUYA_HAL_MEMORY_H__

