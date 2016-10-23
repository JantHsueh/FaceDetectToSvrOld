#ifndef _NETCOMM_H
#define _NETCOMM_H

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>

#define _COMM_DEBUG             1

#define RINGS_SETTING_NUM 5
#define FUNC_KEYS_NUM 10
#define FUNC_KEYS_DES_LEN 32
#define AUTO_SLEEP_NUMS 4



#ifndef SOMAXCONN
	#define SOMAXCONN 1024
#endif
	
/***********************�����һЩ��������ģ�� begin*****************************/

#define ERROR_LOG_NAME "./error.log"

extern int ReadableTimeout(int iFd, int iSec);

extern bool AllocCharMatrix(int RowNum, int LowNum, char ***OutChar);

extern void FreeCharMatrix(char ***OutChar, int RowNum);

extern bool GetHostAllIp(char ***AllIp, int *OutIpNum);

extern void CleanupMutex(void *arg);

/***********************�����һЩ��������ģ�� end*****************************/

/***********************������C++ģ����� begin*****************************/
//using namespace std;

template<class T> struct Node_Cpp
{
	T data;
	Node_Cpp<T> *next;
};

template<class T> class LinkQueue_Cpp
{
public:
	LinkQueue_Cpp();
	~LinkQueue_Cpp();
	bool Push(T x);
	void PrintQueue();
	T* Front();
	bool Pop();
	int Size();
	void Clear();

	bool Empty()
	{
		return(front == rear);//(�նӵ�����)
	}

private:
	Node_Cpp<T> *rear;
	Node_Cpp<T> *front;
};



/**************************************************************\
** �������ƣ� LinkQueue_Cpp
** ���ܣ� ģ�幹�캯��
** ������ 
** ���أ� ��
** �������ߣ� ������
** �������ڣ� 2012-5-21
** �޸����ߣ�
** �޸����ڣ�
\**************************************************************/
template<class T> LinkQueue_Cpp<T>::LinkQueue_Cpp()
{
	front = new Node_Cpp<T>;
	front->next=NULL;
	rear = front;
}

/**************************************************************\
** �������ƣ� ~LinkQueue_Cpp
** ���ܣ� ģ�����캯��
** ������ 
** ���أ� ��
** �������ߣ� ������
** �������ڣ� 2012-5-21
** �޸����ߣ�
** �޸����ڣ�
\**************************************************************/
template<class T> LinkQueue_Cpp<T>::~LinkQueue_Cpp()
{
	printf("front %p rear %p\r\n", front, rear);
	if(front != NULL)
	{
		Node_Cpp<T> *ptr;
		ptr = front;
		front = front->next;
		delete ptr;
		ptr = NULL;
	}	
}

/**************************************************************\
** �������ƣ� Push
** ���ܣ� �����
** ������ 
** ���أ� ��
** �������ߣ� ������
** �������ڣ� 2012-5-21
** �޸����ߣ�
** �޸����ڣ�
\**************************************************************/
template<class T> bool LinkQueue_Cpp<T>::Push(T x)
{
	Node_Cpp<T> *pt = new Node_Cpp<T>;
	pt->data = x;
	pt->next = NULL;	//ע�⣬һ��Ҫ��һ�䣬���һ���ڵ�next��ΪNULL
	rear->next = pt;	//�½ڵ�ŵ������
	rear = pt;			//β��Ϊ�½ڵ�

	return(true);
}

/**************************************************************\
** �������ƣ� Pop
** ���ܣ� ɾ������ͷ�ڵ�
** ������ 
** ���أ� ��
** �������ߣ� ������
** �������ڣ� 2012-5-21
** �޸����ߣ�
** �޸����ڣ�
\**************************************************************/
template<class T> bool LinkQueue_Cpp<T>::Pop()
{
	Node_Cpp<T> *ptr;

	if(front == rear)
		return false;

	ptr = front;
	front = front->next;

	delete ptr;
	ptr = NULL;

	return true;
}

