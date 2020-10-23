#include <string.h>
#include <stdbool.h>
#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <linux/sockios.h>

#include "tuya_hal_wired.h"
#include "tuya_cloud_types.h"
#include "uni_log.h"


#define NET_DEV "eth0"
#define default_wired_ip "192.168.1.254"

int tuya_hal_wired_get_nw_stat(GW_BASE_NW_STAT_T *stat)
{
	return OPRT_OK;
}

// 获取有线网卡的ip地址
OPERATE_RET tuya_hal_wired_get_ip(OUT NW_IP_S *ip)
{
    int sock;
    char ipaddr[50];
    struct   sockaddr_in *sin;
    struct   ifreq ifr;

    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        PR_ERR("socket create failse...GetLocalIp!");
        return OPRT_COM_ERROR;
    }

    memset(&ifr, 0, sizeof(ifr));
    strncpy(ifr.ifr_name, NET_DEV, sizeof(ifr.ifr_name) - 1);
    if( ioctl( sock, SIOCGIFADDR, &ifr) < 0 ) {
        PR_ERR("wired:%s ioctl error", NET_DEV);
        close(sock);
        return OPRT_COM_ERROR;
    }

    sin = (struct sockaddr_in *)&ifr.ifr_addr;
    strcpy(ip->ip,inet_ntoa(sin->sin_addr));
    close(sock);
    return OPRT_OK;
}

// 硬件是否连接外网
bool tuya_hal_wired_station_conn(VOID)
{
    // UserTODO
    return TRUE;
}

// 若硬件形态为wifi+有线模式，而且用户需要连接外部WIFI，那么需要实现连接WIFI回调函数，sdk内部会自动调用
OPERATE_RET tuya_hal_wired_wifi_set_station_connect(IN CONST CHAR_T *ssid,IN CONST CHAR_T *passwd)
{
    // UserTODO
    return OPRT_COM_ERROR;
}

// 若硬件形态为wifi+有线模式，而且用户需要连接外部WIFI，则返回TRUE，否者返回FALSE
bool tuya_hal_wired_wifi_need_cfg(VOID)
{
    // UserTODO
    return FALSE;
}

// 若硬件形态为wifi+有线模式，而且用户需要连接外部WIFI，则返回WIFI实际信号强度，否者返回99
OPERATE_RET tuya_hal_wired_wifi_station_get_conn_ap_rssi(OUT SCHAR_T *rssi)
{
    // UserTODO
    *rssi = 99;
    return OPRT_OK;
}

// 获取有线网卡的MAC地址
OPERATE_RET tuya_hal_wired_get_mac(OUT NW_MAC_S *mac)
{
    // UserTODO

    mac->mac[0] = 0xc8;
    mac->mac[1] = 0x5b;
    mac->mac[2] = 0x76;
    mac->mac[3] = 0x4d;
    mac->mac[4] = 0x75;
    mac->mac[5] = 0xcd;

    return OPRT_OK;
}

// 当前无需实现
OPERATE_RET tuya_hal_wired_set_mac(IN CONST NW_MAC_S *mac)
{
    return OPRT_OK;
}

 // 获取有线的IP地址
STATIC OPERATE_RET wired_inf_get_ip(IN CHAR_T *inf_name, OUT NW_IP_S *ip)
{
    if (inf_name == NULL)
        return OPRT_COM_ERROR;

    char tmpCmd[100] = {0};
    snprintf(tmpCmd, 100, "ifconfig %s",inf_name);

    FILE *pp = popen(tmpCmd, "r");
    if (pp == NULL)
        return OPRT_COM_ERROR;

    char tmp[256];
    memset(tmp, 0, sizeof(tmp));
    while (fgets(tmp, sizeof(tmp), pp) != NULL) {
        char *pIPStart = strstr(tmp, "inet ");
        if(pIPStart != NULL) {/* 目前都是一行里面包含ip gw mask 直接跳出  */
            break;
        }
    }

    /* 查找ip  */
    char *pIPStart = strstr(tmp, "inet addr:");
    if (pIPStart != NULL)
        sscanf(pIPStart + strlen("inet addr:"), "%s", ip->ip);
    else {
        fclose(pp);
        return OPRT_COM_ERROR;
    }

    /* 查找gw  */
    char *pGWStart = strstr(tmp, "broadcast ");
    if(pGWStart != NULL)
        sscanf(pGWStart + strlen("broadcast "), "%s", ip->gw);
    /* 查找mask */
    char *pMASKStart = strstr(tmp, "Mask:");
    if(pMASKStart != NULL)
        sscanf(pMASKStart + strlen("Mask:"), "%s", ip->mask);

    pclose(pp);
    /* PR_DEBUG("%s Get IP:%s", inf_name, ip->ip); */
    return OPRT_OK;
}

int tuya_hal_wired_if_connect_internet(bool *status)
{
    NW_IP_S ipinfo = {0};
    OPERATE_RET op_ret = OPRT_OK;

    op_ret = wired_inf_get_ip(NET_DEV, &ipinfo);
    if(OPRT_OK != op_ret){
        *status = FALSE;
        return OPRT_OK;
    }

    if(0 == strcmp(ipinfo.ip,default_wired_ip)) {
        *status = FALSE;
        return OPRT_OK;
    }

    *status = TRUE;
    return OPRT_OK;
}

