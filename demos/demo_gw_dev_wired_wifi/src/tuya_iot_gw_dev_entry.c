#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include "tuya_cloud_types.h"
#include "tuya_cloud_base_defs.h"
#include "tuya_cloud_wifi_defs.h"
#include "tuya_iot_wired_wifi_api.h"
#include "uni_log.h"

/***********************************************************
*************************micro define***********************
***********************************************************/
// 涂鸦云上的产品KEY，不同产品KEY不相同，需登陆tuya.com创建产品分配唯一key
#define PRODUCT_KEY "GXxoKf27eVjA7x1c" // 此key可用于测试

/***********************************************************
*************************variable define********************
***********************************************************/

/***********************************************************
*************************function define********************
***********************************************************/
OPERATE_RET get_file_data_cb(IN CONST FW_UG_S *fw, IN CONST UINT_T total_len, IN CONST UINT_T offset,
                                 IN CONST BYTE_T *data, IN CONST UINT_T len, OUT UINT_T *remain_len, IN PVOID_T pri_data);
VOID upgrade_notify_cb(IN CONST FW_UG_S *fw, IN CONST INT_T download_result, IN PVOID_T pri_data);
OPERATE_RET dev_get_file_data_cb(IN CONST FW_UG_S *fw, IN CONST UINT_T total_len, IN CONST UINT_T offset,
                                           IN CONST BYTE_T *data, IN CONST UINT_T len, OUT UINT_T *remain_len, IN PVOID_T pri_data);
VOID dev_upgrade_notify_cb(IN CONST FW_UG_S *fw, IN CONST INT_T download_result, IN PVOID_T pri_data);

STATIC VOID app_log_path_cb(OUT CONST CHAR_T *path, IN CONST INT_T len);

// 网关OTA升级通知
// fw->tp <= DEV_NM_NOT_ATH_SNGL:指示网关本身的应用程序更新
// fw->tp > DEV_NM_NOT_ATH_SNGL:指示网关上特定的适配器模块的固件升级
int __gw_dev_rev_upgrade_info_cb(IN CONST FW_UG_S *fw)
{
    PR_DEBUG("Rev GW Upgrade Info\r\n");
    PR_DEBUG("fw->fw_url:%s\r\n", fw->fw_url);
    PR_DEBUG("fw->sw_ver:%s\r\n", fw->sw_ver);
    PR_DEBUG("fw->file_size:%d\r\n", fw->file_size);

    // 启动网关升级流程
    OPERATE_RET op_ret = OPRT_OK;
    op_ret = tuya_iot_upgrade_gw(fw, get_file_data_cb, upgrade_notify_cb, NULL);
    if(OPRT_OK != op_ret) {
        PR_ERR("tuya_iot_upgrade_gw err:%d",op_ret);
    }
    return OPRT_OK;
}

// 网关升级流程固件获取回调
OPERATE_RET get_file_data_cb(IN CONST FW_UG_S *fw, IN CONST UINT_T total_len, IN CONST UINT_T offset,
                                     IN CONST BYTE_T *data, IN CONST UINT_T len, OUT UINT_T *remain_len, IN PVOID_T pri_data)
{
    PR_DEBUG("Rev File Data\r\n");
    PR_DEBUG("Total_len:%d ", total_len);
    PR_DEBUG("Offset:%d Len:%d\r\n", offset, len);

    return OPRT_OK;
}

// 网关升级流程结果通知
VOID upgrade_notify_cb(IN CONST FW_UG_S *fw, IN CONST INT_T download_result, IN PVOID_T pri_data)
{
    
}

