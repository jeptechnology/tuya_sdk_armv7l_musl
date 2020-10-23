#ifndef HOME_SECURITY_H
#define HOME_SECURITY_H

#include "tuya_cloud_types.h"
#include "tuya_cloud_com_defs.h"
#include "uni_msg_queue.h"
#include "ty_cJSON.h"
#if defined(ENABLE_ALARM) && (ENABLE_ALARM==1)


#ifdef __cplusplus
extern "C" {
#endif


#define HOME_SECURITY_COUNTDOWN_TIME 10
#define HOME_SECURITY_MODE_TID_STR_LEN_MAX 64
#define HOME_SECURITY_UID_LEN    32
#define HOME_SECURITY_SID_LEN    32



#define ALARM_CMD_TYPE_MODE               "MODE"
#define ALARM_CMD_ENABLE_DELAY_TIME       "ENABLE_DELAY_TIME"
#define ALARM_CMD_DELAY_TIME              "DELAY_TIMES"
#define ALARM_CMD_IGNORE_DEVICE_QUERY     "IGNORE_DEVICE_QUERY"
#define ALARM_CMD_ABNORMAL_DEFINE_UPD     "ERR_DEFINE_UPD"
#define ALARM_CMD_STATE_QUERY             "STATE_QUERY"

#define ALARM_CMD_PRE_STATE_REPORT        "PRE_STATE_REPORT" //开始布防上报
#define ALARM_CMD_STATE_REPORT            "STATE_REPORT"

#define ALARM_CMD_BYPASS_DEVICE_QUERY     "BYPASS_DEVICE_QUERY"
#define ALARM_CMD_BYPASS_DEVICE           "BYPASS_DEVICE"

#define ALARM_CMD_ENABLE_COUNTDOWN        "ENABLE_COUNTDOWN"
#define ALARM_CMD_ALARM_COUNTDOWN         "ALARM_COUNTDOWN"
#define ALARM_CMD_IGNORE_DEVICE           "IGNORE_DEVICE"
#define ALARM_CMD_NET_MODE                "NET_MODE"
#define ALARM_CMD_CANCEL_ALARM            "CANCEL_ALARM"
#define ALARM_CMD_ALARM_STATTE            "ALARM_STATE"
#define ALARM_CAUSE                       "ALARM_CAUSE" //报警例表上报

#define CONDTION_ALARM_STR   "$mode"

#define IGNORE_DEVICE_LIST_KYE "ignore_list"
#define DELAY_TIME_LIST_KYE    "delay_time_list"
#define ABNORMAL_DEV_LIST_KYE    "abnormal_dev_list"
#define SID_INFO_KEY           "security_sid"



typedef BYTE_T HOME_SECURITY_MODE_STAGE;
#define HOME_SECURITY_STATE_START      0       // start
#define HOME_SECURITY_STATE_DONE       1       // done


typedef enum  {
    HOME_ALARM_CMD_SUCCUESS=0,
    HOME_ALARM_CMD_DONOT_INIT,
}GW_HOME_SECURITY_CMD_RESULT;

#define CMD_DONOT_INIT_MSG "No init"


typedef enum  {
    HOME_ALARM_STATE_DISARMED=0,   //未报警
    HOME_ALARM_STATE_DELAY_ALARM,  //延时中
    HOME_ALARM_STATE_ALARMING,     //正在报警
}GW_HOME_SECURITY_ALARM_STATE;


typedef struct {
    //HOME_SECURITY_MODE security_mode;  //安防模式
    UINT_T staying_delay_time;  //在家布防时设置的布防延时时间
    UINT_T leaving_delay_time;  //离家布防时设置的布防延时时间
    UINT_T enable_countdown;  //延时布防的延时倒计时
    INT_T countdown_time_num;
    BYTE_T *ignore_id_list_tmp;  //忽略设备例表临时缓存
    BYTE_T *ignore_id_list;      //忽略设备例表
    CHAR_T *abnormal_dev_list;
    BOOL_T time_start;    //延时布防的延时，true表明在运行
    BOOL_T init_status;
    TM_MSG_S *tmm_security_time;     //延时触发总倒计时
    TM_MSG_S *tmm_security_countdown;//延时触发倒计时定时器
    TIMER_ID  tmm_security_alarm_time;//延时报警总倒计时
    TM_MSG_S *tmm_security_alarm_countdown;//延时报警倒计时定时器
    TM_MSG_S *tmm_enable_def_time;//获取默认延时时间
    CHAR_T *delay_json_str;
    CHAR_T security_cur_mode[ALARM_SECURITY_MODE_STR_LEN_MAX+1];
    UINT_T alarm_countdown;
    INT_T alarm_countdown_time_num;
    GW_HOME_SECURITY_ALARM_STATE home_alarm_state;
    ALARM_DELAY_STATE home_alarm_delay_state;
    INT_T enable_def_time;
    PVOID_T p_action;
    CHAR_T dis_report_flag;//disalarm保证不重复上报
    UINT_T sid;//模式切换时要加1，撤防不用变
    CHAR_T sid_str[HOME_SECURITY_SID_LEN+1];
    CHAR_T uid[HOME_SECURITY_UID_LEN+1];
    HOME_SECURITY_INFO_SET_E cmd_from;
    CHAR_T *tid;
    
    BOOL_T alarm_status;
    HOME_SECURITY_NET_MODE net_mode;
    CHAR_T cur_mode_tid[HOME_SECURITY_MODE_TID_STR_LEN_MAX+1];
	CHAR_T *alarm_id_list;
}GW_HOME_SECURITY_IF_S;

BOOL_T check_is_loade_ignore_device(CHAR_T *cid);
OPERATE_RET home_secruity_get_security_mode(OUT CHAR_T *mode_str);
OPERATE_RET home_secruity_mq_cmd_handle(ty_cJSON *json);
OPERATE_RET home_secruity_info_set(IN CONST CHAR_T *mode_str, IN CONST CHAR_T *node_id, IN CONST CHAR_T *delay_str);
OPERATE_RET home_security_net_mode_set_report(HOME_SECURITY_NET_MODE net_mode);
VOID home_security_alarm_start_countdown(UINT_T delay_time);
//BOOL_T home_security_alarm_judge_delay(VOID);
VOID home_security_alarm_set_delay_start_status(BOOL_T is_bool);
BOOL_T home_security_alarm_get_delay_start_status(VOID);
ALARM_DELAY_STATE home_security_get_alarm_delay_status(VOID);
VOID home_security_set_alarm_delay_status(ALARM_DELAY_STATE alarm_delay_status);
VOID home_security_clear_save(VOID);
OPERATE_RET home_secruity_set_alarm_status(IN CONST BOOL_T alarm_status, IN CONST HOME_SECURITY_INFO_SET_E cmd_from);
BOOL_T home_secruity_get_alarm_status(VOID);
BOOL_T home_security_get_init_status(VOID);
OPERATE_RET home_security_net_mode_get(CHAR_T *net_mode);
VOID home_security_init(VOID);
OPERATE_RET home_secruity_alarm_info_get(OUT ALARM_INFO_S *alarm_info);
OPERATE_RET home_secruity_alarm_dev_new_cb(IN CHAR_T *cid,  IN ty_cJSON *dp_inf);


#ifdef __cplusplus
};
#endif

#endif

#endif

