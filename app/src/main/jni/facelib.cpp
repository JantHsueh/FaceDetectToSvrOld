#include <unistd.h>
#include <pthread.h> 
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>
#include <netinet/in.h>
#include <linux/route.h>
#include <pthread.h> /* support MULTI-THREADS lock */
#include <asm/types.h>
#include <sys/mman.h>
#include <linux/types.h>
#include <linux/videodev2.h>  
#include<jni.h>

#include "faceLib.h"
#include "Public.h"
#include "FiCe.h"
#include "FiFacial.h"
#include "StdDefEx.h"
#include "FiFacialStd.h"
#include "netComm.h"
#include "NetDeal.h"
#include "gb2312.h"

/*
 * RecognizeFace.c
 *
 *  Created on: 2013-11-8
 *      Author: Administrator
 */
//ʶ��ɼ�jpgͼƬ����
#define DEFAULT_JPGE_QUALITY 100
//ͼƬ�����ת����
#define IMAGE_MAX_ROTATE_TIMES 2

#define INVALID         0xFFFFFFFF
#define INVALID_VALUE       INVALID

#ifndef INVALID_HANDLE_VALUE
#define INVALID_HANDLE_VALUE    ((HANDLE)INVALID)
#endif

/*******************************************ȫ�ֱ���*****************************************/
/* ȫ�ֵĴ����㷨������ָ�� */
char * gpUsersFeature = NULL;

/* ���㷨��ľ�� */
OD_CE_HANDLE ghCe = NULL;

/* ���㷨�����ģ��ľ�� */
OD_CODEC_HANDLE ghFacialCodec;

/* ���㷨�����ģ��Ĵ����� */
int giCodecErrorNo = 0;

/* Ӧ�ó������㷨֮��Ĺ����ڴ棬20M�ֽ� */
void* gpSharedMem = NULL;

/* Ӧ�ó������㷨֮��Ĺ����ڴ��С */
int giSharedMemSize = 0;

/* ������Ŀ */
int giTotalFeatureNum = 0;

/* �洢����ֵ��Կ */
char gcaFeaturePwd[33] = { 0 };

/* ��¼ȫ���û������Ա����Ϣ */
USERINFO gstrUserList[MAX_USER_NUM] = { 0 };

static char s_D1_BUF[D1_WIDTH * D1_HEIGHT * 2];
static char s_SIF_BUF[2][SIF_HEIGHT * SIF_WIDTH * 4];
static char s_SIF_BUF_TMP[2][SIF_HEIGHT * SIF_WIDTH * 2] = { { 0 } };

char g_SIF_BUF_Send[SIF_HEIGHT * SIF_WIDTH * 2] = { 0 };

/* ��ʱ����buf */
char g_cShareMem[20 * 1024] = { 0 };

HANDLE g_hVideoColor = INVALID_HANDLE_VALUE;
HANDLE g_hVideoIrda = INVALID_HANDLE_VALUE;

CMutex g_mtVideoDisp;
CMutex g_mtVideoDetect;

BOOL s_bEnableVideo = TRUE;

pthread_attr_t gtAttr;
pthread_attr_t *gtpAttr = NULL;

pthread_t gtReadIrdaID = 0;

static char *s_pShareMem = NULL;

/* �û����� */
int giUserNum = 0;
//�Ƿ����ץ�ĵ���������Ƭ
int giPictureFaceFlag = 0;


int g_rwPic = 0;
pthread_mutex_t gRwPicMutex;


extern void StartNetModule();

FI_FACIAL_DETECT_PARAM detect_param;

// ���㷨������
OD_CE_HANDLE OD_OpenEngine() {
    OD_CE_HANDLE hCe = NULL;

    /* modified by ytj 20110808������5.2.2������ͨ�㷨�ӿڣ�
     FI_OpenEngine������2��������Ҫָ��cmemk�ڴ����ʼ��ַ�������ַ */
    hCe = (OD_CE_HANDLE) FI_OpenEngine(FI_FALSE, 0x88000000, 0x8BFFFFFF);

    return hCe;
}

