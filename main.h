#ifndef __MAIN_H__
#define __MAIN_H__


struct openGL3D{
       double X;
       double Y; 
       double Z;   
};

struct win2D{
       double X;
       double Y;
       double Z;
};

extern struct openGL3D OpenGL3D;
extern struct win2D    Win2D;

extern HANDLE hCom;    //全局变量,串口句柄
extern DCB    dcb;     //结构体变量用于串口设置 
extern bool ReadComm;  //读串口标志 

extern GLint    viewport[4];
extern GLdouble modelview[16];
extern GLdouble portection[16];

extern bool    CncConfigureFile_YES_NO;
extern bool    mouseLeftDown;
extern bool    mouseRightDown;
extern float   mouseX, mouseY;
extern float   cameraAngleX;
extern float   cameraAngleY;
extern float   cameraDistance;
extern int     screenWidth, screenHeight;

extern char  **G_array;
extern struct  CNC_MachineNumber ** cncConfigure; //要同时包含 NcFileInterpreter.h 
extern int     g_v;
extern char  **ConfigureFile;
extern char    Viewport;
extern bool    G_NC_Code;
extern bool    KnifePath; 
extern bool    CoordinateLineBool;
extern bool    Test;

extern int     OrthoWin;
extern int     windowsSize;
extern bool    OrthoWinBool;
extern int     Row;
extern int     RowNumber;
extern int xxxx, yyyy, zzzz, nnnn;
extern float color1[4];


extern void display(void);
extern void reshape(int width, int height);
extern void visibility(int state);

extern void passivemotion(int x, int y);
extern void motion(int x, int y);
extern void mouse(int button, int state, int x, int y);
extern void mouseWheel(int button, int dir, int x, int y);
extern void entry(int sstate);
extern void keybutton(unsigned char key, int x, int y);
extern void special(int key, int x, int y);

extern void DrawLine(float X_Start, float Y_Start, float Z_Start, float LineWidth, int Color[], float X_End, float Y_End, float Z_End);
extern void DrawSpot(float X, float Y, float Z, float LineWidth, float Color[]);
extern void DrawPolygon(float VertexX0,float VertexY0,float VertexX1,float VertexY1,float VertexX2,float VertexY2,float VertexX3,float VertexY3,int Color[3]);
extern void CoordinateLine(float CoordinateLineLength);
extern bool openfile(void);
extern bool CncConfigureFileInit(void);
extern void drawString3D(const char *str, float pos[3], float color[4], void *font);
extern void drawString2D(char *ch, int x, int y, float color[4], void *font);
extern void drawCNString(const char* str);
extern void selectFont(int size, int charset, const char* face);
extern bool openFile();
extern void FileRead();
extern void MemoryRelease();
extern void CNC_G_MemoryAllocation();
extern void showLine(float x0, float y0, float z0, float x1, float y1, float z1, int color[3], float LineWidth);
extern void showPolygon(float VertexX0,float VertexY0,float VertexX1,float VertexY1,float VertexX2,float VertexY2,float VertexX3,float VertexY3,int Color[3]);
extern void view(int m_width, int m_height);
extern void arc(float X, float Y, int n, float R);
/*
 *x: X 的顶点坐标数组 
 *y: Y 的顶点坐标数组 
 VertexNumber: 顶点坐标数组的长度                                               
 */
extern void DrawTriangle(float *VertexA, float *VertexB, float *VertexC);
extern void DrawArc(float *x, float *y, int VertexNumber, float z, int Color[3]);
extern void drawCNString(const char* str);
extern void selectFont(int size, int charset, const char* face);
extern void drawString2DCN(char *ch, int x, int y, int color[3], int sizeFont);

extern void button(int x, int y, int sizeX, int sizeY, bool ab, char *ch);
extern void ncFileTest(int X, int Y);

extern void ncShow(struct G_M_XYZ **G_M_CODE, int g_v, GLenum mode);
extern void MouseCB(int button, int state, int mousex, int mousey);


extern bool OpenCommWrite(HANDLE hCom, DCB dcb, char *string);//打开串口写 
extern bool OpenCommRead(HANDLE hCom, DCB dcb, char *string);//打开串口读 

extern void PaintScreen(struct G_M_XYZ **G_M_CODE, int g_v, GLenum mode);


extern void DrawGrid(float GridClearance, float GridSize, float LineWidth);
extern void DrawPiontGrid(float GridClearance, float GridSize, float PointSize, GLfloat * color);
extern void DrawCoordinate(float CoordinateSize, float X, float Y, float Z); 
extern void DrawCursor(float CursorX, float CursorY, float CursorZ, float LineWidth);
extern void BlendInit(void);

#define COLORCONVERT(R) R/255.0

#endif
