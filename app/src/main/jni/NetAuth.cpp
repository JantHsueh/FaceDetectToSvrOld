#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <ctype.h>
#include <time.h>

#include "netComm.h"
#include "netLib.h"

#include "NetDeal.h"
#include "maintainOutLib.h"
#include "faceLib.h"


/* 最后一条上传记录时间 */
time_t lastRecordTime = 0;
char s_nVerifyTrapMode = 0;
int g_AuthFlag = 0;
int g_CompareFlag = 0;	//0没返回,1成功2,失败,3接收注册人脸成功,4接收注册人脸失败
//服务器IP设置状态
int g_ServerSockFlag = 0;

#define _TERMINAL_TYPE_NO      "0" /* 编号 */
#define _SIGNATURE		       ""

/*
**  处理客户应答请求
0-成功;负数-失败
*/
int DealResponse(INLINK_SHARE *StrInlinkShare, PROTOCOL_PACK *InStrProPack, PROTOCOL_PACK *OutStrProPack)
{
	int retVal = 0;
#if 0
	char *pSn = NULL;
	char szPlain[64];
	char szFinal[16];
	char sSN[16 + 1];
	int nPlainLen = 0;
	unsigned char md5HashCode[MD5_CHECK_CODE_LEN] = {0};
	LPAUTH_RAND pRandInfo;
	int nRandom;
	//MD5_CONTEXT md5_context;

	/*  判断数据包类型 */
	if ((InStrProPack->head.subType == ACK) 
		&& (InStrProPack->head.dataLen == 4) 
		&& (InStrProPack->head.index == AUTH_INDEX))
	{
		pRandInfo = (LPAUTH_RAND)InStrProPack->data;
		
		memset(szPlain, 0, sizeof(szPlain));
		memset(szFinal, 0, sizeof(szFinal));
		memset(sSN, 0, sizeof(sSN));

		strcpy(szPlain, VIELI_SERVER_PAWD);
		nPlainLen += strlen(VIELI_SERVER_PAWD);

		pSn = GetSerailStr();
		if(pSn)
		{
			strncpy(sSN, pSn, sizeof(sSN) - 1);
		}

		memcpy(&szPlain[nPlainLen], sSN, sizeof(sSN) - 1);
		nPlainLen += sizeof(sSN)-1;

		nRandom = ntohl(pRandInfo->rand);
		memcpy(&szPlain[nPlainLen], &nRandom, sizeof(nRandom));
		nPlainLen += sizeof(nRandom);
		
		//MD5Init(&md5_context);
		//MD5Update(&md5_context, (unsigned char*)szPlain, nPlainLen);
		//MD5Final(md5HashCode, &md5_context);

		/* 终端收到此认证请求后，返回认证随机数 */
		//retVal = TcpSendPack(PROTOCOL_HEAD_FLAG, InStrProPack->head.index, AUTH,
		//	AUTH_AUTHENTICATE, sizeof( md5HashCode ), (void *)md5HashCode, OutStrProPack);
	}
	else if((InStrProPack->head.subType == ACK) 
		&& (InStrProPack->head.dataLen == 0) 
		&& (InStrProPack->head.index == AUTH_INDEX))
	{
		TRACE("Sucess Sucess Sucess Sucess Sucess ..............%s %d\r\n", __FUNCTION__, __LINE__);
		/* 服务器认证成功 */
		StrInlinkShare->authFlag = 1;

		/* 不需要返回 */
		retVal = -1;
	}
	else if((InStrProPack->head.subType == NAK) 
		&& (InStrProPack->head.dataLen == 0) 
		&& (InStrProPack->head.index == AUTH_INDEX))
	{
		TRACE("Validation fails ..............%s %d\r\n", __FUNCTION__, __LINE__);
		/* 服务器认证失败 */
		StrInlinkShare->authFlag = -2;

		/* 不需要返回 */
		retVal = -1;
	}
	else
	{
		retVal = -1;
	}
#endif
	return retVal;	
}

/* ------------------------------------------------------------------------- */
/*
**  处理客户连接认证请求
**  获取认证随机数成功返回0，认证成功返回1，认证失败返回负数
*/
int DealMsgAuth(INLINK_SHARE *StrInlinkShare, PROTOCOL_PACK *InStrProPack, PROTOCOL_PACK *OutStrProPack)
{
	LOGD("%s %d\r\n", __FUNCTION__, __LINE__);
	int retVal = 1;
	CONFIG_SECURITY_STR strCfgSecurity;

	/*  判断数据包类型 */
	if((InStrProPack->head.subType == NMSG_SUBTYPE_RESPONSE) 
		&& (InStrProPack->head.dataLen >0))
	{//登录成功
		retVal = 1;
		
		g_AuthFlag = 1;
		StrInlinkShare->randFlag = 1;
	}
	else if((InStrProPack->head.subType == NMSG_SUBTYPE_ERROR)
		&& (InStrProPack->head.dataLen == 4))
	{
		retVal = -2;
		g_AuthFlag = -1;

	}
	else//其它，返回错误
	{
		retVal = -1;
		g_AuthFlag = 0;
	}
	return retVal;
}

/**********************************
函数名称： ClientSendAuth
函数功能: 发送与服务器连接鉴权函数
入口参数：
		sockID	: socket ID
返回值  ：
		0	: 成功
		其它: 失败
************************************/
int ClientSendAuth(char* pUserName, char* pPwd)
{
	g_AuthFlag = 0;
	int ret = 0;
	char *pSn = NULL;

	int ClieFd = -1;
	PROTOCOL_PACK OutStrProPack;
	
	memset(&OutStrProPack, 0, sizeof(PROTOCOL_PACK));
	OutStrProPack.data = NULL;

	ClieFd = giClieFd;
	
	LOGIN userInfo;
	int nLen = sizeof(LOGIN);

	memset(&userInfo, 0, sizeof(LOGIN));
	
	userInfo.ver = htonl(FACETCP_VERSION);
	strncpy(userInfo.user, pUserName, strlen(pUserName));
	strncpy(userInfo.pass, pPwd, strlen(pPwd));

	LOGD("login in username[%s] pwd[%s] nLen[%d]\r\n",userInfo.user,userInfo.pass,nLen);

	if(TcpSendPack(PROTOCOL_HEAD_FLAG, AUTH_INDEX, NTYPE_FACECAP_WORK,NMSG_CNT_LOGIN,
			NMSG_SUBTYPE_REQUEST, nLen, (void *)&userInfo, &OutStrProPack) < 0)
	{
		ret = -1;
		goto ClientSendAuth_END;
	}
//LOGD(">>>>>>>>>>>>>>>>>>>>>>> %s %d\r\n",__FUNCTION__, __LINE__);
	if(CommLayerTcp::PushSendDataQueue(ClieFd, &OutStrProPack) == false)
	{
		ret = -1;
		goto ClientSendAuth_END;
	}
	//LOGD(">>>>>>>>>>>>>>>>>>>>>>> %s %d\r\n",__FUNCTION__, __LINE__);
ClientSendAuth_END:

	Free(OutStrProPack.data);
	//LOGD(">>>>>>>>>>>>>>>>>>>>>>> %s %d\r\n",__FUNCTION__, __LINE__);
	return ret;
}

int getAuth()
{
	return g_AuthFlag;
}
int getServerSockFlag()
{
	return g_ServerSockFlag;
}
void setServerSockFlag(int serverSockFlag)
{
	g_ServerSockFlag = serverSockFlag;
}
int getCompareFlag()
{
	return g_CompareFlag;
}
void setCompareFlag(int iCompareFlag)
{
	g_CompareFlag = iCompareFlag;
}