/**************************************************************\
** �������ƣ� Front
** ���ܣ� ���ض���ͷ�ڵ�
** ������ 
** ���أ� ��
** �������ߣ� ������
** �������ڣ� 2012-5-21
** �޸����ߣ�
** �޸����ڣ�
\**************************************************************/
template<class T> T* LinkQueue_Cpp<T>::Front()
{
	if (front == NULL)
	{
		printf("front %p %s %d\r\n", front, __FUNCTION__, __LINE__);
		return NULL;
	}

	if (front->next == NULL)
	{
		printf("front->next %p %s %d\r\n", front->next, __FUNCTION__, __LINE__);
		return NULL;
	}
	
	return (&(front->next->data));
}

/**************************************************************\
** �������ƣ� PrintQueue
** ���ܣ� ��ӡ��������
** ������ 
** ���أ� ��
** �������ߣ� ������
** �������ڣ� 2012-5-21
** �޸����ߣ�
** �޸����ڣ�
\**************************************************************/
template<class T> void LinkQueue_Cpp<T>::PrintQueue()
{
	Node_Cpp<T>* ptr;

	if(front->next==NULL)
	{
		return;
	}

	ptr = front->next;
	while(ptr!=NULL)
	{
		//cout<< "ptr->data:" <<ptr->data << endl;
		ptr=ptr->next;
	}
	return;
}

/**************************************************************\
** �������ƣ� Size
** ���ܣ� ���ض��д�С
** ������ 
** ���أ� ��
** �������ߣ� ������
** �������ڣ� 2012-5-21
** �޸����ߣ�
** �޸����ڣ�
\**************************************************************/
template<class T> int LinkQueue_Cpp<T>::Size()
{
	Node_Cpp<T> *ptr = front->next;
	int Count=0;
	
	while(ptr)
	{
		ptr = ptr->next;
		Count++;
	}
	return Count;
}

template<class T> void LinkQueue_Cpp<T>::Clear()
{//�������T data��new����ô����delete�����Clear
	Node_Cpp<T> *ptr;
	
	for(;;)
	{
		ptr = NULL;
		
		if(front == rear)
			break;
	
		ptr = front;
		front = front->next;

		if(ptr != NULL)
		{
			delete ptr;
			ptr = NULL;
		}
	}
	return;
}

/***********************������C++ģ����� end*****************************/

//#ifdef WIN32
	#pragma pack(1)
	#define PACK_ALIGN
//#else
//	#define PACK_ALIGN __attribute__((packed))
//#endif



/***********************�������� begin*****************************/
//����˹����ʶ���ն�ͨ��Э��V2.0.doc/V1.5.doc�����TCP��������

///////////////////Э����غꡢ�ṹ�ȶ��� begin/////////////////
//using namespace std;


typedef int SOCKET;




/* ������ */
#define AUTH                	0	
#define GET						1
#define GET_NEXT				2
#define PUT						3
#define PUT_NEXT				4
#define RESPONSE				5
#define TRAP                	6
#define HEARTBEAT				7

/* ������ */
/* ���ӽ��� */
#define AUTH_RANDOM				0
#define AUTH_AUTHENTICATE		1


/* �������� */
/* �������� */
/* Τ��������� */
/* ������ʱ���� */
/* IO���� */
/* �������� */
/* ��ʱ�������� */
/* �豸ͬ������ */
/* ���ڼ������ */
/* ���ܼ����� */
/* �������� */
/* ������澯���� */
/* �Զ��������� */
/* ʱ����ʾ��ʽ���� */
/* ʶ���������� */
/* ��ȫ�������� */
/* ��ĻУ׼���� */
/* �����Ӧ���� */
/* �ָ�����Ĭ������ */
/* ϵͳ��Ϣ */
/* �洢��Ϣ */
/* ϵͳ���� */
/* ���ö�ʱ����������� */


/* �û����� */
/* �û������Ա���� */
/* ��ȡ�û������Ա��Ϣ */
/* ��ȡ�û������Աģ�� */
/* �·��û������Աģ�� */
/* һ��һʶ���û� */
#define USER_VERIFY_USER		64
/* �Ǽ����� */
#define USER_ENROLL_FACE		65


