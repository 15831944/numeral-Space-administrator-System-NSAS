//#define GLUT_DISABLE_ATEXIT_HACK

#include <windows.h>
#include <GL/glut.h>
//========================================================================================
/*
 *���� 1: GridClearance �����϶ 
 *���� 2: GridSize ���������δ�С 
 *���� 3: LineWidth �����ߵĿ�� 
 *���� 4: color �����ߵ���ɫ
 */ 
void DrawGrid(float GridClearance, float GridSize, float LineWidth){
     GLfloat color[3];
     unsigned int Len = ((int)GridSize / (int)GridClearance) + 1;   //�õ����Ի��������� 
     float lenXY = (float)GridSize / 2;
     //=====================X
     glPushMatrix();       //���ջ
     color[0] = 1.0f; color[1] = 0.5f; color[2] = .0f;//����Ϊ���ղ�������
     glLineWidth(LineWidth); //���������߿�� 
     glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, color);
     //=====================
     glTranslatef(0, lenXY, 0);//ƽ�Ʊ任  
     //glutWireTorus(0.2 ,0.8, 12, 24); 
     glBegin(GL_LINES);   
       glVertex3f(lenXY,  0.0f, 0.0f);//x�ߵ����
       glVertex3f(-lenXY, 0.0f, 0.0f);//x�ߵ��յ� 
     glEnd();
     glTranslatef(0, -GridSize, 0);//ƽ�Ʊ任   
     //glutWireTorus(0.2 ,0.8, 12, 24); 
     for (int i = 0; i < Len; i++){//����ƽ���� X ��ֱ��         
        glBegin(GL_LINES);//���������� 
	       glVertex3f(lenXY,  0.0f, 0.0f);//x�ߵ����
           glVertex3f(-lenXY, 0.0f, 0.0f);//x�ߵ��յ�
        glEnd(); 
        glTranslatef(0, GridClearance, 0);//ƽ�Ʊ任,���� glBegin() ��  glEnd()֮��                
     }                    
     glPopMatrix();       //����ջ   
     //=====================Y
     glPushMatrix();       //���ջ
     glTranslatef(lenXY, 0.0f, 0);//ƽ�Ʊ任  
     //glutWireTorus(0.2 ,0.8, 12, 24); 
     glBegin(GL_LINES);   
       glVertex3f(0.0f, lenXY,   0.0f);//x�ߵ����
       glVertex3f(0.0f, -lenXY,  0.0f);//x�ߵ��յ� 
     glEnd();
     glTranslatef(-GridSize, 0, 0);//ƽ�Ʊ任   
     //glutWireTorus(0.2 ,0.8, 12, 24); 
     for (int i = 0; i < Len; i++){//����ƽ���� X ��ֱ��            
        glBegin(GL_LINES);//���������� 
	       glVertex3f(0.0f,  lenXY, 0.0f);//x�ߵ����
           glVertex3f(0.0f, -lenXY, 0.0f);//x�ߵ��յ�
        glEnd();  
        glTranslatef(GridClearance, 0.0f, 0);//ƽ�Ʊ任,������ glBegin() ��  glEnd()֮��        
     }                    
     glPopMatrix();       //����ջ  
}
//========================================================================================
/*
 *1:GridClearance �����϶
 *2:GridSize ���������δ�С 
 *3:PointSize ���С 
 *4:color  �����ߵ���ɫ
 */
void DrawPiontGrid(float GridClearance, float GridSize, float PointSize, GLfloat * color){
     float X, Y, Z = 0.0;
     glPushMatrix();       //���ջ
     color[0] = 1.0f; color[1] = 1.0f; color[2] = 1.0f;//����Ϊ���ղ�������
     glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, color);//���ù��ղ��� 
     glPointSize(PointSize); //���õ��С 
     glTranslatef(-(GridSize / 2), -(GridSize / 2), 0);//ƽ�Ʊ任 
     //=====================
     for (X = 0; X < GridSize; X += GridClearance){
         for (Y = 0; Y < GridSize; Y += GridClearance){
           glBegin(GL_POINTS);   
             glVertex3f(X, Y, Z);//��ָ���� X, Y, Zλ�û��
           glEnd(); 
         }
     }                 
     glPopMatrix();       //����ջ 
}
//========================================================================================

