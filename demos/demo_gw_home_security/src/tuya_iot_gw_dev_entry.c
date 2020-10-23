#include "tuya_cloud_types.h"
#include "tuya_cloud_error_code.h"
#include "tuya_cloud_com_defs.h"
#include "tuya_iot_com_api.h"
#include "uni_log.h"

#include "tuya_cloud_base_defs.h"
#include "tuya_iot_base_api.h"
#include "tuya_cloud_wifi_defs.h"
#include "tuya_iot_wired_wifi_api.h"

#include <stdio.h>
#include <unistd.h>
#include <stdint.h>

// UserTODO
// 网关固件版本，用于OTA管理，格式必须为"XX.XX.XX"，其中XX必须为数字
//#ifndef USER_SW_VER
#define USER_SW_VER               "3.2.3"
//#endif

// 网关内部通讯模块固件版本，用于OTA管理，格式必须为"XX.XX.XX"，其中XX必须为数字
#define USER_DEV_IN_GW_SW_VER        "3.0.0"

// 涂鸦云上的产品KEY，需登陆tuya.com创建产品分配唯一key
#define PRODUCT_KEY         "keyk9nqrjnhrt9mj"
// SD本地配置存储路径，该路径必须对应一个可读写文件系统分区
#define CFG_STORAGE_PATH    "./"
// UUID和AUTHKEY用于涂鸦云设备的安全认证，每个设备所用key均为唯一
#define UUID                "tuya19342bc6e2d80143"
#define AUTHKEY             "IbOFAiLzNAxgT84zd1mrjpy0sUSbJBt3"
// OTA文件本地保存绝对路径，SDK会自动进行OTA管理
#define SOC_OTA_FILE        "/tmp/gw_upgrade.ota"


STATIC int  __gw_dev_rev_upgrade_info_cb(IN CONST FW_UG_S *fw);// GW设备升级入口
STATIC VOID __gw_dev_status_changed_cb(IN CONST GW_STATUS_E status);// GW设备云端状态变更回调
STATIC VOID __gw_dev_dp_query_cb(IN CONST TY_DP_QUERY_S *dp_qry);// GW设备特定数据查询入口
STATIC int __dev_ug_inform_cb(IN CONST CHAR_T *dev_id,IN CONST FW_UG_S *fw);
STATIC VOID __dev_reset_cb(IN CONST CHAR_T *dev_id,IN CONST DEV_RESET_TYPE_E type);
STATIC VOID __gw_dev_obj_dp_cmd_cb(IN CONST TY_RECV_OBJ_DP_S *dp);// GW设备格式化指令数据下发入口
STATIC VOID __gw_dev_raw_dp_cmd_cb(IN CONST TY_RECV_RAW_DP_S *dp);// GW设备透传指令数据下发入口
STATIC VOID __gw_dev_restart_req_cb(GW_RESET_TYPE_E type);// SOC设备进程重启请求入口

STATIC VOID __get_nw_status(IN CONST GW_BASE_NW_STAT_T stat);
STATIC VOID __get_wf_nw_status(IN CONST GW_WIFI_NW_STAT_E stat);

STATIC BOOL_T __gw_permit_add_dev_cb(IN CONST GW_PERMIT_DEV_TP_T tp,IN CONST BOOL_T permit,IN CONST UINT_T timeout);
STATIC VOID __gw_dev_del_cb(IN CONST CHAR_T *dev_id);
STATIC OPERATE_RET __gw_dev_grp_infm_cb(IN CONST GRP_ACTION_E action,IN CONST CHAR_T *dev_id,IN CONST CHAR_T *grp_id);
STATIC OPERATE_RET __gw_dev_scene_infm_cb(IN CONST SCE_ACTION_E action,IN CONST CHAR_T *dev_id,\
        IN CONST CHAR_T *grp_id,IN CONST CHAR_T *sce_id);
STATIC VOID __gw_bind_dev_inform_cb(IN CONST CHAR_T *dev_id, IN CONST OPERATE_RET op_ret);

STATIC VOID app_log_path_cb(OUT CONST CHAR_T *path, IN CONST INT_T len);

#if defined(ENABLE_ALARM) && (ENABLE_ALARM==1)
STATIC VOID app_gw_home_security_if_cb(IN CONST CHAR_T *mode_str, IN CONST UINT_T time, BOOL_T is_sound);
STATIC VOID app_gw_home_security_alarm_dev_cb(IN CONST CHAR_T *cid, IN ty_cJSON *dp_inf);
STATIC VOID app_gw_home_security_alarm_env_dev_cb(IN CONST CHAR_T *cid, IN ty_cJSON *dp_inf);
STATIC VOID app_gw_home_security_alarm_delay_status(IN ALARM_DELAY_STATE alarm_status);
STATIC VOID app_gw_home_security_event(IN SECURITY_EVENT_E security_event_status, PVOID_T data);
#endif