#define ACK					0
#define NAK					1


#define USER_NAME_LEN		16
#define MD5_CHECK_CODE_LEN	16


#define FACETCP_SERVER_PORT		32108         //������Ĭ�϶˿�
#define FACETCP_VERSION		    0x02000000    //ͨ��Э��汾��

// ���ݰ�����(������������)
#define NTYPE_FACECAP_WORK      0x0001        //������������:�ֻ�����ץ���豸

// ���ݰ���Ϣ����(Э���)
#define NMSG_CNT_LOGIN		    0x00000100    //��¼
#define NMSG_CNT_HEARTBEAT	    0x00000200	  //����			
#define NMSG_FACE_COMPARE		0x00020100	  //ͼƬʶ��ȶ�
#define NMSG_FLIB_GET_SUB		0x00010401	  //��ȡ����ģ����Ƭ


// ���ݰ���Ϣ������(����/Ӧ�����������)
#define NMSG_SUBTYPE_REQUEST    0x0001       //����
#define NMSG_SUBTYPE_RESPONSE   0x0002       //Ӧ��
#define NMSG_SUBTYPE_ERROR      0x0003       //����


/* ����ͷ��־ */
#define PROTOCOL_HEAD_FLAG	0x7E
#define PROTOCOL_TAIL_FLAG	0x7F


#define AUTH_INDEX 			65000
#define UPLOAD_INDEX 		65001
#define CHECK_INDEX 		65002
#define NEEDRECONECT 		65003
#define TRAP_ADD_USER 		65004
#define FACE_HEARTBEAT     	0

#define MAX_CONN_NUM 		5	/* �����֧�ֵ���������� */
#define MAX_SEND_THREADS 	1	/* ������߳� */
#define RECV_TIME_OUT		2

extern int giServerFd[MAX_CONN_NUM];

typedef struct PROTOCOL_HEAD
{ 
	unsigned char	flag;			// ����ʶ	(�̶�Ϊ0x7E)
	unsigned int	index;			// ��� 
	unsigned short 	pack;			// ���ݰ��ķְ���Ϣ��λ�ṹ:15~8�ְ�����;7~0�ְ���ţ�x0000 ��ʾδ�ְ���
	unsigned short	packType;		// ������	
	unsigned int	msgtype;		// ���ݰ���Ϣ����(Э���)
	unsigned short	subType;		// ������	
	unsigned int	dataLen;		// ���ݳ��� 

} PACK_ALIGN PROTOCOL_HEAD,*LPPROTOCOL_HEAD;


typedef struct PROTOCOL_PACK
{ 
	PROTOCOL_HEAD 	head;	    // ����ͷ	
	char			*data;		// �������� 
	unsigned short  chksum;		 // ���ݰ�У���(��Ϊ����ſ�ʼ����������ֽ������ۼ�)
	unsigned char	csuffix;	// ���ݰ���׺���̶�Ϊ0x7F�����ݰ��Ĵ���ʱ��������׺��Ϊ��������־��
}PACK_ALIGN PROTOCOL_PACK,*LPPROTOCOL_PACK;

//���ݰ���Ϣ����(Э���):NMSG_CNT_LOGIN
//���ݰ���Ϣ������(����/Ӧ�����������):NMSG_SUBTYPE_REQUEST)
typedef struct _LOGIN
{
    unsigned int      ver;      //ͨ��Э��汾��
    char       user[32]; //��¼�û���
    char       pass[32]; //��¼����
} LOGIN;

//�û���½�ɹ�,���������ظ��û�����ϸ��Ϣ������Ȩ�ޣ�
//���ݰ���Ϣ����(Э���):NMSG_CNT_LOGIN
//���ݰ���Ϣ������(����/Ӧ�����������): NMSG_SUBTYPE_RESPONSE)
typedef struct _LOGIN_RESP
{
    unsigned int      id;           //�û���ʶID
    char       user[32];     //�û���
    char       pass[32];     //����
    unsigned int      usertype;     //�û�����
    char       RuleList[512];//�û�Ȩ��(ʹ��","�ָ���Ȩ�ޱ�ʶID���ַ�����)
} LOGIN_RESP;

