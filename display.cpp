#include <GL/freeglut.h>
#include <stdio.h>
#include <stdlib.h>
#include "main.h"
#include "NcFileInterpreter.h"
#include "color.h"


#define toOrtho
//#define toPerspective
int   OrthoWin     = 350;
int   windowsSize  = 350;
bool  OrthoWinBool = false;
int   RowNumber    = 0;


int xxxx = 10, yyyy = 0, zzzz = 0, nnnn = 0;
float Color[]    = {220,20,60};
float color[4]   = {1, 1, 0, 1};
float color1[4]  = {COLORCONVERT(PeachPuff[0]), COLORCONVERT(PeachPuff[1]), COLORCONVERT(PeachPuff[2]), 1};

bool  G_NC_Code = false;  //G ������ʾ�����ر�־λ
bool  KnifePath = false; 
bool  CoordinateLineBool = false;
bool  Test      = false;  //���ݲ��Ժ����Ƿ����� 

GLint    viewport[4];
GLdouble modelview[16];
GLdouble portection[16];
//==========================================================================================================================
//==========================================================================================================================
//==========================================================================================================================
//==========================================================================================================================
extern int Test_pthread;
void display(void){  //��ʾ�ص�����
    if (OrthoWinBool == true){                   
        glMatrixMode(GL_PROJECTION);//ָ����ͶӰ����Ӧ�����ľ������ 
    	glLoadIdentity();           //���õ�ǰָ���ľ���Ϊ��λ����.
        if (screenHeight == 0)
    		screenHeight = 1;
    	if (screenWidth <= screenHeight)
    		glOrtho(-OrthoWin, OrthoWin, -OrthoWin * ((GLfloat)screenHeight / (GLfloat)screenWidth), OrthoWin * ((GLfloat)screenHeight / (GLfloat)screenWidth), -100000.0, 100000);
    	else
    		glOrtho(-OrthoWin * ((GLfloat)screenWidth / (GLfloat)screenHeight), OrthoWin * ((GLfloat)screenWidth / (GLfloat)screenHeight), -OrthoWin, OrthoWin, -100000.0, 100000);
    
    	glMatrixMode(GL_MODELVIEW); //���õ�ǰ����Ϊģ����ͼ����
    	glLoadIdentity();
    	OrthoWinBool = false;
    }
    
    /*GL_COLOR_BUFFER_BIT:��ǰ��д����ɫ����,GL_DEPTH_BUFFER_BIT:��Ȼ���,GL_ACCUM_BUFFER_BIT:�ۻ�����,GL_STENCIL_BUFFER_BIT:ģ�建��*/
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 
	glClearColor(0.3, 0.3, 0.5, 1.0);  //�趨��ɫ
    glEnable(GL_LIGHTING);//���ù���
    glEnable(GL_DEPTH_TEST);
	glPushMatrix();	
    	
    PaintScreen(G_M_CODE, g_v, GL_RENDER); 

    glPopMatrix();
    glFlush();                         //ǿ��ˢ�»��壬��֤���еĻ�ͼ���ִ��
	glutSwapBuffers();                 //������ǰ���ڻ���
}

