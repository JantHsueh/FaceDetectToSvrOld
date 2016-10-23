/********************************************************************************
**  Copyright (c) 2012, �����з���˹�Ƽ����޹�˾
**  All rights reserved.
**
**  �ļ����ƣ� netLib.h
**  �ο��� 
**
**  ��ǰ�汾��1.0
**
**  �������ߣ�������
**  ��������:  2012.5.21
**
**  �޸����ߣ�
**  �޸�����:
********************************************************************************/

#ifndef NETLIB_H
#define NETLIB_H

//#include "Comm.h"

#define COMM_PORT                   30000
#define COMM_PORT_SERVER            30001

#ifdef WIN32
	#pragma pack(1)
	#define PACK_ALIGN
#else
	#define PACK_ALIGN __attribute__((packed))
#endif

typedef struct
{	
	int iServPort;		/* �ն���Ϊ�������ˣ������Ķ˿� */
	char *acServIp;		/* �ն���Ϊ�������ˣ�accept��IP */
	int iCliePort;		/* �ն���Ϊ�ͻ��ˣ����ӵķ������˿� */
	char *acClieIp;		/* �ն���Ϊ�ͻ��ˣ����ӵķ�����IP */
}PACK_ALIGN TcpThreadsData;

/**************************************************************\
** �������ƣ� DoingAllForTcpClientThread
** ���ܣ� �ն���Ϊ�ͻ����߳�
** ������   
            arg : 
** ���أ���
** �������ߣ� ��͢��
** �������ڣ� 2012-8-22
** �޸����ߣ� 
** �޸����ڣ� 
\**************************************************************/
void *DoingAllForTcpClientThread(void *arg);

/**************************************************************\
** �������ƣ� DoingAllForTcpServerThread
** ���ܣ� �ն���Ϊ������߳�
** ������   
            arg : 
** ���أ���
** �������ߣ� ��͢��
** �������ڣ� 2012-8-22
** �޸����ߣ� 
** �޸����ڣ� 
\**************************************************************/
void *DoingAllForTcpServerThread(void *arg);



#ifdef WIN32
    #pragma pack()
#endif
	#undef PACK_ALIGN

#endif /* NETLIB_H */