//�û���½ʧ��,���������ش�����Ϣ�����������룩
//���ݰ���Ϣ����(Э���):NMSG_CNT_LOGIN
//���ݰ���Ϣ������(����/Ӧ�����������): NMSG_SUBTYPE_ERROR)
typedef struct _LOGIN_ERR
{
    unsigned int      err;      //������(1:�û������������)
} LOGIN_ERR;


//ͼƬ�ȶ�
typedef struct _COMPARE
{
    unsigned int      score;      //�ȶ����޷�ֵ
    unsigned int      maxres;     //�ȶ����з����������(Ĭ��Ϊ1���Ϊ10)
    unsigned short    imgtype;    //ͼƬ����(��ǰ��֧��JPEG ѹ����ͼƬ���ݣ�ȡֵ0x0001 )
    unsigned int      imgsize;    //ͼƬ���ݳ���
}TCOMPARE;

typedef struct _COMPARE_DETECT//���������Ϣ
{
    int      fdet_l;          //��������������
    int      fdet_t;          //��������������
    int      fdet_r;          //��������������
    int      fdet_b;          //��������������
    int      fdet_quality;    //����ͼƬ��ͼ������
    char       reserved[64];    //�����ֶ�
}COMPARE_DETECT;

typedef struct _COMPARE_RESULT //�ȶԽ��
{
    unsigned int      score;          //�ȶԵ�ʶ���ֵ
    unsigned int      pid;            //����id
    unsigned int      f_id;           //���������ģ��id(ע��һ����������ж������ģ��)
    char      		  f_name[64];     //����
    unsigned short    f_type;         //����(δ֪:0x0000 ������:0x0001 Ա��:0x0100 ��ʻԱ:0x0101)
    unsigned short	  f_sst;          //���еȼ�--��ʱ����δ����Ĭ����
    char      		  f_sex;          //�Ա�(δ֪:0x00 ��:0x01 Ů:0x02)
    char     		  f_age;          //����(5-127)
    char      		  f_remarks[256]; //��ע��Ϣ
    char       		  reserved[64];   //�����ֶ�
}COMPARE_RESULT;


#if 1
typedef struct RECORDS_GET
{
	//RECORD_SET_S* pRecords;		/* ���ݿ����ݵļ�¼�� */
	char *pFeatures;
	int nDateLen;
	int nFieldNum;                  /* pRecords ��¼����Ч�ֶ��� */
	int nPackType;					/* ������ */
	int nSubType;					/* ������ */
	int nMaxRecords;			    /* ���ݿ�����������������¼�� */
	int nCurRecords;			    /* ��ǰ�Ѿ���ȡ�ļ�¼�� */
	int nNo;					    /* ��ǰҪ���͵ļ�¼��� */
	int nUserNo;				    /* �û���� */
	char logType;                   /* ��־���� */
	char logLevel;                  /* ��־���� */
	//char sql[1024];	
    unsigned int searchConditionFlag;
    char photoFlag;         		/* �Ƿ����¼��Ƭ��� */
} RECORDS_GET, *LPRECORDS_GET;

#endif

typedef struct RECORDS_PUT
{
	int nPackType;				/* ������ */
	int nSubType;				/* ������ */
	int nNo;					/* ��ǰҪ���͵ļ�¼��� */	
	int nUserNo;				/* �û���� */
	unsigned int ulFileSize;  	/* �����ļ���С*/	
	unsigned int ulRecivedSize;   /* �Ѿ����ܵ����ݴ�С*/	
} RECORDS_PUT, *LPRECORDS_PUT;

#if 0

/*
**  ������־��Ϣ�ṹ
*/
typedef struct LOG_INFOM
{
	char logTime[32];			/* ��־ʱ�� */
	char logType;				/* ��־���� */
	char logLevel;			/* ��־���� */
	char logTitle[16];			/* ��־���� */
	char logLen;				/* �������� */
	char logDesc[1];			/* ���� */
	
} PACK_ALIGN LOG_INFOM, *LPLOG_INFOM;

