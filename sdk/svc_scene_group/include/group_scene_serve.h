/***********************************************************
*  File: smart_frame.h
*  Author: nzy
*  Date: 20170413
***********************************************************/
#ifndef _GROUP_SCENE_SERVE_H
#define _GROUP_SCENE_SERVE_H

#include "tuya_cloud_types.h"
#include "tuya_hal_mutex.h"
#include "uni_thread.h"

#if defined(ENABLE_SUBDEVICE) && (ENABLE_SUBDEVICE==1)

#ifdef __cplusplus
    extern "C" {
#endif
#if defined(OPERATING_SYSTEM) && (OPERATING_SYSTEM!=SYSTEM_LINUX)
#define GRP_SCE_MAX_DP_NUM 32
#define GRP_SCE_FILE_NAME_LEN_MAX 32
#else
#define GRP_SCE_MAX_DP_NUM 64
#define GRP_SCE_FILE_NAME_LEN_MAX 64
#endif
#define GRP_SCE_FLAG_FILE_NAME     "grp_sce_flag_file"
#define GRP_SCE_FILE_NAME          "grp_sce_save_file"
#define GRP_SCE_BACK_FILE_NAME          "grp_sce_save_bak_file"

typedef enum {
    GRP_SEC_ADD = 0, // add a group sce
    GRP_SEC_DEL      // delete a group sce
}GRP_SEC_ACTION_E;


typedef struct grp_sce_dev_list_s {
    struct grp_sce_dev_list_s *next;
    CHAR_T dev_id[DEV_ID_LEN+1];
    BYTE_T dp_id[GRP_SCE_MAX_DP_NUM];
    BYTE_T dp_flag[GRP_SCE_MAX_DP_NUM];
    INT_T dp_num;
}GRP_SCE_DEV_LIST_S;


typedef struct {
    GRP_SCE_DEV_LIST_S *dev_list;
    UINT_T grp_sce_list_num;
    UINT_T uiTime;
}GRP_SCE_TMP_S;


typedef struct grp_sce_cmd_list_s {
    struct grp_sce_cmd_list_s *next;
    USHORT_T grp_id;
    USHORT_T sce_id;
    CHAR_T dev_id[DEV_ID_LEN+1];
    BYTE_T dp_id;
    CHAR_T use_flag;// 1 表明正常查询设备
    UINT_T uiTime;
}GRP_SCE_CMD_LIST_S;


typedef struct {
    GRP_SCE_CMD_LIST_S *cmd_list;
    UINT_T grp_sce_cmd_num;
    UINT_T uiTime;
}GRP_SCE_CMD_S;


typedef struct{
    BOOL_T is_init;
    MUTEX_HANDLE mutex; // mutex
    THRD_HANDLE grpthread;
    BOOL_T grp_sce_process; //发过场景后，再发dp时，认为是新的一轮
    GRP_SCE_TMP_S sce_tmp; //场景配置时的临时缓存    
    GRP_SCE_CMD_S grp_sce_cmd_tmp; //组、场景命令下发的临时缓存
    GRP_SCE_TMP_S grp_sce_query_list; //触发组或场景后要查询的例表
    CHAR_T is_save_flag; //异步存储，主要是解决存储文件大，导致保存慢的问题
    UCHAR_T file_flag;    //备份存储flag
    CHAR_T flag_file_path[GRP_SCE_FILE_NAME_LEN_MAX+1];     //文件含路径名
    CHAR_T file_path[GRP_SCE_FILE_NAME_LEN_MAX+1];     //文件含路径名
    CHAR_T back_file_path[GRP_SCE_FILE_NAME_LEN_MAX+1]; //备份文件含路径名
    CHAR_T *grp_sce_list; // 存储的场景与子设备例表
}GROUP_SCENE_INFO_S;

/*

typedef struct{
    USHORT_T grp_id;
    USHORT_T sce_id;
    USHORT_T dev_num;
    GROUP_SCENE_DEV_INFO_S dev_info[0];
}SIG_GROUP_SCENE_LIST_S;


typedef struct{
    USHORT_T grp_sce_num;
    SIG_GROUP_SCENE_LIST_S grp_sce[0];
}TOTAL_GROUP_SCENE_LIST_S;
*/






/***********************************************************
*  Function: scene_add_store_handle
*  Input: grp_id, sce_id ,dev_id ,dpid
*  Output: none
*  Return: ok, other error.
***********************************************************/
OPERATE_RET scene_add_store_handle(USHORT_T grp_id, USHORT_T sce_id, CHAR_T *dev_id);


/***********************************************************
*  Function: group_add_store_handle
*  Input: grp_id,dev_id
*  Output: none
*  Return: ok, other error.
***********************************************************/
OPERATE_RET group_add_store_handle(USHORT_T grp_id, CHAR_T *dev_id);


/***********************************************************
*  Function: group_scene_del_store_by_grp_dev
*  Input: grp_id ,dev_id
*  Output: none
*  Return: ok, other error.
***********************************************************/
OPERATE_RET group_scene_del_store_by_grp_dev(USHORT_T grp_id, CHAR_T *dev_id);


/***********************************************************
*  Function: group_scene_del_store_by_dev
*  Input: dev_id
*  Output: none
*  Return: ok, other error.
***********************************************************/
OPERATE_RET group_scene_del_store_by_dev(CHAR_T *dev_id);


/***********************************************************
*  Function: group_scene_del_store_by_grp
*  Input: group id
*  Output: none
*  Return: ok, other error.
***********************************************************/
OPERATE_RET group_scene_del_store_by_grp(IN CONST USHORT_T grp_id);

/***********************************************************
*  Function: group_scene_add_scene_tmp
*  Input: dev_id, dp_id
*  Output: none
*  Return: ok, other error.
***********************************************************/
OPERATE_RET group_scene_add_scene_tmp(IN CONST CHAR_T *dev_id, BYTE_T dp_id);


/***********************************************************
*  Function: group_scene_cmd_add
*  Input: grp_id, sce_id, dev_id, dp_id
*  Output: none
*  Return: ok, other error.
***********************************************************/
OPERATE_RET group_scene_cmd_exce(USHORT_T grp_id, USHORT_T sce_id, CHAR_T *dev_id, BYTE_T dp_id);


/***********************************************************
*  Function: get_one_dev_by_group_scene
*  Input: grp_id, sce_id, index
*  Output: dev_id
*  Return: ok, other error.
***********************************************************/
OPERATE_RET get_one_dev_by_group_scene(USHORT_T grp_id, USHORT_T sce_id, INT_T index, CHAR_T *dev_id);

/***********************************************************
*  Function: get_one_group_by_dev
*  Input: grp_id, sce_id, index,dev_id
*  Output: dev_id
*  Return: ok, other error.
***********************************************************/
OPERATE_RET get_one_group_by_dev(USHORT_T *grp_id, USHORT_T *sce_id, INT_T index, CHAR_T *dev_id);


/***********************************************************
*  Function: group_scene_clear_save_data
*  Input: none
*  Output: none
*  Return: ok, other error.
***********************************************************/
OPERATE_RET group_scene_clear_save_data(VOID);

/***********************************************************
*  Function: group_scene_serve_init
*  Input: p_log_seq_path:save path
*  Output: none
*  Return: ok, other error.
***********************************************************/
OPERATE_RET group_scene_serve_init(CONST CHAR_T *p_log_seq_path, CONST IN BOOL_T engineer_mode);

#ifdef __cplusplus
}
#endif
#endif
#endif