// 子设备OTA升级通知
// dev_id->终端设备id
// fw->tp > DEV_NM_NOT_ATH_SNGL:指示特定类型终端设备设备
int __dev_ug_inform_cb(IN CONST CHAR_T *dev_id,IN CONST FW_UG_S *fw)
{
    PR_DEBUG("DevID Rev Upgrade Info");
    PR_DEBUG("dev_id:%s", dev_id);
    PR_DEBUG("fw->tp:%d", fw->tp);
    PR_DEBUG("fw->fw_url:%s", fw->fw_url);
    PR_DEBUG("fw->sw_ver:%s", fw->sw_ver);
    PR_DEBUG("fw->file_size:%u", fw->file_size);
    OPERATE_RET op_ret = OPRT_OK;

    op_ret = tuya_iot_upgrade_dev(dev_id,fw,dev_get_file_data_cb,dev_upgrade_notify_cb,NULL);
    if(OPRT_OK != op_ret) {
        PR_ERR("tuya_iot_upgrade_dev err:%d",op_ret);
    }
    return OPRT_OK;
}

STATIC VOID __dev_reset_cb(IN CONST CHAR_T *dev_id,IN CONST DEV_RESET_TYPE_E type)
{
    PR_DEBUG("Dev reset, dev id: %s, reset type: %d", dev_id , type);
}

// 终端设备升级流程固件获取回调
OPERATE_RET dev_get_file_data_cb(IN CONST FW_UG_S *fw, IN CONST UINT_T total_len, IN CONST UINT_T offset,
                                           IN CONST BYTE_T *data, IN CONST UINT_T len, OUT UINT_T *remain_len, IN PVOID_T pri_data)
{
    PR_DEBUG("Rev File Data\r\n");
    PR_DEBUG("Total_len:%d ", total_len);
    PR_DEBUG("Offset:%d Len:%d\r\n", offset, len);

    return OPRT_OK;
}

// 终端设备网关升级流程结果通知
VOID dev_upgrade_notify_cb(IN CONST FW_UG_S *fw, IN CONST INT_T download_result, IN PVOID_T pri_data)
{
    
}


// 网关状态通知回调
VOID __gw_dev_status_changed_cb(IN CONST GW_STATUS_E status)
{
    PR_DEBUG("gw status changed to %d \r\n", status);
}

// 设备特定数据查询入口
VOID __gw_dev_dp_query_cb(IN CONST TY_DP_QUERY_S *dp_qry)
{
    PR_DEBUG("GW Rev DP Query Cmd Cid:%s", dp_qry->cid);

    if(dp_qry->cnt == 0) {
        PR_DEBUG("GW rev all dp query");
        // UserTODO
    } else {
        PR_DEBUG("GW rev dp query cnt:%d", dp_qry->cnt);
        UINT_T index = 0;
        for(index = 0; index < dp_qry->cnt; index++) {
            PR_DEBUG("rev dp query:%d", dp_qry->dpid[index]);
            // UserTODO
        }
    }
}