typedef enum {
	PARAM_ERROR,			/* �������� */
	MSG_SUBTYPE_ERR,		/* ��Ϣ�����ʹ��� */
	DEL_RECORD_ERR,			/* ɾ����¼ʧ�� */
	NO_SUCH_USER,			/* �û������� */
	TIME_SET_FAIL,			/* ����ʱ��ʧ�� */
	GET_USER_INFO_FAIL,		/* ��ȡ�û���Ϣʧ�� */
	RECOGNIZE_FACE_FAIL,	/* ����ʶ��ʧ�� */
	CLIENT_QUIT,			/* �ͻ��˵��� */
	GET_USER_TMPL_FAIL,		/* ��ȡ�û�ģ��ʧ�� */
	ENROLL_FACE_FAIL,		/* �Ǽ�����ʧ�� */
	CARD_ALREADY_EXIST,		/* ������ע�� */
	DATA_FORMAT_ERROR       /* ��Ϣ���������ݸ�ʽ����*/
} ERROR_NO;

typedef struct ENROLL_USER{
	unsigned long	userid;	/* �û���ų��ȣ�4�ֽڣ� */
	char			sign;	/* �����û���ʶ��1�ֽڣ� */
	char            cType;	/* �û����ͣ�1�ֽڣ���0-��ͨ�û���1-����Ա */
} PACK_ALIGN ENROLL_USER,LPENROLL_USER;

typedef struct VERIFY_INFO
{
	unsigned int	nUserNo;
	char            nStatus;
	char	        nThreshold;
	char		    nPhototype;
	int             nPhotoLen;
	char            photo[1];
}PACK_ALIGN VERIFY_INFO, *LPVERIFY_INFO;


typedef struct PACK_INFO
{
	char  bRemovable;
	int   sockID;
	int   nPackLen;
	char  *pPack;
	struct  PACK_INFO* next;
}PACK_ALIGN PACK_INFO, *LPPACK_INFO;

typedef struct PACK_LIST{
	int   curNum;
	LPPACK_INFO pPackList;
}PACK_ALIGN PACK_LIST, * LPPACK_LIST;

#endif

typedef struct
{
	int randFlag;		/* ÿ�������ڵ��������־ */
	int randNum;		/* ÿ�������ڵ��������Ŀ */
	int authFlag;		/* ÿ�������ڵ���֤��־ */

	RECORDS_GET recordsGet;
	RECORDS_PUT recordsPut;
}PACK_ALIGN INLINK_SHARE, *LPINLINK_SHARE;

#if 0
typedef struct AUTH_RAND
{
	int rand;
} PACK_ALIGN AUTH_RAND,*LPAUTH_RAND;

typedef struct
{
	unsigned char ucMute;	/* ���������� */
	unsigned char ucMuteVoice;	/* �������� */
	unsigned char iVolSpeaker;	/* �������� */
} PACK_ALIGN AUDIO_SET,*LPAUDIO_SET;

typedef struct
{
	char caIp[16];			/* �ն�IP */
	char caNetmask[16];		/* �������� */
	char caGateway[16];		/* ���� */
	char caDns[16];			/* DNS������IP */
} PACK_ALIGN NETWORK_SET,*LPNETWORK_SET;

typedef struct
{
	unsigned char caWgEvenNum;			/* żУ��λ�� */
	unsigned char caWgOemCode;			/* OEM�� */
	unsigned char caWgSectionCode;		/* ���� */
	unsigned char caWgUserCode;			/* �û��� */
	unsigned char caWgOddNum;			/* ��У��λ�� */
} PACK_ALIGN WIEGANDIN_SET,*LPWIEGANDIN_SET;

typedef struct
{
	char cWiegandOutMode;		/* Τ�����ģʽѡ��,0��Τ��26; 1��Τ��34 */
	char cOutPutFormat;         /* Τ��������ݣ�0-���ţ�1-���� */
	unsigned short usAreaCode;  /* ���� Τ��26 <= 254 Τ��34 <= 65534 */
} PACK_ALIGN WIEGANDOUT_SET,*LPWIEGANDOUT_SET;

