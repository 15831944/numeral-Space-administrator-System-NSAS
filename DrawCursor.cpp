//#define GLUT_DISABLE_ATEXIT_HACK


#include <windows.h>
#include <GL/glut.h>	             
#include <stdio.h>  
     
/*
 *���ƹ����ɫ�ֱ�ΪRGB
 *���� 1:CursorX Ϊ��������� X �����յ�  
 *���� 2:CursorY Ϊ��������� Y �����յ� 
 *���� 3:CursorZ Ϊ��������� Z �����յ� 
 *���� 4:color   Ϊ���ղ�����ɫ 
 *���� 5:LineWidth Ϊ�ߵĴ�С 
 */
 
float CursoX = 0, CursoY = 0, CursoZ = 0; 
//==============================================================================
//============================================================================== 
//==============================================================================        
void DrawCursor(float CursorX, float CursorY, float CursorZ, float LineWidth){  
    glPushMatrix();//ѹ�뵱ǰ��������ջ  
    GLfloat color[3];
    glTranslated(CursoX, CursoY, 10);//ƽ�Ʊ任   
    //-------------------------------------- 
    GLUquadricObj * glUqadric = gluNewQuadric();  
	color[0] = 1.0f; color[1] = 0.0f; color[2] = 0.0f; //����Ϊ���ղ�������
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, color);
	glRotatef(-180, 1, 0, 0);     //��ת�任
	glutSolidCone(2, 10, 50, 50); //����ʵ��Բ׶��
	glTranslatef(0, 0, -10.0);    //ƽ�Ʊ任
    gluCylinder(glUqadric, 1.0, 1.0, 10.0, 50, 50);    //����Բ����
    gluDeleteQuadric(glUqadric);  //ɾ��glUqadric�����ͷ��ڴ湩��������  
    //--------------------------------------
    /*color[0] = 1.0f; 
    color[1] = .0f; 
    color[2] = .0f; //����Ϊ���ղ�������
    glLineWidth(LineWidth);//�趨�߿�      
    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, color); 
    glColor3f (0.0f, 1.0f, 0.0f);   //�ߵ���ɫ	
    glBegin(GL_LINES);     //��ʼ���Ʒ���              
      glVertex3f((float) CursorX, 0.0f,             0.0f);             //x�ߵ����
      glVertex3f((float)-CursorX, 0,                0.0f);             //x�ߵ��յ�
      glVertex3f(0.0f,            (float)  CursorY, 0.0f);             //y�ߵ����
      glVertex3f(0.0f,            (float) -CursorY, 0.0f);             //y�ߵ��յ�
      glVertex3f(0.0f,            0.0f,             (float)  CursorZ); //z�ߵ����
      glVertex3f(0.0f,            0.0f,             (float) -CursorZ); //z�ߵ��յ�          
    glEnd(); 
    //--------------------------------------*/
    glPopMatrix();//ѹ�뵱ǰ��������ջ 
}
//==============================================================================
//============================================================================== 
//============================================================================== 



















