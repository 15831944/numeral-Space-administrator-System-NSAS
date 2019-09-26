#include <stdio.h>
#include <stdlib.h>
#include <GL/freeglut.h>
#include <Windows.h>
#include "main.h"
#include <math.h>
#include "NcFileInterpreter.h"
#include <pthread.h>

GLfloat light_position[] = {1.0, 1.0, 1.0, 0.0 };
GLfloat mat_diffuse[]    = {1.0, 1.0, 1.0, 1.0 };
int Test_pthread = 0;
bool CncConfigureFile_YES_NO = false;
struct openGL3D OpenGL3D;
struct win2D    Win2D;

void timerCB(int millisec){
    glutTimerFunc(millisec, timerCB, millisec);
    glutPostRedisplay();
}

void *pthread_1(void *args){
    while(1){
       if (ReadComm == true){
	      OpenCommRead(hCom, dcb, "com4"); //打开串口读
	      ReadComm = false;
       }      
       Test_pthread++;
       Sleep(1000);//停1秒 
    }
    return NULL;
}

int main(int argc, char *argv[]){
    pthread_t id1;
    int ret = pthread_create(&id1,NULL,pthread_1,NULL);
    //pthread_join(id1,NULL);
    if (ret != 0)
         MessageBox(NULL, TEXT("OFF"), NULL, MB_ICONERROR);   
	glutInit(&argc, argv);				//GLUT 初始化    
	glutInitWindowSize(1000, 600);      //设置初始窗口的大小
	BlendInit();
	//printf("x = %d\n", GetSystemMetrics(SM_CXSCREEN)); //获取屏幕的最大宽度
	//printf("y = %d\n", GetSystemMetrics(SM_CYSCREEN)); //获取屏幕的最大高度
	glutInitWindowPosition((GetSystemMetrics(SM_CXSCREEN) - 1000) / 2, (GetSystemMetrics(SM_CYSCREEN) - 600) / 2); //设置初始窗口的位置
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE); //设置初始显示模式
	glutCreateWindow("上海禹尚精密机械");        //创建顶层窗口
	glClearColor(0.0, 0.0, 0.0, 0.0);   //设定颜色缓存的清除值
	
	CncConfigureFile_YES_NO = CncConfigureFileInit();
	
	mouseLeftDown = mouseRightDown = false;
    mouseX = mouseY = 0;
    cameraAngleY = -45;
    cameraAngleX = -60;
    
    //cameraAngleX = cameraAngleY = 0.0f;
    cameraDistance = 5;

	printf("OpenGL 负责公司:%s\n", glGetString(GL_VENDOR));   //返回描述当前 OpenGL 的字符串
	printf("OpenGL 开发商  :%s\n", glGetString(GL_VENDOR));
	printf("OpenGL 版本    :%s\n", glGetString(GL_VERSION));

    glShadeModel(GL_FLAT);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	glEnable(GL_LIGHTING);//启用光照
	glEnable(GL_LIGHT0);
	float whiteColor[] = { 0.5f,0.5f,0.5f,1.0f };
	float ambientMat[] = { 0.02f,0.02f,0.02f,1.0f };
	glLightfv(GL_LIGHT0, GL_AMBIENT, whiteColor);   //设置环境光 
	glMaterialfv(GL_FRONT, GL_AMBIENT, ambientMat); //设置反射系数
    //设置全局环境光：
    //float Light_Model_Ambient[] = { 0.2 , 0.2 , 0.2 , 1.0 }; // 缺省值
    //glLightModelfv(GL_LIGHT_MODEL_AMBIENT , Light_Model_Ambient);

    glEnable(GL_DEPTH_TEST);
    glEnable( GL_COLOR_MATERIAL );
    glLightModeli( GL_FRONT, GL_AMBIENT_AND_DIFFUSE );
    
	glutDisplayFunc(display);       //注册 GLUT 窗口的显示回调函数
	glutReshapeFunc(reshape);       //注册 GLUT 当前窗口的形状变化回调函数
	glutVisibilityFunc(visibility); //注册 GLUT 当前窗口的可视回调函数
	glutEntryFunc(entry);           //注册 GLUT 当前窗口鼠标进出回调函数

	glutPassiveMotionFunc(passivemotion); //注册 GLUT 当前窗口的鼠标正在移动回调函数
	glutMotionFunc(motion);               //注册 GLUT 当前窗口的鼠标按下并移动回调函数
	//glutMouseFunc(mouse);                 //注册 GLUT 当前窗口的鼠标回调函数
	glutMouseFunc(MouseCB);                 //注册 GLUT 当前窗口的鼠标回调函数
    glutMouseWheelFunc(mouseWheel);       //

	glutKeyboardFunc(keybutton);    //注册按键回调函数
	glutSpecialFunc(special);       //注册特定键回调函数
    
    glutTimerFunc(33, timerCB, 33);   

	glutMainLoop();  //进入 GLUT 事件处理循环     
	return 0;
	
}
