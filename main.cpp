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
	      OpenCommRead(hCom, dcb, "com4"); //�򿪴��ڶ�
	      ReadComm = false;
       }      
       Test_pthread++;
       Sleep(1000);//ͣ1�� 
    }
    return NULL;
}

int main(int argc, char *argv[]){
    pthread_t id1;
    int ret = pthread_create(&id1,NULL,pthread_1,NULL);
    //pthread_join(id1,NULL);
    if (ret != 0)
         MessageBox(NULL, TEXT("OFF"), NULL, MB_ICONERROR);   
	glutInit(&argc, argv);				//GLUT ��ʼ��    
	glutInitWindowSize(1000, 600);      //���ó�ʼ���ڵĴ�С
	BlendInit();
	//printf("x = %d\n", GetSystemMetrics(SM_CXSCREEN)); //��ȡ��Ļ�������
	//printf("y = %d\n", GetSystemMetrics(SM_CYSCREEN)); //��ȡ��Ļ�����߶�
	glutInitWindowPosition((GetSystemMetrics(SM_CXSCREEN) - 1000) / 2, (GetSystemMetrics(SM_CYSCREEN) - 600) / 2); //���ó�ʼ���ڵ�λ��
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE); //���ó�ʼ��ʾģʽ
	glutCreateWindow("�Ϻ����о��ܻ�е");        //�������㴰��
	glClearColor(0.0, 0.0, 0.0, 0.0);   //�趨��ɫ��������ֵ
	
	CncConfigureFile_YES_NO = CncConfigureFileInit();
	
	mouseLeftDown = mouseRightDown = false;
    mouseX = mouseY = 0;
    cameraAngleY = -45;
    cameraAngleX = -60;
    
    //cameraAngleX = cameraAngleY = 0.0f;
    cameraDistance = 5;

	printf("OpenGL ����˾:%s\n", glGetString(GL_VENDOR));   //����������ǰ OpenGL ���ַ���
	printf("OpenGL ������  :%s\n", glGetString(GL_VENDOR));
	printf("OpenGL �汾    :%s\n", glGetString(GL_VERSION));

    glShadeModel(GL_FLAT);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	glEnable(GL_LIGHTING);//���ù���
	glEnable(GL_LIGHT0);
	float whiteColor[] = { 0.5f,0.5f,0.5f,1.0f };
	float ambientMat[] = { 0.02f,0.02f,0.02f,1.0f };
	glLightfv(GL_LIGHT0, GL_AMBIENT, whiteColor);   //���û����� 
	glMaterialfv(GL_FRONT, GL_AMBIENT, ambientMat); //���÷���ϵ��
    //����ȫ�ֻ����⣺
    //float Light_Model_Ambient[] = { 0.2 , 0.2 , 0.2 , 1.0 }; // ȱʡֵ
    //glLightModelfv(GL_LIGHT_MODEL_AMBIENT , Light_Model_Ambient);

    glEnable(GL_DEPTH_TEST);
    glEnable( GL_COLOR_MATERIAL );
    glLightModeli( GL_FRONT, GL_AMBIENT_AND_DIFFUSE );
    
	glutDisplayFunc(display);       //ע�� GLUT ���ڵ���ʾ�ص�����
	glutReshapeFunc(reshape);       //ע�� GLUT ��ǰ���ڵ���״�仯�ص�����
	glutVisibilityFunc(visibility); //ע�� GLUT ��ǰ���ڵĿ��ӻص�����
	glutEntryFunc(entry);           //ע�� GLUT ��ǰ�����������ص�����

	glutPassiveMotionFunc(passivemotion); //ע�� GLUT ��ǰ���ڵ���������ƶ��ص�����
	glutMotionFunc(motion);               //ע�� GLUT ��ǰ���ڵ���갴�²��ƶ��ص�����
	//glutMouseFunc(mouse);                 //ע�� GLUT ��ǰ���ڵ����ص�����
	glutMouseFunc(MouseCB);                 //ע�� GLUT ��ǰ���ڵ����ص�����
    glutMouseWheelFunc(mouseWheel);       //

	glutKeyboardFunc(keybutton);    //ע�ᰴ���ص�����
	glutSpecialFunc(special);       //ע���ض����ص�����
    
    glutTimerFunc(33, timerCB, 33);   

	glutMainLoop();  //���� GLUT �¼�����ѭ��     
	return 0;
	
}
