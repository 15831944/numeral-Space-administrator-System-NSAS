#include <GL/freeglut.h>
#include <stdio.h>
#include "main.h"
#include "NcFileInterpreter.h"


extern volatile int r;
bool GM = false;
bool PF = false, GF = false;
//Beep(do,300);  //响一下 
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
                MemoryRelease();            //释放分配数组内存 
                InterpreterMemoryRelease(); //释放分配结构体内存 
                g_v = 0;
                PF  = openfile();      //打开文件选择对话框 
                GF  = openFile();      //打开文件 
                if ((PF == true) || (GF == true)){ 
                    windowsSize = 0;                   
                    FileRead();      //取得总的行数并复位文件流指针 
                    CNC_G_MemoryAllocation();          //分配内存 
                    CNC_InterpreterMemoryAllocation(); //依据文件总的行数分配内存
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
                   if (G_NC_Code == false)    //G代码显示与隐藏 
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
       case 'T':if (Test == false)    //测试数据显示与隐藏 
                  Test = true;
                else 
                  Test = false; 
                break;  
       case 'W':
       case 'w':OpenCommWrite(hCom, dcb, "com4");//打开串口写   
                break; 
       case 'r':
       case 'R':          
                ReadComm = true; 
                break;                          
    }
}
/*key常量	        描述
  GLUT_KEY_F1	    F1功能键
  GLUT_KEY_F2	    F2功能键
  GLUT_KEY_F3	    F3功能键
  GLUT_KEY_F4	    F4功能键
  GLUT_KEY_F5	    F5功能键
  GLUT_KEY_F6	    F6功能键
  GLUT_KEY_F7	    F7功能键
  GLUT_KEY_F8	    F8功能键
  GLUT_KEY_F9	    F9功能键
  GLUT_KEY_F10	        F10功能键
  GLUT_KEY_F11	        F11功能键
  GLUT_KEY_F12	        F12功能键
  GLUT_KEY_LEFT	        左方向键
  GLUT_KEY_UP	        上方向键
  GLUT_KEY_RIGHT	    右方向键
  GLUT_KEY_DOWN	        下方向键
  GLUT_KEY_PAGE_UP	    PageUp键
  GLUT_KEY_PAGE_DOWN	PageDown键
  GLUT_KEY_HOME	        Home键
  GLUT_KEY_END	        End键
  GLUT_KEY_INSERT	    Insert键
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
