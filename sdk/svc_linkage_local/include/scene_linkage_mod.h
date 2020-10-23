#ifndef _SCENE_LINKAGE_MOD_H
    #define _SCENE_LINKAGE_MOD_H

    #include "tuya_cloud_types.h"
    #include "uni_msg_queue.h"
    #include "uni_time_queue.h"

#ifdef __cplusplus
    extern "C" {
#endif

#ifdef  __SCENE_LINKAGE_MOD_GLOBALS
    #define __SCENE_LINKAGE_MOD_EXT
#else
    #define __SCENE_LINKAGE_MOD_EXT extern
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
__SCENE_LINKAGE_MOD_EXT \
OPERATE_RET scene_linkage_mod_release(VOID);

__SCENE_LINKAGE_MOD_EXT \
OPERATE_RET scene_linkage_mod_init(VOID);

__SCENE_LINKAGE_MOD_EXT \
OPERATE_RET  scene_linkage_mod_get_free_msgid(OUT MSG_ID *msg_id);

__SCENE_LINKAGE_MOD_EXT \
OPERATE_RET  scene_linkage_mod_reg_msgcb(IN CONST MSG_ID msgID,IN CONST MSG_CALLBACK msg_cb);

__SCENE_LINKAGE_MOD_EXT \
OPERATE_RET  scene_linkage_mod_unreg_msgcb(IN CONST MSG_ID msgID);

__SCENE_LINKAGE_MOD_EXT \
OPERATE_RET  scene_linkage_mod_post_msg(IN CONST MSG_ID msgID,IN CONST P_MSG_DATA pMsgData,\
                           IN CONST MSG_DATA_LEN msgDataLen);

__SCENE_LINKAGE_MOD_EXT \
OPERATE_RET  scene_linkage_mod_post_msg_malloc(IN CONST MSG_ID msgID,IN CONST P_MSG_DATA pMsgData,\
                                  IN CONST MSG_DATA_LEN msgDataLen);

__SCENE_LINKAGE_MOD_EXT \
OPERATE_RET  scene_linkage_mod_post_instancy_msg(IN CONST MSG_ID msgID,IN CONST P_MSG_DATA pMsgData,\
                                    IN CONST MSG_DATA_LEN msgDataLen);

__SCENE_LINKAGE_MOD_EXT \
OPERATE_RET  scene_linkage_mod_post_instancy_msg_malloc(IN CONST MSG_ID msgID,IN CONST P_MSG_DATA pMsgData,\
                                           IN CONST MSG_DATA_LEN msgDataLen);

__SCENE_LINKAGE_MOD_EXT \
OPERATE_RET  scene_linkage_mod_cr_tm_msg_hand(IN CONST TM_MSG_CB cb,IN CONST VOID *data,OUT TM_MSG_S **tm_msg);

__SCENE_LINKAGE_MOD_EXT \
OPERATE_RET  scene_linkage_mod_start_tm_msg(IN CONST TM_MSG_S *tm_msg,IN CONST TIME_MS timeCycle,\
                               IN CONST TIMER_TYPE timer_type);

__SCENE_LINKAGE_MOD_EXT \
OPERATE_RET  scene_linkage_mod_stop_tm_msg(IN CONST TM_MSG_S *tm_msg);

__SCENE_LINKAGE_MOD_EXT \
OPERATE_RET  scene_linkage_mod_release_tm_msg(IN CONST TM_MSG_S *tm_msg);

#ifdef __cplusplus
}
#endif
#endif

