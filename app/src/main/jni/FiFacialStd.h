
#ifndef _FI_FACIAL_STD_H
#define _FI_FACIAL_STD_H

#include "StdDefEx.h"

#ifdef __cplusplus
extern "C" {

#endif
#define FEATURESIZE_1K                    1024 /* 1K ������������ */
#define FEATURESIZE_2K                    2048 /* 2K ������������ */
#define FEATURESIZE_JNM                1024 /* JNM ������������*/
#define FEATURESIZE_RAW_JNM            3200 /* ��JNM ������������*/
#define FEATURESIZE_MAX                    FEATURESIZE_2K
#define FEATURESIZE_MBLBP                1024 /* MBLBP ������������*/

typedef struct _FI_FACIAL_ROTATE_PARAM
{
    FI_OPCMD common;
    int nPic; /* ��ͼ������1 or 2 or 4 */
    int angle[4]; /* ��ת�Ƕȣ�������90�ȵı�������90/180/-90 */
    int bMirror[4]; /* �Ƿ���*/
    int offsetx[4], offsety[4], zoom[4]; /* ����ת������ͼ�����ƫ�Ƽ��Ŵ�(�Ŵ����ѳ�100ת�ɶ���) */
} FI_FACIAL_ROTATE_PARAM, *LPFI_FACIAL_ROTATE_PARAM;

typedef struct _FI_FACIAL_INIT_PARAM
{
    FI_OPCMD common;
    FI_AUTH_KEYS answerKey; /* ����оƬ��������뷵�ص���֤��*/
} FI_FACIAL_INIT_PARAM, *LPFI_FACIAL_INIT_PARAM;

typedef struct _FI_FACIAL_INPUT_BASE_PARAM
{    
    FI_OPCMD common;
    int bRegistTemplate;  /* 0-ʶ��1-ע��*/
    int nWidth;
    int nHeight;
} FI_FACIAL_INPUT_BASE_PARAM, *LPFI_FACIAL_INPUT_BASE_PARAM;

typedef struct _FI_FACIAL_DETECT_EYE_STATUS_PARAM
{    
    FI_OPCMD common;
    int nWidth;
    int nHeight;
    FI_POINT ptLeftEye;
    FI_POINT ptRightEye;
} FI_FACIAL_DETECT_EYE_STATUS_PARAM, *LPFI_FACIAL_DETECT_EYE_STATUS_PARAM;

typedef struct _FI_FACIAL_VERSION
{
    unsigned int version;
}FI_FACIAL_VERSION, *LPFI_FACIAL_VERSION;

typedef struct _FI_FACIAL_ALGORSN
{
    char sn[256];
}FI_FACIAL_ALGORSN, *LPFI_FACIAL_ALGORSN;

typedef struct _FI_FACIAL_INPUT_ADJUST_PARAM
{    
    FI_OPCMD common;
    int bRegistTemplate;  /* 0-ʶ��1-ע��*/
    int nWidth;
    int nHeight;
    float fBrightnessReference;
    int nAdjust;
} FI_FACIAL_INPUT_ADJUST_PARAM, *LPFI_FACIAL_INPUT_ADJUST_PARAM;

typedef struct _FI_FACIAL_OUTPUT_INT
{
    int val;
} FI_FACIAL_OUTPUT_INT, *LPFI_FACIAL_OUTPUT_INT;

typedef struct _FI_FACIAL_OUTPUT_FLOAT
{
    float val;
} FI_FACIAL_OUTPUT_FLOAT, *LPFI_FACIAL_OUTPUT_FLOAT;

// ������(  ΪͶӰǰ����)  ���ݽṹ
// ����ȷ��32  λ���룬���ṹ��С�պ�Ϊ4  �ֽڵ�������
typedef struct _FI_FACIAL_MBLBP_RAW_FEA_DATA
{
    int fea_length;
    unsigned char fea[FEATURESIZE_RAW_JNM];
}FI_FACIAL_MBLBP_RAW_FEA_DATA;

// ������(  ΪͶӰǰ����)  ���ݴ洢��
// ����ȷ��32  λ���룬���ṹ��С�պ�Ϊ4  �ֽڵ�������
typedef struct _FI_FACIAL_MBLBP_RAW_FEA_STG
{
    FI_FACIAL_MBLBP_RAW_FEA_DATA    feaData1;
    FI_FACIAL_MBLBP_RAW_FEA_DATA    feaData2;
    unsigned char                     icon[120*142];    /* Сͼ*/
}FI_FACIAL_MBLBP_RAW_FEA_STG;


#if 1
/* ��ⷽʽ*/
typedef enum FI_FACIAL_DETECT_WAY_ENUM
{
    FI_FACIAL_DETECT_WAY_INVALID    = 0,        /*  ��Ч*/
    FI_FACIAL_DETECT_WAY_IR_GN        = 1,        /*  ����ͨ��*/
    FI_FACIAL_DETECT_WAY_IR_FP        = 2,        /*  �������*/
    FI_FACIAL_DETECT_WAY_VR_GN        = 3,        /*  �ɼ���ͨ��*/
    FI_FACIAL_DETECT_WAY_VR_FP        = 4,        /*  �ɼ������*/
    FI_FACIAL_DETECT_WAY_EyeStatus    = 5,        /*  ƣ�ͼ��*/
}FI_FACIAL_DETECT_WAY_ENUM;

/* ������*/
typedef enum FI_FACIAL_DETECT_NUM_ENUM
{
    FI_FACIAL_DETECT_ALL                = 0,        /*  ������������*/
    FI_FACIAL_DETECT_MAX                = 1,        /*  �����������*/
}FI_FACIAL_DETECT_NUM_ENUM;

/* �������ṹ*/
typedef struct _FI_FACIAL_DETECT_PARAM
{
    int width;
    int height;

    enum FI_FACIAL_DETECT_WAY_ENUM     detectWay;
    enum FI_FACIAL_DETECT_NUM_ENUM     detectNum;

    int bDetectFace;            /* �Ƿ��������������������������۾���ʱ���㷨����ָ����detectZone   �����ڼ���۾� */
    int bDetectEye;            /* �Ƿ����۾� */
    int bDetectEyeStatus;        /* �Ƿ���������״̬*/
    int bDetectMouth;        /* �Ƿ������*/

    RECT detectZone;    /* ������򣬼ȿ��Թ涨�����������Ҳ����ָ���۾�������� (����⵽��ʵ����������)   */

    int nMinHeadWidth;    /* ��С��ͷ��� */
    int nMaxHeadWidth;    /* �����ͷ��� */

    int nEyeStatusThreshold; /*
                                nEyeStatusThreshold==-1 ���ر��ۿ��Ŷȷ�����
                                nEyeStatusThreshold==0   ���������۷������������0   ��ʾʹ��Ĭ��ֵ��
                                nEyeStatusThreshold>=1   ���������۷��������nEyeStatusThreshold ��ʾ������ֵ��

                                // ����ֵ:
                                // =============== nEyeStatusThreshold>=0 ���������۷������=========================
                                // nEyeStatus==-1 ���ʧ��;
                                // nEyeStatus==0  ��ȫ����;
                                // nEyeStatus==1  ˫������;
                                // nEyeStatus==2  ��������;
                                // nEyeStatus==3  ��������;
                                // =============== nEyeStatusThreshold==-1 ���ر��ۿ��Ŷ�==============================
                                // nLeftCloseCreditScore ���ۿ��Ŷȣ�
                                // nRightCloseCreditScore   ���ۿ��Ŷȣ�
                                // nEyeStatus == (nLeftCloseCreditScore<<8) + (nRightCloseCreditScore);

                                ���������۷�����ֵ��ȡֵ��Χ[1, 100]��Ĭ��ֵΪ50������0   ��ʾʹ��Ĭ��ֵ��
                                ȡֵԽС�������ͼ��Ϊ����������ϸ�ȣ��������Ͽ����������۵ļ����
                            */

    //////////////////////////////////////////////////////////////////////////////////////////////
    // ����Ԥ����100  ���ֶΣ����ڶ������λ
    // �Ѿ�����Ĳ���Ϊ����Ҫ�ں����ע�ͣ�����������
    //////////////////////////////////////////////////////////////////////////////////////////////
    int nField0;  /* �Ƿ����ø߹����:  ==1 ����;  ==0  ��ֹ; */
    int nField1;  /* �߹�������ֵ��ȡֵ��Χ[0, 255]�� Ĭ��ֵ��190 */
    int nField2;  /* �߹������ֵ��ȡֵ��Χ��[1, 900]��Ĭ��ֵ��9 */
    int nField3;
    int nField4;
    int nField5;
    int nField6;
    int nField7;
    int nField8;
    int nField9;

#if 1
    int nField10;
    int nField11;
    int nField12;
    int nField13;
    int nField14;
    int nField15;
    int nField16;
    int nField17;
    int nField18;
    int nField19;

    int nField20;
    int nField21;
    int nField22;
    int nField23;
    int nField24;
    int nField25;
    int nField26;
    int nField27;
    int nField28;
    int nField29;

    int nField30;
    int nField31;
    int nField32;
    int nField33;
    int nField34;
    int nField35;
    int nField36;
    int nField37;
    int nField38;
    int nField39;

    int nField40;
    int nField41;
    int nField42;
    int nField43;
    int nField44;
    int nField45;
    int nField46;
    int nField47;
    int nField48;
    int nField49;
#endif

} FI_FACIAL_DETECT_PARAM, *LPFI_FACIAL_DETECT_PARAM;
#endif

// ������Ϣ�ṹ
typedef struct _FI_FACIAL_FACE_INFO
{
    int nFaceNum;                     /* ��⵽����������*/
    FI_RECT  rcFace;                 /* �������������*/
    FI_POINT ptLeftEye;            /* ��������*/
    FI_POINT ptRightEye;        /* ��������*/
    FI_POINT ptMouthPos;        /* �������*/
    FI_UINT  nEyeStatus;
                        // �������:
                        //         nEyeStatusThreshold>=0 ���������۷��������nEyeStatusThreshold==-1 ���ر��ۿ��Ŷ�
                        // ����ֵ:
                        // =============== nEyeStatusThreshold>=0 ���������۷������=========================
                        // nEyeStatus==-1 ���ʧ��;
                        // nEyeStatus==0  ��ȫ����;
                        // nEyeStatus==1  ˫������;
                        // nEyeStatus==2  ��������;
                        // nEyeStatus==3  ��������;
                        // =============== nEyeStatusThreshold==-1 ���ر��ۿ��Ŷ�==============================
                        // nLeftCloseCreditScore ���ۿ��Ŷȣ�
                        // nRightCloseCreditScore   ���ۿ��Ŷȣ�
                        // nEyeStatus == (nLeftCloseCreditScore<<8) + (nRightCloseCreditScore);

} FI_FACIAL_FACE_INFO, *LPFI_FACIAL_FACE_INFO;

// ��������ṹ
typedef struct _FI_FACIAL_OBJ_INFO
{
    int id;                                /* ����ID*/

    FI_FACIAL_FACE_INFO  faceInfo;        /* ��������*/

    FI_BOOL bAlarm;                        /* bAlarm==TRUE��    ��������������������ʧ�٣�����ʱ��ȡ���������������Ƭ
                                         bAlarm==FALSE��    û�о�����������������٣�������������...
                                        */

    int   nTrackPointNum;                    /* ���ٹ켣�����*/
    FI_POINT arrTrackPoint[256];            /* ���ٹ켣������*/
} FI_FACIAL_OBJ_INFO, *LPFI_FACIAL_OBJ_INFO;

// ��������
typedef struct _FD_DETECT_FACE_RESULT
{
    int nFaceNum;
    FI_RECT rtFace[10];
} FI_FACIAL_DETECT_FACE_RESULT, *LPFI_FACIAL_DETECT_FACE_RESULT;

// ������ٽ��
typedef struct _FI_FACIAL_DETECTX_RESULT
{
    int faceNum;
    FI_FACIAL_FACE_INFO faceInfo[50];
} FI_FACIAL_DETECTX_RESULT, *LPFI_FACIAL_DETECTX_RESULT;

typedef struct _FI_FACIAL_TRACKX_RESULT
{
    int objNum;
    FI_FACIAL_OBJ_INFO objInfo[50];
} FI_FACIAL_TRACKX_RESULT, *LPFI_FACIAL_TRACKX_RESULT;

/////////////////////////////////////////////////////////////////////////////
// ��ȡ�����������/  �����Ƭ����/  ����ṹ
/////////////////////////////////////////////////////////////////////////////
typedef struct _FI_FACIAL_GET_PHOTO_PARAM
{    
    FI_OPCMD common;
    int nObjID;            /*��������ID*/
} FI_FACIAL_GET_PHOTO_PARAM, *LPFI_FACIAL_GET_PHOTO_PARAM;

typedef struct _FI_FACIAL_GET_PHOTO_RESULT
{    
    int nSucceed;        /*�Ƿ�ִ�гɹ�:  ==1  �ɹ�;  ==0  ʧ��*/
    int nImageWidth;        /*ͼ����*/
    int nImageHeight;    /*ͼ��߶�*/
} FI_FACIAL_GET_PHOTO_RESULT, *LPFI_FACIAL_GET_PHOTO_RESULT;

/////////////////////////////////////////////////////////////////////////////
// 
/////////////////////////////////////////////////////////////////////////////

// ������������ȶ���������ṹ
typedef struct _FI_FACIAL_COMPARE_FEATURE_INPUT
{
    FI_OPCMD common;
    int bRegistTemplate; /* 0-ʶ��1-ע��*/
    int nFeaNum;
} FI_FACIAL_COMPARE_FEATURE_INPUT, *LPFI_FACIAL_COMPARE_FEATURE_INPUT;

// ���������ȶ�ֵ���
typedef struct _FI_FACIAL_COMPARE_FEATURE_RESULT
{
    int nFeaNum;
    int nMaxValueNo;            /* �ȶԷ��������û��бȶԷ�������ģ����� */
    float fMaxCompValue;        /*  �û�ƥ������ȶԷ���ֵ*/

    ////
    float fMaxCompValue_bd; // ���ƥ��ֵ����ֵ/��׼�
    float fCompValueAve;        // ��ֵ
    float fCompValueSd;            // ��׼��
} FI_FACIAL_COMPARE_FEATURE_RESULT, *LPFI_FACIAL_COMPARE_FEATURE_RESULT;

////////////////////////////////////////// compare feature x ///////////////////////////////////////////////////

// ���������ȶԲ����ṹ
typedef struct _FI_FACIAL_COMPARE_FEATURE_PARAM_X
{
    FI_OPCMD common;

    ////
//    int bReturnTemplateMaxCompareValue;    /* �Ƿ񷵻�ģ��ȶ����ֵ*/
//    int bReturnPeopleMaxCompareValue;        /* �Ƿ񷵻���Ա�ȶ����ֵ*/

    int nStatTopRangeNum;                    /* ��ֵ��ͳ��ģ�����*/
    int nStatBottomRangeNum;                /* ��ֵ��ͳ��ģ�����*/

    int nFeaTemplateLength;                    /* ����ģ�峤��*/

    int nReserve1[50];                        /* ����λ*/

    ////
    char curFeaTemplate[2048];                /* ���֧��2048  ��С��ģ��*/

    int nReserve2[50];                        /* ����λ*/
    char szReserve1[2048];                    /* ����λ*/

    ////
    int nRefUserNum;                            /* ����ȶԵ��˻�����*/
    int nRefFeaNum;                            /* ����ȶԵ�ģ������*/
    unsigned char arrRefUserFeaNum[2048];        /* �����ȶԵ�ÿ���˻�ӵ�е�ģ��������*/

    int nReserve3[50];                        /* ����λ*/
    char szReserve2[2048];                    /* ����λ*/

}FI_FACIAL_COMPARE_FEATURE_PARAM_X, *LPFI_FACIAL_COMPARE_FEATURE_PARAM_X;

// ���������ȶ�ֵ���
typedef struct _FI_FACIAL_COMPARE_FEATURE_RESULT_X
{
    /* �ۺϷ���*/
    int nMatchPeopleIndex;                        /* ƥ����Ա���*/
    int nMatchTemplageIndex;                    /* ƥ��ģ�����*/
    float fMaxCompareValue;                        /* ���ȶ�ֵ*/

    /* ȫֵ�����*/
    /* ����ÿ��������ģ���ͳ��ֵ*/
    int nMatchPeopleIndexForAllRange;            /* ƥ����Ա���*/
    int nMatchTemplageIndexForAllRange;            /* ƥ��ģ�����*/
    float fMaxCompareValueForAllRange;            /* ���ȶ�ֵ*/

    /* ��ֵ�����*/
    /* ����ÿ���˱�ֵ�ϸ�ģ���ͳ��ֵ*/
    int nMatchPeopleIndexForTopRange;            /* ƥ����Ա���*/
    int nMatchTemplageIndexForTopRange;        /* ƥ��ģ�����*/
    float fMaxCompareValueForTopRange;            /* ���ȶ�ֵ*/

    /* ��ֵ�����*/
    /* ����ÿ���˱�ֵ����ģ���ͳ��ֵ*/
    int nMatchPeopleIndexForMiddleRange;        /* ƥ����Ա���*/
    int nMatchTemplageIndexForMiddleRange;        /* ƥ��ģ�����*/
    float fMaxCompareValueForMiddleRange;        /* ���ȶ�ֵ*/

    /* ��ֵ�����*/
    /* ����ÿ���˱�ֵ�ϵ�ģ���ͳ��ֵ*/
    int nMatchPeopleIndexForBottomRange;        /* ƥ����Ա���*/
    int nMatchTemplageIndexForBottomRange;        /* ƥ��ģ�����*/
    float fMaxCompareValueForBottomRange;        /* ���ȶ�ֵ*/

    /* ����ƥ���˻�����ģ��(  ���10  ��ģ��)  �ȶ�ֵ*/
    float fCompageValueTemplate0;
    float fCompageValueTemplate1;
    float fCompageValueTemplate2;
    float fCompageValueTemplate3;
    float fCompageValueTemplate4;
    float fCompageValueTemplate5;
    float fCompageValueTemplate6;
    float fCompageValueTemplate7;
    float fCompageValueTemplate8;
    float fCompageValueTemplate9;

    /* ����ÿ��ģ��ȶ�ֵ*/
    float fCompageValue[1024*10];

    ////
    int      nReserve1[50];                        /* ����λ*/
    float     fReserve1[50];                        /* ����λ*/

    ////
    float fMaxCompValue_bd;                     /* ���ƥ��ֵ����ֵ/��׼�    */
    float fCompValueAve;                        /* ��ֵ                                    */
    float fCompValueSd;                        /* ��׼��                                */

    int      nReserve2[50];                        /* ����λ*/
    float     fReserve2[50];                        /* ����λ*/

    int nReserve3[50];                        /* ����λ*/
    char szReserve1[2048];                    /* ����λ*/

} FI_FACIAL_COMPARE_FEATURE_RESULT_X, *LPFI_FACIAL_COMPARE_FEATURE_RESULT_X;

////////////////////////////////////////// compare feature x ///////////////////////////////////////////////////

typedef struct _FI_FACIAL_IMAGEDEAL_PARAM
{
    FI_OPCMD common;
    int bHasUV;
} FI_FACIAL_IMAGEDEAL_PARAM, *LPFI_FACIAL_IMAGEDEAL_PARAM;


// ��arm  �˴���������Ϣ��ģ�����ݵ�dsp  �˵�ȫ����Ϣ�����ṹ
typedef struct _FI_FACIAL_TRANS_CONFIG_GLOBALINFO
{
     char g_guid[48];
     char g_version[48];
     int  g_nModelNum;
     int g_nScoreCF;
    int g_nScoreWeight1;
    int g_nScoreWeight2;
     int g_nThreshold;
     float g_fAlignRatio1;
     float g_fAlignRatio2;

    BOOL g_bEnableAntispooling;
    int  g_nAntispoolingFeaVer;
    int  g_nAntispoolingThreshold;

}FI_FACIAL_TRANS_CONFIG_GLOBALINFO;

// ��arm  �˴���������Ϣ��ģ�����ݵ�dsp  �ˣ�������Ҫ����7  M  �ڴ�ռ�
#if 0
// ��ʼ����İ汾����Ҫ�ϴ���ڴ�ռ�֧��
typedef struct _FI_FACIAL_TRANS_CONFIG_MODEL
{
    int  fbGlobalInfoLen;
    char fbGlobalInfo[1024];                        // FI_FACIAL_TRANS_CONFIG_GLOBALINFO

    ////
    int  fbModel1ParamLen;
    char fbModel1Param[1024];

    int  fbModel1ParaDataLen;
    char fbModel1ParaData[1024*1024*1];

    int  fbModel1ProjDataLen;
    char fbModel1ProjData[1024*1024*2];

    ////
    int  fbModel2ParamLen;
    char fbModel2Param[1024];

    int  fbModel2ParaDataLen;
    char fbModel2ParaData[1024*1024*1];

    int  fbModel2ProjDataLen;
    char fbModel2ProjData[1024*1024*2];

}FI_FACIAL_TRANS_CONFIG_MODEL;
#else
// ��Եͳɱ������޸ĵİ汾��֧�ֽ������ڴ�����������Ҫ����3.5  M  �ڴ�ռ�
typedef struct _FI_FACIAL_TRANS_CONFIG_MODEL
{
    int  fbGlobalInfoLen;
    char fbGlobalInfo[1024];                        // FI_FACIAL_TRANS_CONFIG_GLOBALINFO

    ////
    int  fbModel1ParamLen;
    char fbModel1Param[1024];

    int  fbModel1ParaDataLen;
    char fbModel1ParaData[1024*100];  /* Ŀǰʵ�������Ҫ��36K�����ﶨ�����֧��100K */

    int  fbModel1ProjDataLen;
    char fbModel1ProjData[1024*1500]; /* Ŀǰʵ�������Ҫ��1250K�����ﶨ�����֧��1500K */

    ////
    int  fbModel2ParamLen;
    char fbModel2Param[1024];

    int  fbModel2ParaDataLen;
    char fbModel2ParaData[1024*100];  /* Ŀǰʵ�������Ҫ��36K�����ﶨ�����֧��100K */

    int  fbModel2ProjDataLen;
    char fbModel2ProjData[1024*1500]; /* Ŀǰʵ�������Ҫ��1250K�����ﶨ�����֧��1500K */

}FI_FACIAL_TRANS_CONFIG_MODEL;
#endif

#ifdef __cplusplus
}
#endif

#endif // _FI_FACIAL_STD_H


