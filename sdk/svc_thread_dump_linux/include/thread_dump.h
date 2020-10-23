#ifndef __THREAD_DUMP_H__
#define __THREAD_DUMP_H__

#include "tuya_cloud_types.h"

#ifdef __cplusplus
extern "C" {
#endif



VOID thread_dump_init(CHAR_T* crash_dump_tmp_file_path);

//生成pid_{pid}_all_dump.txt
OPERATE_RET thread_dump_all();

//生成tid_{tid}_dump.txt
OPERATE_RET thread_dump_by_tid(UINT_T tid);

//生成pid_{pid}_{name}_dump.txt
OPERATE_RET thread_dump_by_name(CHAR_T *thread_name);

OPERATE_RET thread_dump_by_addr(ULONG_T addr, UINT_T tid);

#ifdef __cplusplus
} // extern "C"
#endif

#endif // __THREAD_DUMP_H__

