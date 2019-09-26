#include <GL/freeglut.h>
#include <stdio.h>
#include <stdlib.h>
#include "main.h"
#include "NcFileInterpreter.h"
#include "color.h"




void ncShow(struct G_M_XYZ **G_M_CODE, int g_v, GLenum mode){ 
     char ss[25];  
     for (int i = 0; i < g_v; i++){
         if ((mode == GL_SELECT))
             glLoadName(i); //向名称堆栈中装载名称 
         if (G_M_CODE[i] -> NullLine != false){               
              if (G_M_CODE[i] -> G00 == true){
                   if (G_M_CODE[i] ->Selection == true){                                         
                      DrawLine(G_M_CODE[i] ->X_Start, G_M_CODE[i] ->Y_Start, G_M_CODE[i] ->Z_Start, 1, DarkMagenta, G_M_CODE[i] ->X_End, G_M_CODE[i] ->Y_End, G_M_CODE[i] ->Z_End);
                   }else{  
                      DrawLine(G_M_CODE[i] ->X_Start, G_M_CODE[i] ->Y_Start, G_M_CODE[i] ->Z_Start, 1, White, G_M_CODE[i] ->X_End, G_M_CODE[i] ->Y_End, G_M_CODE[i] ->Z_End);    
                   }
              }      
              if (G_M_CODE[i] -> G01 == true){  
                    if (mode != GL_SELECT){          
                        if (G_M_CODE[i] -> F != 0.0){
                           float color[4] = {1,1,1,1};
                           float pos[3] = {G_M_CODE[i] ->X_End, G_M_CODE[i] ->Y_End+2, G_M_CODE[i-1] ->Z_End};
                           drawString3D("F200.", pos, color, GLUT_BITMAP_8_BY_13);          
                           drawString3D(ss, pos, color, GLUT_BITMAP_8_BY_13);
                        }
                    } 
                    if (G_M_CODE[i] ->Selection == true){                                
                          DrawLine(G_M_CODE[i] ->X_Start, G_M_CODE[i] ->Y_Start, G_M_CODE[i] ->Z_Start, 1, DarkMagenta, G_M_CODE[i] ->X_End, G_M_CODE[i] ->Y_End, G_M_CODE[i] ->Z_End);
                    }else{
                          DrawLine(G_M_CODE[i] ->X_Start, G_M_CODE[i] ->Y_Start, G_M_CODE[i] ->Z_Start, 1, Green, G_M_CODE[i] ->X_End, G_M_CODE[i] ->Y_End, G_M_CODE[i] ->Z_End);
                    }
              }
              if ((G_M_CODE[i] -> G02 == true) || (G_M_CODE[i] -> G03 == true)){
                    if (mode != GL_SELECT){            
                        if (G_M_CODE[i] -> F != 0.0){
                           float color[4] = {1,1,1,1};
                           float pos[3] = {G_M_CODE[i-1] ->X_End, G_M_CODE[i-1] ->Y_End+2, G_M_CODE[i-1] ->Z_End};
                           drawString3D("F125.", pos, color, GLUT_BITMAP_8_BY_13);          
                           drawString3D(ss, pos, color, GLUT_BITMAP_8_BY_13);
                        }
                    } 
                    if (G_M_CODE[i] ->Selection == true)            
                         DrawArc(G_M_CODE[i] ->ArcVertexCoordinateX, G_M_CODE[i] ->ArcVertexCoordinateY, G_M_CODE[i] ->VertexNumber, G_M_CODE[i] ->Z_End, DarkMagenta);  
                    else
                         DrawArc(G_M_CODE[i] ->ArcVertexCoordinateX, G_M_CODE[i] ->ArcVertexCoordinateY, G_M_CODE[i] ->VertexNumber, G_M_CODE[i] ->Z_End, Green);         
              }
         }else{}
     }     
}
/*
void PaintScreen(GLenum mode){
     float j = 0;
     glPushMatrix();//压入当前矩阵至堆栈 
     for (int i = 0; i < 5; i++){
        if (mode == GL_SELECT)
           glLoadName(i); //向名称堆栈中装载名称 
        j = j+15;
		glBegin(GL_LINES);   
            glVertex3f(10,  j, 0.0f);//x线的起点
            glVertex3f(-10, j, 0.0f);//x线的终点 
        glEnd();   
     }
     glPopMatrix(); //弹出当前矩阵至堆栈       
}*/

