#include <GL/freeglut.h>
#include <math.h>
#include "main.h"
#include "color.h"
/*控件按钮 
  2                 1
  ******************
  *              *
  *            *
  ************
  3          4
 */
void button(int x, int y, int sizeX, int sizeY, bool ab, char *ch){   
    int i = 2, j = 1;   
    glPushMatrix();                     //入堆栈 
    glLoadIdentity();                   //转换为单位矩阵 
    
    glMatrixMode(GL_PROJECTION);        //指示对投影矩阵堆栈应用随后的矩阵操作 
    glPushMatrix();                     //入堆栈 
    glLoadIdentity();                   //
    gluOrtho2D(0, screenWidth, 0, screenHeight);  //设置2D裁剪  
    
    if (ab){
        DrawPolygon(x,y,         x-sizeX,y,           x-sizeX,y-sizeY,   x,y-sizeY,           LightGrey);
        DrawPolygon(x,y,         x-i,y-i,             x-i,y-sizeY,       x,y-sizeY,           Black);
        DrawPolygon(x,y-sizeY+i, x-sizeX+j,y-sizeY+i, x-sizeX,y-sizeY,   x,y-sizeY,           Black);
        DrawPolygon(x,y,         x-sizeX,y,           x-sizeX,y-j,       x-j,y-j,             White);
        DrawPolygon(x-sizeX+j,y, x-sizeX,y,           x-sizeX,y-sizeY,   x-sizeX+j,y-sizeY+j, White);
    }else{
        DrawPolygon(x,y,         x-sizeX,y,           x-sizeX,y-sizeY,   x,y-sizeY,           Red);
        DrawPolygon(x,y,         x-i,y-i,             x-i,y-sizeY,       x,y-sizeY,           White);
        DrawPolygon(x,y-sizeY+i, x-sizeX+j,y-sizeY+i, x-sizeX,y-sizeY,   x,y-sizeY,           White);
        DrawPolygon(x,y,         x-sizeX,y,           x-sizeX,y-j,       x-j,y-j,             Black);
        DrawPolygon(x-sizeX+j,y, x-sizeX,y,           x-sizeX,y-sizeY,   x-sizeX+j,y-sizeY+j, Black);
    }
    selectFont(15, DEFAULT_CHARSET, "华文仿宋");
    glColor3ub(Black[0],Black[1],Black[2]);    //设置画笔的颜色
    glRasterPos2f(x-100, y-23);
    drawCNString(ch);  
      
    glPopMatrix();              //出栈   
    glMatrixMode(GL_MODELVIEW); //
    glPopMatrix();              //出栈 
}

//垂直或水平的按钮阵列 
void HorizontalVerticalArrayButton(){
}
