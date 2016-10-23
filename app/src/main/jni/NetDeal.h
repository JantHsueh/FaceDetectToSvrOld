
#ifndef NETDEAL_H
#define NETDEAL_H
#include<unistd.h>
#include "netComm.h"
/* 重连服务器超时时间 */
#define RECONNECT_SERVER_TIME 8
extern int g_iCliePort;
/* 终端作为服务器端的IP */
extern char g_acClieIp[16];
extern int g_ipChangeFlag;			//服务器ip　变更标志
extern char g_chUserName[32];
extern char g_chPwd[32];

extern int giClieFd;
//用户模板照片信息
typedef struct
{
	char photoName[256];
	int photoLen;
}tmpl_photo_info_t;

extern int g_AuthFlag;
extern int g_Score;

extern time_t GetTimeFromDate(char *buf);
extern int CopyFileToBuf(char *buf, int n, char *file);
extern char *FeatureDecrypt(char *pEncryFeature, int nFeatureLen, int *pnFeatureSize, int *pnFeatureNum);

int DealMsgGet(INLINK_SHARE *StrInlinkShare, PROTOCOL_PACK *InStrProPack, PROTOCOL_PACK *OutStrProPack);
int DealHeartbeat(INLINK_SHARE *StrInlinkShare, PROTOCOL_PACK *InStrProPack, PROTOCOL_PACK *OutStrProPack);
int DealDoSendHeartbeat(CommLayerTcp *&TcpObj);
int snedGetSvrPicture(unsigned int faceId);
int DealMsgGetFace(INLINK_SHARE *StrInlinkShare, PROTOCOL_PACK *InStrProPack, PROTOCOL_PACK *OutStrProPack);

/* 1.连接建立 */
int DealResponse(INLINK_SHARE *StrInlinkShare, PROTOCOL_PACK *InStrProPack, PROTOCOL_PACK *OutStrProPack);
int DealMsgAuth(INLINK_SHARE *StrInlinkShare, PROTOCOL_PACK *InStrProPack, PROTOCOL_PACK *OutStrProPack);
int ClientSendAuth(char* pUserName, char* pPwd);

int snedPictureToSvr(unsigned char* pPicture, int len);
COMPARE_RESULT get_CompareResult();

int getServerSockFlag();

void setServerSockFlag(int serverSockFlag);

unsigned short check_sum(unsigned char* data, int  data_len); //计算数据校验和

#endif

