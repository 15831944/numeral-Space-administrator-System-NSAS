#include <GL/freeglut.h>
#include <stdio.h>
#include <stdlib.h>
#include "main.h"
#include "NcFileInterpreter.h"
#include "color.h"


#define toOrtho
//#define toPerspective
int   OrthoWin     = 350;
int   windowsSize  = 350;
bool  OrthoWinBool = false;
int   RowNumber    = 0;


int xxxx = 10, yyyy = 0, zzzz = 0, nnnn = 0;
float Color[]    = {220,20,60};
float color[4]   = {1, 1, 0, 1};
float color1[4]  = {COLORCONVERT(PeachPuff[0]), COLORCONVERT(PeachPuff[1]), COLORCONVERT(PeachPuff[2]), 1};

bool  G_NC_Code = false;  //G 代码显示与隐藏标志位
bool  KnifePath = false; 
bool  CoordinateLineBool = false;
bool  Test      = false;  //数据测试函数是否运行 

GLint    viewport[4];
GLdouble modelview[16];
GLdouble portection[16];
//==========================================================================================================================
//==========================================================================================================================
//==========================================================================================================================
//==========================================================================================================================
extern int Test_pthread;
void display(void){  //显示回调函数
    if (OrthoWinBool == true){                   
        glMatrixMode(GL_PROJECTION);//指定对投影矩阵应用随后的矩阵操作 
    	glLoadIdentity();           //重置当前指定的矩阵为单位矩阵.
        if (screenHeight == 0)
    		screenHeight = 1;
    	if (screenWidth <= screenHeight)
    		glOrtho(-OrthoWin, OrthoWin, -OrthoWin * ((GLfloat)screenHeight / (GLfloat)screenWidth), OrthoWin * ((GLfloat)screenHeight / (GLfloat)screenWidth), -100000.0, 100000);
    	else
    		glOrtho(-OrthoWin * ((GLfloat)screenWidth / (GLfloat)screenHeight), OrthoWin * ((GLfloat)screenWidth / (GLfloat)screenHeight), -OrthoWin, OrthoWin, -100000.0, 100000);
    
    	glMatrixMode(GL_MODELVIEW); //设置当前矩阵为模型视图矩阵
    	glLoadIdentity();
    	OrthoWinBool = false;
    }
    
    /*GL_COLOR_BUFFER_BIT:当前可写的颜色缓冲,GL_DEPTH_BUFFER_BIT:深度缓冲,GL_ACCUM_BUFFER_BIT:累积缓冲,GL_STENCIL_BUFFER_BIT:模板缓冲*/
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 
	glClearColor(0.3, 0.3, 0.5, 1.0);  //设定颜色
    glEnable(GL_LIGHTING);//启用光照
    glEnable(GL_DEPTH_TEST);
	glPushMatrix();	
    	
    PaintScreen(G_M_CODE, g_v, GL_RENDER); 

    glPopMatrix();
    glFlush();                         //强制刷新缓冲，保证所有的绘图命令被执行
	glutSwapBuffers();                 //交换当前窗口缓存
}

