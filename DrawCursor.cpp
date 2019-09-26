//#define GLUT_DISABLE_ATEXIT_HACK


#include <windows.h>
#include <GL/glut.h>	             
#include <stdio.h>  
     
/*
 *绘制光标颜色分别为RGB
 *参数 1:CursorX 为法线坐标的 X 起点和终点  
 *参数 2:CursorY 为法线坐标的 Y 起点和终点 
 *参数 3:CursorZ 为法线坐标的 Z 起点和终点 
 *参数 4:color   为光照材质颜色 
 *参数 5:LineWidth 为线的大小 
 */
 
float CursoX = 0, CursoY = 0, CursoZ = 0; 
//==============================================================================
//============================================================================== 
//==============================================================================        
void DrawCursor(float CursorX, float CursorY, float CursorZ, float LineWidth){  
    glPushMatrix();//压入当前矩阵至堆栈  
    GLfloat color[3];
    glTranslated(CursoX, CursoY, 10);//平移变换   
    //-------------------------------------- 
    GLUquadricObj * glUqadric = gluNewQuadric();  
	color[0] = 1.0f; color[1] = 0.0f; color[2] = 0.0f; //两行为光照材质设置
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, color);
	glRotatef(-180, 1, 0, 0);     //旋转变换
	glutSolidCone(2, 10, 50, 50); //绘制实心圆锥体
	glTranslatef(0, 0, -10.0);    //平移变换
    gluCylinder(glUqadric, 1.0, 1.0, 10.0, 50, 50);    //绘制圆柱体
    gluDeleteQuadric(glUqadric);  //删除glUqadric对象释放内存供后面重用  
    //--------------------------------------
    /*color[0] = 1.0f; 
    color[1] = .0f; 
    color[2] = .0f; //两行为光照材质设置
    glLineWidth(LineWidth);//设定线宽      
    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, color); 
    glColor3f (0.0f, 1.0f, 0.0f);   //线的颜色	
    glBegin(GL_LINES);     //开始绘制法线              
      glVertex3f((float) CursorX, 0.0f,             0.0f);             //x线的起点
      glVertex3f((float)-CursorX, 0,                0.0f);             //x线的终点
      glVertex3f(0.0f,            (float)  CursorY, 0.0f);             //y线的起点
      glVertex3f(0.0f,            (float) -CursorY, 0.0f);             //y线的终点
      glVertex3f(0.0f,            0.0f,             (float)  CursorZ); //z线的起点
      glVertex3f(0.0f,            0.0f,             (float) -CursorZ); //z线的终点          
    glEnd(); 
    //--------------------------------------*/
    glPopMatrix();//压入当前矩阵至堆栈 
}
//==============================================================================
//============================================================================== 
//============================================================================== 



















