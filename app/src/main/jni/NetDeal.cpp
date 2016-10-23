/********************************************************************************
**  Copyright (c) 2012, �����з���˹�Ƽ����޹�˾
**  All rights reserved.
**
**  �ļ����ƣ� NetDeal.cpp
**  �ο�����TCP��UDP��232��485���н���ҵ����
**
**  ��ǰ�汾��1.0
**
**  �������ߣ�������
**  ��������:  2012.5.21
**
**  �޸����ߣ�
**  �޸�����:
********************************************************************************/
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
#include "public.h"


extern int g_AuthFlag;
extern int g_CompareFlag;
char g_chUserName[32] = {0};
char g_chPwd[32] = {0};


//using namespace FrameWork;

/* �ն���Ϊ�ͻ���ʱ������ȫ�ֵ�socket id */
int giClieFd = -1;
int giServerFd[MAX_CONN_NUM] = {-1, -1, -1, -1, -1};

unsigned int gaiResponsePkts = 1; 	//�����ϱ���Ӧ����
unsigned int gLastResponsePkt = 0;  //�����ϱ����ϴ����Ӧ����
unsigned int guiUpResponseNum = 0;  //�����ϱ���¼Ӧ���������30��дһ��ʱ������

COMPARE_RESULT g_CompareResult;
int g_Score = 30;


/**************************************************************\
** �������ƣ� DoTcpServer
** ���ܣ� �ն���Ϊ�������ˣ�Э�鴦����
** ������   
         TcpObj : tcp���Ӷ���
         InFd   : socket id
** ���أ�0-�ɹ�������-ʧ��
** �������ߣ� ��͢��
** �������ڣ� 2012-8-22
** �޸����ߣ� 
** �޸����ڣ� 
\**************************************************************/
int DoTcpServer(CommLayerTcp *&TcpObj, int InFd)
{
	int ret;
	int flag = 0;
	PROTOCOL_PACK InStrProPack, OutStrProPack;
	INLINK_SHARE StrInlinkShare;

	time_t tRecvTime = 0;	/* SDK���յ����ݰ���ʱ�� */
	time_t tNoRecvTime = 0; /* SDK�ղ������ݵ�ʱ�� */

	memset(&OutStrProPack, 0, sizeof(PROTOCOL_PACK));
	OutStrProPack.data = NULL;

	memset(&InStrProPack, 0, sizeof(PROTOCOL_PACK));
	InStrProPack.data = NULL;

	memset(&StrInlinkShare, 0, sizeof(INLINK_SHARE));
	StrInlinkShare.randFlag = 0;
	StrInlinkShare.randNum  = 0;
	StrInlinkShare.authFlag = -1;


	tRecvTime = time(NULL);

	while(1)
	{
		memset(&OutStrProPack, 0, sizeof(PROTOCOL_PACK));
		OutStrProPack.data = NULL;

		memset(&InStrProPack, 0, sizeof(PROTOCOL_PACK));
		InStrProPack.data = NULL;

		ret = -1;

		/* ��ȡ����ͷ���� */
		ret = TcpObj->ReadN(InFd, RECV_TIME_OUT, (void *)(&InStrProPack.head), sizeof(InStrProPack.head));
#if 0
		/* ���retΪ-2������û���յ����ݣ���continue */
		if (ret == -2)
		{
			continue;
		}
#endif
		if(ret == -2)
		{
			/* ���retΪ-2������û���յ����ݣ���continue */
			tNoRecvTime = time(NULL);

			/* �ղ���SDKʱ��Ϊ3���������������� */
			//TRACE("%d ... %s %d\r\n", tNoRecvTime - tRecvTime, __FUNCTION__, __LINE__);
			if(tNoRecvTime - tRecvTime > 30)
			{
				break;
			}
			else
			{	
				continue;
			}
		}

		tRecvTime = time(NULL);

		/* ���retΪ0��Ϊ-1�����������쳣����break������ */
		if ((ret == 0) || (ret == -1))
		{
			LOGD("ret %d %s %d\r\n", ret, __FUNCTION__, __LINE__);
			
			flag = -1;
			
			break;
		}
		
		InStrProPack.head.flag = ntohs(InStrProPack.head.flag);
		InStrProPack.head.index = ntohs(InStrProPack.head.index);
		InStrProPack.head.dataLen = ntohl(InStrProPack.head.dataLen);

		if(InStrProPack.head.dataLen > 0)
		{
			InStrProPack.data = (char *)Malloc(sizeof(char)*(InStrProPack.head.dataLen + 1));
			if(InStrProPack.data == NULL)
			{
				flag = -1;

				break;
			}

			ret = -1;
			memset(InStrProPack.data, 0, sizeof(char)*(InStrProPack.head.dataLen + 1));

			/* ��ȡ�������ݲ��� */
			ret = TcpObj->ReadN(InFd, RECV_TIME_OUT, (void *)InStrProPack.data, InStrProPack.head.dataLen);
			if(ret <= 0)
			{
				flag = -1;
				
				break;
			}

			if((unsigned int)ret != InStrProPack.head.dataLen)
			{
				flag = -1;
				
				break;
			}			
		}

		/* ���б������ݽ��� */
		ret = AllDealTcp(InFd, &StrInlinkShare, &InStrProPack, &OutStrProPack);
		if (ret == 0)
		{
			/* ������ݰ�������Ϊʶ���û���ע�����������ڴ˴����з���Ӧ�����
			   ������2�������͵ľ��崦����������Ӧ��� */
			if ((USER_VERIFY_USER != InStrProPack.head.subType) && (USER_ENROLL_FACE != InStrProPack.head.subType))
			{
				if(CommLayerTcp::PushSendDataQueue(InFd, &OutStrProPack) == false)
				{
				}
			}
		}

		Free(InStrProPack.data);
		Free(OutStrProPack.data);

        usleep(20 * 1000);
	}

	//FreeRecordsGet(&(StrInlinkShare.recordsGet));

	Free(InStrProPack.data);
	Free(OutStrProPack.data);

	return(flag);
}

