#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#ifdef _MSC_VER
/* DUMP MEMORY LEAKS */
#include <crtdbg.h>
#endif
//#include <GL/glut.h>
#include <GL/freeglut.h>
#include "main.h"

/*
    GLUT_BITMAP_8_BY_13 一种固定宽度字体,每个字符都放在一个8x13像素的矩形框内 
    GLUT_BITMAP_9_BY_15 一种固定宽度字体,每个字符都放在一个9x15像素的矩形框内 
    GLUT_BITMAP_TIMES_ROMAN_10  一种10点均匀间距的Times Roman字体 
    GLUT_BITMAP_TIMES_ROMAN_24  一种24点均匀间距的Times Roman字体 
    GLUT_BITMAP_HELVETICA_10    一种10点均匀间距的Helvetica字体 
    GLUT_BITMAP_HELVETICA_12    一种12点均匀间距的Helvetica字体 
    GLUT_BITMAP_HELVETICA_18    一种18点均匀间距的Helvetica字体
 */

int screenWidth, screenHeight;
//==========================================================================================================================
//==========================================================================================================================
//==========================================================================================================================
//==========================================================================================================================
void drawString(const char *str, int x, int y, float color[4], void *font){
    glPushAttrib(GL_LIGHTING_BIT | GL_CURRENT_BIT); // lighting and color mask
    glDisable(GL_LIGHTING);     // need to disable lighting for proper text color
    glDisable(GL_TEXTURE_2D);
    glColor4fv(color);          // set text color
    glRasterPos2i(x, y);        // place text position
    // loop all characters in the string
    while(*str){
        if (*str == 10){
           break;
        }
        
        glutBitmapCharacter(font, *str);
        ++str;
    }

    glEnable(GL_TEXTURE_2D);
    glEnable(GL_LIGHTING);
    glPopAttrib();
}
//==========================================================================================================================
//==========================================================================================================================
//==========================================================================================================================
//==========================================================================================================================
void drawString3D(const char *str, float pos[3], float color[4], void *font){
    glPushAttrib(GL_LIGHTING_BIT | GL_CURRENT_BIT); // lighting and color mask
    glDisable(GL_LIGHTING);     // need to disable lighting for proper text color
    glDisable(GL_TEXTURE_2D);
    glColor4fv(color);          // set text color
    glRasterPos3fv(pos);        // place text position
    // loop all characters in the string
    while(*str){      
        glutBitmapCharacter(font, *str);
        ++str;      
    }
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_LIGHTING);
    glPopAttrib();
}

//==========================================================================================================================
//==========================================================================================================================
//==========================================================================================================================
//==========================================================================================================================
void drawString2D(char *ch, int x, int y, float color[4], void *font){
    // backup current model-view matrix
    glPushMatrix();                     // save current modelview matrix
    glLoadIdentity();                   // reset modelview matrix

    // set to 2D orthogonal projection
    glMatrixMode(GL_PROJECTION);     // switch to projection matrix
    glPushMatrix();                  // save current projection matrix
    glLoadIdentity();                // reset projection matrix
    gluOrtho2D(0, screenWidth, 0, screenHeight);  // set to orthogonal projection
    
    drawString(ch, x, y, color, font);
    
    // restore projection matrix
    glPopMatrix();                   // restore to previous projection matrix

    // restore modelview matrix
    glMatrixMode(GL_MODELVIEW);      // switch to modelview matrix
    glPopMatrix();                   // restore to previous modelview matrix
}

//==========================================================================================================================
//==========================================================================================================================
//==========================================================================================================================
//==========================================================================================================================
void drawString2DCN(char *ch, int x, int y, int color[3], int sizeFont){
    // backup current model-view matrix
    glPushMatrix();                     // save current modelview matrix
    glLoadIdentity();                   // reset modelview matrix

    // set to 2D orthogonal projection
    glMatrixMode(GL_PROJECTION);     // switch to projection matrix
    glPushMatrix();                  // save current projection matrix
    glLoadIdentity();                // reset projection matrix
    gluOrtho2D(0, screenWidth, 0, screenHeight);  // set to orthogonal projection
    
    selectFont(sizeFont, DEFAULT_CHARSET, "华文仿宋");
    glColor3ub(color[0],color[1],color[2]);    //设置画笔的颜色
    glRasterPos2f(x, y);
    drawCNString(ch);
    
    // restore projection matrix
    glPopMatrix();                   // restore to previous projection matrix

    // restore modelview matrix
    glMatrixMode(GL_MODELVIEW);      // switch to modelview matrix
    glPopMatrix();                   // restore to previous modelview matrix
}
//==========================================================================================================================
//==========================================================================================================================
//==========================================================================================================================
//==========================================================================================================================
void showString(const char *str, int x, int y, float color[4], void *font){
    // backup current model-view matrix
    glPushMatrix();                     // save current modelview matrix
    glLoadIdentity();                   // reset modelview matrix

    // set to 2D orthogonal projection
    glMatrixMode(GL_PROJECTION);     // switch to projection matrix
    glPushMatrix();                  // save current projection matrix
    glLoadIdentity();                // reset projection matrix
    gluOrtho2D(0, screenWidth, 0, screenHeight);  // set to orthogonal projection
    
    drawString(str, x, y, color, font);
    
    // restore projection matrix
    glPopMatrix();                   // restore to previous projection matrix

    // restore modelview matrix
    glMatrixMode(GL_MODELVIEW);      // switch to modelview matrix
    glPopMatrix();                   // restore to previous modelview matrix
}
//==========================================================================================================================
//==========================================================================================================================
//==========================================================================================================================
//==========================================================================================================================
/*
  x0:
  x1:
  y0:
  y0:
  z0:
  z1:
  color[3]:
  LineWidth:
 */
