#include <stdio.h>
#include <GL/freeglut.h>
#include <Windows.h>
#include "NcFileInterpreter.h"
#include "main.h"
#include "color.h"

volatile int r = 0; 
	
void ncFileTest(int X, int Y){
    char dd[29];

    glPushMatrix();                     // save current modelview matrix
    glLoadIdentity();                   // reset modelview matrix

    glMatrixMode(GL_PROJECTION);     // switch to projection matrix
    glPushMatrix();                  // save current projection matrix
    glLoadIdentity();                // reset projection matrix
    gluOrtho2D(0, screenWidth, 0, screenHeight);  // set to orthogonal projection
    
    selectFont(20, DEFAULT_CHARSET, "华文仿宋");
    glColor3ub(OrangeRed[0],OrangeRed[1],OrangeRed[2]);    //设置画笔的颜色    
    //--------------------------------------------------------------------------
    if (G_NC_Code == true){
        if (G_M_CODE[r]->G00 == true){
              glRasterPos2f(X-300, Y-20);
              drawCNString("G00");
        }
        if (G_M_CODE[r]->G01 == true){
              glRasterPos2f(X-265, Y-20);
              drawCNString("G01");               
        }
        if (G_M_CODE[r]->G02 == true){
              glRasterPos2f(X-230, Y-20);
              drawCNString("G02");               
        }
        if (G_M_CODE[r]->G03 == true){
              glRasterPos2f(X-195, Y-20);
              drawCNString("G03");               
        } 
        gcvt(G_M_CODE[r]->X_End,7,dd);
        glRasterPos2f(X-300, Y-45);
        drawCNString("X:");
        glRasterPos2f(X-280, Y-45);
        drawCNString(dd);
        gcvt(G_M_CODE[r]->Y_End,7,dd);
        glRasterPos2f(X-300, Y-70);
        drawCNString("Y:");
        glRasterPos2f(X-280, Y-70);
        drawCNString(dd);
        gcvt(G_M_CODE[r]->Z_End,7,dd);
        glRasterPos2f(X-300, Y-95);
        drawCNString("Z:");
        glRasterPos2f(X-280, Y-95);
        drawCNString(dd);
        gcvt(G_M_CODE[r]->A_End,7,dd);
        glRasterPos2f(X-300, Y-120);
        drawCNString("A:");
        glRasterPos2f(X-280, Y-120);
        drawCNString(dd);
        gcvt(G_M_CODE[r]->I,7,dd);
        glRasterPos2f(X-300, Y-145);
        drawCNString("I:");
        glRasterPos2f(X-280, Y-145);
        drawCNString(dd);
        gcvt(G_M_CODE[r]->J,7,dd);
        glRasterPos2f(X-300, Y-170);
        drawCNString("J:");
        glRasterPos2f(X-280, Y-170);
        drawCNString(dd);
        gcvt(G_M_CODE[r]->F,7,dd);
        glRasterPos2f(X-300, Y-195);
        drawCNString("F:");
        glRasterPos2f(X-280, Y-195);
        drawCNString(dd);
        itoa(G_M_CODE[r]->H,dd,7);
        glRasterPos2f(X-300, Y-220);
        drawCNString("H:");
        glRasterPos2f(X-280, Y-220);
        drawCNString(dd);
        itoa(G_M_CODE[r]->T,dd,7);
        glRasterPos2f(X-300, Y-245);
        drawCNString("T:");
        glRasterPos2f(X-280, Y-245);
        drawCNString(dd);
        gcvt(G_M_CODE[r]->S,7,dd);
        glRasterPos2f(X-300, Y-270);
        drawCNString("S:");
        glRasterPos2f(X-280, Y-270);
        drawCNString(dd);
        if (G_M_CODE[r]->NullLine == false){
            glRasterPos2f(X-300, Y-295);
            drawCNString("无效空行");
        }else {
            glRasterPos2f(X-300, Y-295);
            drawCNString("有效行");  
        } 
        itoa(G_M_CODE[r]->N,dd,10);
        glRasterPos2f(X-300, Y-320);
        drawCNString("圆度值:");
        glRasterPos2f(X-230, Y-320);
        drawCNString(dd);  
        gcvt(G_M_CODE[r]->R,7,dd);
        glRasterPos2f(X-300, Y-345);
        drawCNString("圆半径:");
        glRasterPos2f(X-230, Y-345);
        drawCNString(dd); 
        
        gcvt(G_M_CODE[r]->CircularHeartX,7,dd);
        glRasterPos2f(X-300, Y-370);
        drawCNString("圆心X:");
        glRasterPos2f(X-230, Y-370);
        drawCNString(dd);  
          
        gcvt(G_M_CODE[r]->CircularHeartY,7,dd);
        glRasterPos2f(X-300, Y-395);
        drawCNString("圆心Y:");
        glRasterPos2f(X-230, Y-395);
        drawCNString(dd); 
        
        gcvt(G_M_CODE[r]->R_Start[0],7,dd);
        glRasterPos2f(X-300, Y-420);
        drawCNString("圆起点X:");
        glRasterPos2f(X-210, Y-420);
        drawCNString(dd);
    
        gcvt(G_M_CODE[r]->R_Start[1],7,dd);        
        glRasterPos2f(X-300, Y-445);
        drawCNString("圆起点Y:");
        glRasterPos2f(X-210, Y-445);
        drawCNString(dd);
        
        gcvt(G_M_CODE[r]->R_End[0],7,dd);      
        glRasterPos2f(X-300, Y-470);
        drawCNString("圆终点X:");
        glRasterPos2f(X-210, Y-470);
        drawCNString(dd);
               
        gcvt(G_M_CODE[r]->R_End[1],7,dd);       
        glRasterPos2f(X-300, Y-495);
        drawCNString("圆终点Y:");
        glRasterPos2f(X-210, Y-495);
        drawCNString(dd);
        
        itoa(G_M_CODE[r]->VertexNumber,dd,10); 
        glRasterPos2f(X-300, Y-520);
        drawCNString("顶点数:");
        glRasterPos2f(X-230, Y-520);
        drawCNString(dd);        
        int j = 40;
        for (int h = 0; h < G_M_CODE[r]->VertexNumber; h++){
           gcvt(G_M_CODE[r]->ArcVertexCoordinateX[h],7,dd);
           glRasterPos2f(X-500, Y-j);
           drawCNString("顶点X:");
           glRasterPos2f(X-440, Y-j);
           drawCNString(dd);           
           j += 20; 
        } 
        j += 10;
        for (int i = 0; i < G_M_CODE[r]->VertexNumber; i++){
           gcvt(G_M_CODE[r]->ArcVertexCoordinateY[i],7,dd);
           glRasterPos2f(X-500, Y-j);
           drawCNString("顶点Y:");
           glRasterPos2f(X-440, Y-j);
           drawCNString(dd);
           j += 20; 
        }
        itoa(r,dd,10); 
        glRasterPos2f(X-110, 60);
        drawCNString(dd);
    }    
    //--------------------------------------------------------------------------
    glPopMatrix();                    

    glMatrixMode(GL_MODELVIEW);       
    glPopMatrix();                   
}	
	