/**************************************************************\
** �������ƣ� DoTcpClient
** ���ܣ� �ն���Ϊ�ͻ��ˣ�Э�鴦����
** ������   
         TcpObj : tcp���Ӷ���
** ���أ�0-�ɹ�������-ʧ��
** �������ߣ� ��͢��
** �������ڣ� 2012-8-22
** �޸����ߣ� 
** �޸����ڣ� 
\**************************************************************/
int DoTcpClient(CommLayerTcp *&TcpObj)
{
	int ret;
	int flag = 0;
	int ClieFd = -1;
	PROTOCOL_PACK InStrProPack;
	PROTOCOL_PACK OutStrProPack;
	INLINK_SHARE StrInlinkShare;
	bool IfReconn = false;
	time_t tLastTime;
	
	time_t tRecvTime = 0;	/* SDK���յ����ݰ���ʱ�� */
	time_t tNoRecvTime = 0; /* SDK�ղ������ݵ�ʱ�� */

	int i = 0;
	int iClientAuthSucc = 0;
	unsigned char ucProTail[3] = {0x00};

	while (1)
	{
		if(iThreadExit >0)
		{
			//LOGD("quite exit  DoTcpClient %s %d\r\n",__FUNCTION__, __LINE__);
			break;
		}
		memset(&OutStrProPack, 0, sizeof(PROTOCOL_PACK));
		OutStrProPack.data = NULL;

		memset(&InStrProPack, 0, sizeof(PROTOCOL_PACK));
		InStrProPack.data = NULL;

		memset(&StrInlinkShare, 0, sizeof(INLINK_SHARE));
		StrInlinkShare.randFlag = 0;
		StrInlinkShare.randNum = 0;
		StrInlinkShare.authFlag = -1;
		
		IfReconn = false;
		ClieFd = -1;
			
		TcpObj->SetSocketClieFd(-1);
		if(1 == g_ipChangeFlag)
		{
			if(TcpObj->CommLayerTcpInit(TCP_CLIENT, g_acClieIp, g_iCliePort) < 0)
			{
				break;
			}
			g_ipChangeFlag = 0;
			LOGD("ret %d %s %d\r\n", ret, __FUNCTION__, __LINE__);
		}

		/* ���ӷ������� */
		if(!(TcpObj->CreateTcpClient()))
		{
			LOGD(">>>>>>>>>>>>>>>>>>>>>>> %s %d\r\n",__FUNCTION__, __LINE__);

			sleep(RECONNECT_SERVER_TIME);			
			continue;
		}
		
		ClieFd = TcpObj->GetSocketClieFd();

		giClieFd = ClieFd;
		
		//socketed to server
		setServerSockFlag(1);

		//sleep(8);
		
		/* ������֤ */
		ClientSendAuth(g_chUserName, g_chPwd);
		
		tRecvTime = time(NULL);

		while(1)
		{
			if( iThreadExit > 0)
			{
				//LOGD("quite exit  DoTcpClient %s %d\r\n",__FUNCTION__, __LINE__);
				break;
			}
			if(1 == g_ipChangeFlag)
			{
				if(TcpObj->CommLayerTcpInit(TCP_CLIENT, g_acClieIp, g_iCliePort) < 0)
				{
					IfReconn = false;
				}
				else
				{
					IfReconn = true;
				}
				g_ipChangeFlag = 0;
				LOGD("ret %d %s %d\r\n", ret, __FUNCTION__, __LINE__);
				break;
			}
		
			memset(&OutStrProPack, 0, sizeof(PROTOCOL_PACK));
			OutStrProPack.data = NULL;

			memset(&InStrProPack, 0, sizeof(PROTOCOL_PACK));
			InStrProPack.data = NULL;
		
			ret = -1;

			/* ��ȡ����ͷ���� */
			ret = TcpObj->ReadN(ClieFd, RECV_TIME_OUT, (void *)(&InStrProPack.head), sizeof(InStrProPack.head));

			/* ���retΪ0��Ϊ-1�����������쳣����break������ */
			if ((ret == 0) || (ret == -1)/* || (iUploadRecord == -3)*/)
			{
				LOGD("ret %d %s %d\r\n", ret, __FUNCTION__, __LINE__);
				
				IfReconn = true;
				
				break;
			}
			else if(ret == -2)
			{
				/* ���retΪ-2������û���յ����ݣ���continue */
				tNoRecvTime = time(NULL);

				/* �ղ���SDKʱ��Ϊ3���������������� */
				//TRACE("%d ... %s %d\r\n", tNoRecvTime - tRecvTime, __FUNCTION__, __LINE__);
				if(tNoRecvTime - tRecvTime > 30)
				{
					IfReconn = true;
					LOGD("ret %d %s %d\r\n", ret, __FUNCTION__, __LINE__);
					break;
				}
				else
				{			
					LOGD("ret %d %s %d\r\n", ret, __FUNCTION__, __LINE__);
					continue;
				}
			}

			tRecvTime = time(NULL);

			//g_AuthFlag = 1;
			//LOGD("ret %d InStrProPack.head.dataLen %d %s %d\r\n", ret, InStrProPack.head.dataLen, __FUNCTION__, __LINE__);
			if (ret != sizeof(InStrProPack.head))
			{
				LOGD("ret %d InStrProPack.head.dataLen %d %s %d\r\n", ret, InStrProPack.head.dataLen, __FUNCTION__, __LINE__);
				
				IfReconn = true;

				break;
			}

			unsigned short chksum = check_sum((unsigned char*)&InStrProPack.head+1, sizeof(PROTOCOL_HEAD)-1);

			/*
			InStrProPack.head.flag = ntohs(InStrProPack.head.flag);
			InStrProPack.head.index = ntohs(InStrProPack.head.index);
			InStrProPack.head.dataLen = ntohl(InStrProPack.head.dataLen);
			*/
			//InStrProPack.head.flag = ntohs(InStrProPack.head.flag);
			InStrProPack.head.index = ntohl(InStrProPack.head.index);
			InStrProPack.head.pack = ntohs(InStrProPack.head.pack);
			InStrProPack.head.packType = ntohs(InStrProPack.head.packType);
			InStrProPack.head.msgtype = ntohl(InStrProPack.head.msgtype);
			InStrProPack.head.subType = ntohs(InStrProPack.head.subType);
			InStrProPack.head.dataLen = ntohl(InStrProPack.head.dataLen);

			LOGD("%06x",InStrProPack.head.msgtype);
			LOGD("packType %d msgtype %d subType %d %s %d \r\n",
				InStrProPack.head.packType, InStrProPack.head.msgtype,InStrProPack.head.subType, __FUNCTION__, __LINE__);

			if(InStrProPack.head.msgtype != NMSG_CNT_HEARTBEAT)
			{
				LOGD("InStrProPack.head.index %d InStrProPack.head.dataLen %d %s %d \r\n",
				InStrProPack.head.index, InStrProPack.head.dataLen, __FUNCTION__, __LINE__);
				LOGD("sizeof(InStrProPack.head)[%d]",sizeof(InStrProPack.head));
			}
   
            if(InStrProPack.head.dataLen > 0)
			{			
				InStrProPack.data = (char *)malloc(sizeof(char)*(InStrProPack.head.dataLen + 1));
				if(InStrProPack.data == NULL)
				{
					break;
				}
				
				memset(InStrProPack.data, 0, sizeof(char)*(InStrProPack.head.dataLen + 1));

				ret = -1;
				
				/* ��ȡ�������ݲ��� */
				ret = TcpObj->ReadN(ClieFd, RECV_TIME_OUT, (void *)InStrProPack.data, InStrProPack.head.dataLen);
				if((unsigned int)ret != InStrProPack.head.dataLen)
				{
					LOGD("ret %d InStrProPack.head.dataLen %d %s %d\r\n", ret, InStrProPack.head.dataLen, __FUNCTION__, __LINE__);
					
					IfReconn = true;

					break;
				}
			}

			chksum += check_sum((unsigned char*)InStrProPack.data, InStrProPack.head.dataLen);

			//��ȡУ��Ͱ�β�� 
			
			memset(ucProTail, 0, sizeof(ucProTail));
			ret = TcpObj->ReadN(ClieFd, RECV_TIME_OUT, (void *)ucProTail, 3);
			if((unsigned int)ret != 3)
			{
				LOGD("ret %d read usTail fail %s %d\r\n", ret, __FUNCTION__, __LINE__);
				
				IfReconn = true;

				break;
			}

			LOGD(" %s %d\r\n", __FUNCTION__, __LINE__);

			//memcpy(&InStrProPack.chksum, ucProTail, 2);
			//InStrProPack.chksum = ntohs(InStrProPack.chksum);
			unsigned short recvSum = 0;
			memcpy(&recvSum, ucProTail,2);
			recvSum = ntohs(recvSum);

			LOGD("chksum[%d] recvSum[%d] %s %d",chksum,recvSum, __FUNCTION__, __LINE__);

			memcpy(&InStrProPack.csuffix, ucProTail+2, 1);
			//InStrProPack.csuffix = ntohs(InStrProPack.csuffix);
			LOGD("InStrProPack.csuffix[%02x]\r\n",InStrProPack.csuffix);

			// У�����ȽϺͰ�β�Ƚ� 
		//	TRACE("ucCheckXor: %x ucProTail: %x %s %d\r\n",ucCheckXor, ucProTail[0],__FUNCTION__, __LINE__);
			//TRACE("InStrProPack.usTail: %x %s %d\r\n",InStrProPack.usTail, __FUNCTION__, __LINE__);
			if(InStrProPack.csuffix != PROTOCOL_TAIL_FLAG)
			{
				LOGD("InStrProPack.csuffix[%02x] %s %d\r\n",InStrProPack.csuffix, __FUNCTION__, __LINE__);
				//continue;
			}
			
			/* ���б������ݽ��������������ص�����OutStrProPack������� */
			ret = AllDealTcp(ClieFd, &StrInlinkShare, &InStrProPack, &OutStrProPack);
			if (ret == 0)
			{
				/* ������ݰ�������Ϊʶ���û���ע�����������ڴ˴����з���Ӧ�����
				   ������2�������͵ľ��崦����������Ӧ��� */
				/*
				if ((USER_VERIFY_USER != InStrProPack.head.subType) && (USER_ENROLL_FACE != InStrProPack.head.subType))
				{
					if(CommLayerTcp::PushSendDataQueue(ClieFd, &OutStrProPack) == false)
					{
					}
				}
				*/
			}
			Free(InStrProPack.data);
			Free(OutStrProPack.data);

			//��ʱʹ����
			//g_AuthFlag = 1;
			//StrInlinkShare.authFlag = 1;

			/* ��������֤ʧ�ܣ����� */
			if(StrInlinkShare.authFlag == -2)
			{
				LOGD("Reconnect... %s %d\r\n", __FUNCTION__, __LINE__);
				//IfReconn = true;

				//break;
			}
			else if(StrInlinkShare.authFlag == 1)
			{
				iClientAuthSucc = TcpObj->GetClientAuthSucc();
				if(iClientAuthSucc == 0)
				{
					/* ��֤�ɹ������Է����� */
					TcpObj->SetClientAuthSucc(1);
				}
			}

            usleep(20 * 1000);
		}
		TcpObj->CloseSocketClieFd();
		//Free(InStrProPack.data);
		//Free(OutStrProPack.data);
		
		//FreeRecordsGet(&(StrInlinkShare.recordsGet));

		LOGD("IfReconn %d %s %d\r\n", IfReconn, __FUNCTION__, __LINE__);
		
		if(IfReconn)
		{

            giClieFd = -1;

			/* ���� */
			TcpObj->SetClientAuthSucc(0);
            
			//sleep(RECONNECT_SERVER_TIME);
			sleep(1);

			IfReconn = false;

			continue;
		}
		else
		{
			break;
		}
	}

	//LOGD("Exit DoTcpClient %s %d \r\n", __FUNCTION__, __LINE__);
	
	return flag;
}