//==========================================================================================================================
//==========================================================================================================================
//==========================================================================================================================
//========================================================================================================================== 
void reshape(int width, int height){   //当前窗口的形状变化回调函数
    screenWidth  = width;
    screenHeight = height;
#ifdef  toOrtho    //窗口改变时的回调函数,使用正投影  
	glViewport(0, 0, (GLsizei)width, (GLsizei)height); //设置视口,OpenGL视见区位置大小,即渲染结果绘制区位置大小

	glMatrixMode(GL_PROJECTION);//指定对投影矩阵应用随后的矩阵操作 
	glLoadIdentity();           //重置当前指定的矩阵为单位矩阵.
	if (height == 0)
		height = 1;
	if (width <= height)
		glOrtho(-OrthoWin, OrthoWin, -OrthoWin * ((GLfloat)height / (GLfloat)width), OrthoWin * ((GLfloat)height / (GLfloat)width), -100000.0, 100000);
	else
		glOrtho(-OrthoWin * ((GLfloat)width / (GLfloat)height), OrthoWin * ((GLfloat)width / (GLfloat)height), -OrthoWin, OrthoWin, -100000.0, 100000);
	glMatrixMode(GL_MODELVIEW); //设置当前矩阵为模型视图矩阵
	glLoadIdentity();
#endif 
  
#ifdef toPerspective //窗口改变时的回调函数,使用透视投影 
	glViewport(0, 0, (GLsizei)width, (GLsizei)height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60.0f, (float)(width) / height, 1.0f, 1000.0f); // FOV, AspectRatio, NearClip, FarClip
	gluLookAt(10, 10, 1, 0, 0, 0, 0, 0, 1);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
#endif 
}
//==========================================================================================================================
//==========================================================================================================================
//==========================================================================================================================
//==========================================================================================================================
void visibility(int state){   //当前窗口的可视回调函数
}
//==========================================================================================================================
//==========================================================================================================================
//==========================================================================================================================
//==========================================================================================================================
void entry(int state){  //当前窗口鼠标进出回调函数  
    if(state == GLUT_LEFT){    //离开 
         G_NC_Code = false;
    }
    if(state == GLUT_ENTERED){ //进入 
         G_NC_Code = true;
    }
}
//==========================================================================================================================
//==========================================================================================================================
//==========================================================================================================================
//==========================================================================================================================
void MouseCB(int button, int state, int mousex, int mousey){
	if(button != GLUT_LEFT || state != GLUT_DOWN){
		return;
	}
	//=========================================================================================== 
	//与旋转相关的代码 
    mouseX = mousex;
    mouseY = mousey;   
    if(button == GLUT_LEFT_BUTTON){
        if(state == GLUT_DOWN){
            mouseLeftDown = true;
        }
        else if(state == GLUT_UP)
            mouseLeftDown = false;
    }

    else if(button == GLUT_RIGHT_BUTTON){
        if(state == GLUT_DOWN){
            mouseRightDown = true;            
        }
        else if(state == GLUT_UP)
            mouseRightDown = false;
    }  
    //===========================================================================================
    //与拾取相关的代码 
	GLuint      selectBuf[512] = {0,0}; //声明无符号整数变量,512个值 
	GLint       hits;                   //声明整数变量 
	GLint       viewport[4];            //声明整数数组 
	static bool IsFirst = true;         //声明静态布尔变量 

	glGetIntegerv (GL_VIEWPORT, viewport); //获得视口信息,指定返回点击记录的数组,GL_VIEWPORT指示返回4个值X,Y坐标与视口宽度,高度,后面是接收数组 
	glSelectBuffer (512, selectBuf);       //分配选择缓冲区,1是长度,2是缓冲区地址指针 

	glRenderMode(GL_SELECT); //进入选择模式 
	glInitNames();           //名字堆栈初始化
	glPushName(1);           //压入名称堆栈 

	glMatrixMode (GL_PROJECTION); //指定一个矩阵为当前矩阵 
	glPushMatrix ();              //压入当前矩阵至堆栈
	   glLoadIdentity ();         //重置当前指定的矩阵为单位矩阵,和上面的函数相配合的 
	   gluPickMatrix((GLdouble)mousex, (GLdouble)(viewport[3] - mousey), 5.0, 5.0, viewport);//定义拾取区域 
	   if (screenWidth <= screenHeight)
		   glOrtho(-OrthoWin, OrthoWin, -OrthoWin * ((GLfloat)screenHeight / (GLfloat)screenWidth), OrthoWin * ((GLfloat)screenHeight / (GLfloat)screenWidth), -100000.0, 100000);
	   else
		   glOrtho(-OrthoWin * ((GLfloat)screenWidth / (GLfloat)screenHeight), OrthoWin * ((GLfloat)screenWidth / (GLfloat)screenHeight), -OrthoWin, OrthoWin, -100000.0, 100000);
	glMatrixMode(GL_MODELVIEW);   //指定一个矩阵为当前矩阵 
	
	PaintScreen(G_M_CODE, g_v, GL_SELECT);//调用上面的函数,旋转模型并向名字堆栈中压入名称,并对选中的目标进行重绘
	
	glPopMatrix ();//弹出当前矩阵至堆栈
	glFlush();

	hits = glRenderMode (GL_RENDER);//当选中时这个函数返回 1,否则返回 0,点记录写入缓冲区,一个点击记录4个整形数据 
	
	glMatrixMode(GL_PROJECTION);    //指定一个矩阵为当前矩阵 
	glLoadIdentity();               //重置当前指定的矩阵为单位矩阵,和上面的函数相配合的 
	glViewport(0, 0, screenWidth, screenHeight);//重设视口,这里有个 bug 即没有将窗口改变后的值设为视口大小 
	if (screenWidth <= screenHeight)
		glOrtho(-OrthoWin, OrthoWin, -OrthoWin * ((GLfloat)screenHeight / (GLfloat)screenWidth), OrthoWin * ((GLfloat)screenHeight / (GLfloat)screenWidth), -100000.0, 100000);
	else
		glOrtho(-OrthoWin * ((GLfloat)screenWidth / (GLfloat)screenHeight), OrthoWin * ((GLfloat)screenWidth / (GLfloat)screenHeight), -OrthoWin, OrthoWin, -100000.0, 100000);
    glMatrixMode(GL_MODELVIEW); //设置当前矩阵为模型视图矩阵
	glLoadIdentity();
    	
    if(hits){     
		int n = 0;
		double minz = selectBuf[1];
		for(int i = 1; i < hits; i++){
			if( selectBuf[1 + i * 4] < minz ){
				n = i;
				minz = selectBuf[1 + i * 4];
			}
		}
		//hit记录第四项为ID编号
		if (KnifePath == true){
    		if (G_M_CODE[selectBuf[3 + n * 4]] -> Selection == false){
    		      G_M_CODE[selectBuf[3 + n * 4]] -> Selection = true;
    		}else{ 
                  G_M_CODE[selectBuf[3 + n * 4]] -> Selection = false; 
            } 
        }         
	} 	
}