typedef struct
{
	unsigned char ucDelayTime;			/* ������ʱ */
} PACK_ALIGN DOORDELAY_SET,*LPDOORDELAY_SET;

typedef struct
{
	unsigned char ucMGInput;        /* �����Ŵ����룬0 - �رգ�1 - ���� */
	unsigned char ucMGDelayTime;	/* �Ŵ���ʱ */
	unsigned char ucFireInput;      /* �������룬0 - �رգ�1 - ���� */
	unsigned char ucIODoorOutPut;   /* �����������0 - �رգ�1 - ���� */
    unsigned char ucAudioOutPut;    /* ���������0 - �رգ�1 - ���� */
} PACK_ALIGN IOCONFIG_SET,*LPIOCONFIG_SET;

typedef struct
{
	char cDoorbell;						/* ���� */
} PACK_ALIGN BELL_SET,*LPBELL_SET;

typedef struct
{
    unsigned char ucRingHour;     /* Сʱ */
    unsigned char ucRingMinute;   /* ���� */
	unsigned char ucRingIndex;	  /* ��� */
	unsigned char ucRingSwitch;   /* ���� */
} PACK_ALIGN RING_SET,*LPRING_SET;

typedef struct
{
	RING_SET stRingsSetting[RINGS_SETTING_NUM];
} PACK_ALIGN RINGS_SET,*LPRINGS_SET;

typedef struct
{
    unsigned char ucSyncMode;       /* ͬ����ʽ��0 - �ֶ���1 - �Զ� */
} PACK_ALIGN DEVICE_SYNC_SET,*LPDEVICE_SYNC_SET;

typedef struct
{
	unsigned short usAttendInterval;    /* ���ڼ�� 0 - ����ʱ��������Ч��1 ~ 300������ʱ��������λΪ���� */
} PACK_ALIGN ATTEND_INTERVAL_SET,*LPATTEND_INTERVAL_SET;

typedef struct
{
    unsigned char ucEnable;         /* ����ʹ�� */
	char caKeysScription[FUNC_KEYS_NUM][FUNC_KEYS_DES_LEN+1];
} PACK_ALIGN FUNCTION_KEYS_SET,*LPFUNCTION_KEYS_SET;

typedef struct
{
	unsigned char ucLanguage;           /* ���� 1-english, 2-�������ģ�3-�������� */
} PACK_ALIGN LANGUAGE_SET,*LPLANGUAGE_SET;

typedef struct
{
	 unsigned char ucAlarmShell;         /* ��ǲ�ж����*/
} PACK_ALIGN LOCK_SHELL_ALARM_SET,*LPLOCK_SHELL_ALARM_SET;

typedef struct
{
	unsigned char ucOffTimeHour;
	unsigned char ucOffTimeMinute;
	unsigned char ucOnTimeHour;
	unsigned char ucOnTimeMinute;
} PACK_ALIGN SINGLE_AUTO_SLEEP_SET,*LPSINGLE_AUTO_SLEEP_SET;

typedef struct
{
	SINGLE_AUTO_SLEEP_SET strAutoSleepCfg[AUTO_SLEEP_NUMS];
} PACK_ALIGN AUTO_SLEEP_SET,*LPAUTO_SLEEP_SET;

typedef struct
{
	unsigned char ucStatus;     /* ������ʹ��״̬��0 - ��ֹ��1 - ���� */
} PACK_ALIGN SCREEN_SAVER_SET,*LPSCREEN_SAVER_SET;

typedef struct
{
	unsigned char ucTimeMode;       /* ʱ����ʾ��ʽ��0.YY-MM-DD, 1.YY/MM/DD, 2.MM-DD-YY, 3.MM/DD/YY, 4.DD-MM-YY, 5.DD/MM/YY */
} PACK_ALIGN SHOW_TIME_MODE_SET,*LPSHOW_TIME_MODE_SET;

