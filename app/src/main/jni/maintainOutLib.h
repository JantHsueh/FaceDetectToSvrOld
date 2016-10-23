#ifndef __USER_OUT_LIB_H__
#define __USER_OUT_LIB_H__
#include <pthread.h>

#ifdef __cplusplus 
extern "C" { 
#endif

#ifdef WIN32
#define PACK_ALIGN
#else
#define PACK_ALIGN __attribute__((packed))
#endif

#define TRACE  printf
#define Free   free
#define Malloc  malloc

#define MAX_SCREEN_NUM 8
#define eWEEKDAY_NUM = 7

extern int iThreadExit;
/*
**  ���ͳ�����Ϣ�ṹ
*/
typedef struct ERROR_INFO
{
	int errorNo;			       /* ������� */
	unsigned char errorLen;	/* ������Ϣ���� */
	char errorMsg[256];		/* ������Ϣ���� */
	
} PACK_ALIGN ERROR_INFO, *LPERROR_INFO;


/* �������� */
typedef struct ConfigNetWork 
{
    char cNetType;              /* �������� 0-DHCP, 1-STATIC, 2-ADSL */
    char caIp[16];              /* �ն������IP��ַ */
    char caNetMask[16];         /* �������� */
    char caGateway[16];         /* ���� */
    char caDns[16];				/* DNS ������ */
	char caDns2[16]; 			
    char caMac[6];              /* MAC��ַ */
    char caNtpServer[16];       /* �ǿ�ʱ���SNTP ������ͬ��ʱ�� */
    char cNtpServerMode;        /* NTP ����ģʽ: 0-���ã�1-����ˣ�2-�ͻ��ˣ�3-�����+�ͻ��� */
    char caServerIP[16];		/* �ն���Ϊ�ͻ��ˣ����ӵķ�����IP */
    int  iPort;					/* �ն���Ϊ�ͻ��ˣ����ӵķ������˿� */
} CONFIG_NETWORK_STR;

typedef struct ConfigSecurity 
{
	char caWifiPwd[32];    	 /* wifi���� */
	
	char caUnlockPwd[32];    /* �������� */
	char caSdPwd[32];        /* SD������ */
    char caWebUser[32];      /* ��ҳ��¼�û��� */
    char caWebPwd[32];       /* ��ҳ��¼���� */
	unsigned char cKeyboardOrder;    /* ���ּ����� 0-������1-���� */
}CONFIG_SECURITY_STR;

#if 0

#define CONFIG_BASE_PATH        "config/configBase.xml"
#define CONFIG_NETWORK_PATH     "config/configNetWork.xml"
#define CONFIG_THRESHOLD_PATH   "config/configThreshold.xml"
#define CONFIG_SECURITY_PATH    "config/configSecurity.xml"
#define CONFIG_ATTENDANCE_PATH  "config/configAttendance.xml"
#define CONFIG_OTHERS_PATH      "config/configOthers.xml"
#define CONFIG_AUDIO_PATH       "config/configAudIo.xml"
#define CONFIG_VIDEO_PATH       "config/configVideo.xml"
#define CONFIG_LOGINREASON_PATH "config/configLoginReason.xml"
#define CONFIG_GROUPATTR_PATH   "config/configGroupAttr.xml"
#define CONFIG_AUTOONOFF_PATH   "config/configAutoOnOff.xml"
#define CONFIG_RING_PATH        "config/configRing.xml"
#define CONFIG_HOLIDAY_PATH     "config/configHoliday.xml"
#define CONFIG_WIEGAND_PATH     "config/configWiegand.xml"
#define CONFIG_SYNC_PATH		"config/configSync.xml"

/* ϵͳ���� */
typedef struct ConfigSys
{ 
	short sRecogRectX, sRecogRectY;		/* ʶ���У׼���� */
} CONFIG_SYS_STR;

/* �������� */
typedef struct ConfigBase
{ 
    unsigned short usAlgorithmVersion;  /* �㷨�汾 */
    unsigned char ucLanguage;           /* ���� 1-english, 2-�������ģ�3-�������� */
    unsigned char ucTz;                 /* ʱ�� */
    unsigned char ucYear, ucMonth, ucDay, ucHour, ucMinute, ucSecond;   /* ϵͳʱ�� */
    short sTouchX, sTouchY, sTouchWidth, sTouchHeight;  /* ������У׼���� */
    unsigned char ucTouchFlip;          /* ��Ļ��ת  0-normal, 1-flip */
    short sVideoOffsetX, sVideoOffsetY, sVideoZoom; /* ��ɫ��ͷУ׼:  ��ɫͼ����Ҫ��ƫ�Ƽ��Ŵ���( ��������100 ��ת�ɶ���) */
    int iAvg_x;                         /* �ڰ�ͼ�����ɫͼ��X����� */
    int iAvg_y;                         /* �ڰ�ͼ�����ɫͼ��Y����� */
    char cWiegandOut;                   /* Τ�������״̬  0��������  1��Τ��26��2: Τ��34 */
	char cDoorbell;						/* ���� */
    unsigned short usWiegandHID;        /* Τ�������HID�� */
    unsigned short usAttendInterval;    /* ���ڼ�� 0 - ����ʱ��������Ч��1 ~ 300������ʱ��������λΪ���� */
	short sRecogRectX, sRecogRectY;		/* ʶ���У׼���� */
	time_t  strLastGetUserRecords;      /* ���ͬ�����û���¼ʱ�� */
} CONFIG_BASE_STR;



typedef struct ConfigThreshold 
{
	unsigned char ucRecognise;      /* ʶ������ */
	unsigned char ucUpdate;         /* �������� */
    unsigned char ucVerifyThreshold;/* ��֤���� */  
	unsigned char ucTemplateNum;    /* ģ������ */
	unsigned char ucTemplateVerify; /* ���ʱ�� 0x01-ע��ʱ���ã�0x02-ʶ��ʱ����*/
	unsigned char ucDiffThreshold;  /* ��ֹͬ��ע������ */
	unsigned char ucSecondAverage;	/* �ڶ�ʶ������ */
	unsigned char ucSecondMaxNum;	/* �ڶ�ʶ�����޴��� */		
}CONFIG_THRESHOLD_STR;
    


typedef struct ConfigAttendance 
{
    int iRecordsPerPage;        /* ÿҳ��ʾ�ļ�¼�� */
    int iTimeDay;               /* ���ڷָ�ʱ��� ����ʱ��֮ǰ��Ϊͷһ�죬������Ϊ���� */
    int iTimeLate;              /* �ϰ���������ʱ����Ϊδ�� */
    int iTimeEarlyLeave;        /* �°����˳�����ʱ����Ϊδ�� */
    int iTimeUp1;               /* �����ϰ�ʱ�� */
    int iTimeDown1;             /* �����°�ʱ�� */
    int iTimeUp2;               /* �����ϰ�ʱ�� */
    int iTimeDown2;             /* �����°�ʱ�� */
    int iTimeUp3;               /* �����ϰ�ʱ�� */
    int iTimeDown3;             /* �����°�ʱ�� */
    int iTimeUp4;               /* ���� */
    int iTimeDown4;             /* ���� */
    int iTimeOverTime;          /* ��ʱ��֮���°����Ϊ�Ӱ� */
    int iTimeLateGrace;       /* �ٵ����޷����� */
    int iTimeEarlyLeaveGrace; /* ���˿��޷����� */
    int iTimeNoon;              /* ����ָ��㣬���������°໹�������ϰ� */
    int iTimeEvening;           /* ���Ϸָ��㣬���������°໹�������ϰ� */
}CONFIG_ATTENDANCE_STR;

typedef struct ConfigScreen
{
    unsigned char ucStatus;     /* ������ʹ��״̬��0 - ��ֹ��1 - ���� */		
    char caPicPath[256];        /* ����ͼƬ·�� */
}CONFIG_SCREEN_STR ;

typedef struct ConfigOthers
{
    unsigned char ucRecogMode;          /* ȫ��ʶ��ģʽ��0,һ��һ;1,һ�Զ�;2,ˢ�� */
    unsigned char ucSense;              /* �����Ӧ */
    unsigned char ucDoorMode;           /* Τ�������ʽ��1, WG26; 2, WG34 */
    char caDoorCommand[16];             /* ��������Э�� */ 
    int iCommLen;                       /* ����� */
    unsigned char ucAlarmShell;         /* ��ǲ�ж������0-�أ�1-�� */
    time_t  strLastGetAccessRecords;    /* ����ȡ���ڼ�¼ʱ�� */
    unsigned short usaRs485ControlCode[4];  /* 485������� */  
    unsigned char ucAccessCode;         /* ���ʷ�ʽ  0, WGCODE; 1, CARD*/
    int iDoorTime;                      /* 485������ʱ */
    unsigned short usaRs485ControlCodeEx[2];
    int iIoDoorTime;                    /* ���ų���ʱ��: ��λ�룬��Χ 1-15 */
    char cFaceCatch;                    /* ����ץ��ѡ��*/
    char cSenseThreshold;               /* ��Ӧ�������� */  
    unsigned char ucMGInput;        /* �����Ŵ����룬0 - �رգ�1 - ���� */	
    unsigned char ucMGDelayTime;    /* �ŴŸ澯�ӳ�ʱ��: ��λ�룬��Χ 1-99 */   
    unsigned char ucFireInput;      /* �������룬0 - �رգ�1 - ���� */	
    unsigned char ucIODoorOutPut;   /* �����������0 - �رգ�1 - ���� */
    unsigned char ucAudioOutPut;    /* ���������0 - �رգ�1 - ���� */
    unsigned char ucSyncMode;       /* ͬ����ʽ��0 - �ֶ���1 - �Զ� */
    unsigned char ucTimeMode;       /* ʱ����ʾ��ʽ��
                                        0.YY-MM-DD, 1.YY/MM/DD, 2.MM-DD-YY
                                        3.MM/DD/YY, 4.DD-MM-YY, 5.DD/MM/YY */
    struct ConfigScreen Screensaver[MAX_SCREEN_NUM];     /* ����ͼƬ·�� */
    unsigned char ucCurScreensaverIndex;    /* ��ǰ���������±� */

	char caPhoneNum[12];					/* �ֻ����� */
} CONFIG_OTHERS_STR;

typedef struct ConfigAudIo 
{
    int iVolMic;                        /* ������ */
    int iVolSpeaker;                    /* �������� ���� 0 �� 1 �� 2 �� 3 */
    unsigned char ucMute;               /* ����������־��0-������1-���� */
    unsigned char ucMuteVoice;          /* ����������־��0-������1-���� */
    unsigned char ucVoiceLang;          /* �������� 0-english, 1-�������ģ�2-��������(hk), 2-��������(tw)*/
}CONFIG_AUDIO_STR;

typedef	struct ConfigVideo 
{
    int iaBright[4];  /* �ڰ�����ͷ���� */
    int iaContrast[4];
    int iaHue[4];
    int iaExposure[4]; 
} CONFIG_VIDEO_STR;

typedef struct ConfigReason 
{
	unsigned char ucEnable;
	char caScription[32+1];
}CONFIG_REASON_STR;

typedef	struct ConfigLoginReason 
{
    unsigned char ucActive;         /* ����ʹ�� */
    unsigned char ucRecogMode[10]; /* 10�����ܼ���Ӧ��ʶ��ģʽ��0,һ��һ;1,һ�Զ�;2, ˢ�� */
    struct ConfigReason Reason[10];
}CONFIG_LOGINREASON_STR;

typedef	struct ConfigGroupAttr
{
    int iGroupId;        /* ��ID */
    int iNumPrefix;     
    char caEquipPwd[32+1];
    unsigned char ucAttr; /*normal--0,disable--1,in--2,out--2*/
    unsigned char ucType; /*0,���Ŷ�; 1,���ƶ�*/
    char caEquipName[64+1];
    int iEquipId;
}CONFIG_GROUPATTR_STR;

typedef	struct ConfigAutoOnOff  
{
	char cHolidayOff;        /* �ڼ����Զ��ػ� */
	char cTimerOff;          /* ��ʱ���ػ� */
	unsigned int auiOffTime[4];   /* �ػ�ʱ�� */
	unsigned int auiOnTime[4];    /* ����ʱ�� */
	char caForceOnPwd[16];    /* ǿ�ƿ������� */
}CONFIG_AUTO_ON_OFF_STR;

typedef struct Ring
{
    unsigned char ucStatus;   /* ���������ʹ��״̬��0 - ��ֹ��1 - ���� */		
    unsigned char ucHour;     /* Сʱ */
    unsigned char ucMinute;   /* ���� */
    unsigned char ucTimes;    /* ���Ŵ��� */
    unsigned char ucIndex;    /* ������� */
    unsigned char ucaWeekday[7]; /* ��һ�����յ�ʹ��״̬��0 - ��ֹ��1 - ���� */
}RING_STR;

typedef struct ConfigRing
{
    struct Ring Ring[10];
	unsigned int ulDuringTimes;		/* ÿ������������� */
}CONFIG_RING_STR;

#define MAX_HOLIDAY_NUM 24
typedef struct Holiday
{
    unsigned char ucStatus;   /* ʹ��״̬��0 - ��ֹ��1 - ���� */		
    char caName[33];          /* �������� */
    int iStartTime;           /* ��ʼʱ�� ��ʽ 20120501 */
    int iEndTime;             /* ����ʱ�� ��ʽ 20120505 */
}HOLIDAY_STR;

typedef struct ConfigHoliday
{
    unsigned char ucaWeekday[7]; /* ��һ�����յ�ʹ��״̬��0 - ��ֹ��1 - ���� */
    struct Holiday Holiday[MAX_HOLIDAY_NUM];
}CONFIG_HOLIDAY_STR;

typedef	struct ConfigWiegand   
{
	int iBitWidth;              /* Τ�������λ�� */
	short sImpulseWidth;        /* Τ������������� ��Χ20��800us�� Ĭ��100us */
	short sImpulseInterval;     /* Τ�������������  ��Χ200-20000us  Ĭ��900us */
	char cOutPutMode;           /* Τ������ĸ�ʽ 0�������� 1:���豸�ŵ�Τ��26  2:�����豸�ŵ�Τ��26  3:���豸�ŵ�Τ��34  4:�����豸�ŵ�Τ��34 */
	unsigned char ucDevId;      /* �豸�� ��Χ0-255 */
	short sImpulseWidthout;     /* Τ�������������  ��Χ20��800us�� Ĭ��100us */
	short sImpulseIntervalout;  /* Τ�������������  ��Χ200-20000us  Ĭ��900us */
	char cCardParse;            /* ���Ž���  0��WG34   1��WG26   2��ABA-10   3��ABA-8   4��ABA-5   Ĭ��ΪWG34 */
	char cOutPutFormat;         /* Τ��������ݣ�0-���ţ�1-���� */
	char caOoutPutContent[16];   /* ѡ��̶�����ʱ�� Ҫ��д���ֶ�  */
	unsigned short cErrorId;    /* ʧ��id  ��Χ��0-65534  ��ѡ��ر�ʱ�������Ĭ��Ϊ�رա� */
	unsigned short usAreaCode;  /* ���� Τ��26 <= 254 Τ��34 <= 65534 */
}CONFIG_WIEGAND_STR;

/* ͬ������ */
typedef struct ConfigSync
{
	char caIp[16];              /* ͬ�����ܵ���IP��ַ */
	char caPwd[10];				/* ͬ�����ܵ�����*/
	unsigned char ucSwitch;		/* ͬ��: 0 �� ; 1 �� */
}CONFIG_SYNC_STR;	




/**************************************************************\
** �������ƣ� GetConfigBase
** ���ܣ� ��ȡ��������
** ������ pstrConfigBase ָ��
** ���أ� 
** �������ߣ� ���
** �������ڣ� 2012-06-21
** �޸����ߣ� 
** �޸����ڣ� 
\**************************************************************/
void GetConfigBase(CONFIG_BASE_STR *pstrConfigBase);

/**************************************************************\
** �������ƣ� GetConfigNetWork
** ���ܣ� ��ȡ��������
** ������ pstrConfigNetwork ָ��
** ���أ� 
** �������ߣ� ���
** �������ڣ� 2012-06-21
** �޸����ߣ� 
** �޸����ڣ� 
\**************************************************************/
void GetConfigNetWork(CONFIG_NETWORK_STR *pstrConfigNetwork);

/**************************************************************\
** �������ƣ� GetConfigThreshold
** ���ܣ� ��ȡ��ֵ����
** ������ pstrConfigThreshold ָ��
** ���أ� 
** �������ߣ� ���
** �������ڣ� 2012-06-21
** �޸����ߣ� 
** �޸����ڣ� 
\**************************************************************/
void GetConfigThreshold(CONFIG_THRESHOLD_STR *pstrConfigThreshold);

/**************************************************************\
** �������ƣ� GetConfigSecurity
** ���ܣ� ��ȡ��ȫ����
** ������ pstrConfigSecurity ָ��
** ���أ� 
** �������ߣ� ���
** �������ڣ� 2012-06-21
** �޸����ߣ� 
** �޸����ڣ� 
\**************************************************************/
void GetConfigSecurity(CONFIG_SECURITY_STR *pstrConfigSecurity);

/**************************************************************\
** �������ƣ� GetConfigAttendance
** ���ܣ� ��ȡ��������
** ������ pstrConfigAttendance ָ��
** ���أ� 
** �������ߣ� ���
** �������ڣ� 2012-06-21
** �޸����ߣ� 
** �޸����ڣ� 
\**************************************************************/
void GetConfigAttendance(CONFIG_ATTENDANCE_STR *pstrConfigAttendance);

/**************************************************************\
** �������ƣ� GetConfigOthers
** ���ܣ� ��ȡ��������
** ������ pstrConfigOthers ָ��
** ���أ� 
** �������ߣ� ���
** �������ڣ� 2012-06-21
** �޸����ߣ� 
** �޸����ڣ� 
\**************************************************************/
void GetConfigOthers(CONFIG_OTHERS_STR *pstrConfigOthers);

/**************************************************************\
** �������ƣ� GetConfigAudIo
** ���ܣ� ��ȡ��������
** ������ pstrConfigAudio ָ��
** ���أ� 
** �������ߣ� ���
** �������ڣ� 2012-06-21
** �޸����ߣ� 
** �޸����ڣ� 
\**************************************************************/
void GetConfigAudIo(CONFIG_AUDIO_STR *pstrConfigAudio);

/**************************************************************\
** �������ƣ� GetConfigVideo
** ���ܣ� ��ȡ���������
** ������ pstrConfigVideo ָ��
** ���أ� 
** �������ߣ� ���
** �������ڣ� 2012-06-21
** �޸����ߣ� 
** �޸����ڣ� 
\**************************************************************/
void GetConfigVideo(CONFIG_VIDEO_STR *pstrConfigVideo);

/**************************************************************\
** �������ƣ� GetConfigLoginReason
** ���ܣ� ��ȡ���ܼ�����
** ������ pstrConfigLoginReason ָ��
** ���أ� 
** �������ߣ� ���
** �������ڣ� 2012-06-21
** �޸����ߣ� 
** �޸����ڣ� 
\**************************************************************/
void GetConfigLoginReason(CONFIG_LOGINREASON_STR *pstrConfigLoginReason);

/**************************************************************\
** �������ƣ� GetConfigGroupAttr
** ���ܣ� ��ȡ������
** ������ pstrConfigGroupAttr ָ��
** ���أ� 
** �������ߣ� ���
** �������ڣ� 2012-06-21
** �޸����ߣ� 
** �޸����ڣ� 
\**************************************************************/
void GetConfigGroupAttr(CONFIG_GROUPATTR_STR *pstrConfigGroupAttr);

/**************************************************************\
** �������ƣ� GetConfigAutoOnOff
** ���ܣ� ��ȡ�Զ����ػ�����
** ������ pstrConfigAutoOnOff ָ��
** ���أ� 
** �������ߣ� ���
** �������ڣ� 2012-06-21
** �޸����ߣ� 
** �޸����ڣ� 
\**************************************************************/
void GetConfigAutoOnOff(CONFIG_AUTO_ON_OFF_STR *pstrConfigAutoOnOff);

/**************************************************************\
** �������ƣ� GetConfigRing
** ���ܣ� ��ȡ��������
** ������ pstrConfigRing ָ��
** ���أ� 
** �������ߣ� ���
** �������ڣ� 2012-06-21
** �޸����ߣ� 
** �޸����ڣ� 
\**************************************************************/
void GetConfigRing(CONFIG_RING_STR *pstrConfigRing);

/**************************************************************\
** �������ƣ� GetConfigHoliday
** ���ܣ� ��ȡ�ڼ�������
** ������ pstrConfigHoliday ָ��
** ���أ� 
** �������ߣ� ���
** �������ڣ� 2012-07-06
** �޸����ߣ� 
** �޸����ڣ� 
\**************************************************************/
void GetConfigHoliday(CONFIG_HOLIDAY_STR *pstrConfigHoliday);

/**************************************************************\
** �������ƣ� GetConfigWiegand
** ���ܣ� ��ȡΤ������
** ������ pstrConfigWiegand ָ��
** ���أ� 
** �������ߣ� ���
** �������ڣ� 2012-06-21
** �޸����ߣ� 
** �޸����ڣ� 
\**************************************************************/
void GetConfigWiegand(CONFIG_WIEGAND_STR *pstrConfigWiegand);
/**************************************************************\
** �������ƣ� GetConfigSync
** ���ܣ� ��ȡͬ����������
** ������ pstrConfigSync ָ��
** ���أ� 
** �������ߣ� ���
** �������ڣ� 2012-10-18
** �޸����ߣ� 
** �޸����ڣ� 
\**************************************************************/
void GetConfigSync(CONFIG_SYNC_STR *pstrConfigSync);


/**************************************************************\
** �������ƣ� SetConfigBase
** ���ܣ� ���û�������
** ������ pstrConfigBase ָ��
** ���أ� 
** �������ߣ� ���
** �������ڣ� 2012-06-21
** �޸����ߣ� 
** �޸����ڣ� 
\**************************************************************/
void SetConfigBase(CONFIG_BASE_STR *pstrConfigBase);

/**************************************************************\
** �������ƣ� SetConfigSys
** ���ܣ� ���û�������
** ������ pstrConfigSys ָ��
** ���أ� 
** �������ߣ� ���
** �������ڣ� 2012-06-21
** �޸����ߣ� 
** �޸����ڣ� 
\**************************************************************/
void SetConfigSys(CONFIG_SYS_STR *pstrConfigSys);

/**************************************************************\
** �������ƣ� SetConfigNetwork
** ���ܣ� ������������
** ������ pstrConfigNetwork ָ��
** ���أ� 
** �������ߣ� ���
** �������ڣ� 2012-06-27
** �޸����ߣ� 
** �޸����ڣ� 
\**************************************************************/
void SetConfigNetwork(CONFIG_NETWORK_STR *pstrConfigNetwork);

/**************************************************************\
** �������ƣ� SetConfigThreshold
** ���ܣ� ����Threshold����
** ������ pstrConfigThreshold ָ��
** ���أ� 
** �������ߣ� ���
** �������ڣ� 2012-06-27
** �޸����ߣ� 
** �޸����ڣ� 
\**************************************************************/
void SetConfigThreshold(CONFIG_THRESHOLD_STR *pstrConfigThreshold);

/**************************************************************\
** �������ƣ� SetConfigSecurity
** ���ܣ� ���ð�ȫ����
** ������ pstrConfigSecurity ָ��
** ���أ� 
** �������ߣ� ���
** �������ڣ� 2012-06-27
** �޸����ߣ� 
** �޸����ڣ� 
\**************************************************************/
void SetConfigSecurity(CONFIG_SECURITY_STR *pstrConfigSecurity);

/**************************************************************\
** �������ƣ� SetConfigAttendance
** ���ܣ� ���ÿ�������
** ������ pstrConfigAttendance ָ��
** ���أ� 
** �������ߣ� ���
** �������ڣ� 2012-06-27
** �޸����ߣ� 
** �޸����ڣ� 
\**************************************************************/
void SetConfigAttendance(CONFIG_ATTENDANCE_STR *pstrConfigAttendance);

/**************************************************************\
** �������ƣ� SetConfigOthers
** ���ܣ� ���ÿ�������
** ������ pstrConfigOthers ָ��
** ���أ� 
** �������ߣ� ���
** �������ڣ� 2012-06-27
** �޸����ߣ� 
** �޸����ڣ� 
\**************************************************************/
void SetConfigOthers(CONFIG_OTHERS_STR *pstrConfigOthers);

/**************************************************************\
** �������ƣ� SetConfigAudIo
** ���ܣ� ���������������
** ������ pstrConfigOthers ָ��
** ���أ� 
** �������ߣ� ���
** �������ڣ� 2012-06-27
** �޸����ߣ� 
** �޸����ڣ� 
\**************************************************************/
void SetConfigAudIo(CONFIG_AUDIO_STR *pstrConfigAudio);

/**************************************************************\
** �������ƣ� SetConfigVideo
** ���ܣ� ��������ͷ�������
** ������ pstrConfigVideo ָ��
** ���أ� 
** �������ߣ� ���
** �������ڣ� 2012-06-27
** �޸����ߣ� 
** �޸����ڣ� 
\**************************************************************/
void SetConfigVideo(CONFIG_VIDEO_STR *pstrConfigVideo);

/**************************************************************\
** �������ƣ� SetConfigLoginReason
** ���ܣ� ���ù��ܼ��������
** ������ pstrConfigLoginReason ָ��
** ���أ� 
** �������ߣ� ���
** �������ڣ� 2012-06-27
** �޸����ߣ� 
** �޸����ڣ� 
\**************************************************************/
void SetConfigLoginReason(CONFIG_LOGINREASON_STR *pstrConfigLoginReason);

/**************************************************************\
** �������ƣ� SetConfigGroupAttr
** ���ܣ� ���ù��ܼ��������
** ������ pstrConfigGroupAttr ָ��
** ���أ� 
** �������ߣ� ���
** �������ڣ� 2012-06-27
** �޸����ߣ� 
** �޸����ڣ� 
\**************************************************************/
void SetConfigGroupAttr(CONFIG_GROUPATTR_STR *pstrConfigGroupAttr);

/**************************************************************\
** �������ƣ� SetConfigAutoOnOff
** ���ܣ� �����Զ����ػ��������
** ������ pstrConfigAutoOnOff ָ��
** ���أ� 
** �������ߣ� ���
** �������ڣ� 2012-06-27
** �޸����ߣ� 
** �޸����ڣ� 
\**************************************************************/
void SetConfigAutoOnOff(CONFIG_AUTO_ON_OFF_STR *pstrConfigAutoOnOff);

/**************************************************************\
** �������ƣ� SetConfigRing
** ���ܣ� ���������������
** ������ pstrConfigRing ָ��
** ���أ� 
** �������ߣ� ���
** �������ڣ� 2012-06-27
** �޸����ߣ� 
** �޸����ڣ� 
\**************************************************************/
void SetConfigRing(CONFIG_RING_STR *pstrConfigRing);

/**************************************************************\
** �������ƣ� SetConfigHoliday
** ���ܣ� ���ýڼ�������
** ������ pstrConfigHoliday ָ��
** ���أ� 
** �������ߣ� ���
** �������ڣ� 2012-07-06
** �޸����ߣ� 
** �޸����ڣ� 
\**************************************************************/
void SetConfigHoliday(CONFIG_HOLIDAY_STR *pstrConfigHoliday);

/**************************************************************\
** �������ƣ� SetConfigWiegand
** ���ܣ� ����Τ������
** ������ pstrConfigWiegand ָ��
** ���أ� 
** �������ߣ� ���
** �������ڣ� 2012-06-21
** �޸����ߣ� 
** �޸����ڣ� 
\**************************************************************/
void SetConfigWiegand(CONFIG_WIEGAND_STR *pstrConfigWiegand);
/**************************************************************\
** �������ƣ� SetConfigSync
** ���ܣ� ����ͬ������
** ������ pstrConfigSync ָ��
** ���أ� 
** �������ߣ� ���
** �������ڣ� 2012-10-18
** �޸����ߣ� 
** �޸����ڣ� 
\**************************************************************/
void SetConfigSync(CONFIG_SYNC_STR *pstrConfigSync);

/**************************************************************\
** �������ƣ� WriteFlash
** ���ܣ� ���豸��д������
** ������ mtd	д����豸
          offsetд���ƫ����
          pData	д������ݻ���
          nLen	д�����ݵĳ���
** ���أ� 
** �������ߣ� ���
** �������ڣ� 2012-10-18
** �޸����ߣ� 
** �޸����ڣ� 
\**************************************************************/
int WriteFlash(char* mtd, int offset, char* pData, int nLen);


/**************************************************************\
** �������ƣ� ReadFlash
** ���ܣ� ���豸�ж�����
** ������ mtd	д����豸
          offsetд���ƫ����
          pData	д������ݻ���
          nLen	д�����ݵĳ���
** ���أ� 
** �������ߣ� ���
** �������ڣ� 2012-10-18
** �޸����ߣ� 
** �޸����ڣ� 
\**************************************************************/
int ReadFlash(char* mtd, int offset, int nBufLen, char* pBuf);

/******************************************************************************
 * �������ƣ�   SetNet
 * ���ܣ�       �����������
 * ������ 	pIfName:�豸��
 		dwIpAddr:IP��ַ
 		dwSubMask:��������
 		pGW:����
 		pDNS:��ѡDNS������
 		pDNS2:��ѡDNS������
 		pMac:Mac��ַ
 		pHostname:������
 		nMTU������䵥Ԫ
 		bOnBoot������ʱ�Ƿ񼤻�����
 * ���أ�       0���ɹ���������ʧ��
 * �������ߣ� 
 * �������ڣ� 2012-6-21
 * �޸����ߣ�
 * �޸����ڣ�
 ******************************************************************************/
int SetNet(char* pIfName, unsigned long dwIpAddr, unsigned long dwSubMask, char* pGW, char* pDNS, char* pDNS2, char* pMac, char* pHostname, unsigned short nMTU, int bOnBoot);

/******************************************************************************
 * �������ƣ� getnet
 * ���ܣ� �õ��������
 * ������ 	pIfName:�豸��
 			pdwIpAddrp:IP��ַ
 			pdwSubMask:��������
 			pGW:����
 			pDNS:DNS������
 			pDNS2:
 			pMac:Mac��ַ
 			pHostname:������
 * ���أ� 
 * �������ߣ� 
 * �������ڣ� 2012-6-21
 * �޸����ߣ�
 * �޸����ڣ�
 ******************************************************************************/
int getnet(char* pIfName, unsigned long* pdwIpAddr, unsigned long* pdwSubMask, char* pGW, char* pDNS, char* pDNS2, char* pMac, char* pHostname);

/**************************************************************\
** �������ƣ� GetIPAddr
** ���ܣ� ��ȡIP��ַ
** ������ pIP	��ȡ����IP
          len	Ҫ��ȡ�ĳ���
** ���أ� ��
** �������ߣ� ���
** �������ڣ� 2012-07-02
** �޸����ߣ� 
** �޸����ڣ� 
\**************************************************************/
int GetIPAddr(char *pIP,int len);

/**************************************************************\
** �������ƣ� DelAllRecord
** ���ܣ� ɾ��ȫ����¼�������ļ�
** ������ ��
** ���أ�  0 : �ɹ�; -1 : ʧ��
** �������ߣ� ���
** �������ڣ� 2012-07-02
** �޸����ߣ� 
** �޸����ڣ� 
\**************************************************************/
int DelAllRecord();


/**************************************************************\
** �������ƣ� DelRecords
** ���ܣ� ɾ��iUserID ĳ��ʱ����ǰ�ļ�¼
** ������ tCurtime��ĳ��ʱ��
          iUserID : ĳ���û�Id
** ���أ� ��
** �������ߣ� ���
** �������ڣ� 2012-07-02
** �޸����ߣ� 
** �޸����ڣ� 
\**************************************************************/
int DelRecords(time_t tCurtime, int iUserID);

/**************************************************************\
** �������ƣ� GetItemList
** ���ܣ�  ��ȡ��Ҫ��¼����ͷָ��
** ������ ��
** ���أ�  
** �������ߣ� ���
** �������ڣ� 2012-07-02
** �޸����ߣ� 
** �޸����ڣ� 
\**************************************************************/
//FEATURE_Q_HEADER * GetItemList();

/**************************************************************\
** �������ƣ� FreeRecordList
** ���ܣ� �ͷ��Ѿ���ѯ�ļ�Ҫ��¼����
** ������ ��
** ���أ� ��
** �������ߣ� ���
** �������ڣ� 2012-07-02
** �޸����ߣ� 
** �޸����ڣ� 
\**************************************************************/
void FreeRecordList();

/**************************************************************\
** �������ƣ� DelRecordsByTime
** ���ܣ� ɾ�������û�ĳ��ʱ����ǰ�ļ�¼
** ������ tCurtime��ĳ��ʱ��
** ���أ� 0���ɹ���-1��ʧ�ܣ�
** �������ߣ� ���
** �������ڣ� 2012-07-02
** �޸����ߣ� 
** �޸����ڣ� 
\**************************************************************/
int DelRecordsByTime(time_t tCurtime);


/******************************************************************************
 * �������ƣ� GetSearchRecordNum
 * ���ܣ� ��ȡ��ǰ��ѯ��¼����
 * ������ ��
 * ���أ� ����ʱ��
 * �������ߣ� LPH
 * �������ڣ� 2012-11-05
 * �޸����ߣ�
 * �޸����ڣ�
 ******************************************************************************/
int GetSearchRecordNum();

/**************************************************************\
** �������ƣ� GetCurUSBTransNum
** ���ܣ� ��ȡ��ǰU���ѵ���/������¼���û�����
** ������ ��
** ���أ� ��ǰ��¼No
** �������ߣ� ���
** �������ڣ� 2012-07-02
** �޸����ߣ� 
** �޸����ڣ� 
\**************************************************************/
int GetCurUSBTransNum();

/**************************************************************\
** �������ƣ� SetCurUSBTransNum
** ���ܣ� ���õ�ǰU���ѵ���/������¼���û�����
** ������ iValue��Ҫ���õ�ֵ
** ���أ� ��
** �������ߣ� ���
** �������ڣ� 2012-07-02
** �޸����ߣ� 
** �޸����ڣ� 
\**************************************************************/
void SetCurUSBTransNum(int iValue);



/******************************************************************************
 * �������ƣ� formatSDEx
 * ���ܣ� ��ʽ��SD��
 * ������ ��
 * ���أ� 0���ɹ���-1��ʧ�ܣ�
 * �������ߣ� LPH
 * �������ڣ� 2012-11-05
 * �޸����ߣ�
 * �޸����ڣ�
 ******************************************************************************/
int formatSDEx();

int ExportAllLog(char *pDstPath);

#endif
#ifdef __cplusplus 
}
#endif


#endif
