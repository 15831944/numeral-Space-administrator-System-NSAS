#include <windows.h>
#include <GL/glut.h>

void BlendInit(void){
	GLfloat lightSpecular[]={1.0, 1.0, 1.0, 1.0};
	GLfloat lightPosition[]={0.5, 0.5, 4.0, 0.0};
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); //指定混合函数,定义混合功能的操作,表示源颜色乘以自身的alpha 值
	//glBlendFunc(GL_DST_ALPHA, GL_ONE_MINUS_SRC_ALPHA); //指定混合函数,定义混合功能的操作,表示源颜色乘以自身的alpha 值
	//glBlendFunc(GL_DST_ALPHA, GL_ONE_MINUS_SRC_ALPHA); //指定混合函数,定义混合功能的操作,表示源颜色乘以自身的alpha 值
    glShadeModel(GL_SMOOTH);//设置着色模式, GL_FLAT:采用恒定着色, GL_SMOOTH 光滑着色
    
    glMaterialfv(GL_FRONT, GL_SPECULAR, lightSpecular);
	glMaterialf(GL_FRONT, GL_SHININESS, 100.0);
	glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);

	glEnable(GL_BLEND);		    //启用混合状态
	glEnable(GL_LIGHTING);		//启用光照
	glEnable(GL_LIGHT0);		//打开光源0
	glEnable(GL_DEPTH_TEST);	//启用深度检测
	glEnable(GL_COLOR_MATERIAL);//材质跟踪当前绘图色
}


