OD_CODEC_HANDLE OD_OpenFacialCodec(OD_CE_HANDLE hCe, char* pStrAuxDir,
        char** ppFeatureInBuf, int* pnErrNo) {
    FI_HANDLE hFiCodec = NULL;
    int nErrNo = 0;
    //LOGD("fun:%s line: %d hCe: %p", __FUNCTION__, __LINE__, hCe);
    if (hCe) {
        char strRandKey[32] = { 0 }, strPlanCode[32] = { 0 };
        hFiCodec = FI_FACIAL_OpenCodec(hCe, strRandKey);
        if (hFiCodec) {
            int ret = -1;

            if (pStrAuxDir)
                FI_FACIAL_SetAuxDir(hFiCodec, pStrAuxDir);
            ret = DM2016_Authentication(strRandKey, strPlanCode);
#ifdef PLAT_TI6446
            int i, j, fd;
            time_t ct = time(NULL);
            struct tm* pTm = gmtime(&ct);

            if(ret<0) {
                // DM2016 key
                struct DM2610_IO {
                    unsigned char msg[20];
                    unsigned char digits[20];
                }val;
                char* pDev = (char *)"/dev/davincicmkey0";
                memcpy(strPlanCode, strRandKey, 20);
                fd = open(pDev, O_RDWR);
                if(fd<0) {
                    TRACE("Open DM2610 chip fail\r\n");
                    nErrNo = -4;
                } else {
                    memcpy(val.msg, strRandKey, 20);

                    for(i=0; i<10; i++) {
                        unsigned char ch = val.msg[i];
                        val.msg[i] = val.msg[20-1-i];
                        val.msg[20-1-i] = ch;
                    }
                    for(i=0; i<10; i++) { /* retry 10 times */
                        if(ioctl(fd, DM2610_IO_READB_CMD, &val)>=0) break;
                        usleep(100*1000);
                    }
                    if(i==10) {
                        TRACE("DM2610_IO_READB_CMD fail\r\n");
                        nErrNo = -5;
                    } else {
                        for(j=0; j<5; j++) {
                            for(i=0; i<2; i++) {
                                unsigned char ch = val.digits[j*4+i];
                                val.digits[j*4+i] = val.digits[(j+1)*4-1-i];
                                val.digits[(j+1)*4-1-i] = ch;
                            }
                        }

                        memcpy(strPlanCode, val.digits, 20);
                    }
                    close(fd);
                }
            }

            fd=socket(AF_INET,SOCK_DGRAM,0);
            if(fd>=0) {
                struct ifreq ifr;
                memset(&ifr,0,sizeof(ifr));
                strncpy(ifr.ifr_name,"eth0",IFNAMSIZ);

                ioctl(fd,SIOCGIFHWADDR,&ifr,sizeof(ifr));

                memcpy(strPlanCode+20,ifr.ifr_hwaddr.sa_data,6);
                close(fd);
            }
            if(pTm) {
                if(pTm->tm_year>110) pTm->tm_year=110;
                i = (pTm->tm_year+1900)*10000+(pTm->tm_mon+1)*100+pTm->tm_mday;
                memcpy(strPlanCode+26, &i, 4);
            }

            if(nErrNo) PassData(strPlanCode, 20, "facial");
            PassData(strPlanCode+20, 12, "facial");
#endif
            ret = FI_FACIAL_InitCodec(hFiCodec, strPlanCode);

            if (ppFeatureInBuf)
                *ppFeatureInBuf = FI_FACIAL_GetInFeatureBuf(hFiCodec);
        } else
            printf("FI_FACIAL_OpenCodec: Fail!");
    }

    return (OD_CODEC_HANDLE) hFiCodec;
}

void OD_freeShareMem() {
    int nlen = OD_CODEC_BUF_SIZE;
    if (s_pShareMem) {
        FI_FreeContigBuf(s_pShareMem, nlen);
    }
}

char *OD_mallocShareMem() {
    int nlen = OD_CODEC_BUF_SIZE;

    s_pShareMem = FI_MallocContigBuf(nlen);

    return s_pShareMem;
}

void OD_CloseFacialCodec(OD_CODEC_HANDLE hCodec) {
    if (hCodec) {
        FI_FACIAL_CloseCodec(hCodec);
    }
}

// �ر��㷨������
void OD_CloseEngine(OD_CE_HANDLE hCe) {
    //TRACE("%s %d \r\n", __FUNCTION__, __LINE__);

    if (hCe)
        FI_CloseEngine((FI_HANDLE) hCe);
}

