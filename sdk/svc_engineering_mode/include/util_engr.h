 /*
 util_engr.h
 Copyright(C),2018-2020, 涂鸦科技 www.tuya.comm
 */
#ifndef _ENG_UTIL_H
#define _ENG_UTIL_H

#include "tuya_cloud_types.h"
#include "ty_cJSON.h"

#ifdef __cplusplus
 extern "C" {
#endif

#ifdef  __ENG_UTIL_GLOBALS
    #define __ENG_UTIL_EXT
#else
    #define __ENG_UTIL_EXT extern
#endif

#define CLOUD_CFG_FILE          "cloud.config"
#define APP_CFG_FILE            "app.config"
#define SUB_DEV_FILE           "sub_dev.config"

#define USER_DB_FILE           "tuya_user.db"
#define ENCODE_KEY_FILE        "tuya_enckey.db"
#define ZIGBEE_HOST_CFG        "devices.txt"
#define ZIGBEE_HOST_CFG1       "devices1.txt"
#define ZIGBEE_HOST_IAS        "iaszone.txt"
#define DEV_FLAG_CFG           "devflag.ini"

__ENG_UTIL_EXT \
OPERATE_RET gw_get_schema(IN CONST CHAR_T *product_key, OUT ty_cJSON **pp_result);

__ENG_UTIL_EXT \
OPERATE_RET gw_set_channel(IN CONST CHAR_T *data, IN CONST UINT_T len);

__ENG_UTIL_EXT \
OPERATE_RET gw_get_channel(OUT INT_T *channel);

__ENG_UTIL_EXT \
VOID set_config_path(IN CONST CHAR_T *fs_storge_path);

__ENG_UTIL_EXT \
CHAR_T *get_config_path(VOID);

__ENG_UTIL_EXT \
OPERATE_RET gw_engineer_delete_config(VOID);

__ENG_UTIL_EXT \
OPERATE_RET gw_engineer_sync_default_config(VOID);

__ENG_UTIL_EXT \
OPERATE_RET gw_read_file(IN CHAR_T *path, OUT CHAR_T **pp_result);

__ENG_UTIL_EXT \
OPERATE_RET gw_single_dev_bind(VOID);

__ENG_UTIL_EXT \
VOID set_eng_gw_active(GW_WORK_STAT_T stat, INT_T *channel);

__ENG_UTIL_EXT \
OPERATE_RET gw_engr_to_normal(IN CONST CHAR_T *engineer_path, ENGR_TO_NORMAL_FINISH_CB cb);

__ENG_UTIL_EXT \
OPERATE_RET gw_engineer_add_sub_dev_to_list(IN CONST CHAR_T *dev_info, IN CONST UINT_T len);

#ifdef __cplusplus
}
#endif
#endif


