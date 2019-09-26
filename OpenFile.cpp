#include <windows.h>
#include <Commdlg.h>
#include <Shlobj.h>
#include <stdio.h>
#include <string.h>
#include <GL/freeglut.h>
#include "main.h"
#include "error.h"
#include "NcFileInterpreter.h"

static char SzFile[300] = {0}; //用户选择的 G 代码文件的绝对路径 

char **G_array = NULL;  //以二维数组形式保存的 G 代码文件,行数 + 字符串 
int    g_v     = 0;     //用户选择的 G 代码文件的总的行数 

char **ConfigureFile = NULL;
struct CNC_MachineNumber ** cncConfigure = NULL; 

#define MAX_LINE 1024

FILE   *fpWrite = NULL;
FILE   *fpRead  = NULL;

FILE   *pCncMachineToolConfigure = NULL;
static char *CncMachineToolConfigureFile = ".\\cncMachineTool.configure";

//==========================================================================================================================
//==========================================================================================================================
//==========================================================================================================================
//==========================================================================================================================
bool CncConfigureFileInit(void){
    int CncConfigure = 0;
    char cc[256];
    pCncMachineToolConfigure = fopen(CncMachineToolConfigureFile, "r");
	if(pCncMachineToolConfigure == NULL){  
          MessageBox(NULL, TEXT("没有在当前目录下发现 \"cncMachineTool.configure\" 配置文件."), NULL, MB_ICONERROR);                                          
          return  false;
	}
	while(fgets(cc,256,pCncMachineToolConfigure) != NULL){ //总行数                               
		CncConfigure++;
    }
    rewind(pCncMachineToolConfigure);  //指针复位 
      
	ConfigureFile = (char **)malloc(CncConfigure*sizeof(char *));
    for(int i = 0; i < CncConfigure; i++)
       ConfigureFile[i] = (char *)malloc(256*sizeof(char));
       
    int n = 0;   
	while(fgets(ConfigureFile[n],256,pCncMachineToolConfigure) != NULL){                                          
	    n++;	   
    } 
    
    cncConfigure = (struct CNC_MachineNumber **)malloc(((CncConfigure-3) * sizeof(struct CNC_MachineNumber *))); //为第一重指针分配内存 
    for (int i = 0; i < (CncConfigure-3); i++)
       cncConfigure[i] = (struct CNC_MachineNumber *)malloc((sizeof(struct CNC_MachineNumber)));   //为第二重指针分配内存 
    int j = 0,u = 0,k = 0;  
    char c;
    char ch[12] = {'\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0'}; 
    for (int i = 2; i < CncConfigure - 1; i++){
        while (*(*(ConfigureFile + i) + j) != 10){ 
          A1:    
            c = *(*(ConfigureFile + i) + j); 
            if ((c == ',') || (c == ';')){
               if (ch[0] == 's' || ch[0] == 'S'){
                    for (int t = 0; t < 12; t++){
                        if (t < 11)
                           cncConfigure[k]->s[t] = ch[t + 1];
                        ch[t] = ' ';
                    }     
               }else if (ch[0] == 'x' || ch[0] == 'X'){
                    for (int t = 0; t < 12; t++){
                        if (t < 11)
                           cncConfigure[k]->x[t] = ch[t + 1];
                        ch[t] = ' ';
                    } 
               }else if (ch[0] == 'y' || ch[0] == 'Y'){
                     for (int t = 0; t < 12; t++){
                        if (t < 11)
                           cncConfigure[k]->y[t] = ch[t + 1];
                        ch[t] = ' ';
                    }
               }else if (ch[0] == 'z' || ch[0] == 'Z'){
                     for (int t = 0; t < 12; t++){
                        if (t < 11) 
                           cncConfigure[k]->z[t] = ch[t + 1];
                        ch[t] = ' ';
                    }
               }else if (ch[0] == 't' || ch[0] == 'T'){
                        if (ch[1] == '1')
                            cncConfigure[k]->ToolMagazine = true;
                        else
                            cncConfigure[k]->ToolMagazine = false; 
                        break;        
               }else{
                    for (int t = 0; t < 12; t++){
                        cncConfigure[k]->CncName[t] = ch[t];
                        ch[t] = ' ';
                    } 
                    cncConfigure[k]->CncName[12] = '\0';
                    cncConfigure[k]->CncName[13] = '\0';
               }
               j++;
               u = 0;
               goto A1;
               //continue;
            } 
            if (u < 12)                   
              ch[u] = c;
            u++;   
            j++;
        }//while
        cncConfigure[k]->StructLength = CncConfigure-3;
        j = 0;
        u = 0;
        k++;
    }//for 
    return  true; 
}
//==========================================================================================================================
//==========================================================================================================================
//==========================================================================================================================
//==========================================================================================================================
bool openfile(void){   //打开文件选择对话框 
        OPENFILENAME ofn;
        static char szFile[256];
        static char szFileTitle[256];
        memset(&ofn,0,sizeof(ofn));
        ofn.lStructSize  = sizeof(ofn);
        ofn.hwndOwner    = NULL;
        ofn.lpstrFilter  = "Picture File(*.nc,*.ptc)\0*.nc;*.ptc;\0\0";
        ofn.nFilterIndex = 1;
        ofn.lpstrFile    = szFile;         //全路径和用户指定的文件名
        ofn.nMaxFile     = sizeof(szFile); //以字符数计算,lpstrFile 所指向的缓冲区的大小
        ofn.lpstrFileTitle = szFileTitle;
        ofn.nMaxFileTitle  = sizeof(szFileTitle);
        ofn.Flags          = OFN_PATHMUSTEXIST|OFN_FILEMUSTEXIST|OFN_EXPLORER;
        //当按下确定按钮
        if(GetOpenFileName(&ofn)){//点击OK按钮,返回值为非零
             //MessageBox(NULL, szFile, TEXT("选择的文件"), 0);
        }else{     //用户取消或关闭Open对话框或错误出现,返回值为零
              MessageBox(NULL, TEXT("请选择一个文件"), NULL, MB_ICONERROR);
              return false;
        }
        int j = 0;
        for (int i = 0; i < 256; i++){
            SzFile[j] = szFile[i];
            j++;
            if (szFile[i] == '\\'){
                SzFile[j] = '\\';
                j++;          
            }
        }
        return true;
}

