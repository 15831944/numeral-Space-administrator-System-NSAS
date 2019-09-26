#include <GL/freeglut.h>
#include <math.h>
#include "main.h"
#include "color.h"
/*�ؼ���ť 
  2                 1
  ******************
  *              *
  *            *
  ************
  3          4
 */
void button(int x, int y, int sizeX, int sizeY, bool ab, char *ch){   
    int i = 2, j = 1;   
    glPushMatrix();                     //���ջ 
    glLoadIdentity();                   //ת��Ϊ��λ���� 
    
    glMatrixMode(GL_PROJECTION);        //ָʾ��ͶӰ�����ջӦ�����ľ������ 
    glPushMatrix();                     //���ջ 
    glLoadIdentity();                   //
    gluOrtho2D(0, screenWidth, 0, screenHeight);  //����2D�ü�  
    
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
    selectFont(15, DEFAULT_CHARSET, "���ķ���");
    glColor3ub(Black[0],Black[1],Black[2]);    //���û��ʵ���ɫ
    glRasterPos2f(x-100, y-23);
    drawCNString(ch);  
      
    glPopMatrix();              //��ջ   
    glMatrixMode(GL_MODELVIEW); //
    glPopMatrix();              //��ջ 
}

//��ֱ��ˮƽ�İ�ť���� 
void HorizontalVerticalArrayButton(){
}
