#include <jni.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <android/log.h>
#include "FiCe.h"
#include "FiFacial.h"
#include "StdDefEx.h"
#include "FiFacialStd.h"

static const char *TAG="Face_Identify";
#define LOGD(fmt, args...) __android_log_print(ANDROID_LOG_DEBUG, TAG, fmt, ##args)
static const char* classname = "com/firs/facedetecttosvr/common/Face_Native";

/********************************************************************************
**  Copyright (c) 2012, �����з���˹�Ƽ����޹�˾
**  All rights reserved.
**
**  �ļ����ƣ� faceLib.h
**  �ο�������˹����ʶ��
**
**  ��ǰ�汾��1.0
**
**  �������ߣ�jason
**  ��������:  2013.11.08
**
**  �޸����ߣ� 
**  �޸�����: 
********************************************************************************/

#ifndef __FACE_LIB_H__
#define __FACE_LIB_H__

/* �ýṹ��Ϊ��⵽�����ۡ���λ������ */
typedef struct __FacePositionData
{
    int iFaceLeft;          /* �����X���� */
    int iFaceTop;           /* ���ϱ�Y���� */
    int iFaceRight;         /* ���ұ�X���� */
    int iFaceBottom;        /* ���±�Y���� */
    int iEyeLeftX;          /* ����X���� */
    int iEyeLeftY;          /* ����Y���� */
    int iEyeRightX;         /* ����X���� */
    int iEyeRightY;         /* ����Y���� */
    int iDistance;			/* ���� 1̫Զ ��1̫�� */
	int iMatchPos;			/* ʶ����� */
}FACE_POSITION_DATA_STR;

// �㷨����
typedef void* OD_CE_HANDLE;
typedef void* OD_CODEC_HANDLE;

typedef struct
{
	unsigned long id;
	int numMBLBP;
} USERINFO, *LPUSERINFO, **LLPUSERINFO;