int main(int argc, const char *argv[])
{
    printf("IOT SDK Version: %s \r\n", tuya_iot_get_sdk_info());
    printf("PRODUCT_KEY: %s \r\n", PRODUCT_KEY);
    printf("USER_SW_VER: %s \r\n", USER_SW_VER);
    printf("USER_DEV_IN_GW_SW_VER: %s \r\n", USER_DEV_IN_GW_SW_VER);
    printf("CFG_STORAGE_PATH: %s \r\n", CFG_STORAGE_PATH);
    printf("UUID: %s \r\n", UUID);
    printf("AUTHKEY: %s \r\n", AUTHKEY);

    OPERATE_RET op_ret = tuya_iot_init(CFG_STORAGE_PATH);
    if(OPRT_OK != op_ret) {
        PR_ERR("tuya_iot_init err:%d PATH:%s", op_ret, CFG_STORAGE_PATH);
        return -1;
    }
    PR_DEBUG("tuya_iot_init success");

    WF_GW_PROD_INFO_S prod_info = {UUID, AUTHKEY, NULL, NULL};
    op_ret = tuya_iot_set_wired_wifi_gw_prod_info(&prod_info);
    if(OPRT_OK != op_ret) {
        PR_ERR("tuya_iot_set_wired_wifi_gw_prod_info err:%d", op_ret);
        return -2;
    }
    PR_DEBUG("tuya_iot_set_gw_prod_info success");

    TY_IOT_CBS_S iot_cbs = {
        __gw_dev_status_changed_cb,
        __gw_dev_rev_upgrade_info_cb,
        __gw_dev_restart_req_cb,
        __gw_dev_obj_dp_cmd_cb,
        __gw_dev_raw_dp_cmd_cb,
        __gw_dev_dp_query_cb,
        __dev_ug_inform_cb,
        __dev_reset_cb,
#if defined(TUYA_GW_OPERATOR) && (TUYA_GW_OPERATOR==1)
        NULL,
#endif
#if defined(ENABLE_ALARM) && (ENABLE_ALARM==1)
        NULL,
#endif
    };

    TY_IOT_GW_CBS_S gw_cbs = {
        __gw_permit_add_dev_cb,
        __gw_dev_del_cb,
        __gw_dev_grp_infm_cb,
        __gw_dev_scene_infm_cb,
        __gw_bind_dev_inform_cb,
    };

    GW_ATTACH_ATTR_T attr[] = {
        {GP_DEV_ZB, USER_DEV_IN_GW_SW_VER},
    };

    TY_IOT_APP_CBS_S iot_app_cbs =  {
        app_log_path_cb,
#if defined(ENABLE_ALARM) && (ENABLE_ALARM==1)
        app_gw_home_security_if_cb,
        app_gw_home_security_alarm_dev_cb,
        app_gw_home_security_alarm_env_dev_cb,
        app_gw_home_security_alarm_delay_status,
        app_gw_home_security_event,
#endif
    };

    op_ret = tuya_iot_wired_wifi_gw_dev_init(IOT_GW_NET_WIRED_WIFI, GWCM_OLD, WF_START_SMART_FIRST, \
                                                &iot_cbs,&gw_cbs,PRODUCT_KEY,USER_SW_VER,attr,CNTSOF(attr));
    if(OPRT_OK != op_ret) {
        PR_ERR("tuya_iot_wired_wifi_gw_dev_init err:%d",op_ret);
        return -3;
    }
    PR_DEBUG("tuya_iot_wired_wifi_gw_dev_init success");

    op_ret = tuya_iot_reg_get_wired_wifi_nw_stat_cb(__get_nw_status, __get_wf_nw_status);
    if(OPRT_OK != op_ret) {
        PR_ERR("tuya_iot_reg_get_wired_wifi_nw_stat_cb err:%d",op_ret);
        return -4;
    }
    PR_DEBUG("tuya_iot_reg_get_wired_wifi_nw_stat_cb success");

    tuya_iot_app_cbs_init(&iot_app_cbs);

    while (1)
    {
        sleep(5);
    }

    return 0;
}


STATIC VOID app_log_path_cb(OUT CONST CHAR_T *path, IN CONST INT_T len)
{
    PR_DEBUG("log info");
}