//==========================================================================================================================
//==========================================================================================================================
//==========================================================================================================================
//==========================================================================================================================
void Savefile(void){    
	OPENFILENAME ofn = { 0 };
	TCHAR strFilename[MAX_PATH] = { 0 };  //用于接收文件名
	ofn.lStructSize             = sizeof(OPENFILENAME);//结构体大小
	ofn.hwndOwner               = NULL;   //拥有着窗口句柄，为NULL表示对话框是非模态的,实际应用中一般都要有这个句柄
	ofn.lpstrFilter             = TEXT("所有文件\0*.*\0C/C++ Flie\0*.cpp;*.c;*.h\0\0");//设置过滤
	ofn.nFilterIndex            = 1;      //过滤器索引
	ofn.lpstrFile               = strFilename;          //接收返回的文件名,注意第一个字符需要为NULL
	ofn.nMaxFile                = sizeof(strFilename);  //缓冲区长度
	ofn.lpstrInitialDir         = NULL;   //初始目录为默认
    ofn.Flags       =  OFN_PATHMUSTEXIST | OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT;//目录必须存在,覆盖文件前发出警告
	ofn.lpstrTitle  = TEXT("保存到");//使用系统默认标题留空即可
	ofn.lpstrDefExt = TEXT("cpp");   //默认追加的扩展名
	if (GetSaveFileName(&ofn)){
		MessageBox(NULL, strFilename, TEXT("保存到"), 0);
	}else{
		MessageBox(NULL, TEXT("请输入一个文件名"), NULL, MB_ICONERROR);
	}
 
	TCHAR szBuffer[MAX_PATH] = { 0 };
	BROWSEINFO bi = { 0 };
	bi.hwndOwner  = NULL;//拥有着窗口句柄,为NULL表示对话框是非模态的,实际应用中一般都要有这个句柄
	bi.pszDisplayName = szBuffer;//接收文件夹的缓冲区
	bi.lpszTitle      = TEXT("选择一个文件夹");//标题
	bi.ulFlags        = BIF_NEWDIALOGSTYLE;
	LPITEMIDLIST idl  = SHBrowseForFolder(&bi);
	if (SHGetPathFromIDList(idl, szBuffer)){
		MessageBox(NULL, szBuffer, TEXT("你选择的文件夹"), 0);
	}
	else{
		MessageBox(NULL, TEXT("请选择一个文件夹"), NULL, MB_ICONERROR);
	}
}
//==========================================================================================================================
//==========================================================================================================================
//==========================================================================================================================
//==========================================================================================================================
bool openFile(){  //以只读方式打开用户选择的 G 代码文件	
	char strLine[MAX_LINE]; 
	fpRead = NULL;
	fpRead = fopen(SzFile, "r");
	if(fpRead == NULL){  
         //Beep(do,300);  //响一下 
         return false;     
	}
	return true;
}
//==========================================================================================================================
//==========================================================================================================================
//==========================================================================================================================
//==========================================================================================================================
void FileRead(){  //获取文件的总的行数 
	char strLine[MAX_LINE];     
    while(fgets(strLine,MAX_LINE,fpRead) != NULL){ //总行数                               
		g_v++;
    }
    //fseek(fpRead, 0, SEEK_SET);
    rewind(fpRead);  //指针复位 2
}
//==========================================================================================================================
//==========================================================================================================================
//==========================================================================================================================
//==========================================================================================================================
void openFileWrite(void){
    //下面是写数据,将数字0~9写入到data.txt文件中
	fpWrite = fopen("data.txt", "w");
	if(fpWrite == NULL){
       printf("error\n");   
    }
	for(int i = 0; i < 10; i++)
		fprintf(fpWrite, "%d ", i);
	fclose(fpWrite);
}
//==========================================================================================================================
//==========================================================================================================================
//==========================================================================================================================
//==========================================================================================================================
void CNC_G_MemoryAllocation(void){  //依据文件总的行数分配给二维数组内存 
    int n = 0;
    G_array = (char **)malloc(g_v * sizeof(char *));
    for(int i = 0; i < g_v; i++)
       G_array[i] = (char *)malloc(256 * sizeof(char));
    //rewind(fpRead);
    while(fgets(G_array[n],256,fpRead) != NULL){                                          
	    n++;	   
    } 
}
//==========================================================================================================================
//==========================================================================================================================
//==========================================================================================================================
//==========================================================================================================================
void MemoryRelease(void){//释放分配给二维数组的内存 
    for (int i = 0; i < g_v; i++)
       free(G_array[i]);  /*释放列*/
    free(G_array);        /*释放行*/   
}