int dataRcr(unsigned char* data, int len)
{
	unsigned short sum = 0;
	int i = 0;
	for(i = 0; i<len ;i++)
	{
		sum += data[i];
	}
	return sum;
}

unsigned short check_sum(unsigned char* data, int  data_len) //��������У���
{
    if ((NULL == data) || (0 == data_len))
    {
        return 0;
    }

    unsigned short sum = 0;

    unsigned int n = data_len;
    unsigned char * src = data;

    while (n-- > 0)
    {
    	//LOGD("%02x ",*src);
        sum += *src++;
    }

    return sum;
}


/**************************************************************\
** �������ƣ� TcpSendPack
** ���ܣ� ���췢�����ݰ�
** ������
			flag 	 : ����ʶ
			index    : ���
			packType : ������
			subType  : ������
			dataLen  : ���ݳ���
			data     :��������
			OutStrProPack : ��Ӧ��
** ���أ� 0-�ɹ�
          ����-ʧ��
** �������ߣ� ������
** �������ڣ� 2012-5-21
** �޸����ߣ�
** �޸����ڣ�
\**************************************************************/
int TcpSendPack(unsigned short flag, unsigned short index,
			unsigned short packType,unsigned int msgtype, unsigned short subType, unsigned int dataLen, void *data, PROTOCOL_PACK *OutStrProPack)
{
	int i = 0;
	unsigned short  chksum = 0;	
	OutStrProPack->data = NULL;
	OutStrProPack->data = (char *)Malloc(dataLen + 1);
	
	if(OutStrProPack->data == NULL)
	{
		return -1;
	}
	LOGD("send packType[%d] msgtype[%d] subTyep[%d]\r\n",packType,msgtype,subType);
	
	memset(OutStrProPack->data, 0, (dataLen + 1));
	OutStrProPack->head.flag = flag;
	OutStrProPack->head.index = htonl(index);
	OutStrProPack->head.pack = htons(0x00);
	OutStrProPack->head.packType = htons(packType);
	OutStrProPack->head.msgtype = htonl(msgtype);
	OutStrProPack->head.subType	= htons(subType);
	OutStrProPack->head.dataLen	= htonl(dataLen);
	memcpy(OutStrProPack->data, (char *)data, dataLen);

	OutStrProPack->chksum = check_sum((unsigned char*)&OutStrProPack->head+1, sizeof(PROTOCOL_HEAD)-1);

	OutStrProPack->chksum += check_sum((unsigned char*)OutStrProPack->data, dataLen);

	OutStrProPack->chksum = htons(OutStrProPack->chksum);
	//LOGD("<<<<<<<<<<<<<<<<<<<<<<chksum[%02x] OutStrProPack->chksum[%02x]\r\n",chksum,OutStrProPack->chksum);
	OutStrProPack->csuffix = PROTOCOL_TAIL_FLAG;

	//LOGD("<<<<<<<<<<<<<<<<<<<<<<sizeof(OutStrProPack->head)[%d] \r\n",sizeof(OutStrProPack->head));
	
	return 0;
}