// SOC设备升级相关代码开始
STATIC VOID __upgrade_notify_cb(IN CONST FW_UG_S *fw, IN CONST INT_T download_result, IN PVOID_T pri_data)
{
    FILE *p_upgrade_fd = (FILE *)pri_data;
    fclose(p_upgrade_fd);

    if(download_result == 0) {
        PR_DEBUG("Upgrade File Download Success");
        // UserTODO

        //if(fw->tp == FIRMWARE_COMMON_MCU)
        //{
        //    GW_ATTACH_ATTR_T attr[2] = {0};
        //    attr[0].tp = FIRMWARE_COMMON_MCU;
        //    strcpy(attr[0].ver, "1.0.1");
        //    attr[1].tp = FIRMWARE_BLE_MODULE;
        //    strcpy(attr[1].ver, "1.0.0");
        //    tuya_iot_gw_subdevice_update_versions("changcheng05", "1.0.0", attr, 2);
        //}

        //if(fw->tp == FIRMWARE_ZIGBEE_MODULE)
        //{
        //    GW_ATTACH_ATTR_T attr[2] = {0};
        //    attr[0].tp = FIRMWARE_COMMON_MCU;
        //    strcpy(attr[0].ver, "1.0.1");
        //    attr[1].tp = FIRMWARE_BLE_MODULE;
        //    strcpy(attr[1].ver, "1.0.0");
        //    tuya_iot_gw_subdevice_update_versions("changcheng05", "1.2.3", attr, 2);
        //}

        //if(fw->tp == FIRMWARE_BLE_MODULE)
        //{
        //    GW_ATTACH_ATTR_T attr[2] = {0};
        //    attr[0].tp = FIRMWARE_COMMON_MCU;
        //    strcpy(attr[0].ver, "1.0.1");
        //    attr[1].tp = FIRMWARE_BLE_MODULE;
        //    strcpy(attr[1].ver, "1.0.2");
        //    tuya_iot_gw_subdevice_update_versions("changcheng05", "1.2.3", attr, 2);
        //}


    }else {
        PR_ERR("Upgrade File Download Fail.ret = %d", download_result);
    }
}

STATIC OPERATE_RET __get_file_data_cb(IN CONST FW_UG_S *fw, IN CONST UINT_T total_len, IN CONST UINT_T offset,
        IN CONST BYTE_T *data, IN CONST UINT_T len, OUT UINT_T *remain_len, IN PVOID_T pri_data)
{
    //    PR_DEBUG("Rev File Data");
    //    PR_DEBUG("Total_len:%u", total_len);
    //    PR_DEBUG("Offset:%u Len:%u", offset, len);
    FILE *p_upgrade_fd = (FILE *)pri_data;
    fwrite(data, 1, len, p_upgrade_fd);
    *remain_len = 0;

    return OPRT_OK;
}

// GW设备升级入口
int __gw_dev_rev_upgrade_info_cb(IN CONST FW_UG_S *fw)
{
    PR_DEBUG("GW Rev Upgrade Info");
    PR_DEBUG("fw->tp:%d", fw->tp);
    PR_DEBUG("fw->fw_url:%s", fw->fw_url);
    PR_DEBUG("fw->sw_ver:%s", fw->sw_ver);
    PR_DEBUG("fw->file_size:%u", fw->file_size);

    FILE *p_upgrade_fd = fopen(SOC_OTA_FILE, "w+b");
    if(NULL == p_upgrade_fd){
        PR_ERR("open upgrade file fail. upgrade fail %s", SOC_OTA_FILE);
        return OPRT_FILE_NOT_FIND;
    }
    OPERATE_RET op_ret = tuya_iot_upgrade_gw(fw, __get_file_data_cb, __upgrade_notify_cb, p_upgrade_fd);
    if(OPRT_OK != op_ret) {
        PR_ERR("tuya_iot_upgrade_gw err:%d",op_ret);
    }

    return op_ret;
}
// 升级相关代码结束

