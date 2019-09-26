//#define GLUT_DISABLE_ATEXIT_HACK
#include <windows.h>
#include <GL/freeglut.h>
#include "main.h"
#include "color.h"
/*
 *绘制坐标轴颜色分别为RGB
 *参数 1:CoordinateSize 为坐标大小 
 *参数 2:X 为坐标的 X 点 绿(箭头方向正) 
 *参数 3:Y 为坐标的 Y 点 蓝(箭头方向正)  
 *参数 4:Z 为坐标的 Z 点 红(箭头方向正)  
 */
 
float CoordinateSize = 20, CoordinateX = 0, CoordinateY = 0, CoordinateZ = 50;


void DrawCoordinate(float CoordinateSize, float X, float Y, float Z){
	GLfloat color[] = {0.0f, 0.0f, 0.0f, 1.0f };
	GLUquadricObj * glUqadric = gluNewQuadric();
	glPushMatrix();       //入堆栈
	glTranslatef(X, Y, Z);//平移变换

	glMaterialfv(GL_FRONT, GL_SPECULAR, color);
	glMatrixMode(GL_MODELVIEW);
	//=========================== O
	color[0] = 1.0f; color[1] = 0.5f; color[2] = 0.0f; //两行为光照材质设置
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, color);
	glutSolidSphere(CoordinateSize / 30, 50, 50);      //绘制实心球体
	//=========================== Z
	color[0] = 1.0f; color[1] = 0.0f; color[2] = 0.0f; //两行为光照材质设置
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, color);
	glColor4fv(color);          // set text color
	gluCylinder(glUqadric, CoordinateSize/50.0, CoordinateSize/50.0, CoordinateSize, 50, 50);//绘制圆柱体
	glTranslatef(0, 0, CoordinateSize);//平移变换
	glutSolidCone(CoordinateSize / 25, CoordinateSize / 7, 50, 50);//绘制实心圆锥体
	float pos[] = {0,10,0};
	drawString3D("Z+", pos, color, GLUT_BITMAP_TIMES_ROMAN_24);//绘制英文字符 
	//=========================== X
	color[0] = .0f; color[1] = 1.0f; color[2] = .0f;//两行为光照材质设置
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, color);
	glColor4fv(color);          // set text color
	glTranslatef(0, 0, -CoordinateSize);//平移变换
	glRotatef(90, 0, 1, 0);//旋转变换
	gluCylinder(glUqadric, CoordinateSize / 50.0, CoordinateSize / 50.0, CoordinateSize, 50, 50);//绘制圆柱体
	glTranslatef(0, 0, CoordinateSize);//平移变换
	glutSolidCone(CoordinateSize / 25, CoordinateSize / 7, 50, 50);//绘制实心圆锥体
	pos[0] = 0;pos[1] = 10;pos[2] = 0;
	drawString3D("X+", pos, color, GLUT_BITMAP_TIMES_ROMAN_24);//绘制英文字符 
	//=========================== Y
	color[0] = .0f; color[1] = .0f; color[2] = 1.0f;//两行为光照材质设置
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, color);
	glColor4fv(color);          // set text color
	glTranslatef(0, 0, -CoordinateSize);//平移变换
	glRotatef(-90, 1, 0, 0);//旋转变换
	gluCylinder(glUqadric, CoordinateSize / 50.0, CoordinateSize / 50.0, CoordinateSize, 50, 50);//绘制圆柱体
	glTranslatef(0, 0, CoordinateSize);//平移变换
	glutSolidCone(CoordinateSize / 25, CoordinateSize / 7, 50, 50);//绘制实心圆锥体
	gluDeleteQuadric(glUqadric);  //删除glUqadric对象释放内存供后面重用
	pos[0] = 0;pos[1] = 10;pos[2] = 0;
	drawString3D("Y+", pos, color, GLUT_BITMAP_TIMES_ROMAN_24);//绘制英文字符     
	glPopMatrix();//出堆栈
	//===========================
}