int OD_DetectFacial(OD_CODEC_HANDLE hCodec, char* pYBuf, int nWidth,
        int nHeight, FI_FACIAL_DETECTX_RESULT* pResult) {
    int ret = -1;

    detect_param.width = nWidth;
    detect_param.height = nHeight;
    detect_param.detectWay = FI_FACIAL_DETECT_WAY_VR_GN;
    detect_param.detectNum = FI_FACIAL_DETECT_MAX;//FI_FACIAL_DETECT_ALL;
    detect_param.bDetectFace = 1;
    detect_param.bDetectEye = 1;
    detect_param.bDetectEyeStatus = 1;
    detect_param.bDetectMouth = 1;

    /* added by ytj 20110808��6446ƽ̨ʹ�� ��ͨ�㷨 */
    if (hCodec) {
        //LOGD("OD_DetectFacial() hCodec:%p line:%d", hCodec, __LINE__);
        ret = FI_FACIAL_DetectX(hCodec, pYBuf, &detect_param, pResult);
    }
    return ret;
}

/* ��׮ */
int DM2016_Authentication(char* pEncryptedCode, char* pPlanCode) {
    return 0;
}

void InitFacial() {
    int i = 0;

    if (ghCe != NULL) {
        return;
    }

    ghCe = OD_OpenEngine();
    LOGD("InitFaceLib()ghCe: %p fun:%s line: %d", ghCe, __FUNCTION__, __LINE__);
    if (ghCe) {
        (void) FI_FACIAL_SetInFeatureBufSize(OD_CODEC_BUF_SIZE / (1024 * 1024));

        for (i = 0; i < 10; i++) {
            ghFacialCodec = OD_OpenFacialCodec(ghCe, (char*) FACIAL_LIB_PATH,
                    &gpUsersFeature, &giCodecErrorNo);
            LOGD("fun:%s line: %d ghFacialCodec: %p", __FUNCTION__, __LINE__, ghFacialCodec);
            if (ghFacialCodec) {
                if (gpUsersFeature) {
                    giSharedMemSize = OD_CODEC_BUF_SIZE;
                    gpSharedMem = gpUsersFeature;
                }
                break;
            }

            usleep(10 * 1000);
        }

        if (ghFacialCodec == NULL) {
            gpSharedMem = OD_mallocShareMem();
            if (gpSharedMem) {
                giSharedMemSize = OD_CODEC_BUF_SIZE;
            }
        }
    } else {
        giCodecErrorNo = -1;
    }
}

void DeInitFacial() {
    if (ghCe) {
        if (ghFacialCodec == NULL) {
            if (gpSharedMem) {
                OD_freeShareMem();
            }
        }

        gpSharedMem = NULL;
        giSharedMemSize = 0;
    }
    if (ghFacialCodec) {
        OD_CloseFacialCodec(ghFacialCodec);
        ghFacialCodec = NULL;
    }
    if (ghCe) {
        OD_CloseEngine(ghCe);
        ghCe = NULL;
    }
}

/**
 *  ��ʼ������ʶ���㷨��
 *  return ����ִ�н��
 */

//void initFaceLib(JNIEnv* env, jobject thiz) {
JNIEXPORT void JNICALL Java_com_firs_cn_FaceNative_initFaceLib(JNIEnv* env, jobject thiz) {
    //LOGD("initFaceLib()");
    //��ʼ���㷨
    InitFacial();
    LOGD("DoTcpSendThreadPool %s %d\r\n", __FUNCTION__, __LINE__);
}

/**
 *  �ͷų�ʼ������ʶ���㷨��
 *  return ����ִ�н��
 */

//void releaseFaceLib(JNIEnv* env, jobject thiz) {
JNIEXPORT void JNICALL Java_com_firs_cn_FaceNative_releaseFaceLib(JNIEnv* env, jobject thiz) {
    //LOGD("releaseFaceLib()");
    //�ͷ�������Դ
    DeInitFacial();
}

void RoateData(LONG width, LONG height, unsigned char * srcData, unsigned char * dstData, int Bpp)
{
	//Bpp  byte per pixel
	int i, j;
	int k;
	unsigned char *tmpSrc = srcData;
	unsigned char *tmpDst = dstData;
	
	width *= Bpp;		//ÿ����ռ���ֽ�
	for (i = 0; i < width; i += Bpp)
		//for (i = width - 3; i >= 0 ; i -= 3)
	{
		//for(j = 0; j < height; j++)
		for(j = height - 1; j >= 0; j--)
		{
			for (k = 0; k < Bpp; k++)
			{				
				*tmpDst = *(tmpSrc + j * width + i + k);
				tmpDst++;
			}
		}
	}
}

 void rotateYUV240SP(unsigned char* src,unsigned char* des,int width,int height)  
    {  
         
        int wh = width * height;  
        //��תY  
        int k = 0;  
        for(int i=0;i<width;i++) {  
            for(int j=0;j<height;j++)   
            {  
                  des[k] = src[width*j + i];              
                  k++;  
            }  
        }  
          
        for(int i=0;i<width;i+=2) {  
            for(int j=0;j<height/2;j++)   
            {     
                  des[k] = src[wh+ width*j + i];      
                  des[k+1]=src[wh + width*j + i+1];  
                  k+=2;  
            }  
        }  
          
          
    } 