void showLine(float x0, float x1, float y0, float y1, float z0, float z1, int color[3], float LineWidth){
    glPushMatrix();                     // save current modelview matrix
    glLoadIdentity();                   // reset modelview matrix
    
    glMatrixMode(GL_PROJECTION);     // switch to projection matrix
    glPushMatrix();                  // save current projection matrix
    glLoadIdentity();                // reset projection matrix
    gluOrtho2D(0, screenWidth, 0, screenHeight);  // set to orthogonal projection
    
    DrawLine(x0, y0, z0, LineWidth, color, x1, y1, z1);
    glPopMatrix();                   // restore to previous projection matrix
    
    glMatrixMode(GL_MODELVIEW);      // switch to modelview matrix
    glPopMatrix();                   // restore to previous modelview matrix
}
//==========================================================================================================================
//==========================================================================================================================
//==========================================================================================================================
//==========================================================================================================================
void showPolygon(float VertexX0,float VertexY0,float VertexX1,float VertexY1,float VertexX2,float VertexY2,float VertexX3,float VertexY3,int Color[3]){
    glPushMatrix();                     // save current modelview matrix
    glLoadIdentity();                   // reset modelview matrix
    
    glMatrixMode(GL_PROJECTION);        // switch to projection matrix
    glPushMatrix();                     // save current projection matrix
    glLoadIdentity();                   // reset projection matrix
    gluOrtho2D(0, screenWidth, 0, screenHeight);  // set to orthogonal projection  
    DrawPolygon(VertexX0,VertexY0, VertexX1,VertexY1, VertexX2,VertexY2, VertexX3,VertexY3,Color);
    
    glPopMatrix();                      // restore to previous projection matrix
    
    glMatrixMode(GL_MODELVIEW);         // switch to modelview matrix
    glPopMatrix();                      // restore to previous modelview matrix
}
//==========================================================================================================================
//==========================================================================================================================
//==========================================================================================================================
//==========================================================================================================================
//中文字体显示 
void drawCNString(const char* str) {
    int len, i;
    wchar_t* wstring;
    HDC hDC     = wglGetCurrentDC();
    GLuint list = glGenLists(1);

    // 计算字符的个数
    // 如果是双字节字符的（比如中文字符），两个字节才算一个字符
    // 否则一个字节算一个字符
    len = 0;
    for(i = 0; str[i] != '\0'; ++i){
        if(IsDBCSLeadByte(str[i]))
            ++i;
        ++len;
    }

    //将混合字符转化为宽字符
    wstring = (wchar_t*)malloc((len + 1) * sizeof(wchar_t));
    MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, str, -1, wstring, len);
    wstring[len] = L'\0';

    //逐个输出字符
    for(i = 0; i < len; ++i){
        wglUseFontBitmapsW(hDC, wstring[i], 1, list);
        glCallList(list);
    }

    //回收所有临时资源
    free(wstring);
    glDeleteLists(list, 1);
}
//==========================================================================================================================
//==========================================================================================================================
//==========================================================================================================================
//==========================================================================================================================
/*字体设置 
  size:字体大小
  charset:字体编码 
  face:字库名       
 */ 
void selectFont(int size, int charset, const char* face) {
    HFONT hFont = CreateFontA(size,    0, 0, 0, FW_MEDIUM, 0, 0, 0,
                              charset, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
                              DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, face);
    HFONT hOldFont = (HFONT)SelectObject(wglGetCurrentDC(), hFont);
    DeleteObject(hOldFont);
}



