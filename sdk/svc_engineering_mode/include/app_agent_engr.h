/***********************************************************
*  File: app_agent_engr.h
*  Author: nzy
*  Date: 20150618
***********************************************************/
#ifndef _APP_AGENT_ENGR_H
#define _APP_AGENT_ENGR_H

#include "tuya_cloud_types.h"
#include "tuya_cloud_com_defs.h"
#include "gw_intf.h"
#include "app_agent.h"


#ifdef __cplusplus
    extern "C" {
#endif

#ifdef  __APP_AGENT_ENGR_GLOBALS
    #define __APP_AGENT_ENGR_EXT
#else
    #define __APP_AGENT_ENGR_EXT extern
#endif

//group test
#define FRM_GRP_OPER_ENGR 0xd0
#define FRM_GRP_CMD 0xd1
// engineering cmd
#define FRM_GET_SUB_DEV_LINE_STAT 0xe0
#define UPDATE_SUB_DEV_LINE_STAT 0xe1
#define FROM_SCENE_LINKAGE 0xe2
#define RPT_GW_UNACTIVE 0xe3
#define RPT_SUB_DEV_UNBIND 0xe4
#define FROM_SUB_DEV_UPGRADE_INFO 0xe5
#define FROM_DEV_UPGRADE 0xe6
#define RPT_DOWNLOAD_FIRMWARE 0xe7
#define RPT_UPGRADE_PROGRESS 0xe8
#define RPT_UPGRADE_VERSION 0xe9
#define FRM_USER_BIND_REQ_ENGR 0xf0
#define FRM_GET_SUB_DEV_LIST_ENGR 0Xf1
#define FRM_DEL_SUB_DEV_ENGR 0xf2
#define RPT_ADD_SUB_DEV_ENGR 0xf3
#define FRM_SCE_OPER_ENGR 0xf4
#define FRM_DOWNLOAD_CLOUD_CFG_ENGR 0xf5
#define FRM_DOWNLOAD_APP_CFG_ENGR 0xf6
#define FRM_EXPORT_APP_CFG_ENGR 0xf7
#define FRM_SCE_PANEL_CMD 0xf8
#define FRM_GW_RESET 0xf9
#define FRM_CHK_SOFT_VER 0xfa
#define FRM_UPDATE_GW_SOFT 0xfb
#define FRM_SET_CHANNEL 0xfc
#define FRM_GET_CHANNEL 0xfd
#define FRM_GET_TUYA_LOG 0xfe

#define RET_CODE_OK     0
#define RET_CODE_ERR    1

/* tuya sdk definition of log level */
typedef UINT_T FIRMWARE_TYPE;
#define GW_LINK_MOD_FIRMWARE      0
#define GW_ZIGBEE_MOD_FIRMWARE    3 
#define SUB_DEV_LINK_MOD_FIRMWARE 10
#define SUB_DEV_MCU_MOD_FIRMWARE  19

//typedef OPERATE_RET (*AP_CFG_FINISH_CB)(IN CONST CHAR_T *ssid,IN CONST CHAR_T *passwd, IN CONST CHAR_T *token);

__APP_AGENT_ENGR_EXT \
OPERATE_RET engr_lan_pro_cntl_init(BOOL_T wechat);

__APP_AGENT_ENGR_EXT \
OPERATE_RET engr_lan_pro_cntl_exit(VOID);

//__APP_AGENT_ENGR_EXT \
OPERATE_RET engr_lan_ap_nw_cfg_start(AP_CFG_FINISH_CB finish_cb);

__APP_AGENT_ENGR_EXT \
VOID engr_lan_ap_nw_cfg_stop(VOID);

__APP_AGENT_ENGR_EXT \
VOID engr_lan_ap_nw_cfg_error_report(IN CONST CHAR_T *p_desc,IN CONST INT_T error_code);

__APP_AGENT_ENGR_EXT \
OPERATE_RET engr_lan_dp_sata_report(IN CONST VOID *data,IN CONST UINT_T len);

__APP_AGENT_ENGR_EXT \
OPERATE_RET engr_lan_data_report_cb(IN CONST UINT_T fr_type,IN CONST UINT_T ret_code, \
                                     IN CONST BYTE_T *data,IN CONST UINT_T len);

__APP_AGENT_ENGR_EXT \
INT_T engr_lan_pro_cntl_get_valid_connect_cnt(VOID);

__APP_AGENT_ENGR_EXT \
OPERATE_RET engr_lan_sub_dev_bind_report(IN CONST CHAR_T *dev_id, IN CONST CHAR_T *product_key);     

__APP_AGENT_ENGR_EXT \
INT_T engr_lan_pro_cntl_get_valid_connect_cnt(VOID);

__APP_AGENT_ENGR_EXT \
OPERATE_RET engr_lan_gw_unactive_report();

__APP_AGENT_ENGR_EXT \
OPERATE_RET engr_lan_sub_dev_unbind_report(IN CONST CHAR_T *dev_id);

__APP_AGENT_ENGR_EXT \
OPERATE_RET engr_lan_gw_subdevice_update(IN CONST CHAR_T *id,IN CONST CHAR_T *ver, IN CONST GW_ATTACH_ATTR_T *attr,IN CONST UINT_T attr_num, IN CONST BOOL_T is_force);

__APP_AGENT_ENGR_EXT \
VOID lpc_close_all_socket(VOID);

__APP_AGENT_ENGR_EXT \
OPERATE_RET engr_rpt_upload_upgrade_percent(IN CONST UINT_T percent, IN CONST CHAR_T *dev_id, IN CONST CLOUD_DEV_TP_DEF_T tp);

#ifdef __cplusplus
}
#endif
#endif

