/***********************************************************
*  File: scene_linkage_engr.h
*  Author: nzy
*  Date: 20180723
***********************************************************/
#ifndef __SCENE_LINKAGE_ENGR_H
    #define __SCENE_LINKAGE_ENGR_H

    #include "tuya_cloud_types.h"
    #include "tuya_cloud_com_defs.h"
    #include "uni_hlist.h"
    #include "action.h"
    #include "condition.h"
    #include "scene_linkage.h"

#ifdef __cplusplus
	extern "C" {
#endif

#ifdef  __SCENE_LINKAGE_ENGR_GLOBALS
    #define __SCENE_LINKAGE_ENGR_EXT
#else
    #define __SCENE_LINKAGE_ENGR_EXT extern
#endif

/***********************************************************
*************************micro define***********************
***********************************************************/

/***********************************************************
*************************variable define********************
***********************************************************/

/***********************************************************
*************************function define********************
***********************************************************/
/***********************************************************
*  Function: engr_scene_linkage_module_init
*  Input: none
*  Output: none
*  Return: OPERATE_RET
*  Note:
***********************************************************/
__SCENE_LINKAGE_ENGR_EXT \
OPERATE_RET engr_scene_linkage_module_init(VOID);

/***********************************************************
*  Function: scene_linkage_module_uninit
*  Input: none
*  Output: none
*  Return: none
*  Note:
***********************************************************/
__SCENE_LINKAGE_ENGR_EXT \
VOID engr_scene_linkage_module_uninit(VOID);

__SCENE_LINKAGE_ENGR_EXT \
VOID engr_scene_linkage_update_from_server(LINKAGE_RULE_TYPE_T linkage_type, INT_T delaymS);

__SCENE_LINKAGE_ENGR_EXT \
OPERATE_RET engr_scene_linkage_rule_update(IN CONST ty_cJSON *json);


#if defined(LOCAL_SCENE) && (LOCAL_SCENE==1)
__SCENE_LINKAGE_ENGR_EXT \
VOID engr_scene_linkage_scene_exe(CHAR_T *sceneId);
#endif
/***********************************************************
*  Function: engr_dp_condition_detect
*  Input: id
*  Input: dp_cmd
*  Output: none 
*  Return: OPERATE_RET
***********************************************************/
// Note:
// id:means gwid,if(cid == NULL) then gwid is actual cid
// dp_cmd:{"cid":"xxxxxx",dps:{"1":2,"2":"111"}} or {"devid":"xxxxxx",dps:{"1":2,"2":"111"}}
__SCENE_LINKAGE_ENGR_EXT \
OPERATE_RET engr_dp_condition_detect(IN CONST CHAR_T *id,IN CONST CHAR_T *dp_cmd, IN CONST CHAR_T *gw_id, IN LINKAGE_RULE_TYPE_T rule_type);

__SCENE_LINKAGE_ENGR_EXT \
OPERATE_RET engr_scene_linkage_parse_and_add_2_mag(IN CONST CHAR_T *sl_str);

#if defined(ENABLE_ALARM) && (ENABLE_ALARM==1)
__SCENE_LINKAGE_ENGR_EXT \
OPERATE_RET engr_secne_linkage_parse_alarm(IN CHAR_T *alarmed_mode, OUT CHAR_T **id_list);
#endif


#if defined(ENABLE_LAN_LINKAGE_MASTER) && (ENABLE_LAN_LINKAGE_MASTER==1)
__SCENE_LINKAGE_ENGR_EXT \
OPERATE_RET engr_scene_linkage_reg_lan_action(IN LAN_ACTION_SET_EXECUTE func);

__SCENE_LINKAGE_ENGR_EXT \
OPERATE_RET engr_scene_linkage_reg_lan_toggle_action(IN LAN_TOGGLE_ACTION_EXECUTE func);

__SCENE_LINKAGE_ENGR_EXT \
OPERATE_RET engr_scene_linkage_reg_lan_scene_action(LAN_SCENE_EXECUTE func);

__SCENE_LINKAGE_ENGR_EXT \
OPERATE_RET engr_scene_linkage_reg_lan_local_scene_action(LAN_LOCAL_SCENE_EXECUTE func);
#endif

__SCENE_LINKAGE_ENGR_EXT \
OPERATE_RET engr_scene_linkage_module_reinit(VOID);

__SCENE_LINKAGE_ENGR_EXT \
VOID engr_reset_scene_info_to_disk(VOID);

__SCENE_LINKAGE_ENGR_EXT \
BOOL_T engr_check_modify_time_changed(INT_T input_last_mod_time);

__SCENE_LINKAGE_ENGR_EXT \
VOID engr_load_scene_info_from_disk(VOID);

__SCENE_LINKAGE_ENGR_EXT \
OPERATE_RET engr_save_scene_link(INT_T total_cnt, INT_T sl_last_mod_time, ty_cJSON *rules);

#ifdef __cplusplus
}
#endif
#endif