/**
 *  ����ʶ��
 *  return ����ִ�н��
 */
JNIEXPORT jintArray JNICALL Java_com_firs_cn_FaceNative_recognizeFace(JNIEnv* env, jobject thiz, jint width,
        jint height, jboolean isFront, jbyteArray imgData) {
    int nFeatureBufSize = 1024;
    char* pFeatureJNM = NULL;
    int ret = -1;
    int ret1 = 10;
    int yWidth = width;
    int yHeight = height;
    //ʶ��ģʽ��Ĭ��Ϊ1ע��ģʽ
    int iMode = 1;
    int iEyeWidthMin = 0;
    int iEyeWidthMax = 0;
    int iEyeWidth = 0;
    int i = 0;

    //Ĭ����ת90��
    signed short rotateValue = 0;
    // ������������(left, top, right, bottom)
    jint pFace[13] = { 0 };
    FI_FACIAL_DETECTX_RESULT strFacialInfo;
    // �����ͼ������
    jbyte* jbts = (env)->GetByteArrayElements(imgData, 0);
    //��������ݳ���
    jsize len = (env)->GetArrayLength(imgData);
    // ͼƬ���ݴ洢�ռ�

    char* g_SIF_BUF = new char[yWidth * yHeight * 2];
    memset(g_SIF_BUF, 0, yWidth * yHeight * 2);
    char* g_Temp_BUF = new char[yWidth * yHeight * 2];
    memset(g_Temp_BUF, 0, yWidth * yHeight * 2);

	char* g_Temp_BUF2 = new char[yWidth * yHeight * 2];
    memset(g_Temp_BUF2, 0, yWidth * yHeight * 2);
    memcpy(g_Temp_BUF, jbts, len);

	char* g_Temp_BUF3 = new char[yWidth * yHeight * 2];
    memset(g_Temp_BUF3, 0, yWidth * yHeight * 2);
    //saveRaw((unsigned char*) g_Temp_BUF, SIF_HEIGHT * SIF_WIDTH * 2);
    //LOGD("recognizeFace() width:%d height:%d imgData.length:%d %s %d\r\n",width, height, len, __FUNCTION__, __LINE__);
    if (JNI_TRUE == isFront) {
        rotateValue = 270;
        //ʶ����ʱ����ת90��
        //void YUV420SPToYUV420P ( void * yuv420sp,  void * yuv420,  int  width,  int  height )
        //LOGD("yWidth[%d] yHeight[%d]\r\n",yWidth,yHeight);

		YUV420SPToYUV420P( g_Temp_BUF,  g_Temp_BUF2,  yWidth,yHeight);
        rotate90_yuv420p(g_Temp_BUF2, yWidth, yHeight, g_SIF_BUF, TRUE, 0);
			
        memset(&strFacialInfo, 0, sizeof(strFacialInfo));
        ret = OD_DetectFacial(ghFacialCodec, g_SIF_BUF, yHeight, yWidth,
                &strFacialInfo);
        //LOGD("rotateValue:%d ret:%d strFacialInfo.faceNum:%d %s %d\r\n", rotateValue,ret, strFacialInfo.faceNum, __FUNCTION__, __LINE__);
        if (ret <= 0 || strFacialInfo.faceNum <= 0) {
            //LOGD("ret <= 0 || strFacialInfo.faceNum <= 0 %s %d\r\n",
            //        __FUNCTION__, __LINE__);
            //����ת
            rotateValue = 0;
            memset(g_SIF_BUF, 0, yWidth * yHeight * 2);
            memcpy(g_SIF_BUF, jbts, len);
            // ʶ��ԭʼͼƬ
            memset(&strFacialInfo, 0, sizeof(strFacialInfo));
            ret = OD_DetectFacial(ghFacialCodec, g_SIF_BUF, yWidth, yHeight,
                    &strFacialInfo);
            //LOGD("rotateValue:%d ret:%d strFacialInfo.faceNum:%d %s %d\r\n", rotateValue,ret, strFacialInfo.faceNum, __FUNCTION__, __LINE__);
            if (ret <= 0 || strFacialInfo.faceNum <= 0) {
                int j = 0;
                //LOGD("ret <= 0 %s %d\r\n", __FUNCTION__, __LINE__);
                //�������ݵ�Ŀ������
                memset(g_Temp_BUF, 0, yWidth * yHeight * 2);
                memcpy(g_Temp_BUF, jbts, len);
                // ʶ�����������Ƕȵ�ͼƬ
                for (j = 0; j < IMAGE_MAX_ROTATE_TIMES; j++) {
                    int mWidth = yHeight;
                    rotateValue = ((j + 1) * 90);
                    memset(g_SIF_BUF, 0, yWidth * yHeight * 2);
                    rotate90_yuv420p(g_Temp_BUF, yWidth, yHeight, g_SIF_BUF, 0,
                            1);
                    yHeight = yWidth;
                    yWidth = mWidth;

                    //LOGD("yWidth:%d yHeight:%d rotateValue:%d %s %d\r\n",
                    //        yWidth, yHeight, rotateValue, __FUNCTION__, __LINE__);
                    memset(&strFacialInfo, 0, sizeof(strFacialInfo));
                    ret = OD_DetectFacial(ghFacialCodec, g_SIF_BUF, yWidth,
                            yHeight, &strFacialInfo);
                    //LOGD("rotateValue:%d ret:%d strFacialInfo.faceNum:%d %s %d\r\n", rotateValue,ret, strFacialInfo.faceNum, __FUNCTION__, __LINE__);
                    if (ret > 0 && strFacialInfo.faceNum > 0) {
                        break;
                    }
                    //�������ݵ�Ŀ������
                    memset(g_Temp_BUF, 0, yWidth * yHeight * 2);
                    memcpy(g_Temp_BUF, g_SIF_BUF, yWidth * yHeight * 2);
                }

            }
        }
    } else {
        rotateValue = 90;
        //ʶ����ʱ����ת90��
		YUV420SPToYUV420P( g_Temp_BUF,  g_Temp_BUF2,  yWidth,yHeight);
        rotate90_yuv420p(g_Temp_BUF2, yWidth, yHeight, g_SIF_BUF, TRUE, 1);
		
       // rotate90_yuv420p(g_Temp_BUF, yWidth, yHeight, g_SIF_BUF, 0, 1);
        memset(&strFacialInfo, 0, sizeof(strFacialInfo));
        //LOGD("rotateValue:%d ret:%d strFacialInfo.faceNum:%d %s %d\r\n", rotateValue,ret, strFacialInfo.faceNum, __FUNCTION__, __LINE__);
        ret = OD_DetectFacial(ghFacialCodec, g_SIF_BUF, yHeight, yWidth,
                &strFacialInfo);
       // LOGD("rotateValue:%d ret:%d strFacialInfo.faceNum:%d %s %d\r\n", rotateValue,ret, strFacialInfo.faceNum, __FUNCTION__, __LINE__);
        if (ret <= 0 || strFacialInfo.faceNum <= 0) {
            //LOGD("ret <= 0 || strFacialInfo.faceNum <= 0 %s %d\r\n",
            //        __FUNCTION__, __LINE__);
            //����ת
            rotateValue = 0;
            memset(g_Temp_BUF, 0, yWidth * yHeight * 2);
            memcpy(g_Temp_BUF, jbts, len);
            // ʶ��ԭʼͼƬ
            memset(&strFacialInfo, 0, sizeof(strFacialInfo));
            ret = OD_DetectFacial(ghFacialCodec, g_Temp_BUF, yWidth, yHeight,
                    &strFacialInfo);
            //LOGD("rotateValue:%d ret:%d strFacialInfo.faceNum:%d %s %d\r\n", rotateValue,ret, strFacialInfo.faceNum, __FUNCTION__, __LINE__);
            if (ret <= 0 || strFacialInfo.faceNum <= 0) {
                int j = 0;
                //LOGD("ret <= 0 %s %d\r\n", __FUNCTION__, __LINE__);
                //�������ݵ�Ŀ������
                memset(g_Temp_BUF, 0, yWidth * yHeight * 2);
                memcpy(g_Temp_BUF, g_SIF_BUF,  yWidth * yHeight * 2);
                // ʶ�����������Ƕȵ�ͼƬ
                for (j = 0; j < IMAGE_MAX_ROTATE_TIMES; j++) {
                    int mWidth = yHeight;
                    rotateValue = ((j + 1) * 90);
                    memset(g_SIF_BUF, 0, yWidth * yHeight * 2);
                    rotate90_yuv420p(g_Temp_BUF, yWidth, yHeight, g_SIF_BUF, 0,
                            1);
                    yHeight = yWidth;
                    yWidth = mWidth;

                    //LOGD("yWidth:%d yHeight:%d rotateValue:%d %s %d\r\n",
                    //        yWidth, yHeight, rotateValue, __FUNCTION__, __LINE__);
                    memset(&strFacialInfo, 0, sizeof(strFacialInfo));
                    ret = OD_DetectFacial(ghFacialCodec, g_SIF_BUF, yWidth,
                            yHeight, &strFacialInfo);
                    //LOGD("rotateValue:%d ret:%d strFacialInfo.faceNum:%d %s %d\r\n", rotateValue,ret, strFacialInfo.faceNum, __FUNCTION__, __LINE__);
                    if (ret > 0 && strFacialInfo.faceNum > 0) {
                        break;
                    }
                    //�������ݵ�Ŀ������
                    memset(g_Temp_BUF, 0, yWidth * yHeight * 2);
                    memcpy(g_Temp_BUF, g_SIF_BUF, yWidth * yHeight * 2);
                }

            }
        }
    }
    //delete[] g_Temp_BUF;
   // delete[] g_SIF_BUF;
    //LOGD("recognizeFace() ret:%d nFaceNum:%d %s %d\r\n", ret, strFacialInfo.faceInfo[0].nFaceNum, __FUNCTION__, __LINE__);
    if (ret > 0) {
        int index = 0; //��Ƿ��ص���������
        int maxFace = 0;
        int sumLen = 0;
		int result = 0;
		giPictureFaceFlag = 1;
		
		//char strFile[] = "/mnt/sdcard/rong/rong.jpg";
		char strFile[] = "/data/data/com.firs.facedetecttosvr/temp/facePic.jpg";
		memcpy(g_SIF_BUF_Send, g_SIF_BUF,SIF_HEIGHT * SIF_WIDTH * 2);
		

		//rotateYUV240SP((unsigned char*)jbts,(unsigned char*)g_SIF_BUF_Send,width,height);
		
        // ����ԭʼͼ��YUV420SP��ʽ
        //saveRaw((unsigned char*) g_SIF_BUF_Send, SIF_HEIGHT * SIF_WIDTH * 2);
        RGB24x2 *bmp = (RGB24x2*) malloc(3 * width * height);
        memset((unsigned char*) bmp, 0, 3 * width * height);
		
        // ת��ԭʼͼƬΪRGB��ʽ
        //yuv420sp_to_rgb(240, 320, (unsigned char*) g_SIF_BUF_Send, bmp);

		yuv420p_to_rgb565_ex(240, 320, g_SIF_BUF_Send, bmp);
		//cvt_420p_to_rgb565(240, 320, (unsigned char*)g_SIF_BUF_Send, bmp2);
		//unsigned char *yuv420[3];
		//void YUV420p_to_RGB24(unsigned char *yuv420[3], unsigned char *rgb24, int width, int height)   
		//YUV420p_to_RGB24((unsigned char **)g_SIF_BUF_Send, (unsigned char *)bmp2, 240, 320);

		//yuv420sp_to_rgb(width, height, (unsigned char*) g_SIF_BUF, bmp);
        // ����RGB��ʽͼ��
         //saveRgb((unsigned char*)bmp, 3 * width * height);
        // ����ͼ��ΪJPG��ʽ
        //LOGD("strFile[%s]\r\n",strFile);

		//RoateData(width,height, (unsigned char*)bmp, (unsigned char*)bmp1, 1);

		pthread_mutex_lock(&gRwPicMutex);
        result = jpeg_compress((char*) strFile, (unsigned char*)bmp, 240,
                320, TRUE, 100);
        if (0 != result) {
            //  JPGͼ��ת���ɹ���
            ret = result;
        }
		pthread_mutex_unlock(&gRwPicMutex);
        Free(bmp);

		/*
	        if(pFeatureJNM == NULL)
	        {
	        	pFeatureJNM = new char[nFeatureBufSize*2];//(char*)malloc(nFeatureBufSize*2);
	        	memset(pFeatureJNM, 0x0, nFeatureBufSize*2);
	        }
	        ret1 = FI_FACIAL_GetJNMFeature(ghFacialCodec, g_SIF_BUF, yWidth,
	        		yHeight, &strFacialInfo.faceInfo[0], pFeatureJNM, nFeatureBufSize);
	        if(pFeatureJNM != NULL)
	        {
	        	delete[] pFeatureJNM;
	        	//free(pFeatureJNM);
	        	pFeatureJNM = NULL;
	        }
		*/

        // ���صĽ����
        pFace[0] = ret;
        pFace[1] = strFacialInfo.faceInfo[0].rcFace.left;
        pFace[2] = strFacialInfo.faceInfo[0].rcFace.top;
        pFace[3] = strFacialInfo.faceInfo[0].rcFace.right;
        pFace[4] = strFacialInfo.faceInfo[0].rcFace.bottom;
        pFace[5] = rotateValue;
        if(ret1 == 0)
        {
        	pFace[6] = 1;
        	//system("reboot");
        }else if(ret1 == -1)
        {
        	pFace[6] = 2;
        }
        else if(ret1 == -2)
        {
        	pFace[6] = 3;
        }
        else
        {

        	pFace[6] = 0;
        }
       // system("reboot");


        pFace[7] = strFacialInfo.faceInfo[0].ptLeftEye.x;
        pFace[8] = strFacialInfo.faceInfo[0].ptLeftEye.y;
        pFace[9] = strFacialInfo.faceInfo[0].ptRightEye.x;
        pFace[10] = strFacialInfo.faceInfo[0].ptRightEye.y;
        pFace[11] = strFacialInfo.faceInfo[0].ptMouthPos.x;
        pFace[12] = strFacialInfo.faceInfo[0].ptMouthPos.y;
    }
	else
	{
		giPictureFaceFlag = 0;
	}
	
    delete[] g_Temp_BUF;
    delete[] g_SIF_BUF;

    //LOGD("recognizeFace() ret:%d rotateValue:%d %s %d\r\n",ret, rotateValue, __FUNCTION__, __LINE__);
    jintArray intArray = (env)->NewIntArray(sizeof(pFace) / 4);
    (env)->SetIntArrayRegion(intArray, 0, sizeof(pFace) / 4, pFace);

    (env)->ReleaseByteArrayElements(imgData, jbts, 0);
    return intArray;

}

