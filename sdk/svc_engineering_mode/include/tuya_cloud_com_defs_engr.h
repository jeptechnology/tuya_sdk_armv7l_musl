 /*
 tuya_cloud_com_defs.h
 Copyright(C),2018-2020, 涂鸦科技 www.tuya.comm
 */
 
 #ifndef TUYA_CLOUD_COM_DEFS_ENGR_H
 #define TUYA_CLOUD_COM_DEFS_ENGR_H
 
 #include "tuya_cloud_types.h"
 #include "tuya_cloud_com_defs.h"
 
#ifdef __cplusplus
extern "C" {
#endif


/***********************************************************
*  callback function: GW_UG_LAN_INFORM_CB
*  Desc:    gateway ota firmware available nofity callback in lan.
*  Input:   fw: firmware info
***********************************************************/

typedef OPERATE_RET (*GW_UG_LAN_INFORM_CB)(IN CONST FW_UG_S *fw);

typedef OPERATE_RET (*DEV_UG_LAN_INFORM_CB)(IN CONST CHAR_T *dev_id,IN CONST FW_UG_S *fw);

typedef OPERATE_RET (*GW_SET_CHANNEL_CB)(IN INT_T channel);
typedef OPERATE_RET (*GW_GET_CHANNEL_CB)(OUT INT_T *channel);
typedef OPERATE_RET (*GW_GET_LOG_CB)(OUT CHAR_T *path, IN CONST INT_T len);
typedef OPERATE_RET (*GW_SYNC_CONFIG_CB)(VOID);
typedef OPERATE_RET (*GW_ENG_FIN_CB)(VOID);
typedef OPERATE_RET (*LAN_GET_FILE_DATA_CB)(IN CONST FW_UG_S *fw, IN CONST UINT_T total_len,IN CONST UINT_T offset,\
                                                IN CONST BYTE_T *data,IN CONST UINT_T len,OUT UINT_T *remain_len, IN PVOID_T pri_data);
typedef VOID (*LAN_UPGRADE_NOTIFY_CB)(IN CONST FW_UG_S *fw, IN CONST INT_T download_result, IN PVOID_T pri_data);

typedef VOID (*ENGR_TO_NORMAL_FINISH_CB)(IN CONST CHAR_T* path);

/* tuya-sdk gateway callback functions */
typedef struct {
    GW_STATUS_CHANGED_CB gw_status_cb;
	GW_UG_LAN_INFORM_CB gw_lan_ug_cb;  /*!< 局域网网关固件升级通知信息回调函数，可以为NULL */
    GW_RESET_IFM_CB gw_reset_cb;
    DEV_OBJ_DP_CMD_CB dev_obj_dp_cb;
    DEV_RAW_DP_CMD_CB dev_raw_dp_cb;
    DEV_DP_QUERY_CB dev_dp_query_cb;
	DEV_UG_LAN_INFORM_CB dev_lan_ug_cb;

    DEV_RESET_IFM_CB dev_reset_cb;
    GW_SET_CHANNEL_CB gw_set_channel_cb;
    GW_GET_CHANNEL_CB gw_get_channel_cb;
    GW_GET_LOG_CB gw_get_log_cb;
    GW_SYNC_CONFIG_CB gw_sync_config_cb;
    GW_ENG_FIN_CB gw_engineer_finish_cb;
}TY_IOT_CBS_ENGR_S;


/*! 
\typedef typedef VOID (*GW_SCE_PANEL_BIND_CB)(IN CONST CHAR_T *dev_id,IN CONST SCE_PANEL_S *sce_panel,IN CONST INT btn_num);
\brief 场景面板绑定通知回调

\param dev_id [in] 子设备ID
\param sce_panel [in] 场景相关操作
\param btn_num [in] 场景面板操作数
\return VOID
*/
#define GRP_LEN 5
#define SCE_LEN 3
#define SCE_NAME_LEN 4
#define MAX_BTN 9
typedef struct {
    INT_T btn;
    CHAR_T *grp;
    CHAR_T *sce;
    CHAR_T *sce_name;
}SCE_PANEL_S;
typedef OPERATE_RET (*GW_SCE_PANEL_BIND_CB)(IN CONST CHAR_T *dev_id,IN CONST SCE_PANEL_S *sce_panel, \
                                                   IN CONST INT_T btn_num);

/* tuya-sdk gateway subdevices management callback functions */
typedef struct {
    GW_PERMIT_ADD_DEV_CB gw_add_dev_cb;
    GW_DEV_DEL_CB gw_del_cb;
    GW_DEV_GRP_INFM_CB gw_dev_grp_cb;
    GW_DEV_SCENE_INFM_CB gw_dev_scene_cb;
    GW_BIND_DEV_INFORM_CB gw_ifm_cb;
    GW_SCE_PANEL_BIND_CB gw_sce_panel_cb;
}TY_IOT_GW_CBS_ENGR_S;

#ifdef __cplusplus
}
#endif

#endif  /* TUYA_CLOUD_COM_DEFS_ENGR_H */
