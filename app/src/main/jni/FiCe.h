
#ifndef _FI_CE_H
#define _FI_CE_H

#ifdef __cplusplus
extern "C" {
#endif

#include "StdDefEx.h"

/***************************************************
 * �������ƣ�������
 * ���������bIOCmemBuf �Ƿ����û���CMEM�����I/O������==FI_TRUE��app����CMEM�������; ==FI_FALSE��app����arm�˷������
 * 			cmemStart CMEM ��ʼ��ַ
 * 			cmemEnd   CMEM ������ַ
 * �����������
 * ���ؽ����!=NULL ����������; ==NULL ������ʧ��
 ***************************************************
*/
FI_HANDLE FI_OpenEngine(FI_BOOL bIOCmemBuf, unsigned long cmemStart, unsigned long cmemEnd);

/***************************************************
 * �������ƣ��ر�����
 * ���������hFiCe �� FI_OpenEngine �򿪵�������
 * �����������
 * ���ؽ������
 ***************************************************
*/
void FI_CloseEngine(FI_HANDLE hFiCe);


// ���乲������
char *FI_MallocContigBuf(int bufSize);
void FI_FreeContigBuf(char *pBuf, int bufSize);


#ifdef __cplusplus
}
#endif

#endif // _FI_CE_H


