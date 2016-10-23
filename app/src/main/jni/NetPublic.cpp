#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>


#include "netComm.h"
#include "netLib.h"

/**************************************************************\
** �������ƣ� ReadableTimeout
** ���ܣ� ȡʱ��
** ������ 
** ���أ� 0-�ɹ�������-ʧ�ܡ�
** �������ߣ� ������
** �������ڣ� 2012-5-21
** �޸����ߣ�
** �޸����ڣ�
\**************************************************************/
int ReadableTimeout(int iFd, int iSec)
{
	fd_set			rset;
	struct timeval	tv;

	if(iFd < 0)
		return(-1);

	FD_ZERO(&rset);
	FD_SET(iFd, &rset);
	tv.tv_sec = iSec;
	tv.tv_usec = 0;

	return(select(iFd+1, &rset, NULL, NULL, &tv));

}

bool AllocCharMatrix(int RowNum, int LowNum, char ***OutChar)
{
	int i;
	char **TmpC=NULL;

	TmpC = (char **)malloc(RowNum * sizeof(char *));
	if(TmpC == NULL)
	{
		return(false);
	}

	for(i=0; i<RowNum; i++)
	{
		TmpC[i] = (char *)malloc(LowNum * sizeof(char));
		if(TmpC[i] == NULL)
		{
			return(false);
		}
		memset(TmpC[i], 0, LowNum * sizeof(char));
	}
	
	*OutChar = TmpC;

	return(true);
}

void FreeCharMatrix(char ***OutChar, int RowNum)
{
	int i;
	char **TmpC=NULL;
	
	TmpC = *OutChar;

	for(i=0; i<RowNum; i++)
	{
		free(TmpC[i]);
		TmpC[i] = NULL;
	}

	if(TmpC != NULL)
	{
		free(TmpC);
		TmpC = NULL;
	}

	return;
}

/**************************************************************\
** �������ƣ� GetHostAllIp
** ���ܣ� �������IP
** ������ 
** ���أ� 
** �������ߣ� ������
** �������ڣ� 2012-5-21
** �޸����ߣ�
** �޸����ڣ�
\**************************************************************/
bool GetHostAllIp(char ***AllIp, int *OutIpNum)
{
	struct hostent *hptr;
	char **pptr, **pTip = NULL;
	char HostName[100];
	char CharTmp[100];
	int IpNum=0, IpLen=15+1, i;

	memset(HostName, 0x00, sizeof(HostName));
	
	if(gethostname(HostName, sizeof(HostName)))
	{
		return(false);
	}

	if((hptr=gethostbyname(HostName)) == NULL)
	{
		return(false);
	}

	IpNum = 0;
	pptr = hptr->h_addr_list;
	
	for (; *pptr != NULL; pptr++)
	{
		IpNum++;
	}
	
	if (!AllocCharMatrix(IpNum, IpLen, AllIp))
	{
		return(false);
	}

	i = 0;
	pTip = *AllIp;
	pptr=hptr->h_addr_list;
	for(; *pptr!=NULL; pptr++)
	{
		memset(CharTmp, 0x00, sizeof(CharTmp));
		inet_ntop(hptr->h_addrtype, *pptr, CharTmp, sizeof(CharTmp));
		strncpy(pTip[i], CharTmp, IpLen-1);
	}

	*OutIpNum =  IpNum;

	return(true);
}

/**************************************************************\
** �������ƣ� CleanupMutex
** ���ܣ� �ͷ�����Դ
** ������ 
** ���أ� 
** �������ߣ� ������
** �������ڣ� 2012-5-21
** �޸����ߣ�
** �޸����ڣ�
\**************************************************************/
void CleanupMutex(void *arg)
{
	pthread_mutex_unlock((pthread_mutex_t *)arg);
}