char Viewport = 1;
void DrawGraphics(struct G_M_XYZ **G_M_CODE, int g_v, GLenum mode);
void PaintScreen(struct G_M_XYZ **G_M_CODE, int g_v, GLenum mode){ 
     int j = screenHeight - 10;       
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++  
     switch(Viewport){
         case 1:               
                glRotatef(cameraAngleX, 1, 0, 0);   //鼠标旋转 X
                glRotatef(cameraAngleY, 0, 0, 1);   //鼠标旋转 Y  
                DrawGraphics(G_M_CODE, g_v, mode);
                break;
         case 2:DrawGraphics(G_M_CODE, g_v, mode); //俯 
                break; 
         case 3:glRotatef(-90, 1, 0, 0);   
                DrawGraphics(G_M_CODE, g_v, mode); //前 
                break;  
         case 4:glRotatef(90, 0, 1, 0); 
                glRotatef(-90, 1, 0, 0); 
                DrawGraphics(G_M_CODE, g_v, mode); //左 
                break;  
         case 5:glRotatef(-90, 0, 1, 0); 
                glRotatef(-90, 1, 0, 0); 
                DrawGraphics(G_M_CODE, g_v, mode); //右 
                break;  
         case 6:glRotatef(90, 1, 0, 0);  
                glRotatef(180, 0, 1, 0);  
                DrawGraphics(G_M_CODE, g_v, mode); //后 
                break;  
         case 7:glRotatef(-180, 1, 0, 0);   
                DrawGraphics(G_M_CODE, g_v, mode); //底 
                break;         
    }
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++    
    glGetIntegerv(GL_VIEWPORT,viewport);//取得用于屏幕坐标到 3D 坐标变换需要的值 
    glGetDoublev(GL_MODELVIEW_MATRIX,modelview);
    glGetDoublev(GL_PROJECTION_MATRIX,portection);
      
    glDisable(GL_DEPTH_TEST);   //关闭深度测试 
    glDisable(GL_LIGHTING);     //关闭光照 
    //--------------------------------------------------------------------------	
    //必须在此区域,绘制窗口组件 
    if (mode != GL_SELECT){
            RowNumber = Row;
            if (G_NC_Code == true){
                for (RowNumber; RowNumber < (screenHeight / 15) + Row; RowNumber++){
                    if (RowNumber >= g_v)
                       break;
                    drawString2D(G_array[RowNumber], 0, j, color1, GLUT_BITMAP_8_BY_13);
                    j -= 15;
                }       
            }
            showPolygon(screenWidth,screenHeight, screenWidth-120,screenHeight, screenWidth - 120,0, screenWidth,0,LightGrey);
            showLine(screenWidth - 120,screenWidth - 120, 0,screenHeight, 1,1, DimGray,2);
            button(screenWidth-5, screenHeight-5,   110, 35, true, "13号大龙门");
            button(screenWidth-5, screenHeight-45,  110, 35, true, "12号哈挻");
            button(screenWidth-5, screenHeight-85,  110, 35, true, "11号哈斯");
            button(screenWidth-5, screenHeight-125, 110, 35, true, "10号小龙门");
            button(screenWidth-5, screenHeight-165, 110, 35, true, "9 号");
            button(screenWidth-5, screenHeight-205, 110, 35, true, "8 号");
            button(screenWidth-5, screenHeight-245, 110, 35, true, "7 号");
            button(screenWidth-5, screenHeight-285, 110, 35, false,"6 号");
            
            char ccc[0]; //测试代码 
            itoa(xxxx,ccc,10); //测试代码    
            drawString2DCN(ccc, screenWidth-110, 90, OrangeRed, 12);
            itoa(yyyy,ccc,10); //测试代码 
            drawString2DCN(ccc, screenWidth-60, 90, OrangeRed, 12);
            itoa(zzzz,ccc,10); //测试代码 
            drawString2DCN(ccc, screenWidth-60, 190, OrangeRed, 12); 
            itoa(nnnn,ccc,10); //测试代码 
            drawString2DCN(ccc, screenWidth-110, 190, OrangeRed, 12); 
            
            if (Test == true)
                ncFileTest(screenWidth, screenHeight);
            
            drawString2DCN("上海禹尚精密机械有限公司", screenWidth-410, 10, DarkRed, 24);  
    }   
     //测试代码    
     /*float j = 0;
     glColor3ub(SpringGreen[0], SpringGreen[1], SpringGreen[2]);    //设置画笔的颜色     
     for (int i = 0; i < 5; i++){
        if (mode == GL_SELECT)
           glLoadName(i); //向名称堆栈中装载名称 
        j = j+15;
		glBegin(GL_LINES);   
            glVertex3f(10,  j, 0.0f);//x线的起点
            glVertex3f(-10, j, 0.0f);//x线的终点 
        glEnd();   
     }*/   
}
//==========================================================================================================================
//==========================================================================================================================
//==========================================================================================================================
//==========================================================================================================================
void DrawGraphics(struct G_M_XYZ **G_M_CODE, int g_v, GLenum mode){                           
        //==========================================================================
        /*glEnable(GL_BLEND);    //启用混合       
        glDepthMask(GL_FALSE); //深度缓存只读 
        glPushMatrix();        //压入当前矩阵至堆栈 
        float polygonSize = 600 / 2;
        //GLfloat color[3] = {0.0f, 0.4f, 0.0f};//两行为光照材质设置
        //glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, color);
        glColor4f(0.3, 0.3, 0.5, 0.0f);//最后一个值是透明度,此时多边形全透明,只能看见网格
        glBegin(GL_QUADS);
               glVertex2f(-polygonSize,-polygonSize);
               glVertex2f( polygonSize,-polygonSize);  
        	   glVertex2f( polygonSize, polygonSize);
        	   glVertex2f(-polygonSize, polygonSize);
        glEnd();
        glPopMatrix();        //弹出当前矩阵至堆栈 
        glDepthMask(GL_TRUE); //深度缓存可读写
        glDisable(GL_BLEND);  //关混合*/
        //==========================================================================
                 
        if (KnifePath == true)
             ncShow(G_M_CODE, g_v, mode); //显示刀具路径 
             //==============================================================
        /*float VertexA[3] = {0,0,120}; 
        float VertexB[3] = {-120,0,0}; 
        float VertexC[3] = {120,0,0};
        glColor3f(0, 1.0, 0);
        DrawTriangle(VertexA, VertexB, VertexC);*/
        //==============================================================
        if (CoordinateLineBool == true)
             CoordinateLine(800000);
        //DrawGrid(50, 600, 1);
        DrawCoordinate(120, 0.0, 0.0, 0.0);
        //DrawCursor(5, 5, 0, 1);
}


