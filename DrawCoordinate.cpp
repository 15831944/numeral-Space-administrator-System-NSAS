//#define GLUT_DISABLE_ATEXIT_HACK
#include <windows.h>
#include <GL/freeglut.h>
#include "main.h"
#include "color.h"
/*
 *������������ɫ�ֱ�ΪRGB
 *���� 1:CoordinateSize Ϊ�����С 
 *���� 2:X Ϊ����� X �� ��(��ͷ������) 
 *���� 3:Y Ϊ����� Y �� ��(��ͷ������)  
 *���� 4:Z Ϊ����� Z �� ��(��ͷ������)  
 */
 
float CoordinateSize = 20, CoordinateX = 0, CoordinateY = 0, CoordinateZ = 50;


void DrawCoordinate(float CoordinateSize, float X, float Y, float Z){
	GLfloat color[] = {0.0f, 0.0f, 0.0f, 1.0f };
	GLUquadricObj * glUqadric = gluNewQuadric();
	glPushMatrix();       //���ջ
	glTranslatef(X, Y, Z);//ƽ�Ʊ任

	glMaterialfv(GL_FRONT, GL_SPECULAR, color);
	glMatrixMode(GL_MODELVIEW);
	//=========================== O
	color[0] = 1.0f; color[1] = 0.5f; color[2] = 0.0f; //����Ϊ���ղ�������
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, color);
	glutSolidSphere(CoordinateSize / 30, 50, 50);      //����ʵ������
	//=========================== Z
	color[0] = 1.0f; color[1] = 0.0f; color[2] = 0.0f; //����Ϊ���ղ�������
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, color);
	glColor4fv(color);          // set text color
	gluCylinder(glUqadric, CoordinateSize/50.0, CoordinateSize/50.0, CoordinateSize, 50, 50);//����Բ����
	glTranslatef(0, 0, CoordinateSize);//ƽ�Ʊ任
	glutSolidCone(CoordinateSize / 25, CoordinateSize / 7, 50, 50);//����ʵ��Բ׶��
	float pos[] = {0,10,0};
	drawString3D("Z+", pos, color, GLUT_BITMAP_TIMES_ROMAN_24);//����Ӣ���ַ� 
	//=========================== X
	color[0] = .0f; color[1] = 1.0f; color[2] = .0f;//����Ϊ���ղ�������
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, color);
	glColor4fv(color);          // set text color
	glTranslatef(0, 0, -CoordinateSize);//ƽ�Ʊ任
	glRotatef(90, 0, 1, 0);//��ת�任
	gluCylinder(glUqadric, CoordinateSize / 50.0, CoordinateSize / 50.0, CoordinateSize, 50, 50);//����Բ����
	glTranslatef(0, 0, CoordinateSize);//ƽ�Ʊ任
	glutSolidCone(CoordinateSize / 25, CoordinateSize / 7, 50, 50);//����ʵ��Բ׶��
	pos[0] = 0;pos[1] = 10;pos[2] = 0;
	drawString3D("X+", pos, color, GLUT_BITMAP_TIMES_ROMAN_24);//����Ӣ���ַ� 
	//=========================== Y
	color[0] = .0f; color[1] = .0f; color[2] = 1.0f;//����Ϊ���ղ�������
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, color);
	glColor4fv(color);          // set text color
	glTranslatef(0, 0, -CoordinateSize);//ƽ�Ʊ任
	glRotatef(-90, 1, 0, 0);//��ת�任
	gluCylinder(glUqadric, CoordinateSize / 50.0, CoordinateSize / 50.0, CoordinateSize, 50, 50);//����Բ����
	glTranslatef(0, 0, CoordinateSize);//ƽ�Ʊ任
	glutSolidCone(CoordinateSize / 25, CoordinateSize / 7, 50, 50);//����ʵ��Բ׶��
	gluDeleteQuadric(glUqadric);  //ɾ��glUqadric�����ͷ��ڴ湩��������
	pos[0] = 0;pos[1] = 10;pos[2] = 0;
	drawString3D("Y+", pos, color, GLUT_BITMAP_TIMES_ROMAN_24);//����Ӣ���ַ�     
	glPopMatrix();//����ջ
	//===========================
}
