/***********************************************************
*  File: gw_intf.h
*  Author: nzy
*  Date: 20150822
***********************************************************/
#ifndef _GW_INTF_ENGR_H
#define _GW_INTF_ENGR_H


#include "gw_intf.h"
#include "tuya_cloud_com_defs_engr.h"

#include "tuya_cloud_types.h"
#include "tuya_os_adapter.h"
#include "tuya_base_utilities.h"

#include "uni_msg_queue.h"
#include "tuya_cloud_com_defs.h"
#include "tuya_cloud_wifi_defs.h"
#include "tuya_hal_wifi.h"
#include "tuya_cloud_base_defs.h"
#include "tuya_iot_internal_api.h"

#include "hashmap.h"

#ifdef __cplusplus
    extern "C" {
#endif

#ifdef  __GW_INTF_ENGR_GLOBALS
    #define __ENGR_GW_INTF_EXT
#else
    #define __ENGR_GW_INTF_EXT extern
#endif

/***********************************************************
*************************micro define***********************
***********************************************************/

typedef struct {
    // gateway manage
    MUTEX_HANDLE gw_mutex;
    CHAR_T *serial_no; // gw serial num

    GW_WF_CFG_MTHD_SEL mthd;
    BOOL_T enable_station_ap;
    GW_WF_START_MODE wifi_start_mode;

    GW_EXT_STAT_T ext_stat;

    BOOL_T is_oem; // is oem?
    BOOL_T is_init; // has inited?
    BOOL_T is_stride;  //stride pid or key

#if TY_SECURITY_CHIP
    GW_AUTH_INFO_S secu_auth_info;
#endif

    GW_BASE_IF_S gw_base;
    GW_WORK_STAT_MAG_S gw_wsm;
    GW_DESC_IF_S gw_if;
    GW_ACTV_IF_S gw_actv;
    GW_SIGMESH_IF_S gw_sigmesh;

    // device manage
    MUTEX_HANDLE dev_mutex;
    UINT_T dev_num;
    DEV_CNTL_N_S *dev;
    map_t dev_maps;

    /****** gw attribute ******/
    // nofity callback
    TY_IOT_CBS_ENGR_S cbs;
    TY_IOT_GW_CBS_ENGR_S gw_cbs;
    TY_IOT_APP_CBS_S app_cbs;

    // timer message
    TM_MSG_S *tmm_get_url_cfg;
    #if defined(ENABLE_AP_FAST_CONNECT) && (ENABLE_AP_FAST_CONNECT==1)
    TM_MSG_S *tmm_update_connect_ap_info;
    #endif
    TM_MSG_S *tmm_gw_register;
    TM_MSG_S *tmm_gw_actv;
	TM_MSG_S *tmm_gw_pskkey_get;
    TM_MSG_S *tmm_gw_if_sync;
    TM_MSG_S *tmm_dev_bind;
    TM_MSG_S *tmm_dev_if_sync;
    TM_MSG_S *tmm_dev_sigmesh_sync;
    TM_MSG_S *tmm_gw_sigmesh;
#if OPERATING_SYSTEM != SYSTEM_LINUX	
	TM_MSG_S *tmm_mf_close;
#endif
    TM_MSG_S *tmm_force_upd_log;
    TM_MSG_S *tmm_rst_log;//firmware rst log
    TM_MSG_S *tmm_gw_ugcfg;
    TM_MSG_S *tmm_get_dev_list;
    TM_MSG_S *tmm_dev_delete;

    #if defined(WIFI_GW) && (WIFI_GW==1)
    // use for GWCM_LOW_POWER or GWCM_SPCL_MODE
    TM_MSG_S *tmm_short; // 10s
    TM_MSG_S *tmm_long; // 3*60s
    TM_MSG_S *tmm_gw_monitor; // to monitor whether gw is connect to router lql
    TM_MSG_S *tmm_gw_security_verify; // to monitor whether gw is connect to router lql

    // message
    MSG_ID mid_gw_nw_lowpower; // network set low power
    MSG_ID mid_gw_nw_smt_cfg; // network set smartconfig
    MSG_ID mid_gw_nw_cfg_stop; // network stop smartconfig, ap config or bt config
    MSG_ID mid_gw_nw_ap_cfg; // network set ap config
    MSG_ID mid_gw_nw_ap_cfg_stop; // network stop ap config
    MSG_ID mid_wf_reset;
	MSG_ID mid_gw_nw_cfg; //network cfg async routine for bt netcfg, ffs netcfg, etc.
    #endif
    MSG_ID mid_gw_net_conn_set; // network connect set
    MSG_ID mid_gw_actived; // gw actived

    MSG_ID mid_gw_upload_log;
    MSG_ID mid_gw_unregister;
    MSG_ID mid_gw_unactive;

#if defined(ENABLE_SUBDEVICE) && (ENABLE_SUBDEVICE==1)
    MSG_ID mid_dev_unregister;
#endif
    // MSG_ID mid_dev_unbind;

    TM_MSG_S *nw_stat_moni_tm;
    #if defined(WIFI_GW) && (WIFI_GW==1)
    GET_WF_NW_STAT_CB get_wf_nw_stat_cb;
	WF_NW_CFG_ERR_CODE_CB wf_nw_err_code_cb;
	NW_CFG_ERR_CODE_E wf_nw_cfg_err_code;
#if defined(GW_SUPPORT_WIRED_WIFI) && (GW_SUPPORT_WIRED_WIFI==1)
    IOT_GW_NET_TYPE_T net_mode;//网络模式
    CUR_NET_STATUS_T current_net;        //0有线，1无线
    GET_NW_STAT_CB get_nw_stat_cb;
#endif
    #else
    GET_NW_STAT_CB get_nw_stat_cb;
    #endif
	/*用于标识共存配网时，用户实际是通过哪种配网类型，执行配网操作的*/
	int concurrent_nw_cfg_type;
    TIME_S data_update_time;//global data update time
}GW_CNTL_ENGR_S;

/***********************************************************
*************************variable define********************
***********************************************************/


/***********************************************************
*************************function define********************
***********************************************************/
/***********************************************************
*  Function: __gw_cntl_init
*  Input: cfg->gw cfg
*         product_key->gw product key
*         sw_ver->gw version
*.........dev_sw_ver->device version
*.........tp->device type (DEV_NM_ATH_SNGL,DEV_NM_NOT_ATH_SNGL)
*         attr->gw attach attribute array when (cfg->abi >= GW_ENTITY_ATTH_DEV) valid
*         attr_num->gw attach attribute number when (cfg->abi >= GW_ENTITY_ATTH_DEV) valid
*  Output: none
*  Return: OPERATE_RET
***********************************************************/
__ENGR_GW_INTF_EXT \
OPERATE_RET engr_gw_cntl_init(IN CONST GW_CFG_S *cfg,IN CONST CHAR_T *product_key,IN CONST CHAR_T *firmware_key,\
                               IN CONST CHAR_T *sw_ver,IN CONST CHAR_T *dev_sw_ver,\
                               IN CONST DEV_TYPE_T tp,IN CONST GW_ATTACH_ATTR_T *attr,\
                               IN CONST UINT_T attr_num);

#if defined(WIFI_GW) && (WIFI_GW==1)

/***********************************************************
*  Function: gw_wifi_modify_and_conn
*  Input: ssid passwd
*  Output:
*  Return:
*  Note: use at the gateway is active,modify the wifi info
***********************************************************/
__ENGR_GW_INTF_EXT \
OPERATE_RET engr_gw_wifi_modify_and_conn(IN CONST CHAR_T *ssid,IN CONST CHAR_T *passwd);
#endif

/***********************************************************
*  Function: gw_cloud_connect
*  Input: 
*  Output:
*  Return:
***********************************************************/
VOID engr_gw_cloud_connect(VOID);

/***********************************************************
*  Function: gw_unregister
*  Input: rst_t
*  Output:
*  Return:
***********************************************************/
__ENGR_GW_INTF_EXT \
OPERATE_RET engr_gw_unregister(IN CONST GW_RESET_S *rst_t);

/***********************************************************
*  Function: gw_unactive
*  Input: rst_t
*  Output:
*  Return:
***********************************************************/
__ENGR_GW_INTF_EXT \
OPERATE_RET engr_gw_unactive(IN CONST GW_RESET_S *rst_t);

/***********************************************************
*  Function: gw_register_cbs
*  Input: cbs
*  Output: none
*  Return: OPERATE_RET
***********************************************************/
__ENGR_GW_INTF_EXT \
VOID engr_gw_register_cbs(IN CONST TY_IOT_CBS_ENGR_S *cbs);

/***********************************************************
*  Function: gw_register_gw_cbs
*  Input: cbs
*  Output: none
*  Return: OPERATE_RET
***********************************************************/
__ENGR_GW_INTF_EXT \
VOID engr_gw_register_gw_cbs(IN CONST TY_IOT_GW_CBS_ENGR_S *gw_cbs);

/***********************************************************
*  Function: get_gw_dev_cntl
*  Input:
*  Output:
*  Return: GW_CNTL_S
***********************************************************/
__ENGR_GW_INTF_EXT \
DEV_CNTL_N_S *engr_get_gw_dev_cntl(VOID);

/***********************************************************
*  Function: get_gw_nw_status
*  Input:
*  Output:
*  Return: GW_NW_STAT_T
***********************************************************/
__ENGR_GW_INTF_EXT \
GW_NW_STAT_T engr_get_gw_nw_status(VOID);

#if defined(WIFI_GW) && (WIFI_GW==1)
/***********************************************************
*  Function: engr_set_user_def_ap_if
*  Input: ssid passwd
*  Output:
*  Return: OPERATE_RET
*  Note: Must call on the front of the program
***********************************************************/
__ENGR_GW_INTF_EXT \
OPERATE_RET engr_set_user_def_ap_if(IN CONST CHAR_T *ssid,IN CONST CHAR_T *passwd);
#endif

__ENGR_GW_INTF_EXT \
VOID engr_set_max_sta_conn(IN CONST UCHAR_T max_conn);
__ENGR_GW_INTF_EXT \
VOID engr_set_nw_ip(IN CONST CHAR_T* ip, IN CONST CHAR_T* mask, IN CONST CHAR_T* gw);
__ENGR_GW_INTF_EXT \
VOID engr_gw_register_app_cbs(IN CONST TY_IOT_APP_CBS_S *app_cbs);

/***********************************************************
*  Function: set_gw_ext_stat
*  Input: ext_stat
*  Output:
*  Return:
***********************************************************/
__ENGR_GW_INTF_EXT \
VOID engr_set_gw_ext_stat(IN CONST GW_EXT_STAT_T ext_stat);

/***********************************************************
*  Function: get_gw_ext_stat
*  Input:
*  Output:
*  Return: GW_EXT_STAT_T
***********************************************************/
__ENGR_GW_INTF_EXT \
GW_EXT_STAT_T engr_get_gw_ext_stat(VOID);

/***********************************************************
*  Function: gw_lc_bind_device_pk
*  Input:
*  Output:
*  Return:
*  Note: use the prodect key to register
***********************************************************/
__ENGR_GW_INTF_EXT \
OPERATE_RET engr_gw_lc_bind_device_pk(IN CONST DEV_DESC_IF_S *dev_if,IN CONST BOOL_T attach_dp_if);

/***********************************************************
*  Function: gw_lc_unbind_device
*  Input:
*  Output:
*  Return:
*  Note:
***********************************************************/
__ENGR_GW_INTF_EXT \
OPERATE_RET engr_gw_lc_unbind_device(IN CONST CHAR_T *id);

/***********************************************************
*  Function: get_dev_cntl_unlock
*  Input:
*  Output:
*  Return:
*  Note:
***********************************************************/
__ENGR_GW_INTF_EXT \
DEV_CNTL_N_S *engr_get_dev_cntl_unlock(IN CONST CHAR_T *id);

/***********************************************************
*  Function: get_dev_cntl
*  Input:
*  Output:
*  Return:
*  Note:
***********************************************************/
__ENGR_GW_INTF_EXT \
DEV_CNTL_N_S *engr_get_dev_cntl(IN CONST CHAR_T *id);

/***********************************************************
*  Function: get_dev_dp_cntl
*  Input:dev_cntl
*        dpid
*  Output:
*  Return:
*  Note:
***********************************************************/
__ENGR_GW_INTF_EXT \
DP_CNTL_S *engr_get_dev_dp_cntl(IN CONST CHAR_T *id,IN CONST BYTE_T dpid);

/***********************************************************
*  Function: engr_get_gw_cntl
*  Input:
*  Output:
*  Return: GW_CNTL_S
***********************************************************/
__ENGR_GW_INTF_EXT \
GW_CNTL_ENGR_S *engr_get_gw_cntl(VOID);

__ENGR_GW_INTF_EXT \
OPERATE_RET engr_gw_update_version(IN GW_PERMIT_DEV_TP_T type, IN CONST CHAR_T *ver);

/***********************************************************
*  Function: gw_user_token_bind
*  Input: token->user token
*  Output: none
*  Return: OPERATE_RET
***********************************************************/
__ENGR_GW_INTF_EXT \
OPERATE_RET engr_gw_user_token_bind(IN CONST CHAR_T *token);

/***********************************************************
*  Function: gw_get_ip
*  Input: none
*  Output: p_ip
*  Return: OPERATE_RET
***********************************************************/
__ENGR_GW_INTF_EXT \
OPERATE_RET engr_gw_get_ip(NW_IP_S *p_ip);

/***********************************************************
*  Function: gw_get_mac
*  Input: none
*  Output: p_ip
*  Return: OPERATE_RET
***********************************************************/
__ENGR_GW_INTF_EXT \
OPERATE_RET engr_gw_get_mac(NW_MAC_S *p_mac);

/***********************************************************
*  Function: gw_get_rssi
*  Input: none
*  Output: p_rssi if(*p_rssi == 99) then do't support get rssi
*  Return: OPERATE_RET
***********************************************************/
__ENGR_GW_INTF_EXT \
OPERATE_RET engr_gw_get_rssi(SCHAR_T *p_rssi);

#if defined(WIFI_GW) && (WIFI_GW==1)
/***********************************************************
*  Function: get_wf_gw_nw_status
*  Input:
*  Output:nw_stat
*  Return:
***********************************************************/
__ENGR_GW_INTF_EXT \
OPERATE_RET engr_get_wf_gw_nw_status(OUT GW_WIFI_NW_STAT_E *nw_stat);

/***********************************************************
*  Function: get_wf_gw_nw_cfg_err_code
*  Input:
*  Output:wifi nw err code
*  Return:
***********************************************************/
__ENGR_GW_INTF_EXT \
OPERATE_RET engr_get_wf_gw_nw_cfg_err_code(OUT NW_CFG_ERR_CODE_E *nw_cfg_err_code);
#if defined(GW_SUPPORT_WIRED_WIFI) && (GW_SUPPORT_WIRED_WIFI==1)
__ENGR_GW_INTF_EXT \
OPERATE_RET engr_get_base_gw_nw_status(OUT GW_BASE_NW_STAT_T *nw_stat);

__ENGR_GW_INTF_EXT \
VOID engr_get_wired_wifi_gw_net_mode(OUT IOT_GW_NET_TYPE_T *net_mode);

__ENGR_GW_INTF_EXT \
VOID engr_get_wired_wifi_current_net(OUT CUR_NET_STATUS_T *current_net);

__ENGR_GW_INTF_EXT \
VOID engr_set_wired_wifi_current_net(IN CUR_NET_STATUS_T current_net);
#endif
#else
/***********************************************************
*  Function: get_base_gw_nw_status
*  Input:
*  Output:
*  Return:
***********************************************************/
__ENGR_GW_INTF_EXT \
OPERATE_RET engr_get_base_gw_nw_status(OUT GW_BASE_NW_STAT_T *nw_stat);
#endif


#if defined(WIFI_GW) && (WIFI_GW==1)
/***********************************************************
*  Function: gw_wifi_user_cfg
*  Input: ssid passwd token
*  Output: none
*  Return: OPERATE_RET
***********************************************************/
__ENGR_GW_INTF_EXT \
OPERATE_RET gw_wifi_user_cfg(IN CONST CHAR_T *ssid,IN CONST CHAR_T *passwd,IN CONST CHAR_T *token);
#endif

/***********************************************************
*  Function: gw_bind_device
*  Input: tp uddd id pk ver
*  Output: none
*  Return: OPERATE_RET
***********************************************************/
__ENGR_GW_INTF_EXT \
OPERATE_RET engr_gw_bind_device(IN CONST GW_PERMIT_DEV_TP_T tp,IN CONST USER_DEV_DTL_DEF_T uddd, IN CONST USER_DEV_DTL_DEF_T uddd2,\
                           IN CONST CHAR_T *id,IN CONST CHAR_T *pk, IN CONST CHAR_T *firmware_key, IN CONST CHAR_T *ver, \
                           IN CONST GW_ATTACH_ATTR_T *attr,IN CONST UINT_T attr_num, IN CONST BOOL_T oem, \
                           IN CONST CHAR_T *mac, IN CONST CHAR_T *uuid, IN CONST CHAR_T *dev_key);
/***********************************************************
*  Function: gw_subdevice_update
*  Input: id ver
*  Output: none
*  Return: OPERATE_RET
***********************************************************/
__ENGR_GW_INTF_EXT \
OPERATE_RET engr_gw_subdevice_update(IN CONST CHAR_T *id,IN CONST CHAR_T *ver, IN CONST GW_ATTACH_ATTR_T *attr,IN CONST UINT_T attr_num, IN CONST BOOL_T is_force);

/***********************************************************
*  Function: gw_bind_device_inner
*  Input: dev_cntl schema
*  Output: none
*  Return: OPERATE_RET
***********************************************************/
__ENGR_GW_INTF_EXT \
OPERATE_RET engr_gw_bind_device_inner(IN CONST DEV_CNTL_N_S *dev_cntl,IN CONST CHAR_T *schema);

/***********************************************************
*  Function: gw_unbind_device_inner
*  Input: dev_cntl
*  Output: none
*  Return: OPERATE_RET
***********************************************************/
__ENGR_GW_INTF_EXT \
OPERATE_RET engr_gw_unbind_device_inner(IN CONST DEV_CNTL_N_S *dev_cntl);

/***********************************************************
*  Function: gw_unbind_device
*  Input: dev_cntl
*  Output: none
*  Return: OPERATE_RET
***********************************************************/
__ENGR_GW_INTF_EXT \
OPERATE_RET engr_gw_unbind_device(IN CONST DEV_CNTL_N_S *dev_cntl);

__ENGR_GW_INTF_EXT \
VOID engr_gw_dev_lock(VOID);

__ENGR_GW_INTF_EXT \
VOID engr_gw_dev_unlock(VOID);

__ENGR_GW_INTF_EXT \
OPERATE_RET engr_dev_unregister(IN CONST DEV_RESET_S *rst_t);

/***********************************************************
*  Function: gw_dev_traversal
*  Input: iterator->if(*iterator == NULL) then get the first device
*  Output: none
*  Return: OPERATE_RET
***********************************************************/
__ENGR_GW_INTF_EXT \
DEV_DESC_IF_S *engr_gw_dev_traversal_unlock(BOOL_T include_gw, INOUT VOID **iterator);

__ENGR_GW_INTF_EXT \
VOID engr_gw_oem_set(IN CONST BOOL_T oem);

__ENGR_GW_INTF_EXT \
VOID engr_gw_upload_rst_log_set(IN CONST BOOL_T upload);

__ENGR_GW_INTF_EXT \
BOOL_T engr_is_gw_oem(VOID);

__ENGR_GW_INTF_EXT \
OPERATE_RET engr_dev_update_grp_info(IN CONST GRP_ACTION_E action,IN CONST CHAR_T *dev_id, \
                                      IN CONST CHAR_T *grp_id);

__ENGR_GW_INTF_EXT \
OPERATE_RET engr_dev_update_scene_info(IN CONST SCE_ACTION_E action,IN CONST CHAR_T *dev_id, \
                                  IN CONST CHAR_T *grp_id, IN CONST CHAR_T *sce_id);

__ENGR_GW_INTF_EXT \
OPERATE_RET engr_gw_upload_log(CHAR_T *cData, UINT_T iLen);

__ENGR_GW_INTF_EXT \
OPERATE_RET engr_gw_reset_device(IN CONST DEV_CNTL_N_S *dev_cntl);

__ENGR_GW_INTF_EXT \
OPERATE_RET engr_gw_update_versions(VOID);

__ENGR_GW_INTF_EXT \
VOID engr_set_gw_active(GW_WORK_STAT_T stat);

#if defined(WIFI_GW) && (WIFI_GW==1)
__ENGR_GW_INTF_EXT \
OPERATE_RET engr_gw_fast_unactive(IN CONST GW_WF_CFG_MTHD_SEL mthd,IN CONST GW_WF_START_MODE wifi_start_mode);

__ENGR_GW_INTF_EXT \
OPERATE_RET engr_gw_fast_get_nc_type(GW_WF_NWC_FAST_STAT_T *nc_type);
#endif

__ENGR_GW_INTF_EXT \
OPERATE_RET engr_get_gw_sigmesh_net_info(TY_SIGMESH_NET_INFO_S *sigmesh_net_info);

__ENGR_GW_INTF_EXT \
OPERATE_RET engr_get_dev_sigmesh_free_node_list(IN CONST INT_T node_num, INOUT TY_SIGMESH_NODE_LIST_S *node_alloc_list);

__ENGR_GW_INTF_EXT \
OPERATE_RET engr_get_dev_sigmesh_source_node(USHORT_T *source_node);

__ENGR_GW_INTF_EXT \
BOOL_T engr_gw_is_support_sigmesh(VOID);

__ENGR_GW_INTF_EXT \
VOID engr_tmm_get_dev_token_cb(struct s_tm_msg *tm_msg);

__ENGR_GW_INTF_EXT \
VOID engr_gw_dev_schema_delete_all(VOID);

//设置低功耗模式
__ENGR_GW_INTF_EXT \
VOID engr_tuya_cpu_set_lp_mode(BOOL_T lp_enable);


//获取低功耗模式
__ENGR_GW_INTF_EXT \
BOOL_T engr_tuya_cpu_get_lp_mode(VOID);


//获取mac地址
__ENGR_GW_INTF_EXT \
CHAR_T *engr_tuya_get_serialno(VOID);

__ENGR_GW_INTF_EXT \
BOOL_T gw_is_support_home_security(VOID);

__GW_INTF_EXT \
OPERATE_RET engr_gw_set_support_home_security(BOOL_T is_support);

#if defined(ENABLE_SUBDEVICE) && (ENABLE_SUBDEVICE==1)
/**
 * @brief Set the maximum count of sub-devices
 * @param cnt : Maximum count of user allowed
 * @return TRUE is success
 */
__ENGR_GW_INTF_EXT \
BOOL_T engr_gw_set_subdev_max_cnt(UINT_T cnt);

/**
 * @brief Get the count of sub-devices
 * @return UINT_T, The count of sub-devices.
 *         If upgrad from an unrestricted version, the current count may exceed the limit.
 */
__ENGR_GW_INTF_EXT \
UINT_T engr_gw_get_subdev_cnt(VOID);

/**
 * @brief Check current count of sub-devices
 * @return TRUE is over max
 */
__ENGR_GW_INTF_EXT \
BOOL_T engr_gw_is_subdev_cnt_over_max(VOID);
#endif

__ENGR_GW_INTF_EXT \
OPERATE_RET engr_gw_cntl_init_local(OUT UINT_T *channel);

__ENGR_GW_INTF_EXT \
VOID engr_del_all_sub_dev(VOID);

#ifdef __cplusplus
}
#endif
#endif