#ifdef __cplusplus
extern "C" {
#endif

// ���㷨������
OD_CE_HANDLE OD_OpenEngine();

OD_CODEC_HANDLE OD_OpenFacialCodec(OD_CE_HANDLE hCe, char* pStrAuxDir, char** ppFeatureInBuf, int* pnErrNo);


void OD_freeShareMem();

char *OD_mallocShareMem();

void OD_CloseFacialCodec(OD_CODEC_HANDLE hCodec);


// �ر��㷨������
void OD_CloseEngine(OD_CE_HANDLE hCe);


int OD_DetectFacial(OD_CODEC_HANDLE hCodec, char* pYBuf, int nWidth, int nHeight, FI_FACIAL_DETECTX_RESULT* pResult);


/* ��׮ */
int DM2016_Authentication(char* pEncryptedCode, char* pPlanCode);


/******************************************************************************
 * �������ƣ� InitFaceLib
 * ���ܣ� ��ʼ��������
 * ������ piUserId: �û�id���� iUserNum: �û���
 * ���أ� ��
 * �������ߣ� Jason
 * �������ڣ� 2013-11-27
 * �޸����ߣ�
 * �޸����ڣ�
 ******************************************************************************/
void InitFaceLib(int* piUserId, int iUserNum);

/******************************************************************************
 * �������ƣ� DeinitFaceLib
 * ���ܣ� �ͷ���������Դ
 * ������ ��
 * ���أ� ��
 * �������ߣ� Jason
 * �������ڣ� 2013-11-27
 * �޸����ߣ�
 * �޸����ڣ�
 ******************************************************************************/
void DeinitFaceLib();

/******************************************************************************
 * �������ƣ� GetFacePosition
 * ���ܣ� ��ȡ��⵽��������λ������
 * ������ pFacePositionData�� 			
 * ���أ� �ޣ�
 * �������ߣ� 
 * �������ڣ� 2012-12-3
 * �޸����ߣ�
 * �޸����ڣ�
 ******************************************************************************/
void GetFacePosition(FACE_POSITION_DATA_STR *pFacePositionData);

/******************************************************************************
 * �������ƣ� ClearFacePosition
 * ���ܣ� �������������Ϣ
 * ������ 
 * ���أ� 
 * �������ߣ� Jason
 * �������ڣ� 2013-4-11
 * �޸����ߣ�
 * �޸����ڣ�
 ******************************************************************************/
void ClearFacePosition();

/******************************************************************************
 * �������ƣ� VerifyFace
 * ���ܣ� ʶ������ 
 * ������ puiMaxCompId:��������Ӧid��piMaxCompVal:��������
          uiId:����uiIdΪ0ʱ����1:N������Ϊ1:1ʶ�����������id
 * ���أ� -255û��⵽������-1ʧ�ܣ�0�ɹ�
 * �������ߣ� Jason
 * �������ڣ� 2013-11-20
 * �޸����ߣ�
 * �޸����ڣ�
 ******************************************************************************/
int VerifyFace(unsigned int* puiMaxCompId, int* piMaxCompVal, unsigned int uiId);

/******************************************************************************
 * �������ƣ� EnrollFace
 * ���ܣ� ע������
 * ������ uiId:�û�id��iMaxNum:���ע������
 * ���أ� ��ע������
 *        ע�ᴫ��uiId > 0�����ﵽiMaxNumʱ�洢���������ص��㷨�ڴ�,
 *        �ⲿ���жϷ���ֵ������ﵽ���ֵiMaxNum�����ע����������
 * �������ߣ� Jason
 * �������ڣ� 2013-11-20
 * �޸����ߣ�
 * �޸����ڣ�
 ******************************************************************************/
int EnrollFace(unsigned int uiId, int iMaxNum);

JNIEXPORT void JNICALL Java_com_firs_cn_FaceNative_initFaceLib(JNIEnv* env, jobject thiz);
JNIEXPORT jintArray JNICALL Java_com_firs_cn_FaceNative_recognizeFace(JNIEnv* env, jobject thiz, jint width,
        jint height, jboolean isFront, jbyteArray imgData);
JNIEXPORT void JNICALL Java_com_firs_cn_FaceNative_releaseFaceLib(JNIEnv* env, jobject thiz);


JNIEXPORT jint JNICALL Java_com_firs_cn_FaceNative_initTcp( JNIEnv* env,jobject obj);
JNIEXPORT jint JNICALL Java_com_firs_cn_FaceNative_UserAuth( JNIEnv* env,jobject obj,jbyteArray username,jbyteArray pwd);
JNIEXPORT void JNICALL Java_com_firs_cn_FaceNative_SetServerIP( JNIEnv* env,jobject obj, jbyteArray ip, jint iPort, jint ipChanged);
JNIEXPORT jint JNICALL Java_com_firs_cn_FaceNative_getAuth( JNIEnv* env,jobject obj);
JNIEXPORT jint JNICALL Java_com_firs_cn_FaceNative_getCompareFlag( JNIEnv* env,jobject obj);
JNIEXPORT jint JNICALL Java_com_firs_cn_FaceNative_getPictureFaceFlag( JNIEnv* env,jobject obj);
JNIEXPORT void JNICALL Java_com_firs_cn_FaceNative_sendPicture( JNIEnv* env,jobject obj);
JNIEXPORT void JNICALL Java_com_firs_cn_FaceNative_setThreadExit( JNIEnv* env,jobject obj);

JNIEXPORT jobject  JNICALL Java_com_firs_cn_FaceNative_getCompareResult(JNIEnv * env, jobject obj, jobject classobj);

JNIEXPORT jint JNICALL Java_com_firs_cn_FaceNative_getServerSockFlag( JNIEnv* env,jobject obj);

JNIEXPORT void JNICALL Java_com_firs_cn_FaceNative_SetScore( JNIEnv* env,jobject obj, jint iScore);


#ifdef __cplusplus
}
#endif

#endif//__TEMPLATE_LIB_H__