typedef struct
{
	unsigned char ucRecognise;      /* ʶ������ */
	unsigned char ucUpdate;         /* �������� */
    unsigned char ucVerifyThreshold;/* ��֤���� */
} PACK_ALIGN RECOG_THRESHOLD_SET,*LPRECOG_THRESHOLD_SET;

typedef struct
{
	char cNumPre;			/* ����ǰ׺*/
	char caWebPwd[8];       /* ��ҳ��¼���� */
	char caSdPwd[8];        /* SD������ */
	char caUnlockPwd[8];    /*��������*/   //modify defect 72 2012-10-16  add caUnlockPwd
} PACK_ALIGN SAFE_CONFIG_SET,*LPSAFE_CONFIG_SET;

typedef struct
{
	char cSenseThreshold;              /* �����Ӧ���� */
} PACK_ALIGN HUMAN_SENSE_SET,*LPHUMAN_SENSE_SET;

typedef struct
{
	char caProductID[16];
	char caSN[16];
	char caSoftwareVer[16];
	char caHardwareVer[16];
	char caUbootVer[16];
	char caKernelVer[16];
	char caProtocolVer[16];
	char caArithmeticVer[16];
} PACK_ALIGN DEVICE_INFO_GET,*LPDEVICE_INFO_GET;

typedef struct
{
	int iCurUserNum;
	int iLimitUserNum;
	int iCurRecordNum;
	int iLimitRecordNum;
	int iCurFeatureNum;
	int iLimitFeatureNum;
	int iCurLogNum;
	int iLimitLogNum;
} PACK_ALIGN STORAGE_INFO_GET,*LPSTORAGE_INFO_GET;	

#endif

///////////////////Э����غꡢ�ṹ�ȶ��� end/////////////////

///////////////////������Ĳ��� begin/////////////////
enum SERVER_OR_CLIENT
{
	TCP_CLIENT,
	TCP_SERVER
};

/* �������ݽṹ */
typedef struct //SendData
{
	int iFd;			/* ����socket��������accept������ */
	int iSendBufLen;	/* �������ݳ��� */
	char *pSendBuf;	/* �������� */
}PACK_ALIGN StrTcpSendData;

typedef struct //TCP_BUF
{
	unsigned short usFlag;		/* ����ʶ	*/
	unsigned short usIndex;		/* ��� */
	char cPackType;				/* ������	*/
	char cSubType;			/* ������	*/
	
	int iFd;					/* ����socket��������accept������ */
	
	int iTcpBufLen;				/* ���ݳ��� */
	char *pTcpBuf;				/* ���� */
}PACK_ALIGN PROTOCOL_PACK111;


typedef int ( *TREAD_DOING )(int acceptFd);

class CommLayerTcp
{
private:
	int iServPort;				/* �ն���Ϊ�������ˣ������Ķ˿� */
	char acServIp[16];			/* �ն���Ϊ�������ˣ�accept��IP */
	int iCliePort;				/* �ն���Ϊ�ͻ��ˣ����ӵķ������˿� */
	char acClieIp[16];			/* �ն���Ϊ�ͻ��ˣ����ӵķ�����IP */
	int iSocketServFd;			/* �����SOCKET���� */
	int iSocketClieFd;			/* �ͻ���SOCKET���� */
	int iAllFd[MAX_CONN_NUM];	/* ������������ACCEPT���ػ���Ϊ�ͻ��˵�socket���� */
	int iCurrConnNum;			/* ��ǰ������Ŀ */
	int iClientAuthSucc;		/* �ն���Ϊ�ͻ��ˣ�������֤�ɹ� 0-���ɹ���1-�ɹ� */

	pthread_mutex_t AllFdMutex;					/* iAllFd�Ļ����� */
	pthread_mutex_t RecvThreadIDMutex;			/* RecvThreadId�Ļ����� */

	static unsigned short DataIndex;

	pthread_t SendThreadId[MAX_SEND_THREADS];
	pthread_t RecvThreadId[MAX_CONN_NUM];