/**************************************************************\
** �������ƣ� AllDealTcp
** ���ܣ� Э�������������Ӧ�����ʹ���
** ������ 
		SockFd			: socket id
		StrInlinkShare	: 
		InStrProPack	: ���յ����ݰ�
		OutStrProPack	: Ҫ���͵����ݰ�
** ���أ� 0�ɹ���-1ʧ��
** �������ߣ� ������
** �������ڣ� 2012-5-21
** �޸����ߣ�
** �޸����ڣ�
\**************************************************************/
int AllDealTcp(int SockFd, INLINK_SHARE *StrInlinkShare, PROTOCOL_PACK *InStrProPack,
	PROTOCOL_PACK *OutStrProPack)
{
	int retVal = -1;
	
	LOGD("recv InStrProPack->head.msgtype %d InStrProPack->head.subType %d %s %d\r\n", 
				InStrProPack->head.msgtype, InStrProPack->head.subType, __FUNCTION__, __LINE__);


	if(InStrProPack->head.msgtype != NMSG_CNT_HEARTBEAT)
	{
		//LOGD("InStrProPack->head.packType %d InStrProPack->head.subType %d %s %d\r\n", 
			//InStrProPack->head.packType, InStrProPack->head.subType, __FUNCTION__, __LINE__);
	}

	//�����ֻ����ݱ������ر���
	if(InStrProPack->head.packType != NTYPE_FACECAP_WORK)
	{
		return -1;
	}


	switch(InStrProPack->head.msgtype)		/* �������ݰ����� */
	{
		case NMSG_CNT_LOGIN://�����¼���ؽ��
		{
			StrInlinkShare->authFlag = DealMsgAuth(StrInlinkShare, InStrProPack, OutStrProPack);
			if(StrInlinkShare->authFlag < 0)
			{
				retVal = -1;
			}
			else
			{
				retVal = 0;
			}
			
			break;
		}
		case NMSG_FACE_COMPARE://������֤���ؽ��
		{
			LOGD("InStrProPack->head.packType %d InStrProPack->head.subType %d %s %d\r\n", 
			InStrProPack->head.packType, InStrProPack->head.subType, __FUNCTION__, __LINE__);
			if (StrInlinkShare->authFlag == 1)
			{
				//FreeRecordsGet(&(StrInlinkShare->recordsGet));
				
				retVal = DealMsgGet(StrInlinkShare, InStrProPack, OutStrProPack);
			}
			else
			{
				retVal = -1;
			}
			
			break;
		}
		case NMSG_FLIB_GET_SUB://�����ȡ����ģ����Ƭ
		{
			LOGD("InStrProPack->head.packType %d InStrProPack->head.subType %d %s %d\r\n", 
			InStrProPack->head.packType, InStrProPack->head.subType, __FUNCTION__, __LINE__);
			if (StrInlinkShare->authFlag == 1)
			{
				//FreeRecordsGet(&(StrInlinkShare->recordsGet));
				
				retVal = DealMsgGetFace(StrInlinkShare, InStrProPack, OutStrProPack);
			}
			else
			{
				retVal = -1;
			}
			
			break;
		}
		case NMSG_CNT_HEARTBEAT:
		{
			if ((StrInlinkShare->authFlag == 1) && (InStrProPack->head.subType == 0))
			{
				retVal = DealHeartbeat(StrInlinkShare, InStrProPack, OutStrProPack);
			}
			else
			{
				retVal = -1;
			}

			break;
		}
		
		default://�����ʹ��� 
		{
			retVal = -1;

			break;
		}
	}

	return retVal;
}