//==========================================================================================================================
//==========================================================================================================================
//==========================================================================================================================
//========================================================================================================================== 
void reshape(int width, int height){   //��ǰ���ڵ���״�仯�ص�����
    screenWidth  = width;
    screenHeight = height;
#ifdef  toOrtho    //���ڸı�ʱ�Ļص�����,ʹ����ͶӰ  
	glViewport(0, 0, (GLsizei)width, (GLsizei)height); //�����ӿ�,OpenGL�Ӽ���λ�ô�С,����Ⱦ���������λ�ô�С

	glMatrixMode(GL_PROJECTION);//ָ����ͶӰ����Ӧ�����ľ������ 
	glLoadIdentity();           //���õ�ǰָ���ľ���Ϊ��λ����.
	if (height == 0)
		height = 1;
	if (width <= height)
		glOrtho(-OrthoWin, OrthoWin, -OrthoWin * ((GLfloat)height / (GLfloat)width), OrthoWin * ((GLfloat)height / (GLfloat)width), -100000.0, 100000);
	else
		glOrtho(-OrthoWin * ((GLfloat)width / (GLfloat)height), OrthoWin * ((GLfloat)width / (GLfloat)height), -OrthoWin, OrthoWin, -100000.0, 100000);
	glMatrixMode(GL_MODELVIEW); //���õ�ǰ����Ϊģ����ͼ����
	glLoadIdentity();
#endif 
  
#ifdef toPerspective //���ڸı�ʱ�Ļص�����,ʹ��͸��ͶӰ 
	glViewport(0, 0, (GLsizei)width, (GLsizei)height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60.0f, (float)(width) / height, 1.0f, 1000.0f); // FOV, AspectRatio, NearClip, FarClip
	gluLookAt(10, 10, 1, 0, 0, 0, 0, 0, 1);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
#endif 
}
//==========================================================================================================================
//==========================================================================================================================
//==========================================================================================================================
//==========================================================================================================================
void visibility(int state){   //��ǰ���ڵĿ��ӻص�����
}
//==========================================================================================================================
//==========================================================================================================================
//==========================================================================================================================
//==========================================================================================================================
void entry(int state){  //��ǰ�����������ص�����  
    if(state == GLUT_LEFT){    //�뿪 
         G_NC_Code = false;
    }
    if(state == GLUT_ENTERED){ //���� 
         G_NC_Code = true;
    }
}
//==========================================================================================================================
//==========================================================================================================================
//==========================================================================================================================
//==========================================================================================================================
void MouseCB(int button, int state, int mousex, int mousey){
	if(button != GLUT_LEFT || state != GLUT_DOWN){
		return;
	}
	//=========================================================================================== 
	//����ת��صĴ��� 
    mouseX = mousex;
    mouseY = mousey;   
    if(button == GLUT_LEFT_BUTTON){
        if(state == GLUT_DOWN){
            mouseLeftDown = true;
        }
        else if(state == GLUT_UP)
            mouseLeftDown = false;
    }

    else if(button == GLUT_RIGHT_BUTTON){
        if(state == GLUT_DOWN){
            mouseRightDown = true;            
        }
        else if(state == GLUT_UP)
            mouseRightDown = false;
    }  
    //===========================================================================================
    //��ʰȡ��صĴ��� 
	GLuint      selectBuf[512] = {0,0}; //�����޷�����������,512��ֵ 
	GLint       hits;                   //������������ 
	GLint       viewport[4];            //������������ 
	static bool IsFirst = true;         //������̬�������� 

	glGetIntegerv (GL_VIEWPORT, viewport); //����ӿ���Ϣ,ָ�����ص����¼������,GL_VIEWPORTָʾ����4��ֵX,Y�������ӿڿ��,�߶�,�����ǽ������� 
	glSelectBuffer (512, selectBuf);       //����ѡ�񻺳���,1�ǳ���,2�ǻ�������ַָ�� 

	glRenderMode(GL_SELECT); //����ѡ��ģʽ 
	glInitNames();           //���ֶ�ջ��ʼ��
	glPushName(1);           //ѹ�����ƶ�ջ 

	glMatrixMode (GL_PROJECTION); //ָ��һ������Ϊ��ǰ���� 
	glPushMatrix ();              //ѹ�뵱ǰ��������ջ
	   glLoadIdentity ();         //���õ�ǰָ���ľ���Ϊ��λ����,������ĺ�������ϵ� 
	   gluPickMatrix((GLdouble)mousex, (GLdouble)(viewport[3] - mousey), 5.0, 5.0, viewport);//����ʰȡ���� 
	   if (screenWidth <= screenHeight)
		   glOrtho(-OrthoWin, OrthoWin, -OrthoWin * ((GLfloat)screenHeight / (GLfloat)screenWidth), OrthoWin * ((GLfloat)screenHeight / (GLfloat)screenWidth), -100000.0, 100000);
	   else
		   glOrtho(-OrthoWin * ((GLfloat)screenWidth / (GLfloat)screenHeight), OrthoWin * ((GLfloat)screenWidth / (GLfloat)screenHeight), -OrthoWin, OrthoWin, -100000.0, 100000);
	glMatrixMode(GL_MODELVIEW);   //ָ��һ������Ϊ��ǰ���� 
	
	PaintScreen(G_M_CODE, g_v, GL_SELECT);//��������ĺ���,��תģ�Ͳ������ֶ�ջ��ѹ������,����ѡ�е�Ŀ������ػ�
	
	glPopMatrix ();//������ǰ��������ջ
	glFlush();

	hits = glRenderMode (GL_RENDER);//��ѡ��ʱ����������� 1,���򷵻� 0,���¼д�뻺����,һ�������¼4���������� 
	
	glMatrixMode(GL_PROJECTION);    //ָ��һ������Ϊ��ǰ���� 
	glLoadIdentity();               //���õ�ǰָ���ľ���Ϊ��λ����,������ĺ�������ϵ� 
	glViewport(0, 0, screenWidth, screenHeight);//�����ӿ�,�����и� bug ��û�н����ڸı���ֵ��Ϊ�ӿڴ�С 
	if (screenWidth <= screenHeight)
		glOrtho(-OrthoWin, OrthoWin, -OrthoWin * ((GLfloat)screenHeight / (GLfloat)screenWidth), OrthoWin * ((GLfloat)screenHeight / (GLfloat)screenWidth), -100000.0, 100000);
	else
		glOrtho(-OrthoWin * ((GLfloat)screenWidth / (GLfloat)screenHeight), OrthoWin * ((GLfloat)screenWidth / (GLfloat)screenHeight), -OrthoWin, OrthoWin, -100000.0, 100000);
    glMatrixMode(GL_MODELVIEW); //���õ�ǰ����Ϊģ����ͼ����
	glLoadIdentity();
    	
    if(hits){     
		int n = 0;
		double minz = selectBuf[1];
		for(int i = 1; i < hits; i++){
			if( selectBuf[1 + i * 4] < minz ){
				n = i;
				minz = selectBuf[1 + i * 4];
			}
		}
		//hit��¼������ΪID���
		if (KnifePath == true){
    		if (G_M_CODE[selectBuf[3 + n * 4]] -> Selection == false){
    		      G_M_CODE[selectBuf[3 + n * 4]] -> Selection = true;
    		}else{ 
                  G_M_CODE[selectBuf[3 + n * 4]] -> Selection = false; 
            } 
        }         
	} 	
}