	pthread_t ClientSendHeartbeatThreadId;

public:

	static LinkQueue_Cpp<StrTcpSendData> SendDataQueue;	/* �������ݶ��� */
	static pthread_mutex_t SendDataQueueMutex;	/* ���Ͷ��л����� */
	static pthread_cond_t SendDataQueueCond;	/* ���Ͷ����������� */
	static int iInstance;

	CommLayerTcp();

	CommLayerTcp(const CommLayerTcp &orig);

	CommLayerTcp &operator = (const CommLayerTcp &);

	~CommLayerTcp();

	void CommLayerResetIpAndPort(const SERVER_OR_CLIENT InModel, const char *Ip, const int Port);
	
	int CommLayerTcpInit(const SERVER_OR_CLIENT InModel, const char *Ip, const int Port);

	void CommLayerTcpDestroy();

	bool CreateTcpServ();

	void CreateTcpAccept();

	bool CreateTcpClient();

	int ReadableTimeout(int iFd, int iSec);

	int WriteN(int iFd, const void *vpBuf, int iBufLen);

	int ReadN(int iFd, int iTimeOut, void *vpBuf, int iBufLen);

	void DoTcpSendThreadPool(pthread_t InThreadId);

	int GetClientAuthSucc();

	void SetClientAuthSucc(int flag);

	int GetSocketServFd();
	
	int GetSocketClieFd();

	void SetSocketClieFd(int InFd);

	void SetSendThreadId(pthread_t ThreadId, int i);

	void GetSendThreadId(pthread_t *OutThreadId, int i);

	void SetRecvThreadId(pthread_t ThreadId, int i);

	void GetRecvThreadId(pthread_t *OutThreadId, int i);

	pthread_t GetClientSendHeartbeatThreadId();

	void SetClientSendHeartbeatThreadId(pthread_t Inid);

	void CloseSocketClieFd();

	int GetCurrConnNum();

	bool AddAllFd(int InFd);

	bool SubAllFd(int InFd);

	bool AddRecvThreadID(pthread_t rThreadid);

	bool SubRecvThreadID(pthread_t rThreadid);


	bool SubAllCliFd();

	static bool PushSendDataQueue(int iFd, PROTOCOL_PACK *InStrProPack);

};

typedef struct
{
	CommLayerTcp *Obj;
	int iFd;
}StrLayerTcpTmp;

extern void *TcpSendThreadPool(void *arg);

extern void *TcpServSubThread(void *arg);

extern void *DoingAllForTcpServerThread(void *arg);

extern void *ClientSendHeartbeatThread(void *arg);

/********************************************
*********************************************
********************************************/

///////////////////������Ĳ��� end/////////////////

///////////////////ҵ����صĴ����� begin/////////////////
extern int DoTcpServer(CommLayerTcp *&TcpObj, int InFd);

extern int DoTcpClient(CommLayerTcp *&TcpObj);

extern int AllDealTcp(int SockFd, INLINK_SHARE *StrInlinkShare, PROTOCOL_PACK *InStrProPack, PROTOCOL_PACK *OutStrProPack);

extern int DealDoSendHeartbeat(CommLayerTcp *&TcpObj);

extern int CheckFeatureFile(char *pFileName, int *pnFeatureNum, int *pnFeatureSize);

extern int GetTcpFeatureData(char *pInFeature[], int InFeatureNum, int InFeatureSize, 
	char **OutFeatures , int *OutFeaturesLen);

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
extern int TcpSendPack(unsigned short flag, unsigned short index,
			unsigned short packType,unsigned int msgtype, unsigned short subType, unsigned int dataLen, void *data, PROTOCOL_PACK *OutStrProPack);

///////////////////ҵ����صĴ����� end/////////////////


int SendErrorMsg(unsigned short usPackIndex, int iErrorNo, char * pErrorMsg, PROTOCOL_PACK *OutStrProPack);


/***********************�������� end*****************************/

#ifdef WIN32
    #pragma pack()
#endif
	#undef PACK_ALIGN

#endif  /* _NETCOMM_H */
