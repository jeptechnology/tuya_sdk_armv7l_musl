#ifndef __LAN_SECURITY_PUB_SUB_H
#define __LAN_SECURITY_PUB_SUB_H

#include "tuya_cloud_com_defs.h"
#include "home_security.h"

#ifdef __cplusplus
	extern "C" {
#endif

#if defined(ENABLE_ALARM) && (ENABLE_ALARM==1)

//布撤防模式，在局域网中同步
typedef struct	{
	CHAR_T mode[ALARM_SECURITY_MODE_STR_LEN_MAX+1];
	CHAR_T sid[HOME_SECURITY_SID_LEN+1];
	INT_T delay;
    TIME_T trig_time;
} LAN_SECURITY_MODE_PARAM_S;

//报警状态，在局域网中同步
typedef struct	{
	BOOL_T alarm;
	INT_T delay;
} LAN_ALARM_STATE_PARAM_S;

typedef OPERATE_RET (*LAN_SECURITY_MODE_REMOTE_CB)(IN CONST LAN_SECURITY_MODE_PARAM_S *param);
typedef OPERATE_RET (*LAN_ALARM_STATE_REMOTE_CB)(IN CONST LAN_ALARM_STATE_PARAM_S *param);

typedef struct	{
	LAN_SECURITY_MODE_REMOTE_CB sec_mode_cb;
	LAN_ALARM_STATE_REMOTE_CB alarm_st_cb;
} LAN_SECURITY_PARAM_S;

OPERATE_RET lan_security_pub_sub_init(LAN_SECURITY_PARAM_S *param);

//布撤防模式 在局域中同步
OPERATE_RET lan_security_mode_set(IN CONST LAN_SECURITY_MODE_PARAM_S *param);

//报警状态 在局域中同步
OPERATE_RET lan_alarm_state_set(IN CONST LAN_ALARM_STATE_PARAM_S *param);

#endif
#ifdef __cplusplus
}
#endif

#endif

