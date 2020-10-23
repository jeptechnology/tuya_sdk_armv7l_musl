#ifndef __TUYA_WS_DB_HOME_SECURITY_H__
#define __TUYA_WS_DB_HOME_SECURITY_H__


#include "tuya_ws_db.h"
#include "gw_intf.h"
#include "home_security.h"


#ifdef __cplusplus
    extern "C" {
#endif

#if defined(ENABLE_ALARM) && ((ENABLE_ALARM)==1)
/***********************************************************
*  Function: wd_gw_home_security_if_write
*  Input: hsi
*  Output: none
*  Return: OPERATE_RET
***********************************************************/
OPERATE_RET wd_gw_home_security_if_write(IN GW_HOME_SECURITY_IF_S *hsi);


/***********************************************************
*  Function: wd_gw_home_security_if_read
*  Input: none
*  Output: hsi
*  Return: OPERATE_RET
***********************************************************/
OPERATE_RET wd_gw_home_security_if_read(OUT GW_HOME_SECURITY_IF_S *hsi);
#endif

#ifdef __cplusplus
}
#endif

#endif  // __TUYA_WS_DB_DEV_H__