// GW设备云端状态变更回调
VOID __gw_dev_status_changed_cb(IN CONST GW_STATUS_E status)
{
    PR_DEBUG("GW TUYA-Cloud Status:%d", status);

    if(GW_ACTIVED == status) {
        OPERATE_RET op_ret = OPRT_OK;

        //        op_ret = tuya_iot_gw_bind_dev(GP_DEV_ZB, 0x02000200, "changcheng04", "vaus4pvz", "1.0.0");
        //        if(op_ret != OPRT_OK) {
        //            PR_ERR("add subdevice fail. %d", op_ret);
        //        }

        //        GW_ATTACH_ATTR_T attr[2] = {0};
        //        attr[0].tp = FIRMWARE_COMMON_MCU;
        //        strcpy(attr[0].ver, "1.0.0");
        //        attr[1].tp = FIRMWARE_BLE_MODULE;
        //        strcpy(attr[1].ver, "1.0.0");
        //        op_ret = tuya_iot_gw_bind_dev_attr(GP_DEV_ZB, 0x02000200, "changcheng05", "vaus4pvz", "1.0.0", attr, 2);
        //        if(op_ret != OPRT_OK) {
        //            PR_ERR("add subdevice fail. %d", op_ret);
        //        }
    }
}

// GW设备特定数据查询入口
VOID __gw_dev_dp_query_cb(IN CONST TY_DP_QUERY_S *dp_qry)
{
    PR_DEBUG("GW Rev DP Query Cmd Cid:%s", dp_qry->cid);

    if(dp_qry->cnt == 0) {
        PR_DEBUG("GW rev all dp query");
        // UserTODO
    }else {
        PR_DEBUG("GW rev dp query cnt:%d", dp_qry->cnt);
        UINT_T index = 0;
        for(index = 0; index < dp_qry->cnt; index++) {
            PR_DEBUG("rev dp query:%d", dp_qry->dpid[index]);
            // UserTODO
        }
    }
}

// GW设备格式化指令数据下发入口
VOID __gw_dev_obj_dp_cmd_cb(IN CONST TY_RECV_OBJ_DP_S *dp)
{
    PR_DEBUG("GW Rev DP Obj Cmd t1:%d t2:%d CNT:%u Cid:%s", dp->cmd_tp, dp->dtt_tp, dp->dps_cnt, dp->cid);

    UINT_T index = 0;
    for(index = 0; index < dp->dps_cnt; index++)
    {
        CONST TY_OBJ_DP_S *p_dp_obj = dp->dps + index;
        PR_DEBUG("idx:%d dpid:%d type:%d ts:%u", index, p_dp_obj->dpid, p_dp_obj->type, p_dp_obj->time_stamp);
        switch (p_dp_obj->type) {
            case PROP_BOOL:     { PR_DEBUG("bool value:%d", p_dp_obj->value.dp_bool); break;}
            case PROP_VALUE:    { PR_DEBUG("INT value:%d", p_dp_obj->value.dp_value); break;}
            case PROP_STR:      { PR_DEBUG("str value:%s", p_dp_obj->value.dp_str); break;}
            case PROP_ENUM:     { PR_DEBUG("enum value:%u", p_dp_obj->value.dp_enum); break;}
            case PROP_BITMAP:   { PR_DEBUG("bits value:0x%X", p_dp_obj->value.dp_bitmap); break;}
            default:            { PR_ERR("idx:%d dpid:%d type:%d ts:%u is invalid", index, p_dp_obj->dpid, p_dp_obj->type, p_dp_obj->time_stamp); break;}
        }// end of switch
    }
    // UserTODO

    // 用户处理完成之后需要主动上报最新状态，这里简单起见，直接返回收到的数据，认为处理全部成功。
    OPERATE_RET op_ret = dev_report_dp_json_async(dp->cid,dp->dps,dp->dps_cnt);
    if(OPRT_OK != op_ret) {
        PR_ERR("dev_report_dp_json_async op_ret:%d",op_ret);
    }
}

// GW设备透传指令数据下发入口
VOID __gw_dev_raw_dp_cmd_cb(IN CONST TY_RECV_RAW_DP_S *dp)
{
    PR_DEBUG("SOC Rev DP Raw Cmd t1:%d t2:%d dpid:%d len:%u Cid:%s", dp->cmd_tp, dp->dtt_tp, dp->dpid, dp->len, dp->cid);

    // UserTODO

    // 用户处理完成之后需要主动上报最新状态，这里简单起见，直接返回收到的数据，认为处理全部成功。
    OPERATE_RET op_ret = dev_report_dp_raw_sync(dp->cid,dp->dpid,dp->data,dp->len,0);
    if(OPRT_OK != op_ret) {
        PR_ERR("dev_report_dp_json_async op_ret:%d",op_ret);
    }
}

// GW设备进程重启请求入口
VOID __gw_dev_restart_req_cb(GW_RESET_TYPE_E type)
{
    PR_DEBUG("GW Rev Restart Req, type: %d", type);

    // UserTODO
}

