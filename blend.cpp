#include <windows.h>
#include <GL/glut.h>

void BlendInit(void){
	GLfloat lightSpecular[]={1.0, 1.0, 1.0, 1.0};
	GLfloat lightPosition[]={0.5, 0.5, 4.0, 0.0};
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); //ָ����Ϻ���,�����Ϲ��ܵĲ���,��ʾԴ��ɫ���������alpha ֵ
	//glBlendFunc(GL_DST_ALPHA, GL_ONE_MINUS_SRC_ALPHA); //ָ����Ϻ���,�����Ϲ��ܵĲ���,��ʾԴ��ɫ���������alpha ֵ
	//glBlendFunc(GL_DST_ALPHA, GL_ONE_MINUS_SRC_ALPHA); //ָ����Ϻ���,�����Ϲ��ܵĲ���,��ʾԴ��ɫ���������alpha ֵ
    glShadeModel(GL_SMOOTH);//������ɫģʽ, GL_FLAT:���ú㶨��ɫ, GL_SMOOTH �⻬��ɫ
    
    glMaterialfv(GL_FRONT, GL_SPECULAR, lightSpecular);
	glMaterialf(GL_FRONT, GL_SHININESS, 100.0);
	glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);

	glEnable(GL_BLEND);		    //���û��״̬
	glEnable(GL_LIGHTING);		//���ù���
	glEnable(GL_LIGHT0);		//�򿪹�Դ0
	glEnable(GL_DEPTH_TEST);	//������ȼ��
	glEnable(GL_COLOR_MATERIAL);//���ʸ��ٵ�ǰ��ͼɫ
}


