char Viewport = 1;
void DrawGraphics(struct G_M_XYZ **G_M_CODE, int g_v, GLenum mode);
void PaintScreen(struct G_M_XYZ **G_M_CODE, int g_v, GLenum mode){ 
     int j = screenHeight - 10;       
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++  
     switch(Viewport){
         case 1:               
                glRotatef(cameraAngleX, 1, 0, 0);   //�����ת X
                glRotatef(cameraAngleY, 0, 0, 1);   //�����ת Y  
                DrawGraphics(G_M_CODE, g_v, mode);
                break;
         case 2:DrawGraphics(G_M_CODE, g_v, mode); //�� 
                break; 
         case 3:glRotatef(-90, 1, 0, 0);   
                DrawGraphics(G_M_CODE, g_v, mode); //ǰ 
                break;  
         case 4:glRotatef(90, 0, 1, 0); 
                glRotatef(-90, 1, 0, 0); 
                DrawGraphics(G_M_CODE, g_v, mode); //�� 
                break;  
         case 5:glRotatef(-90, 0, 1, 0); 
                glRotatef(-90, 1, 0, 0); 
                DrawGraphics(G_M_CODE, g_v, mode); //�� 
                break;  
         case 6:glRotatef(90, 1, 0, 0);  
                glRotatef(180, 0, 1, 0);  
                DrawGraphics(G_M_CODE, g_v, mode); //�� 
                break;  
         case 7:glRotatef(-180, 1, 0, 0);   
                DrawGraphics(G_M_CODE, g_v, mode); //�� 
                break;         
    }
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++    
    glGetIntegerv(GL_VIEWPORT,viewport);//ȡ��������Ļ���굽 3D ����任��Ҫ��ֵ 
    glGetDoublev(GL_MODELVIEW_MATRIX,modelview);
    glGetDoublev(GL_PROJECTION_MATRIX,portection);
      
    glDisable(GL_DEPTH_TEST);   //�ر���Ȳ��� 
    glDisable(GL_LIGHTING);     //�رչ��� 
    //--------------------------------------------------------------------------	
    //�����ڴ�����,���ƴ������ 
    if (mode != GL_SELECT){
            RowNumber = Row;
            if (G_NC_Code == true){
                for (RowNumber; RowNumber < (screenHeight / 15) + Row; RowNumber++){
                    if (RowNumber >= g_v)
                       break;
                    drawString2D(G_array[RowNumber], 0, j, color1, GLUT_BITMAP_8_BY_13);
                    j -= 15;
                }       
            }
            showPolygon(screenWidth,screenHeight, screenWidth-120,screenHeight, screenWidth - 120,0, screenWidth,0,LightGrey);
            showLine(screenWidth - 120,screenWidth - 120, 0,screenHeight, 1,1, DimGray,2);
            button(screenWidth-5, screenHeight-5,   110, 35, true, "13�Ŵ�����");
            button(screenWidth-5, screenHeight-45,  110, 35, true, "12�Ź���");
            button(screenWidth-5, screenHeight-85,  110, 35, true, "11�Ź�˹");
            button(screenWidth-5, screenHeight-125, 110, 35, true, "10��С����");
            button(screenWidth-5, screenHeight-165, 110, 35, true, "9 ��");
            button(screenWidth-5, screenHeight-205, 110, 35, true, "8 ��");
            button(screenWidth-5, screenHeight-245, 110, 35, true, "7 ��");
            button(screenWidth-5, screenHeight-285, 110, 35, false,"6 ��");
            
            char ccc[0]; //���Դ��� 
            itoa(xxxx,ccc,10); //���Դ���    
            drawString2DCN(ccc, screenWidth-110, 90, OrangeRed, 12);
            itoa(yyyy,ccc,10); //���Դ��� 
            drawString2DCN(ccc, screenWidth-60, 90, OrangeRed, 12);
            itoa(zzzz,ccc,10); //���Դ��� 
            drawString2DCN(ccc, screenWidth-60, 190, OrangeRed, 12); 
            itoa(nnnn,ccc,10); //���Դ��� 
            drawString2DCN(ccc, screenWidth-110, 190, OrangeRed, 12); 
            
            if (Test == true)
                ncFileTest(screenWidth, screenHeight);
            
            drawString2DCN("�Ϻ����о��ܻ�е���޹�˾", screenWidth-410, 10, DarkRed, 24);  
    }   
     //���Դ���    
     /*float j = 0;
     glColor3ub(SpringGreen[0], SpringGreen[1], SpringGreen[2]);    //���û��ʵ���ɫ     
     for (int i = 0; i < 5; i++){
        if (mode == GL_SELECT)
           glLoadName(i); //�����ƶ�ջ��װ������ 
        j = j+15;
		glBegin(GL_LINES);   
            glVertex3f(10,  j, 0.0f);//x�ߵ����
            glVertex3f(-10, j, 0.0f);//x�ߵ��յ� 
        glEnd();   
     }*/   
}
//==========================================================================================================================
//==========================================================================================================================
//==========================================================================================================================
//==========================================================================================================================
void DrawGraphics(struct G_M_XYZ **G_M_CODE, int g_v, GLenum mode){                           
        //==========================================================================
        /*glEnable(GL_BLEND);    //���û��       
        glDepthMask(GL_FALSE); //��Ȼ���ֻ�� 
        glPushMatrix();        //ѹ�뵱ǰ��������ջ 
        float polygonSize = 600 / 2;
        //GLfloat color[3] = {0.0f, 0.4f, 0.0f};//����Ϊ���ղ�������
        //glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, color);
        glColor4f(0.3, 0.3, 0.5, 0.0f);//���һ��ֵ��͸����,��ʱ�����ȫ͸��,ֻ�ܿ�������
        glBegin(GL_QUADS);
               glVertex2f(-polygonSize,-polygonSize);
               glVertex2f( polygonSize,-polygonSize);  
        	   glVertex2f( polygonSize, polygonSize);
        	   glVertex2f(-polygonSize, polygonSize);
        glEnd();
        glPopMatrix();        //������ǰ��������ջ 
        glDepthMask(GL_TRUE); //��Ȼ���ɶ�д
        glDisable(GL_BLEND);  //�ػ��*/
        //==========================================================================
                 
        if (KnifePath == true)
             ncShow(G_M_CODE, g_v, mode); //��ʾ����·�� 
             //==============================================================
        /*float VertexA[3] = {0,0,120}; 
        float VertexB[3] = {-120,0,0}; 
        float VertexC[3] = {120,0,0};
        glColor3f(0, 1.0, 0);
        DrawTriangle(VertexA, VertexB, VertexC);*/
        //==============================================================
        if (CoordinateLineBool == true)
             CoordinateLine(800000);
        //DrawGrid(50, 600, 1);
        DrawCoordinate(120, 0.0, 0.0, 0.0);
        //DrawCursor(5, 5, 0, 1);
}


