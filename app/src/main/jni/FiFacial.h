
#ifndef _FI_FACIAL_H
#define _FI_FACIAL_H

#include "StdDefEx.h"
#include "FiFacialStd.h"

#ifdef __cplusplus
extern "C" {
#endif

#if defined(WIN32)
	#if defined(LIB_EXPORTS)	// ��̬��
		#define FI_FACIAL_API __declspec(dllexport)
	#else
		#if defined(_LIB)		// ��̬��
			#define FI_FACIAL_API 
		#elif defined(_VRE)
			#define FI_FACIAL_API 
		#else
			#define FI_FACIAL_API __declspec(dllimport)
		#endif
	#endif
#else
		#define FI_FACIAL_API 
#endif

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//									��ʼ��/����ʼ������
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ȫ�ֽ���Ҫ����һ�Σ�����ϵͳ��ʼ������ϵͳ�˳�ʱ������һ��
FI_FACIAL_API FI_BOOL FI_FACIAL_InitContext();
FI_FACIAL_API void FI_FACIAL_UninitContext();

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//									ͨ����������
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// ������������������С��Ĭ����20M��
// ������FI_FACIAL_OpenCodec ֮ǰ���÷���Ч
// Ŀǰ�������õĴ�С��>0 && <=20
FI_FACIAL_API FI_BOOL FI_FACIAL_SetInFeatureBufSize(int buf_size_m/*��λ��M*/);

// ��Codec���ɹ�����0��ʧ�ܷ���-1
FI_FACIAL_API FI_HANDLE FI_FACIAL_OpenCodec(FI_HANDLE hFiCe, char authKeys[32]);

// ��ʼ��Codec
// ����ֵ:
// ==0 ��ʼ��ʧ��
// ==1 ��ʼ���ɹ���������֤ͨ��
// ==2 ��ʼ���ɹ���ǿ�ư�Ȩ����	
FI_FACIAL_API int FI_FACIAL_InitCodec(FI_HANDLE hFiCodec, char randkeys[32]);

// �ر�Codec
FI_FACIAL_API void FI_FACIAL_CloseCodec(FI_HANDLE hFiCodec);

// ���ø���Ŀ¼
// ����ģ�����ݵȣ�
// ���û�����ã���Ĭ��Ϊ��ǰ·����
// ��FI_FACIAL_OpenCodec ֮����FI_FACIAL_InitCodec  ֮ǰ���á�
FI_FACIAL_API void FI_FACIAL_SetAuxDir(FI_HANDLE hFiCodec, char strAuxDir[256]);

// ��ȡ�㷨�汾��
// �汾�Ŷ����ʽ:  ModelMainVers . ModelSubVers . ProjDiffVers . Reserve
// ����arm  ʹ��С���򣬹ʵõ���unsigned int  version ��������:
// ModelMainVers 	= (unsigned char)(version>>0);
// ModelSubVers 	= (unsigned char)(version>>8);
// ProjDiffVers 	= (unsigned char)(version>>16);
// Reserve 		= (unsigned char)(version>>24);
// ������صİ汾����0�����ʾ��ȡ�汾��ʧ��
FI_FACIAL_API unsigned int FI_FACIAL_GetVersion(FI_HANDLE hFiCodec);

// ��ȡ�㷨���к�
// ����ֵ��SN  ���ַ�����
// ������ص�SN  �ַ�������0�����ʾ��ȡ���к�ʧ��
FI_FACIAL_API int FI_FACIAL_GetAlgorSN(FI_HANDLE hFiCodec, char strSN[256]);


// ��ȡ����������������( 20 M )   ��ַ
FI_FACIAL_API char * FI_FACIAL_GetInFeatureBuf(FI_HANDLE hFiCodec);


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//									�������ۼ��
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


// ������Ϣ���(  ����ͨ�ð汾)
FI_FACIAL_API int FI_FACIAL_Detect_IR_GN(FI_HANDLE hFiCodec, const char* pYBuf, int nWidth, int nHeight, LPFI_FACIAL_FACE_INFO pResult);

// ������Ϣ���(  ������ٰ汾)
FI_FACIAL_API int FI_FACIAL_Detect_IR_FP(FI_HANDLE hFiCodec, const char* pYBuf, int nWidth, int nHeight, LPFI_FACIAL_FACE_INFO pResult);

// ������Ϣ���(  �ɼ���ͨ�ð汾)
FI_FACIAL_API int FI_FACIAL_Detect_VR_GN(FI_HANDLE hFiCodec, const char* pYBuf, int nWidth, int nHeight, LPFI_FACIAL_FACE_INFO pResult);

// ������Ϣ���(  �ɼ�����ٰ汾)
FI_FACIAL_API int FI_FACIAL_Detect_VR_FP(FI_HANDLE hFiCodec, const char* pYBuf, int nWidth, int nHeight, LPFI_FACIAL_FACE_INFO pResult);


//
// ������Ϣ���
// ����ֵ:
//	== -1 	���ʧ��
// 	>=  0	��⵽����������
//
// FI_FACIAL_DETECT_WAY_INVALID        ��Ч
// FI_FACIAL_DETECT_WAY_IR_GN		�������������ͨ�汾
// FI_FACIAL_DETECT_WAY_IR_FP		�������������ٰ汾
// FI_FACIAL_DETECT_WAY_VR_GN		�ɼ������������ͨ�汾
// FI_FACIAL_DETECT_WAY_VR_FP		�ɼ������������ٰ汾
// FI_FACIAL_DETECT_WAY_EyeStatus	ר���۾��պ�״̬���(��ϰ汾�������������)
//							���۱պ�״̬���ר�ð汾��ʹ�ÿɼ�������+  ��������+  ��������״̬
FI_FACIAL_API int FI_FACIAL_DetectX(FI_HANDLE hFiCodec, const char* pYBuf, const FI_FACIAL_DETECT_PARAM* pDetectParam, FI_FACIAL_DETECTX_RESULT *pResult/*, unsigned char * pIcon*/);

typedef enum FI_FACIAL_CROP_ICON_TYPE
{
	FI_FACIAL_CROP_FACE_ICON_FOR_DETECT,		/* �������ʱ������Сͼ���ֱ�����120x142 */
	FI_FACIAL_CROP_FACE_ICON_FOR_EYESTATUS,	/* ƣ�ͼ��ʱ������Сͼ���ֱ�����120x142 */
	FI_FACIAL_CROP_FACE_ICON_FOR_FEATURE1,		/* ��ȡ����ʱ������Сͼ���ֱ�����120x142 */
	FI_FACIAL_CROP_FACE_ICON_FOR_FEATURE2,		/* ��ȡ����ʱ������Сͼ���ֱ�����128x128 */
	FI_FACIAL_CROP_EYE_ICON_FOR_EYESTATUS,		/* ƣ�ͼ��ʱ���۾�Сͼ���ֱ�����24x12 */
}FI_FACIAL_CROP_ICON_TYPE;

typedef struct FI_FACIAL_CROP_ICON_PARAM
{
	int width;							/* ͼ����*/
	int height;							/* ͼ��߶�*/
	FI_RECT  rtFace;						/* ��������*/
	FI_POINT ptLeftEye;					/* ��������*/
	FI_POINT ptRightEye;					/* ��������*/
	FI_POINT ptMouth;					/* ��������*/
	FI_FACIAL_CROP_ICON_TYPE type;		/* �ü�����*/
}FI_FACIAL_CROP_ICON_PARAM;

FI_FACIAL_API FI_BOOL FI_FACIAL_CropIcon(FI_HANDLE hFiCodec, const unsigned char* pYBuf, FI_FACIAL_CROP_ICON_PARAM param, unsigned char* pIconBuf, int* nIconWidth, int* nIconHeight);


//// old��don't use
//// ���ӿ��Ѿ���ʱ�������Ǽ�����ǰ�ϰ汾
static int FI_FACIAL_Detect(FI_HANDLE hFiCodec, const char* pYBuf, int nWidth, int nHeight, LPFI_FACIAL_FACE_INFO pResult)
{
	return FI_FACIAL_Detect_IR_GN(hFiCodec, pYBuf, nWidth, nHeight, pResult);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//									�����������
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////
// �������ƣ�FI_FACIAL_TrackX
// ���������������������
// ���������
//				hFiCodec		ͨ�������FI_HANDLE��
//				pBufYUV420	ͼ������( RGB  ��ʽ)
//				detectParam	������
// ���������
//				pResult		������ٵ�������������Ϣ
// ���ؽ����
//				==-1 ����ʧ��; >=0  ���ٵ��Ķ������
// ����˵����
//				���ñ��ӿڻ��������������٣����������и��ٵ�������������Ϣ��
//				ÿ�η��صĶ������ʵʱ����;�������
//				ʵʱ����:  ����ǰ֡��Ч���ٵĶ��󣬴�ʱ����FI_FACIAL_GetNewlyPhoto  �ӿڻ�ȡʵʱ��Ƭ��
//				��������:  ���Ѿ�ʧ�ٵĶ��󣬴�ʱ����FI_FACIAL_GetBestPhoto  �ӿڻ�ȡ�����Ƭ��
//				ͨ��FI_FACIAL_OBJ_INFO  �ṹ��bAlarm  ���ж϶�����ʵʱ�����Ǿ�������
//				
//				FI_FACIAL_OBJ_INFO objInfo;
//				if (objInfo.bAlarm)
//				{
//					// ��������
//
//					// ��ȡ�����Ƭ�����ڱ���
//					FI_FACIAL_GetBestPhoto(...);
//				}
//				else
//				{
//					// ʵʱ����
//					
//					// ��ȡʵʱ��Ƭ��������ʾ
//					FI_FACIAL_GetNewlyPhoto(...);
//				}
//
///////////////////////////////////////////////////////////
FI_FACIAL_API int FI_FACIAL_TrackX(FI_HANDLE hFiCodec, const char* pBufYUV420, const FI_FACIAL_DETECT_PARAM* pDetectParam, FI_FACIAL_TRACKX_RESULT *pResult);

///////////////////////////////////////////////////////////
// �������ƣ�FI_FACIAL_GetNewlyPhoto
// ������������ȡ��������������Ƭ
// ���������
//				hFiCodec		ͨ�������FI_HANDLE��
//				nObjID			����ID
// ���������
//				pImageData		�����Ƭ���ݣ�Ϊ RGB 24 ��ʽ���ռ��ɵ����߷�����ͷţ�����ǰ��������㹻��С�Ŀռ�
//				nImageWidth		�����Ƭ���
//				nImageHeight	�����Ƭ�߶�
// ���ؽ����
//				�Ƿ�ɹ���==TRUE �ɹ���==FALSE ʧ�ܣ�
// ����˵����
///////////////////////////////////////////////////////////
FI_FACIAL_API FI_BOOL FI_FACIAL_GetNewlyPhoto(FI_HANDLE hFiCodec, int nObjID, 
													unsigned char *pImageData, int *nImageWidth, int *nImageHeight);


///////////////////////////////////////////////////////////
// �������ƣ�FI_FACIAL_GetBestPhoto
// ������������ȡ�������������Ƭ
// ���������
//				hFiCodec		ͨ�������FI_HANDLE��
//				nObjID			����ID
// ���������
//				pImageData		�����Ƭ���ݣ�Ϊ RGB 24 ��ʽ���ռ��ɵ����߷�����ͷţ�����ǰ��������㹻��С�Ŀռ�
//				nImageWidth		�����Ƭ���
//				nImageHeight	�����Ƭ�߶�
// ���ؽ����
//				�Ƿ�ɹ���==TRUE �ɹ���==FALSE ʧ�ܣ�
// ����˵����
///////////////////////////////////////////////////////////
FI_FACIAL_API FI_BOOL FI_FACIAL_GetBestPhoto(FI_HANDLE hFiCodec,  int nObjID,
									   				unsigned char *pImageData, int *nImageWidth, int *nImageHeight);

///////////////////////////////////////////////////////////
// �������ƣ�FI_FACIAL_GetBestPhotoBackground
// ������������ȡ�������������Ƭ����ͼ��
// ���������
//				hFiCodec		ͨ�������FI_HANDLE��
//				nObjID			����ID
// ���������
//				pImageData		�����Ƭ���ݣ�Ϊ RGB 24 ��ʽ���ռ��ɵ����߷�����ͷţ�����ǰ��������㹻��С�Ŀռ�
//				nImageWidth		�����Ƭ���
//				nImageHeight	�����Ƭ�߶�
// ���ؽ����
//				�Ƿ�ɹ���==TRUE �ɹ���==FALSE ʧ�ܣ�
// ����˵����
///////////////////////////////////////////////////////////
FI_FACIAL_API FI_BOOL FI_FACIAL_GetBestPhotoBackground(FI_HANDLE hFiCodec,  int nObjID,
									   				unsigned char *pImageData, int *nImageWidth, int *nImageHeight);

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//									����״̬���
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// ��������״̬���
// *pEyeStatus
// 		==0 ��ȫ����
//		==1 ˫������
//		==2 ��������
//		==3 ��������
// ���������պ�״̬���������㷨ԭ��Ч���ȿɼ���汾(�ȽϾɵ��㷨ԭ��) ��
FI_FACIAL_API int FI_FACIAL_DetectEyeStatusIR(FI_HANDLE hFiCodec, char* pYBuf, int nWidth, int nHeight, FI_POINT ptLeftEye, FI_POINT ptRightEye, int *pEyeStatus);

// �ɼ�������״̬���
// *pEyeStatus
// 		==0 ��ȫ����
//		==1 ˫������
//		==2 ��������
//		==3 ��������
FI_FACIAL_API int FI_FACIAL_DetectEyeStatusVR(FI_HANDLE hFiCodec, char* pYBuf, int nWidth, int nHeight, FI_POINT ptLeftEye, FI_POINT ptRightEye, int *pEyeStatus);


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//									������ȡ�ȶ�
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// JNM ��������
/***************************************************************
 * ���ص�����:
 * 1. ��ȡ����������(1024 Byte)
 * 2. ���ݽ����ĻҶ�ͼ(width x height)
 * 3. ICON1, �ü��ĳ�ʼСͼ(120 x 142)
 * 4. ICON2, ����ֱ��ͼ�˲���Сͼ(120 x 142)
 ����ֵ:
 ==0    �ɹ�
 ==-1  �߼��쳣ʧ��
 ==-2  ��Ƭ����ʧ��(Ŀǰ����֧��FI_FACIAL_GetJNMFeature  �ӿ�)
 ****************************************************************
*/ 
FI_FACIAL_API int FI_FACIAL_GetJNMFeature(FI_HANDLE hFiCodec, const char* pYBuf, int nWidth, int nHeight, LPFI_FACIAL_FACE_INFO pFacial, char* pFeatureJNM, int nFeatureBufSize);
FI_FACIAL_API int FI_FACIAL_GetJNMFeatureAdjust(FI_HANDLE hFiCodec, const char* pYBuf, int nWidth, int nHeight, LPFI_FACIAL_FACE_INFO pFacial, char* pFeatureJNM, int nFeatureBufSize, float fBrightnessReference, int nAdjust);
FI_FACIAL_API int FI_FACIAL_GetJNMFeature_FromIcon(FI_HANDLE hFiCodec, const char* pIconYBuf, char* pFeatureJNM);

// ��ȡ�ֲ���������������ѵ��
FI_FACIAL_API FI_BOOL FI_FACIAL_GetJNMRawFeature(FI_HANDLE hFiCodec, char* pYBuf, int nWidth, int nHeight, LPFI_FACIAL_FACE_INFO pFacial, FI_FACIAL_MBLBP_RAW_FEA_STG* pJNMRawFea);

// �������������
FI_FACIAL_API float FI_FACIAL_CompareJNMFeature(FI_HANDLE hFiCodec, char* pCurFeature, char* pRefFeature);

// ����ԭʼchar  ����
FI_FACIAL_API float FI_FACIAL_CompareJNMFeatureByRawFea(FI_HANDLE hFiCodec, char* pCurFeature, char* pRefFeature, int nFeaLength);

/*
	pCurFeature = Feature(1024Byte) + UserFeaNum(1Byte) + UserFeaNum(1Byte) + UserFeaNum(1Byte) + UserFeaNum(1Byte) ...
	pRefFeature = Feature(1024Byte) + Feature(1024Byte) + Feature(1024Byte) + Feature(1024Byte) + Feature(1024Byte) + ...
	UserFeaNum ��ʾpRefFeature  �洢�Ĳο����������У��ڼ����û�ӵ�м����ο�������
*/
FI_FACIAL_API int FI_FACIAL_CompareJNMFeatureMore(FI_HANDLE hFiCodec, char* pCurFeature, char* pRefFeature, int nRefFeatureNum, LPFI_FACIAL_COMPARE_FEATURE_RESULT pResult);
FI_FACIAL_API int FI_FACIAL_Proj_Comp(FI_HANDLE hFiCodec, char* pDataInBuf, char* pCompResult);

//
FI_FACIAL_API int FI_FACIAL_CompareJNMFeatureX(FI_HANDLE hFiCodec, LPFI_FACIAL_COMPARE_FEATURE_PARAM_X pParam, char* pRefFeatureBuf, LPFI_FACIAL_COMPARE_FEATURE_RESULT_X pResult);


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////// 
//
//									����ͼ����
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////// 

// ͼ����
FI_FACIAL_API int FI_FACIAL_Rolate_Yuv422_D1_To_Y_VGA_AND_SIF(FI_HANDLE hFiCodec, char* pD1Yuv422, char* pVga, char* pSif);
FI_FACIAL_API int FI_FACIAL_Rolate_Yuv422_D1_To_YUV420P_SIF(FI_HANDLE hFiCodec, char* pD1Yuv422, char* pSif, int nAngle, int bMirror, int nOffsetX, int nOffsetY, int nZoom);
FI_FACIAL_API int FI_FACIAL_Rolate_Yuv422_D1_To_YUV420P_2SIF(FI_HANDLE hFiCodec, char* pD1Yuv422, char* pSif1, char* pSif2, int nOffsetX, int nOffsetY, int nZoom);

FI_FACIAL_API int FI_FACIAL_Rolate_Y_Rotate_320x240_to_240x320(FI_HANDLE hFiCodec, unsigned char *bufYSrc, int nWidth, int nHeight, unsigned char *bufYDst);

FI_FACIAL_API void FI_FACIAL_YUV422VGA_TO_YUV420P_And_Rotate90(FI_HANDLE hFiCodec, char* pYuv422Vga, char* pVga, char* pSif, FI_BOOL bUV);

// ����
FI_FACIAL_API int FI_FACIAL_Test_Mem(FI_HANDLE hFiCodec, char* pInBuf, char* pOutBuf, int nLen);


////
#ifdef __cplusplus
}
#endif

#endif //_FI_FACIAL_H

