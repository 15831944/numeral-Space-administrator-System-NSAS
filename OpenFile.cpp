#include <windows.h>
#include <Commdlg.h>
#include <Shlobj.h>
#include <stdio.h>
#include <string.h>
#include <GL/freeglut.h>
#include "main.h"
#include "error.h"
#include "NcFileInterpreter.h"

static char SzFile[300] = {0}; //�û�ѡ��� G �����ļ��ľ���·�� 

char **G_array = NULL;  //�Զ�ά������ʽ����� G �����ļ�,���� + �ַ��� 
int    g_v     = 0;     //�û�ѡ��� G �����ļ����ܵ����� 

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
          MessageBox(NULL, TEXT("û���ڵ�ǰĿ¼�·��� \"cncMachineTool.configure\" �����ļ�."), NULL, MB_ICONERROR);                                          
          return  false;
	}
	while(fgets(cc,256,pCncMachineToolConfigure) != NULL){ //������                               
		CncConfigure++;
    }
    rewind(pCncMachineToolConfigure);  //ָ�븴λ 
      
	ConfigureFile = (char **)malloc(CncConfigure*sizeof(char *));
    for(int i = 0; i < CncConfigure; i++)
       ConfigureFile[i] = (char *)malloc(256*sizeof(char));
       
    int n = 0;   
	while(fgets(ConfigureFile[n],256,pCncMachineToolConfigure) != NULL){                                          
	    n++;	   
    } 
    
    cncConfigure = (struct CNC_MachineNumber **)malloc(((CncConfigure-3) * sizeof(struct CNC_MachineNumber *))); //Ϊ��һ��ָ������ڴ� 
    for (int i = 0; i < (CncConfigure-3); i++)
       cncConfigure[i] = (struct CNC_MachineNumber *)malloc((sizeof(struct CNC_MachineNumber)));   //Ϊ�ڶ���ָ������ڴ� 
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
bool openfile(void){   //���ļ�ѡ��Ի��� 
        OPENFILENAME ofn;
        static char szFile[256];
        static char szFileTitle[256];
        memset(&ofn,0,sizeof(ofn));
        ofn.lStructSize  = sizeof(ofn);
        ofn.hwndOwner    = NULL;
        ofn.lpstrFilter  = "Picture File(*.nc,*.ptc)\0*.nc;*.ptc;\0\0";
        ofn.nFilterIndex = 1;
        ofn.lpstrFile    = szFile;         //ȫ·�����û�ָ�����ļ���
        ofn.nMaxFile     = sizeof(szFile); //���ַ�������,lpstrFile ��ָ��Ļ������Ĵ�С
        ofn.lpstrFileTitle = szFileTitle;
        ofn.nMaxFileTitle  = sizeof(szFileTitle);
        ofn.Flags          = OFN_PATHMUSTEXIST|OFN_FILEMUSTEXIST|OFN_EXPLORER;
        //������ȷ����ť
        if(GetOpenFileName(&ofn)){//���OK��ť,����ֵΪ����
             //MessageBox(NULL, szFile, TEXT("ѡ����ļ�"), 0);
        }else{     //�û�ȡ����ر�Open�Ի����������,����ֵΪ��
              MessageBox(NULL, TEXT("��ѡ��һ���ļ�"), NULL, MB_ICONERROR);
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
	TCHAR strFilename[MAX_PATH] = { 0 };  //���ڽ����ļ���
	ofn.lStructSize             = sizeof(OPENFILENAME);//�ṹ���С
	ofn.hwndOwner               = NULL;   //ӵ���Ŵ��ھ����ΪNULL��ʾ�Ի����Ƿ�ģ̬��,ʵ��Ӧ����һ�㶼Ҫ��������
	ofn.lpstrFilter             = TEXT("�����ļ�\0*.*\0C/C++ Flie\0*.cpp;*.c;*.h\0\0");//���ù���
	ofn.nFilterIndex            = 1;      //����������
	ofn.lpstrFile               = strFilename;          //���շ��ص��ļ���,ע���һ���ַ���ҪΪNULL
	ofn.nMaxFile                = sizeof(strFilename);  //����������
	ofn.lpstrInitialDir         = NULL;   //��ʼĿ¼ΪĬ��
    ofn.Flags       =  OFN_PATHMUSTEXIST | OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT;//Ŀ¼�������,�����ļ�ǰ��������
	ofn.lpstrTitle  = TEXT("���浽");//ʹ��ϵͳĬ�ϱ������ռ���
	ofn.lpstrDefExt = TEXT("cpp");   //Ĭ��׷�ӵ���չ��
	if (GetSaveFileName(&ofn)){
		MessageBox(NULL, strFilename, TEXT("���浽"), 0);
	}else{
		MessageBox(NULL, TEXT("������һ���ļ���"), NULL, MB_ICONERROR);
	}
 
	TCHAR szBuffer[MAX_PATH] = { 0 };
	BROWSEINFO bi = { 0 };
	bi.hwndOwner  = NULL;//ӵ���Ŵ��ھ��,ΪNULL��ʾ�Ի����Ƿ�ģ̬��,ʵ��Ӧ����һ�㶼Ҫ��������
	bi.pszDisplayName = szBuffer;//�����ļ��еĻ�����
	bi.lpszTitle      = TEXT("ѡ��һ���ļ���");//����
	bi.ulFlags        = BIF_NEWDIALOGSTYLE;
	LPITEMIDLIST idl  = SHBrowseForFolder(&bi);
	if (SHGetPathFromIDList(idl, szBuffer)){
		MessageBox(NULL, szBuffer, TEXT("��ѡ����ļ���"), 0);
	}
	else{
		MessageBox(NULL, TEXT("��ѡ��һ���ļ���"), NULL, MB_ICONERROR);
	}
}
//==========================================================================================================================
//==========================================================================================================================
//==========================================================================================================================
//==========================================================================================================================
bool openFile(){  //��ֻ����ʽ���û�ѡ��� G �����ļ�	
	char strLine[MAX_LINE]; 
	fpRead = NULL;
	fpRead = fopen(SzFile, "r");
	if(fpRead == NULL){  
         //Beep(do,300);  //��һ�� 
         return false;     
	}
	return true;
}
//==========================================================================================================================
//==========================================================================================================================
//==========================================================================================================================
//==========================================================================================================================
void FileRead(){  //��ȡ�ļ����ܵ����� 
	char strLine[MAX_LINE];     
    while(fgets(strLine,MAX_LINE,fpRead) != NULL){ //������                               
		g_v++;
    }
    //fseek(fpRead, 0, SEEK_SET);
    rewind(fpRead);  //ָ�븴λ 2
}
//==========================================================================================================================
//==========================================================================================================================
//==========================================================================================================================
//==========================================================================================================================
void openFileWrite(void){
    //������д����,������0~9д�뵽data.txt�ļ���
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
void CNC_G_MemoryAllocation(void){  //�����ļ��ܵ������������ά�����ڴ� 
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
void MemoryRelease(void){//�ͷŷ������ά������ڴ� 
    for (int i = 0; i < g_v; i++)
       free(G_array[i]);  /*�ͷ���*/
    free(G_array);        /*�ͷ���*/   
}