JNIEXPORT jint JNICALL Java_com_firs_cn_FaceNative_initTcp( JNIEnv* env,jobject obj)
{
	StartNetModule();
	return 1;
}
extern int ClientSendAuth(char* pUserName, char* pPwd);

JNIEXPORT jint JNICALL Java_com_firs_cn_FaceNative_UserAuth( JNIEnv* env,jobject obj,jbyteArray username,jbyteArray pwd)
{
	char chUserName[32] = {0};
	char chPwd[32] = {0};

	//��ȡ�û���
	jbyte* jbtsUser = (env)->GetByteArrayElements(username, 0);
    jsize lenuser = (env)->GetArrayLength(username);
	if(lenuser > sizeof(chUserName))
	{
		lenuser = sizeof(chUserName);
	}
	memcpy(chUserName, jbtsUser, lenuser);

	//��ȡ����
	jbyte* jbtspwd = (env)->GetByteArrayElements(pwd, 0);
	jsize lenpwd = (env)->GetArrayLength(pwd);
	if(lenpwd > sizeof(chPwd))
	{
		lenpwd = sizeof(chPwd);
	}
	memcpy(chPwd, jbtspwd, lenpwd);


	memset(g_chUserName,0x00,sizeof(g_chUserName));
	memset(g_chPwd,0x00,sizeof(g_chPwd));
	strncpy(g_chUserName, chUserName, strlen(chUserName));
	strncpy(g_chPwd, chPwd, strlen(chPwd));
	ClientSendAuth(g_chUserName,g_chPwd);

	(env)->ReleaseByteArrayElements(username, jbtsUser, 0);
	(env)->ReleaseByteArrayElements(pwd, jbtspwd, 0);
	
	return 1;
}

