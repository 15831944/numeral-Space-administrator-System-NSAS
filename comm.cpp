#include <windows.h>
#include <stdio.h>
#include <GL/freeglut.h>
#include "main.h"

/*
  API编写串口,过程一般是这样的:
   1. 创建串口句柄,用CreateFile;
   2. 对串口的参数进行设置,其中比较重要的是波特率(BaudRate),数据宽度(BytesBits),奇偶校验(Parity),停止位(StopBits),当然，重要的还有端口号(Port);
   3. 然后对串口进行相应的读写操作,这时候用到ReadFile和WriteFile函数;
   4. 写结束后,要关闭串口句柄,用CloseFile.
 */
//==============================================================================
//==============================================================================
//==============================================================================
//==============================================================================
/*
 * hCom:串口的句柄 
 * dcb:串口的结构体变量 
 * string:串口号(如: COM3等) 
 */
HANDLE hCom = NULL;    //全局变量,串口句柄
DCB    dcb;            //结构体变量用于串口设置 
bool ReadComm = false; //读串口标志 

bool OpenCommWrite(HANDLE hCom, DCB dcb, char *string){//打开串口写  
     hCom = CreateFile(string,     //COM3
                       GENERIC_READ|GENERIC_WRITE, //允许读和写
                       0,          //独占方式
                       NULL,
                       OPEN_EXISTING, //打开而不是创建
                       0,             //异步方式
                       NULL);
     if(hCom == INVALID_HANDLE_VALUE){
        CloseHandle(hCom);
        printf("打开 %s 失败!\n", string);
        return false;
     }else{
        printf("已成功打开 %s\n", string);
     }
     //-------------------------------------------------------------------------
     if(!GetCommState(hCom,&dcb))//获取当前DCB配置
       return false;
     else{
           printf("%s 当前通信字节位数   = %d\n", string, dcb.ByteSize);  
           printf("%s 当前波特率         = %d\n", string, dcb.BaudRate);  
           printf("%s 当前奇偶校验使能位 = %d\n", string, dcb.fParity);  
           printf("%s 当前奇偶校验方法   = %d\n", string, dcb.Parity);  
           printf("%s 当前停止位的位数   = %d\n", string, dcb.StopBits);  
           dcb.BaudRate        = CBR_57600;    //波特率   
           dcb.Parity          = NOPARITY;     //奇偶校验方式,值0~4分别对应无校验,奇校验,偶校验,校验置位,校验清零 
           dcb.fParity         = 0;            //是否进行奇偶校验,为1的话激活奇偶校验检查 
           dcb.StopBits        = TWOSTOPBITS;   //停止位个数,0~2分别对应1位,1.5位,2位停止位
           dcb.ByteSize        = 8; //数据宽度,一个字节的数据位个数,范围是5~8
     }  
     if(!SetCommState(hCom,&dcb)) //设置串口状态,常用的更改串口号,波特率,奇偶校验方式,数据位数等
       return false;              //不成功返回 false 
     else{
       printf("串口设好了\n");
     }
     if(!GetCommState(hCom,&dcb)) //获取当前DCB配置,无效时返回 false 
       return false;
     else{                        //有效时返回 true 
       printf("----------------------------------------------\n"); 
       printf("%s 当前通信字节位数   = %d\n", string, dcb.ByteSize);  
       printf("%s 当前波特率         = %d\n", string, dcb.BaudRate);  
       printf("%s 当前奇偶校验使能位 = %d\n", string, dcb.fParity);  
       printf("%s 当前奇偶校验方法   = %d\n", string, dcb.Parity);  
       printf("%s 当前停止位的位数   = %d\n", string, dcb.StopBits);        
     }
     //------------------------------------------------------------------------- 
     SetupComm(hCom,2048,2048); //输入缓冲区和输出缓冲区的大小都是2048  
     //-------------------------------------------------------------------------
     printf("----------------------------------------------\n"); 
     //-------------------------------------------------------------------------
     COMMTIMEOUTS TimeOuts;
     //设定读超时
     TimeOuts.ReadIntervalTimeout        = MAXDWORD;    //读时两字节之间间隔时间,毫秒计算,任意两个字符之间的时隔超过本限制,ReadFile操作将完成并返回任何已缓冲的数据.0代表本参数未设置. 
     TimeOuts.ReadTotalTimeoutMultiplier = 0;    //时间系数 
     TimeOuts.ReadTotalTimeoutConstant   = 0;    //读取总时长 
     //设定写超时
     TimeOuts.WriteTotalTimeoutMultiplier = 500;  //写时两字节之间间隔时间,毫秒计算 
     TimeOuts.WriteTotalTimeoutConstant   = 5000; //写入总时长 
     SetCommTimeouts(hCom, &TimeOuts); //设置超时
     
     char strW[13] = {0x13,0x14,0x15,0x16,0x17,0x18,0x19,0x20,0x21,0x22,0x23,0x24,0x5B};
     BOOL       bResult;
     DWORD      wCount;//写入的字节数
     OVERLAPPED wrOverlapped;
     DWORD      dwError; 
     ZeroMemory(&wrOverlapped, sizeof(wrOverlapped));//用0来填充一块内存区域
     if (wrOverlapped.hEvent != NULL){
         ResetEvent(wrOverlapped.hEvent);//指定的事件对象设置为无信号状态
         wrOverlapped.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);//创建或打开一个命名的或无名的事件对象
     }
     
     ClearCommError(hCom, &dwError, NULL);
     PurgeComm(hCom, PURGE_TXABORT | PURGE_TXCLEAR);//清除串口的所有操作

     bResult = WriteFile(hCom,   //串口的句柄
				         strW,   //要写入数据的首地址
				         13,     //要写入的数据的长度
				         &wCount,//函数写操作完成后返回的实际写入的数据长度信息的存储地址
				         //&wrOverlapped);  //重叠I/O结构的指针
				         NULL);  //重叠I/O结构的指针
	 if(!bResult){
          printf("写串口失败!\n");
     }
     else
          printf("写串口成功!\n");
     if (!CloseHandle(hCom))
           printf("关闭失败\n");
     else
           printf("关闭成功\n");           
}
//==============================================================================
//==============================================================================
//==============================================================================
//==============================================================================
bool OpenCommRead(HANDLE hCom, DCB dcb, char *string){//打开串口读 
    hCom = CreateFile(string,     //COM3
                       GENERIC_READ|GENERIC_WRITE, //允许读和写
                       0,          //独占方式
                       NULL,
                       OPEN_EXISTING, //打开而不是创建
                       0,             //异步方式
                       NULL);
     if(hCom == INVALID_HANDLE_VALUE){
        CloseHandle(hCom);
        printf("打开 %s 失败!\n", string);
        return false;
     }else{
        printf("已成功打开 %s\n", string);
     }
     //-------------------------------------------------------------------------
     if(!GetCommState(hCom,&dcb))//获取当前DCB配置
       return false;
     else{
           printf("%s 当前通信字节位数   = %d\n", string, dcb.ByteSize);  
           printf("%s 当前波特率         = %d\n", string, dcb.BaudRate);  
           printf("%s 当前奇偶校验使能位 = %d\n", string, dcb.fParity);  
           printf("%s 当前奇偶校验方法   = %d\n", string, dcb.Parity);  
           printf("%s 当前停止位的位数   = %d\n", string, dcb.StopBits);  
           dcb.BaudRate        = CBR_57600;    //波特率   
           dcb.Parity          = NOPARITY;     //奇偶校验方式,值0~4分别对应无校验,奇校验,偶校验,校验置位,校验清零 
           dcb.fParity         = 0;            //是否进行奇偶校验,为1的话激活奇偶校验检查 
           dcb.StopBits        = TWOSTOPBITS;   //停止位个数,0~2分别对应1位,1.5位,2位停止位
           dcb.ByteSize        = 8; //数据宽度,一个字节的数据位个数,范围是5~8
     }  
     if(!SetCommState(hCom,&dcb)) //设置串口状态,常用的更改串口号,波特率,奇偶校验方式,数据位数等
       return false;              //不成功返回 false 
     else{
       printf("串口设好了\n");
     }
     if(!GetCommState(hCom,&dcb)) //获取当前DCB配置,无效时返回 false 
       return false;
     else{                        //有效时返回 true 
       printf("----------------------------------------------\n"); 
       printf("%s 当前通信字节位数   = %d\n", string, dcb.ByteSize);  
       printf("%s 当前波特率         = %d\n", string, dcb.BaudRate);  
       printf("%s 当前奇偶校验使能位 = %d\n", string, dcb.fParity);  
       printf("%s 当前奇偶校验方法   = %d\n", string, dcb.Parity);  
       printf("%s 当前停止位的位数   = %d\n", string, dcb.StopBits);        
     }
     //------------------------------------------------------------------------- 
     SetupComm(hCom,128,128); //输入缓冲区和输出缓冲区的大小都是2048  
     //-------------------------------------------------------------------------
     printf("----------------------------------------------\n"); 
     //-------------------------------------------------------------------------
     COMMTIMEOUTS TimeOuts;
     //设定读超时
     TimeOuts.ReadIntervalTimeout        = 1000;    //读时两字节之间间隔时间,毫秒计算,任意两个字符之间的时隔超过本限制,ReadFile操作将完成并返回任何已缓冲的数据.0代表本参数未设置. 
     TimeOuts.ReadTotalTimeoutMultiplier = 50;    //时间系数 
     TimeOuts.ReadTotalTimeoutConstant   = 5000;    //读取总时长 
     //设定写超时
     TimeOuts.WriteTotalTimeoutMultiplier = 500;  //写时两字节之间间隔时间,毫秒计算 
     TimeOuts.WriteTotalTimeoutConstant   = 5000; //写入总时长 
     SetCommTimeouts(hCom, &TimeOuts); //设置超时
     
     DWORD dwCommEvent;
     if (!SetCommMask(hCom, EV_RXCHAR)) //设定需要接收通知的事件,响应接收到一个字符并且已放入输入缓冲区事件 
        printf("zzzzzzzzzzz");  
     if (!WaitCommEvent(hCom, &dwCommEvent, NULL))//检测事件发生,注意:如果没有事件发生,调用线程将无限阻塞,此时 openGL 窗口渲染也会中止,直到下位机发来数据 
        printf("xxxxxxxxxxx");    
     char strR[13] = {0};
     BOOL       bResult;
     DWORD      RCount;//写入的字节数
     OVERLAPPED wrOverlapped;
     DWORD      dwError; 
     ZeroMemory(&wrOverlapped, sizeof(wrOverlapped));//用0来填充一块内存区域
     if (wrOverlapped.hEvent != NULL){
         ResetEvent(wrOverlapped.hEvent);//指定的事件对象设置为无信号状态
         wrOverlapped.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);//创建或打开一个命名的或无名的事件对象
     }
     
     ClearCommError(hCom, &dwError, NULL);
     PurgeComm(hCom, PURGE_TXABORT | PURGE_TXCLEAR);//清除串口的所有操作

     bResult = ReadFile(hCom,   //串口的句柄
				        strR,   //要写入数据的首地址
				        13,     //要写入的数据的长度
				        &RCount,//函数写操作完成后返回的实际写入的数据长度信息的存储地址
				        //&wrOverlapped);  //重叠I/O结构的指针
				        NULL);  //重叠I/O结构的指针
	 if(!bResult){
          printf("读串口失败!\n");
     }
     else{
          printf("读串口成功!\n");
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
           printf("关闭失败\n");
     else
           printf("关闭成功\n");  
}
//==============================================================================
//==============================================================================
//==============================================================================
//==============================================================================
