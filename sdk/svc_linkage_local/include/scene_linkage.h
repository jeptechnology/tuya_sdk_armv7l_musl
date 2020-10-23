/***********************************************************
*  File: scene_linkage.h
*  Author: nzy
*  Date: 20180723
***********************************************************/
#ifndef __SCENE_LINKAGE_H
    #define __SCENE_LINKAGE_H

    #include "tuya_cloud_types.h"
    #include "tuya_cloud_com_defs.h"
    #include "uni_hlist.h"
    #include "action.h"
    #include "condition.h"

#ifdef __cplusplus
	extern "C" {
#endif

#ifdef  __SCENE_LINKAGE_GLOBALS
    #define __SCENE_LINKAGE_EXT
#else
    #define __SCENE_LINKAGE_EXT extern
#endif

/***********************************************************
*************************micro define***********************
***********************************************************/
typedef BYTE_T LINKAGE_RULE_TYPE_T;

//��https://wiki.tuya-inc.com:7799/pages/viewpage.action?pageId=22086410
#define LINKAGE_RULE_LOCAL     		0 
#define LINKAGE_RULE_LAN     		1 

#define RULE_TYPE_LOCAL_2_PROTOCOL(local) ((local) + 1)
#define RULE_TYPE_PROTOCOL_2_LOCAL(pro)	((pro) - 1)	

#ifdef ENABLE_LAN_LINKAGE_MASTER
#define LINKAGE_RULE_NUM     		2 
#else
#define LINKAGE_RULE_NUM     		1 
#endif

/***********************************************************
*************************variable define********************
***********************************************************/

/***********************************************************
*************************function define********************
***********************************************************/
/***********************************************************
*  Function: scene_linkage_module_init
*  Input: none
*  Output: none
*  Return: OPERATE_RET
*  Note:
***********************************************************/
__SCENE_LINKAGE_EXT \
OPERATE_RET scene_linkage_module_init(VOID);

/***********************************************************
*  Function: scene_linkage_module_uninit
*  Input: none
*  Output: none
*  Return: none
*  Note:
***********************************************************/
__SCENE_LINKAGE_EXT \
VOID scene_linkage_module_uninit(VOID);

__SCENE_LINKAGE_EXT \
VOID scene_linkage_update_from_server(LINKAGE_RULE_TYPE_T linkage_type, INT_T delaymS);

__SCENE_LINKAGE_EXT \
OPERATE_RET scene_linkage_rule_update(IN CONST ty_cJSON *json);


#if defined(LOCAL_SCENE) && (LOCAL_SCENE==1)
__SCENE_LINKAGE_EXT \
VOID scene_linkage_scene_exe(CHAR_T *sceneId);
#endif
/***********************************************************
*  Function: dp_condition_detect
*  Input: id
*  Input: dp_cmd
*  Output: none 
*  Return: OPERATE_RET
***********************************************************/
// Note:
// id:means gwid,if(cid == NULL) then gwid is actual cid
// dp_cmd:{"cid":"xxxxxx",dps:{"1":2,"2":"111"}} or {"devid":"xxxxxx",dps:{"1":2,"2":"111"}}
__SCENE_LINKAGE_EXT \
OPERATE_RET dp_condition_detect(IN CONST CHAR_T *id,IN CONST CHAR_T *dp_cmd, IN CONST CHAR_T *gw_id, IN LINKAGE_RULE_TYPE_T rule_type);

__SCENE_LINKAGE_EXT \
OPERATE_RET scene_linkage_parse_and_add_2_mag(IN CONST CHAR_T *sl_str);

#if defined(ENABLE_ALARM) && (ENABLE_ALARM==1)
__SCENE_LINKAGE_EXT \
OPERATE_RET secne_linkage_parse_alarm(IN CHAR_T *alarmed_mode, OUT CHAR_T **id_list);
#endif


#if defined(ENABLE_LAN_LINKAGE_MASTER) && (ENABLE_LAN_LINKAGE_MASTER==1)

typedef OPERATE_RET (*LAN_ACTION_SET_EXECUTE)(IN CONST CHAR_T *gw_id, IN CONST CHAR_T *dp_cmd, IN BOOL_T check, OUT BOOL_T *is_local);
typedef OPERATE_RET (*LAN_TOGGLE_ACTION_EXECUTE)(IN CONST ACTION_S *action, OUT BOOL_T *is_local);
typedef OPERATE_RET (*LAN_SCENE_EXECUTE)(IN CONST ACTION_S *action, OUT BOOL_T *is_local);
typedef OPERATE_RET (*LAN_LOCAL_SCENE_EXECUTE)(IN CONST ACTION_S *action, OUT BOOL_T *is_local);


__SCENE_LINKAGE_EXT \
OPERATE_RET scene_linkage_reg_lan_action(IN LAN_ACTION_SET_EXECUTE func);

__SCENE_LINKAGE_EXT \
OPERATE_RET scene_linkage_reg_lan_toggle_action(IN LAN_TOGGLE_ACTION_EXECUTE func);

__SCENE_LINKAGE_EXT \
OPERATE_RET scene_linkage_reg_lan_scene_action(LAN_SCENE_EXECUTE func);

__SCENE_LINKAGE_EXT \
OPERATE_RET scene_linkage_reg_lan_local_scene_action(LAN_LOCAL_SCENE_EXECUTE func);

#endif

#ifdef __cplusplus
}
#endif
#endif