extern int SetClientIP(char *pclientIp, int clientPort, int ipChanged);
JNIEXPORT void JNICALL Java_com_firs_cn_FaceNative_SetServerIP( JNIEnv* env,jobject obj, jbyteArray ip, jint iPort, jint ipChanged)
{
	char acIp[16] = {0};
	
	 // ���������
    jbyte* jbts = (env)->GetByteArrayElements(ip, 0);
    //���ݳ���
    jsize len = (env)->GetArrayLength(ip);

	if(len > sizeof(acIp))
	{
		len = sizeof(acIp); 
	}

    memcpy(acIp, jbts, len);

	setServerSockFlag(0);

	SetClientIP(acIp, iPort ,ipChanged);

	(env)->ReleaseByteArrayElements(ip, jbts, 0);
	
}

extern int getAuth();
JNIEXPORT jint JNICALL Java_com_firs_cn_FaceNative_getAuth( JNIEnv* env,jobject obj)
{
	return getAuth();
}
extern int getCompareFlag();
JNIEXPORT jint JNICALL Java_com_firs_cn_FaceNative_getCompareFlag( JNIEnv* env,jobject obj)
{
	return getCompareFlag();
}
JNIEXPORT jint JNICALL Java_com_firs_cn_FaceNative_getPictureFaceFlag( JNIEnv* env,jobject obj)
{
	return giPictureFaceFlag;
}

