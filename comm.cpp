#include <windows.h>
#include <stdio.h>
#include <GL/freeglut.h>
#include "main.h"

/*
  API��д����,����һ����������:
   1. �������ھ��,��CreateFile;
   2. �Դ��ڵĲ�����������,���бȽ���Ҫ���ǲ�����(BaudRate),���ݿ��(BytesBits),��żУ��(Parity),ֹͣλ(StopBits),��Ȼ����Ҫ�Ļ��ж˿ں�(Port);
   3. Ȼ��Դ��ڽ�����Ӧ�Ķ�д����,��ʱ���õ�ReadFile��WriteFile����;
   4. д������,Ҫ�رմ��ھ��,��CloseFile.
 */
//==============================================================================
//==============================================================================
//==============================================================================
//==============================================================================
/*
 * hCom:���ڵľ�� 
 * dcb:���ڵĽṹ����� 
 * string:���ں�(��: COM3��) 
 */
HANDLE hCom = NULL;    //ȫ�ֱ���,���ھ��
DCB    dcb;            //�ṹ��������ڴ������� 
bool ReadComm = false; //�����ڱ�־ 

bool OpenCommWrite(HANDLE hCom, DCB dcb, char *string){//�򿪴���д  
     hCom = CreateFile(string,     //COM3
                       GENERIC_READ|GENERIC_WRITE, //�������д
                       0,          //��ռ��ʽ
                       NULL,
                       OPEN_EXISTING, //�򿪶����Ǵ���
                       0,             //�첽��ʽ
                       NULL);
     if(hCom == INVALID_HANDLE_VALUE){
        CloseHandle(hCom);
        printf("�� %s ʧ��!\n", string);
        return false;
     }else{
        printf("�ѳɹ��� %s\n", string);
     }
     //-------------------------------------------------------------------------
     if(!GetCommState(hCom,&dcb))//��ȡ��ǰDCB����
       return false;
     else{
           printf("%s ��ǰͨ���ֽ�λ��   = %d\n", string, dcb.ByteSize);  
           printf("%s ��ǰ������         = %d\n", string, dcb.BaudRate);  
           printf("%s ��ǰ��żУ��ʹ��λ = %d\n", string, dcb.fParity);  
           printf("%s ��ǰ��żУ�鷽��   = %d\n", string, dcb.Parity);  
           printf("%s ��ǰֹͣλ��λ��   = %d\n", string, dcb.StopBits);  
           dcb.BaudRate        = CBR_57600;    //������   
           dcb.Parity          = NOPARITY;     //��żУ�鷽ʽ,ֵ0~4�ֱ��Ӧ��У��,��У��,żУ��,У����λ,У������ 
           dcb.fParity         = 0;            //�Ƿ������żУ��,Ϊ1�Ļ�������żУ���� 
           dcb.StopBits        = TWOSTOPBITS;   //ֹͣλ����,0~2�ֱ��Ӧ1λ,1.5λ,2λֹͣλ
           dcb.ByteSize        = 8; //���ݿ��,һ���ֽڵ�����λ����,��Χ��5~8
     }  
     if(!SetCommState(hCom,&dcb)) //���ô���״̬,���õĸ��Ĵ��ں�,������,��żУ�鷽ʽ,����λ����
       return false;              //���ɹ����� false 
     else{
       printf("���������\n");
     }
     if(!GetCommState(hCom,&dcb)) //��ȡ��ǰDCB����,��Чʱ���� false 
       return false;
     else{                        //��Чʱ���� true 
       printf("----------------------------------------------\n"); 
       printf("%s ��ǰͨ���ֽ�λ��   = %d\n", string, dcb.ByteSize);  
       printf("%s ��ǰ������         = %d\n", string, dcb.BaudRate);  
       printf("%s ��ǰ��żУ��ʹ��λ = %d\n", string, dcb.fParity);  
       printf("%s ��ǰ��żУ�鷽��   = %d\n", string, dcb.Parity);  
       printf("%s ��ǰֹͣλ��λ��   = %d\n", string, dcb.StopBits);        
     }
     //------------------------------------------------------------------------- 
     SetupComm(hCom,2048,2048); //���뻺����������������Ĵ�С����2048  
     //-------------------------------------------------------------------------
     printf("----------------------------------------------\n"); 
     //-------------------------------------------------------------------------
     COMMTIMEOUTS TimeOuts;
     //�趨����ʱ
     TimeOuts.ReadIntervalTimeout        = MAXDWORD;    //��ʱ���ֽ�֮����ʱ��,�������,���������ַ�֮���ʱ������������,ReadFile��������ɲ������κ��ѻ��������.0��������δ����. 
     TimeOuts.ReadTotalTimeoutMultiplier = 0;    //ʱ��ϵ�� 
     TimeOuts.ReadTotalTimeoutConstant   = 0;    //��ȡ��ʱ�� 
     //�趨д��ʱ
     TimeOuts.WriteTotalTimeoutMultiplier = 500;  //дʱ���ֽ�֮����ʱ��,������� 
     TimeOuts.WriteTotalTimeoutConstant   = 5000; //д����ʱ�� 
     SetCommTimeouts(hCom, &TimeOuts); //���ó�ʱ
     
     char strW[13] = {0x13,0x14,0x15,0x16,0x17,0x18,0x19,0x20,0x21,0x22,0x23,0x24,0x5B};
     BOOL       bResult;
     DWORD      wCount;//д����ֽ���
     OVERLAPPED wrOverlapped;
     DWORD      dwError; 
     ZeroMemory(&wrOverlapped, sizeof(wrOverlapped));//��0�����һ���ڴ�����
     if (wrOverlapped.hEvent != NULL){
         ResetEvent(wrOverlapped.hEvent);//ָ�����¼���������Ϊ���ź�״̬
         wrOverlapped.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);//�������һ�������Ļ��������¼�����
     }
     
     ClearCommError(hCom, &dwError, NULL);
     PurgeComm(hCom, PURGE_TXABORT | PURGE_TXCLEAR);//������ڵ����в���

     bResult = WriteFile(hCom,   //���ڵľ��
				         strW,   //Ҫд�����ݵ��׵�ַ
				         13,     //Ҫд������ݵĳ���
				         &wCount,//����д������ɺ󷵻ص�ʵ��д������ݳ�����Ϣ�Ĵ洢��ַ
				         //&wrOverlapped);  //�ص�I/O�ṹ��ָ��
				         NULL);  //�ص�I/O�ṹ��ָ��
	 if(!bResult){
          printf("д����ʧ��!\n");
     }
     else
          printf("д���ڳɹ�!\n");
     if (!CloseHandle(hCom))
           printf("�ر�ʧ��\n");
     else
           printf("�رճɹ�\n");           
}
//==============================================================================
//==============================================================================
//==============================================================================
//==============================================================================
bool OpenCommRead(HANDLE hCom, DCB dcb, char *string){//�򿪴��ڶ� 
    hCom = CreateFile(string,     //COM3
                       GENERIC_READ|GENERIC_WRITE, //�������д
                       0,          //��ռ��ʽ
                       NULL,
                       OPEN_EXISTING, //�򿪶����Ǵ���
                       0,             //�첽��ʽ
                       NULL);
     if(hCom == INVALID_HANDLE_VALUE){
        CloseHandle(hCom);
        printf("�� %s ʧ��!\n", string);
        return false;
     }else{
        printf("�ѳɹ��� %s\n", string);
     }
     //-------------------------------------------------------------------------
     if(!GetCommState(hCom,&dcb))//��ȡ��ǰDCB����
       return false;
     else{
           printf("%s ��ǰͨ���ֽ�λ��   = %d\n", string, dcb.ByteSize);  
           printf("%s ��ǰ������         = %d\n", string, dcb.BaudRate);  
           printf("%s ��ǰ��żУ��ʹ��λ = %d\n", string, dcb.fParity);  
           printf("%s ��ǰ��żУ�鷽��   = %d\n", string, dcb.Parity);  
           printf("%s ��ǰֹͣλ��λ��   = %d\n", string, dcb.StopBits);  
           dcb.BaudRate        = CBR_57600;    //������   
           dcb.Parity          = NOPARITY;     //��żУ�鷽ʽ,ֵ0~4�ֱ��Ӧ��У��,��У��,żУ��,У����λ,У������ 
           dcb.fParity         = 0;            //�Ƿ������żУ��,Ϊ1�Ļ�������żУ���� 
           dcb.StopBits        = TWOSTOPBITS;   //ֹͣλ����,0~2�ֱ��Ӧ1λ,1.5λ,2λֹͣλ
           dcb.ByteSize        = 8; //���ݿ��,һ���ֽڵ�����λ����,��Χ��5~8
     }  
     if(!SetCommState(hCom,&dcb)) //���ô���״̬,���õĸ��Ĵ��ں�,������,��żУ�鷽ʽ,����λ����
       return false;              //���ɹ����� false 
     else{
       printf("���������\n");
     }
     if(!GetCommState(hCom,&dcb)) //��ȡ��ǰDCB����,��Чʱ���� false 
       return false;
     else{                        //��Чʱ���� true 
       printf("----------------------------------------------\n"); 
       printf("%s ��ǰͨ���ֽ�λ��   = %d\n", string, dcb.ByteSize);  
       printf("%s ��ǰ������         = %d\n", string, dcb.BaudRate);  
       printf("%s ��ǰ��żУ��ʹ��λ = %d\n", string, dcb.fParity);  
       printf("%s ��ǰ��żУ�鷽��   = %d\n", string, dcb.Parity);  
       printf("%s ��ǰֹͣλ��λ��   = %d\n", string, dcb.StopBits);        
     }
     //------------------------------------------------------------------------- 
     SetupComm(hCom,128,128); //���뻺����������������Ĵ�С����2048  
     //-------------------------------------------------------------------------
     printf("----------------------------------------------\n"); 
     //-------------------------------------------------------------------------
     COMMTIMEOUTS TimeOuts;
     //�趨����ʱ
     TimeOuts.ReadIntervalTimeout        = 1000;    //��ʱ���ֽ�֮����ʱ��,�������,���������ַ�֮���ʱ������������,ReadFile��������ɲ������κ��ѻ��������.0��������δ����. 
     TimeOuts.ReadTotalTimeoutMultiplier = 50;    //ʱ��ϵ�� 
     TimeOuts.ReadTotalTimeoutConstant   = 5000;    //��ȡ��ʱ�� 
     //�趨д��ʱ
     TimeOuts.WriteTotalTimeoutMultiplier = 500;  //дʱ���ֽ�֮����ʱ��,������� 
     TimeOuts.WriteTotalTimeoutConstant   = 5000; //д����ʱ�� 
     SetCommTimeouts(hCom, &TimeOuts); //���ó�ʱ
     
     DWORD dwCommEvent;
     if (!SetCommMask(hCom, EV_RXCHAR)) //�趨��Ҫ����֪ͨ���¼�,��Ӧ���յ�һ���ַ������ѷ������뻺�����¼� 
        printf("zzzzzzzzzzz");  
     if (!WaitCommEvent(hCom, &dwCommEvent, NULL))//����¼�����,ע��:���û���¼�����,�����߳̽���������,��ʱ openGL ������ȾҲ����ֹ,ֱ����λ���������� 
        printf("xxxxxxxxxxx");    
     char strR[13] = {0};
     BOOL       bResult;
     DWORD      RCount;//д����ֽ���
     OVERLAPPED wrOverlapped;
     DWORD      dwError; 
     ZeroMemory(&wrOverlapped, sizeof(wrOverlapped));//��0�����һ���ڴ�����
     if (wrOverlapped.hEvent != NULL){
         ResetEvent(wrOverlapped.hEvent);//ָ�����¼���������Ϊ���ź�״̬
         wrOverlapped.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);//�������һ�������Ļ��������¼�����
     }
     
     ClearCommError(hCom, &dwError, NULL);
     PurgeComm(hCom, PURGE_TXABORT | PURGE_TXCLEAR);//������ڵ����в���

     bResult = ReadFile(hCom,   //���ڵľ��
				        strR,   //Ҫд�����ݵ��׵�ַ
				        13,     //Ҫд������ݵĳ���
				        &RCount,//����д������ɺ󷵻ص�ʵ��д������ݳ�����Ϣ�Ĵ洢��ַ
				        //&wrOverlapped);  //�ص�I/O�ṹ��ָ��
				        NULL);  //�ص�I/O�ṹ��ָ��
	 if(!bResult){
          printf("������ʧ��!\n");
     }
     else{
          printf("�����ڳɹ�!\n");
          for (int i = 0; i < 13; i++){
             printf("  %#x",strR[i]);                        
          }  
          xxxx = strR[0];
          yyyy = strR[1];
          zzzz = strR[2];
          nnnn = strR[3]; 
          printf("\n");   
     }     
     if (!CloseHandle(hCom))
           printf("�ر�ʧ��\n");
     else
           printf("�رճɹ�\n");  
}
//==============================================================================
//==============================================================================
//==============================================================================
//==============================================================================