/* ------------------------------------------------------------------------- */
/*
**  ����: ��ѯ����GET��
**  ����: ����һ�����ݰ����������ݰ�������������ͬ�Ĵ���
**  ����: �ú�������ɹ�����0�������򷵻�-1
*/
int DealMsgGet(INLINK_SHARE *StrInlinkShare, PROTOCOL_PACK *InStrProPack, PROTOCOL_PACK *OutStrProPack)
{
	int retVal = -1;
	int iret  = 0;
	COMPARE_RESULT compareResult;
	int score = 0;
	
	// �������������Ϣ
	switch(InStrProPack->head.subType)
	{
		case NMSG_SUBTYPE_RESPONSE:
				//g_CompareFlag = 1;
				LOGD("NMSG_SUBTYPE_RESPONSE InStrProPack->head.dataLen[%d]sizeof(COMPARE_RESULT)[%d]\r\n",InStrProPack->head.dataLen,sizeof(COMPARE_RESULT));
				if(InStrProPack->head.dataLen >0 )
				{
					if(InStrProPack->head.dataLen-sizeof(COMPARE_DETECT) == sizeof(COMPARE_RESULT))
					{
						memset(&g_CompareResult, 0x00, sizeof(COMPARE_RESULT));
						memcpy(&g_CompareResult,InStrProPack->data+sizeof(COMPARE_DETECT),sizeof(COMPARE_RESULT));
						//memcpy(&score, InStrProPack->data+sizeof(COMPARE_DETECT),sizeof(int));
						g_CompareResult.score = htonl(g_CompareResult.score);
						g_CompareResult.f_id = htonl(g_CompareResult.f_id);

						/*
						LOGD("\r\n");
						for(int j = 0;j < InStrProPack->head.dataLen ;j++)
						{
							LOGD("%02x ",(char)InStrProPack->data[j]);
						}
						LOGD("\r\n");
						*/
						LOGD("compareResult.score[%u] name[%s]",g_CompareResult.score,g_CompareResult.f_name);
						if(g_CompareResult.score/1000000 > g_Score)
						{
							g_CompareFlag = 1;
							//���Ͷ�ȡע����Ƭ
							
							//int snedGetSvrPicture(unsigned int faceId);
							snedGetSvrPicture(g_CompareResult.f_id);
						}
						else
						{
							g_CompareFlag = 2;
						}
							
					}else
					{
						g_CompareFlag = 2;
					}
					
				}

			break;
		case NMSG_SUBTYPE_ERROR:
				g_CompareFlag = 2;
				LOGD("NMSG_SUBTYPE_ERROR  InStrProPack->head.dataLen[%d]\r\n",InStrProPack->head.dataLen);
				
				memcpy(&iret,InStrProPack->data, sizeof(int));
				iret = htonl(iret);
				LOGD(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>iret[%d]\r\n",iret);

			break;
			
		default:
		{
			retVal = -1;

			break;
		}
	}

	return retVal;
}

int DealMsgGetFace(INLINK_SHARE *StrInlinkShare, PROTOCOL_PACK *InStrProPack, PROTOCOL_PACK *OutStrProPack)
{
	int retVal = -1;
	int iret  = 0;
	COMPARE_RESULT compareResult;
	int score = 0;
	unsigned int faceLen = 0;
	unsigned int faceId = 0;
	char bufFile[256] = {0};
	
	// �������������Ϣ
	switch(InStrProPack->head.subType)
	{
		case NMSG_SUBTYPE_RESPONSE:
				//g_CompareFlag = 1;
				LOGD("NMSG_SUBTYPE_RESPONSE InStrProPack->head.dataLen[%d]sizeof(COMPARE_RESULT)[%d]\r\n",InStrProPack->head.dataLen,sizeof(COMPARE_RESULT));
				if(InStrProPack->head.dataLen >0 )
				{
					memcpy(&faceId, InStrProPack->data,sizeof(unsigned int));
					memcpy(&faceLen, InStrProPack->data+344,sizeof(unsigned int));
					faceId = htonl(faceId);
					faceLen = htonl(faceLen);

					LOGD("79999999999999999999999 InStrProPack->head.dataLen[%d]faceLen[%d]\r\n",InStrProPack->head.dataLen,faceLen);
					
					
					if(InStrProPack->head.dataLen - 344 - 4 == faceLen)
					{
						
						sprintf(bufFile,"/data/data/com.firs.facedetecttosvr/temp/%d.jpg",faceId);
						//������Ƭ
						saveFile(bufFile,(unsigned char*)InStrProPack->data+344+sizeof(unsigned int), faceLen);
						g_CompareFlag = 3;
					}
					/*
					if(InStrProPack->head.dataLen-sizeof(COMPARE_DETECT) == sizeof(COMPARE_RESULT))
					{
						memset(&g_CompareResult, 0x00, sizeof(COMPARE_RESULT));
						memcpy(&g_CompareResult,InStrProPack->data+sizeof(COMPARE_DETECT),sizeof(COMPARE_RESULT));
						//memcpy(&score, InStrProPack->data+sizeof(COMPARE_DETECT),sizeof(int));
						g_CompareResult.score = htonl(g_CompareResult.score);
						g_CompareResult.f_id = htonl(g_CompareResult.f_id);

					
						LOGD("compareResult.score[%u] name[%s]",g_CompareResult.score,g_CompareResult.f_name);
						if(g_CompareResult.score/1000000 > 55)
						{
							g_CompareFlag = 1;
							//���Ͷ�ȡע����Ƭ
							
							//int snedGetSvrPicture(unsigned int faceId);
							snedGetSvrPicture(g_CompareResult.f_id);
						}
						else
						{
							g_CompareFlag = 2;
						}
							
					}else
					{
						g_CompareFlag = 2;
					}

					*/
					
				}

			break;
		case NMSG_SUBTYPE_ERROR:
				g_CompareFlag = 4;
				LOGD("NMSG_SUBTYPE_ERROR  InStrProPack->head.dataLen[%d]\r\n",InStrProPack->head.dataLen);
				
				memcpy(&iret,InStrProPack->data, sizeof(int));
				iret = htonl(iret);
				LOGD(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>iret[%d]\r\n",iret);

			break;
			
		default:
		{
			retVal = -1;

			break;
		}
	}

	return retVal;
}
/**************************************************************\
** �������ƣ� DealHeartbeat
** ���ܣ� ����������
** ������ �ú�������ɹ�����0�������򷵻ظ���
** ���أ�
** �������ߣ� ��͢��
** �������ڣ� 2012-8-22
** �޸����ߣ�
** �޸����ڣ�
\**************************************************************/
int DealHeartbeat(INLINK_SHARE *StrInlinkShare, PROTOCOL_PACK *InStrProPack, PROTOCOL_PACK *OutStrProPack)
{
	int retVal = -1;

	retVal = TcpSendPack(PROTOCOL_HEAD_FLAG, InStrProPack->head.index, NTYPE_FACECAP_WORK,NMSG_CNT_HEARTBEAT,
			NMSG_SUBTYPE_RESPONSE, 0, NULL, OutStrProPack);

	return retVal;
}

/**************************************************************\
** �������ƣ� DealDoSendHeartbeat
** ���ܣ� ��������
** ������ �ú�������ɹ�����0�������򷵻ظ���
** ���أ�
** �������ߣ� ��͢��
** �������ڣ� 2012-8-22
** �޸����ߣ�
** �޸����ڣ�
\**************************************************************/
int DealDoSendHeartbeat(CommLayerTcp *&TcpObj)
{
	PROTOCOL_PACK OutStrProPack;
	int ret = 0;
	int ClieFd;

	ClieFd = TcpObj->GetSocketClieFd();
	
	memset(&OutStrProPack, 0, sizeof(PROTOCOL_PACK));
	OutStrProPack.data = NULL;
	
	if(TcpSendPack(PROTOCOL_HEAD_FLAG, CHECK_INDEX, NTYPE_FACECAP_WORK,NMSG_CNT_HEARTBEAT,
			NMSG_SUBTYPE_REQUEST, 0, NULL, &OutStrProPack) < 0)
	{
		ret = -1;
	}

	LOGD("NMSG_CNT_HEARTBEAT:[%d]\r\n",ClieFd);
	
	if(CommLayerTcp::PushSendDataQueue(ClieFd, &OutStrProPack) == false)
	{
		ret = -1;
	}

	Free(OutStrProPack.data);

	return(ret);	
}

/* ����NAKӦ��� */
int SendErrorMsg(unsigned short usPackIndex, int iErrorNo, char * pErrorMsg, PROTOCOL_PACK *OutStrProPack)
{
	ERROR_INFO strErrInfo;
	int iMsgLen;
	int iErrLen;

	iErrLen = sizeof(strErrInfo) - sizeof(strErrInfo.errorMsg);
	
	memset(&strErrInfo, 0, sizeof(ERROR_INFO));
	strErrInfo.errorNo = htonl(iErrorNo);

	if (pErrorMsg != NULL)
	{
		iMsgLen = strlen(pErrorMsg);
		if (iMsgLen > 255)
		{
			iMsgLen = 255;
		}
		
		strncpy(strErrInfo.errorMsg, pErrorMsg, iMsgLen);
		iErrLen += iMsgLen;
	}

	TcpSendPack(PROTOCOL_HEAD_FLAG, usPackIndex,NTYPE_FACECAP_WORK,
					NMSG_CNT_LOGIN, NMSG_SUBTYPE_ERROR, iErrLen, (void *)&strErrInfo, OutStrProPack);

	return 0;
}
int snedPictureToSvr(char* pPicture, int len)
{
	//g_AuthFlag = 0;
	int ret = 0;

	int ClieFd = -1;
	PROTOCOL_PACK OutStrProPack;
	char *pTemp = NULL;
	
	memset(&OutStrProPack, 0, sizeof(PROTOCOL_PACK));
	OutStrProPack.data = NULL;

	ClieFd = giClieFd;
	
	TCOMPARE picCompare;
	int nLen = sizeof(TCOMPARE);

	memset(&picCompare, 0, sizeof(TCOMPARE));

	picCompare.score = htonl(50000000);
	picCompare.maxres = htonl(1);
	picCompare.imgtype = htons(0x0001);
	picCompare.imgsize  = htonl(len);

	pTemp = (char*)malloc(nLen+len+1);
	memcpy(pTemp,&picCompare,nLen);
	LOGD("memcpy(pTemp+nLen,pPicture,nLen[%d])sizeof(unsigned short)[%d]picCompare.maxres[%d]",nLen,sizeof(unsigned short),picCompare.maxres);
	memcpy(pTemp+nLen,pPicture,len);
	

	LOGD("login in imgtype[%d] imgsize[%lu] len[%d]\r\n",picCompare.imgtype,picCompare.imgsize,len);

	if(TcpSendPack(PROTOCOL_HEAD_FLAG, AUTH_INDEX, NTYPE_FACECAP_WORK,NMSG_FACE_COMPARE,
			NMSG_SUBTYPE_REQUEST, nLen+len, (void *)pTemp, &OutStrProPack) < 0)
	{
		ret = -1;
		goto ClientSendAuth_END;
	}
	
	if(CommLayerTcp::PushSendDataQueue(ClieFd, &OutStrProPack) == false)
	{
		ret = -1;
		goto ClientSendAuth_END;
	}
	
ClientSendAuth_END:

	Free(OutStrProPack.data);
	
	return ret;
}

int snedGetSvrPicture(unsigned int faceId)
{
	//g_AuthFlag = 0;
	int ret = 0;

	int ClieFd = -1;
	PROTOCOL_PACK OutStrProPack;
	char *pTemp = NULL;
	
	memset(&OutStrProPack, 0, sizeof(PROTOCOL_PACK));
	OutStrProPack.data = NULL;

	ClieFd = giClieFd;

	faceId = htonl(faceId);
	
	int nLen = sizeof(unsigned int);
	
	if(TcpSendPack(PROTOCOL_HEAD_FLAG, AUTH_INDEX, NTYPE_FACECAP_WORK,NMSG_FLIB_GET_SUB,
			NMSG_SUBTYPE_REQUEST, nLen, (void *)&faceId, &OutStrProPack) < 0)
	{
		ret = -1;
		goto ClientSendAuth_END;
	}
	
	if(CommLayerTcp::PushSendDataQueue(ClieFd, &OutStrProPack) == false)
	{
		ret = -1;
		goto ClientSendAuth_END;
	}
	
ClientSendAuth_END:

	Free(OutStrProPack.data);
	
	return ret;
}
COMPARE_RESULT get_CompareResult()
{
	return g_CompareResult;
}
