#include <GL/freeglut.h>
#include <stdio.h>
#include "main.h"
#include "NcFileInterpreter.h"


extern volatile int r;
bool GM = false;
bool PF = false, GF = false;
//Beep(do,300);  //��һ�� 
extern int xxxx, yyyy, zzzz, nnnn;
void keybutton(unsigned char key, int x, int y){
    switch(key){
       case '1':Viewport = 1;break; 
       case '2':Viewport = 2;break; 
       case '3':Viewport = 3;break; 
       case '4':Viewport = 4;break; 
       case '5':Viewport = 5;break; 
       case '6':Viewport = 6;break; 
       case '7':Viewport = 7;break; 
       case 'o':
       case 'O': 
                MemoryRelease();            //�ͷŷ��������ڴ� 
                InterpreterMemoryRelease(); //�ͷŷ���ṹ���ڴ� 
                g_v = 0;
                PF  = openfile();      //���ļ�ѡ��Ի��� 
                GF  = openFile();      //���ļ� 
                if ((PF == true) || (GF == true)){ 
                    windowsSize = 0;                   
                    FileRead();      //ȡ���ܵ���������λ�ļ���ָ�� 
                    CNC_G_MemoryAllocation();          //�����ڴ� 
                    CNC_InterpreterMemoryAllocation(); //�����ļ��ܵ����������ڴ�
                    StructInit(G_M_CODE, g_v);
                    NcFileInterpreter(G_M_CODE, G_array, g_v, cncConfigure, 2);
                    GM           = true;
                    G_NC_Code    = true;
                    KnifePath    = true; 
                    OrthoWin     = windowsSize;  
                    OrthoWinBool = true;
                    r            = 0;
                    cameraAngleY = -45;
                    cameraAngleX = -60;
                    glutPostRedisplay();
                }else{
                    GM        = false;
                    KnifePath = false; 
                    G_NC_Code = false;  
                }
                break;  
       case 's':
       case 'S':if (GM == true){
                   if (G_NC_Code == false)    //G������ʾ������ 
                      G_NC_Code = true;
                   else 
                      G_NC_Code = false; 
                }       
                break;   
       case 'v':
       case 'V':if (r < (g_v - 1))
                  r++;
                glutPostRedisplay();
                break; 
       case 't':
       case 'T':if (Test == false)    //����������ʾ������ 
                  Test = true;
                else 
                  Test = false; 
                break;  
       case 'W':
       case 'w':OpenCommWrite(hCom, dcb, "com4");//�򿪴���д   
                break; 
       case 'r':
       case 'R':          
                ReadComm = true; 
                break;                          
    }
}
/*key����	        ����
  GLUT_KEY_F1	    F1���ܼ�
  GLUT_KEY_F2	    F2���ܼ�
  GLUT_KEY_F3	    F3���ܼ�
  GLUT_KEY_F4	    F4���ܼ�
  GLUT_KEY_F5	    F5���ܼ�
  GLUT_KEY_F6	    F6���ܼ�
  GLUT_KEY_F7	    F7���ܼ�
  GLUT_KEY_F8	    F8���ܼ�
  GLUT_KEY_F9	    F9���ܼ�
  GLUT_KEY_F10	        F10���ܼ�
  GLUT_KEY_F11	        F11���ܼ�
  GLUT_KEY_F12	        F12���ܼ�
  GLUT_KEY_LEFT	        �����
  GLUT_KEY_UP	        �Ϸ����
  GLUT_KEY_RIGHT	    �ҷ����
  GLUT_KEY_DOWN	        �·����
  GLUT_KEY_PAGE_UP	    PageUp��
  GLUT_KEY_PAGE_DOWN	PageDown��
  GLUT_KEY_HOME	        Home��
  GLUT_KEY_END	        End��
  GLUT_KEY_INSERT	    Insert��
 */
void special(int key, int x, int y){
     if (G_NC_Code == true){
         if (key == GLUT_KEY_UP){
            if ((g_v - Row) > (screenHeight / 15))     
              Row += 1;
         }  
         if (key == GLUT_KEY_DOWN){
            if (Row != 0)   
              Row -= 1;
         }
     }
     switch(key){
         case GLUT_KEY_F1: 
                          break;
         case GLUT_KEY_F2:
                          break;
         case GLUT_KEY_F3:
                          break;
         case GLUT_KEY_F4:break;
         case GLUT_KEY_F5:break;
         case GLUT_KEY_F6:break;
         case GLUT_KEY_F7:break;
         case GLUT_KEY_F8:break;        
         case GLUT_KEY_F9:if (CoordinateLineBool == false)
                               CoordinateLineBool = true;
                          else 
                               CoordinateLineBool = false;   
                          break;
         case GLUT_KEY_F10:break;
         case GLUT_KEY_F11:break;
         case GLUT_KEY_F12:break;                 
                          
     }
}