//JNIEXPORT jobject  JNICALL Java_com_re_struct_MainActivity_call(JNIEnv * env, jobject obj, jobject classobj)  
JNIEXPORT jobject  JNICALL Java_com_firs_cn_FaceNative_getCompareResult(JNIEnv * env, jobject obj, jobject classobj)  
{  
	char outbuf[64]={0};
	COMPARE_RESULT cmprest;
	jclass objectClass = env->FindClass("com/firs/cn/compareresult");  

	jfieldID name = env->GetFieldID(objectClass, "name", "Ljava/lang/String;");  
	jfieldID num = env->GetFieldID(objectClass, "score", "I");  
	jfieldID num2 = env->GetFieldID(objectClass, "faceId", "I");  

	cmprest = get_CompareResult();
	GB2312ToUTF_8(outbuf,cmprest.f_name, strlen(cmprest.f_name));
	
	env->SetObjectField(classobj, name, env->NewStringUTF(outbuf));
	env->SetIntField(classobj, num, cmprest.score);
	env->SetIntField(classobj, num2, cmprest.f_id);
	return classobj;
}


extern int snedPictureToSvr(char* pPicture, int len);
extern void setCompareFlag(int iCompareFlag);
JNIEXPORT void JNICALL Java_com_firs_cn_FaceNative_sendPicture( JNIEnv* env,jobject obj)
{
	char* cSIF_BUF = new char[SIF_HEIGHT * SIF_WIDTH * 2]; 
	memcpy(cSIF_BUF, g_SIF_BUF_Send, SIF_HEIGHT * SIF_WIDTH * 2);

	char strFile[] = "/data/data/com.firs.facedetecttosvr/temp/facePic.jpg";
	char strFile2[] = "/data/data/com.firs.facedetecttosvr/temp/facePic_temp.jpg";
	int len = 0;
	char* pPic = NULL;

	LOGD("READ PIC FILE");
	pthread_mutex_lock(&gRwPicMutex);
	ReadFile(strFile, &pPic, &len);
	pthread_mutex_unlock(&gRwPicMutex);
	LOGD("SEND PIC FILE  len[%d]",len);
	setCompareFlag(0);
	snedPictureToSvr(pPic, len);
	LOGD("SENDED PIC FILE");
	//void saveFile(const char* pSrcFile,unsigned char* pRawBuf, int iLen)
	saveFile(strFile2,(unsigned char*)pPic, len);
	if(pPic != NULL)
	{
		free(pPic);
		pPic = NULL;
	}
    delete[] cSIF_BUF;
}

extern void setThreadExit();
JNIEXPORT void JNICALL Java_com_firs_cn_FaceNative_setThreadExit( JNIEnv* env,jobject obj)
{
	setThreadExit();
}
JNIEXPORT jint JNICALL Java_com_firs_cn_FaceNative_getServerSockFlag( JNIEnv* env,jobject obj)
{
	return getServerSockFlag();
}
JNIEXPORT void JNICALL Java_com_firs_cn_FaceNative_SetScore( JNIEnv* env,jobject obj, jint iScore)
{
	g_Score = iScore;
}