int __dev_ug_inform_cb(IN CONST CHAR_T *dev_id,IN CONST FW_UG_S *fw)
{
    PR_DEBUG("DevID Rev Upgrade Info");
    PR_DEBUG("dev_id:%s", dev_id);
    PR_DEBUG("fw->tp:%d", fw->tp);
    PR_DEBUG("fw->fw_url:%s", fw->fw_url);
    PR_DEBUG("fw->sw_ver:%s", fw->sw_ver);
    PR_DEBUG("fw->file_size:%u", fw->file_size);
    OPERATE_RET op_ret = OPRT_OK;

    FILE *p_upgrade_fd = fopen(SOC_OTA_FILE, "w+b");
    if(NULL == p_upgrade_fd){
        PR_ERR("open upgrade file fail. upgrade fail %s", SOC_OTA_FILE);
        return OPRT_FILE_NOT_FIND;
    }
    op_ret = tuya_iot_upgrade_dev(dev_id, fw, __get_file_data_cb, __upgrade_notify_cb, p_upgrade_fd);
    if(OPRT_OK != op_ret) {
        PR_ERR("tuya_iot_upgrade_gw err:%d",op_ret);
    }
    return op_ret;
}

STATIC VOID __dev_reset_cb(IN CONST CHAR_T *dev_id,IN CONST DEV_RESET_TYPE_E type)
{
    PR_DEBUG("Dev reset, dev id: %s, reset type: %d", dev_id , type);
}

// wired net status callback
STATIC VOID __get_nw_status(IN CONST GW_BASE_NW_STAT_T stat)
{
    PR_DEBUG("wired network status is :%d.", stat);
}

// wifi net status callback
STATIC VOID __get_wf_nw_status(IN CONST GW_WIFI_NW_STAT_E stat)
{
    PR_DEBUG("wifi network status is :%d.", stat);
}

STATIC BOOL_T __gw_permit_add_dev_cb(IN CONST GW_PERMIT_DEV_TP_T tp,IN CONST BOOL_T permit,IN CONST UINT_T timeout)
{
    PR_DEBUG("add dev permit changed. type:%d permit:%s timeout:%u", tp, permit?"true":"false", timeout);
    return TRUE;
}

STATIC VOID __gw_dev_del_cb(IN CONST CHAR_T *dev_id)
{
    PR_DEBUG("device delete cb. dev_id:%s", dev_id);
}

STATIC OPERATE_RET __gw_dev_grp_infm_cb(IN CONST GRP_ACTION_E action,IN CONST CHAR_T *dev_id,IN CONST CHAR_T *grp_id)
{
    PR_DEBUG("group inform cb. action:%d dev_id:%s grp_id:%s", action, dev_id, grp_id);
    return OPRT_OK;
}

STATIC OPERATE_RET __gw_dev_scene_infm_cb(IN CONST SCE_ACTION_E action,IN CONST CHAR_T *dev_id,\
        IN CONST CHAR_T *grp_id,IN CONST CHAR_T *sce_id)
{
    PR_DEBUG("scene inform cb. action:%d dev_id:%s grp_id:%s sce_id:%s", action, dev_id, grp_id, sce_id);
    return OPRT_OK;
}

STATIC VOID __gw_bind_dev_inform_cb(IN CONST CHAR_T *dev_id, IN CONST OPERATE_RET op_ret)
{
    PR_DEBUG("dev bind inform cb. dev_id:%s op_ret:%d", dev_id, op_ret);
}

#if defined(ENABLE_ALARM) && (ENABLE_ALARM==1)
VOID app_gw_home_security_if_cb(IN CONST CHAR_T *mode_str, IN CONST UINT_T time, BOOL_T is_sound)
{
    PR_DEBUG("%s: mode:%s, time: %d, is_sound:%d", __func__, mode_str, time, is_sound);
}

VOID app_gw_home_security_alarm_dev_cb(IN CONST CHAR_T *cid, IN ty_cJSON *dp_inf)
{
    PR_DEBUG("%s: dev id: %s", __func__, cid);
}

VOID app_gw_home_security_alarm_env_dev_cb(IN CONST CHAR_T *cid, IN ty_cJSON *dp_inf)
{
    PR_DEBUG("%s: dev id: %s", __func__, cid);
}

VOID app_gw_home_security_alarm_delay_status(IN ALARM_DELAY_STATE alarm_status)
{
    PR_DEBUG("%s: alarm delay status: %d", __func__, alarm_status);
}

VOID app_gw_home_security_event(IN SECURITY_EVENT_E security_event_status, PVOID_T data)
{
    PR_DEBUG("%s: security event: %d", __func__, security_event_status);
}
#endif