//GW设备格式化指令数据下发入口
VOID __gw_dev_obj_dp_cmd_cb(IN CONST TY_RECV_OBJ_DP_S *dp)
{
    PR_DEBUG("GW Rev DP Obj Cmd t1:%d t2:%d CNT:%u Cid:%s", dp->cmd_tp, dp->dtt_tp, dp->dps_cnt, dp->cid);

    UINT_T index = 0;
    for(index = 0; index < dp->dps_cnt; index++) {
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

// 网关复位回调通知，设备应用程序需要重启
VOID __gw_dev_restart_req_cb(VOID)
{
    PR_DEBUG("gw reset!!!! \r\n");

    // to do add application reset code
    CHAR_T buf[256];
    sprintf(buf,"kill %d",getpid());
    PR_DEBUG("buf:%s",buf);
    system(buf);
}

// 使能设备入网通知回调
BOOL_T gw_add_dev_cb(IN CONST GW_PERMIT_DEV_TP_T tp,IN CONST BOOL_T permit,IN CONST UINT_T timeout)
{
    PR_DEBUG("add dev permit changed. type:%d permit:%s timeout:%u", tp, permit?"true":"false", timeout);
    return TRUE;
}

// 网关设备删除回调通知
VOID gw_del_cb(IN CONST CHAR_T *dev_id)
{
    PR_DEBUG("gw_del_cb id:%s",dev_id);
}


STATIC OPERATE_RET gw_dev_grp_infm_cb(IN CONST GRP_ACTION_E action,IN CONST CHAR_T *dev_id,IN CONST CHAR_T *grp_id)
{
    PR_DEBUG("group inform cb. action:%d dev_id:%s grp_id:%s", action, dev_id, grp_id);
    return OPRT_OK;
}
 
STATIC OPERATE_RET gw_dev_scene_infm_cb(IN CONST SCE_ACTION_E action,IN CONST CHAR_T *dev_id,\
                                         IN CONST CHAR_T *grp_id,IN CONST CHAR_T *sce_id)
{
    PR_DEBUG("scene inform cb. action:%d dev_id:%s grp_id:%s sce_id:%s", action, dev_id, grp_id, sce_id);
    return OPRT_OK;
}

// 子设备涂鸦云绑定通知
// op_ret==OPRT_OK 成功 其他:失败
VOID gw_bind_ifm_cb(IN CONST CHAR_T *dev_id,IN CONST OPERATE_RET op_ret)
{
    PR_DEBUG("gw bind device op_ret:%d",op_ret);

}

STATIC VOID app_log_path_cb(OUT CONST CHAR_T *path, IN CONST INT_T len)
{
    PR_DEBUG("log info");
}

/***********************************************************
*  Function: main
*  Input: argc argv
*  Output: none
*  Return: int
***********************************************************/
int main(int argc,char **argv)
{
    // tuya iot system init
    OPERATE_RET op_ret = OPRT_OK;
    op_ret = tuya_iot_init("./"); // 路径必须对应一个可读写文件系统分区
    if(OPRT_OK != op_ret) {
        PR_ERR("tuya_iot_init err:%d \r\n",op_ret);
        return -1;
    }

    // 设备的uuid ahth_key,用于涂鸦云设备的安全认证，此处为默认调试用，正式产品需向涂鸦申请，每个设备所用key均为唯一
    WF_GW_PROD_INFO_S prod_info = {"0306760068572d0159f1","9OZsAtjjvWL6f8OCSQO4VL5hZAakuU7v"};
    op_ret = tuya_iot_set_wired_wifi_gw_prod_info(&prod_info);
    if(OPRT_OK != op_ret) {
        PR_ERR("tuya_iot_set_gw_prod_info err:%d \r\n",op_ret);
        return -2;
    }

    // 涂鸦网关提供的设备状态/指令下发回调接口
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

    // 涂鸦网关提供的设备添加使能、删除回调接口
    TY_IOT_GW_CBS_S iot_gw_cbs =  {
        gw_add_dev_cb,
        gw_del_cb,
        gw_dev_grp_infm_cb,
        gw_dev_scene_infm_cb,
        gw_bind_ifm_cb,
    };

    // 涂鸦网关上对应的无线管理适配器配置，可支持多种不同的无线适配器，比如zigbee、433共存的情况
    // 版本信息对应着网关上适配器的固件版本，用以维护适配器固件升级用
    GW_ATTACH_ATTR_T attr[] = {
        {GP_DEV_ZB,"1.0.0"},
    };

    TY_IOT_APP_CBS_S iot_app_cbs =  {
        app_log_path_cb,
#if defined(ENABLE_ALARM) && (ENABLE_ALARM==1)
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
#endif
    };

    // 涂鸦iot网关初始化
    op_ret = tuya_iot_wired_wifi_gw_dev_init(IOT_GW_NET_WIRED_WIFI, GWCM_OLD, WF_START_SMART_FIRST, \
                                            &iot_cbs,&iot_gw_cbs,PRODUCT_KEY,USER_SW_VER,attr,CNTSOF(attr));
    if(OPRT_OK != op_ret) {
        PR_ERR("tuya_iot_gw_init err:%d \r\n",op_ret);
        return -3;
    }

    // 涂鸦iot网络状态管理回调注册
    op_ret = tuya_iot_reg_get_wired_wifi_nw_stat_cb(__get_nw_status, __get_wf_nw_status);
    if(OPRT_OK != op_ret) {
        PR_ERR("tuya_iot_reg_get_nw_stat_cb err:%d \r\n",op_ret);
        return -4;
    }

    tuya_iot_app_cbs_init(&iot_app_cbs);

    while (1) {
        sleep(5);
    }

    return 0;
}
