//#define GLUT_DISABLE_ATEXIT_HACK

#include <windows.h>
#include <GL/glut.h>
//========================================================================================
/*
 *参数 1: GridClearance 网格间隙 
 *参数 2: GridSize 网格区矩形大小 
 *参数 3: LineWidth 网格线的宽度 
 *参数 4: color 网格线的颜色
 */ 
void DrawGrid(float GridClearance, float GridSize, float LineWidth){
     GLfloat color[3];
     unsigned int Len = ((int)GridSize / (int)GridClearance) + 1;   //得到可以画多少条线 
     float lenXY = (float)GridSize / 2;
     //=====================X
     glPushMatrix();       //入堆栈
     color[0] = 1.0f; color[1] = 0.5f; color[2] = .0f;//两行为光照材质设置
     glLineWidth(LineWidth); //设置网格线宽度 
     glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, color);
     //=====================
     glTranslatef(0, lenXY, 0);//平移变换  
     //glutWireTorus(0.2 ,0.8, 12, 24); 
     glBegin(GL_LINES);   
       glVertex3f(lenXY,  0.0f, 0.0f);//x线的起点
       glVertex3f(-lenXY, 0.0f, 0.0f);//x线的终点 
     glEnd();
     glTranslatef(0, -GridSize, 0);//平移变换   
     //glutWireTorus(0.2 ,0.8, 12, 24); 
     for (int i = 0; i < Len; i++){//绘制平行于 X 的直线         
        glBegin(GL_LINES);//表明绘制线 
	       glVertex3f(lenXY,  0.0f, 0.0f);//x线的起点
           glVertex3f(-lenXY, 0.0f, 0.0f);//x线的终点
        glEnd(); 
        glTranslatef(0, GridClearance, 0);//平移变换,不在 glBegin() 与  glEnd()之间                
     }                    
     glPopMatrix();       //出堆栈   
     //=====================Y
     glPushMatrix();       //入堆栈
     glTranslatef(lenXY, 0.0f, 0);//平移变换  
     //glutWireTorus(0.2 ,0.8, 12, 24); 
     glBegin(GL_LINES);   
       glVertex3f(0.0f, lenXY,   0.0f);//x线的起点
       glVertex3f(0.0f, -lenXY,  0.0f);//x线的终点 
     glEnd();
     glTranslatef(-GridSize, 0, 0);//平移变换   
     //glutWireTorus(0.2 ,0.8, 12, 24); 
     for (int i = 0; i < Len; i++){//绘制平行于 X 的直线            
        glBegin(GL_LINES);//表明绘制线 
	       glVertex3f(0.0f,  lenXY, 0.0f);//x线的起点
           glVertex3f(0.0f, -lenXY, 0.0f);//x线的终点
        glEnd();  
        glTranslatef(GridClearance, 0.0f, 0);//平移变换,不能在 glBegin() 与  glEnd()之间        
     }                    
     glPopMatrix();       //出堆栈  
}
//========================================================================================
/*
 *1:GridClearance 网格间隙
 *2:GridSize 网格区矩形大小 
 *3:PointSize 点大小 
 *4:color  网格线的颜色
 */
void DrawPiontGrid(float GridClearance, float GridSize, float PointSize, GLfloat * color){
     float X, Y, Z = 0.0;
     glPushMatrix();       //入堆栈
     color[0] = 1.0f; color[1] = 1.0f; color[2] = 1.0f;//两行为光照材质设置
     glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, color);//设置光照材质 
     glPointSize(PointSize); //设置点大小 
     glTranslatef(-(GridSize / 2), -(GridSize / 2), 0);//平移变换 
     //=====================
     for (X = 0; X < GridSize; X += GridClearance){
         for (Y = 0; Y < GridSize; Y += GridClearance){
           glBegin(GL_POINTS);   
             glVertex3f(X, Y, Z);//在指定的 X, Y, Z位置绘点
           glEnd(); 
         }
     }                 
     glPopMatrix();       //出堆栈 
}
//========================================================================================

