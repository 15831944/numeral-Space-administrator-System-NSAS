#ifndef __NCFILEINTERPRETER_H__
#define __NCFILEINTERPRETER_H__


//------------------------------------------------------------------------------
struct G_M_XYZ{
    bool G00;  //���ٶ�λ
    bool G01;  //ֱ�߲岹
    bool G02;  //˳ʱ�뷽��Բ���岹
    bool G03;  //��ʱ�뷽��Բ���岹
    bool G04;  //��ʱ��ͣ
    bool G05;  //ͨ���м��Բ���岹
    bool G06;  //�����߲岹
    bool G07;  //Z �������߲岹
    bool G08;  //��������
    bool G09;  //��������
    bool G10;  //��������
    bool G16;  //��������
    bool G17;  //�ӹ�XYƽ��
    bool G18;  //�ӹ�XZƽ��
    bool G19;  //�ӹ�YZƽ��
    bool G20;  //Ӣ�Ƴߴ�(������ϵͳ)
    bool G21;  //���Ƴߴ�(������ϵͳ)
    bool G22;  //�뾶�ߴ��̷�ʽ
    bool G220; //ϵͳ����������ʹ��
    bool G23;  //ֱ���ߴ��̷�ʽ
    bool G230; //ϵͳ����������ʹ��
    bool G24;  //�ӳ������
    bool G25;  //��ת�ӹ�
    bool G26;  //ѭ���ӹ�
    bool G30;  //����ע��
    bool G31;  //���ʶ���
    bool G32;  //���ݾ���������,Ӣ��
    bool G33;  //���ݾ���������,����
    bool G34;  //���ݾ���������
    bool G35;  //���ݾ���������
    bool G40;  //���߲���/����ƫ��ע��
    bool G41;  //���߲���������
    bool G42;  //���߲���������
    bool G43;  //����ƫ�á�����
    bool G44;  //����ƫ�á�����
    bool G45;  //����ƫ��+/+
    bool G46;  //����ƫ��+/-
    bool G47;  //����ƫ��-/-
    bool G48;  //����ƫ��-/+
    bool G49;  //����ƫ��0/+
    bool G50;  //����ƫ��0/-
    bool G51;  //����ƫ��+/0
    bool G52;  //����ƫ��-/0
    bool G53;  //ֱ��ƫ��,ע��
    bool G54;  //ֱ��ƫ�� x
    bool G55;  //ֱ��ƫ�� y
    bool G56;  //ֱ��ƫ�� z
    bool G57;  //ֱ��ƫ�� xy
    bool G58;  //ֱ��ƫ�� xz
    bool G59;  //ֱ��ƫ�� yz
    bool G60;  //׼ȷ·����ʽ(��)
    bool G61;  //׼ȷ·����ʽ(��)
    bool G62;  //׼ȷ·����ʽ(��)
    bool G63;  //������
    bool G68;  //����ƫ��,�ڽ�
    bool G69;  //����ƫ��,���
    bool G70;  //Ӣ�Ƴߴ�(����������ӵ�,�����˵��� G21)
    bool G71;  //���Ƴߴ� ����
    bool G74;  //�زο���(�������)
    bool G75;  //���ر���������
    bool G76;  //�����Ƹ���ѭ��
    bool G80;  //�̶�ѭ��ע��
    bool G81;  //��Բ�̶�ѭ��
    bool G83;  //
    bool G84;  //
    bool G331; //���ƹ̶�ѭ��
    bool G90;  //���Գߴ�
    bool G91;  //��Գߴ�
    bool G92;  //Ԥ������
    bool G93;  //ʱ�䵹��,������
    bool G94;  //������,ÿ���ӽ���
    bool G95;  //������,ÿת����
    bool G96;  //�����ٶȿ���
    bool G97;  //ȡ�������ٶȿ���
    
    bool  Selection;  //��ѡ�� 
    
    bool M00;  // ����ֹͣ
    bool M01;  // ��ѡֹͣ
    bool M02;  // �������
    bool M03;  // ���������� (��ת)
    bool M04;  // ���������� (��ת)
    bool M05;  // ����ֹͣ
    bool M06;  // ���߽���
    bool M07;  //
    bool M08;  // ��ȴҺ��
    bool M09;  // ��ȴҺ��
    bool M10;  // ����㶯��
    bool M11;  // ����㶯��
    bool M12;  // ����������ֹͣ
    bool M13;  // ������������ת
    bool M14;  // ���������ᷴת
    bool M15;  // C������λ
    bool M16;  // C�ᷴ��λ
    bool M17;  // �����������ͨ��RS232C��������
    bool M18;  // ���ᶨ��ȡ��
    bool M19;  // ���ᶨ��
    bool M20;  // β�ܸ����������������ӹ�(����˫������)
    bool M21;  // β�ܸ����������������ӿ�(����˫������)
    bool M22;  // ���ǹ�
    bool M23;  // ���ǿ�
    bool M24;  // ���̸�������,���߸�������
    bool M25;  // ���̸�������,���߸�������
    bool M26;  // ���Ƶ�����Ч��Z��ָ��
    bool M27;  // ���Ƶ�����Ч��X��ָ��
    bool M28;  // ���߸����鹦�ܹ�
    bool M29;  // ���߸����鹦�ܿ�
    bool M30;  // �������
    bool M31;  //
    bool M32;  // ���Ƴ�����������ģʽ
    bool M33;  // ���Ƴ���ʱ��������ģʽ
    bool M34;  // ���Ƴ�������������ģʽ
    bool M35;  // װ�����г���Z�򻬶�����
    bool M36;  // װ�����г���Z�򻬶�ǰ��
    bool M37;  // װ�����ۺ���
    bool M38;  // װ������ǰ����ж��λ��
    bool M39;  // װ������ǰ��������λ��
    bool M40;  // ������ֿյ�
    bool M41;  // �������1���������Ȧ
    bool M42;  // �������2���������Ȧ
    bool M43;  // �������3��
    bool M44;  // �������4��
    bool M45;  //
    bool M46;  //
    bool M47;  //
    bool M48;  // ����ת�ٱ�����Чȡ��
    bool M49;  // ����ת�ٱ�����Ч
    bool M50;  // ���Ӵ�����1��
    bool M51;  // ���Ӵ�����1��
    bool M52;  //
    bool M53;  //
    bool M54;  // �ֶȿ����Զ��ֶ�
    bool M55;  // β�ܺ���
    bool M56;  // β��ǰ��
    bool M57;  // M63ȡ��
    bool M58;  // ���̵�ѹ
    bool M59;  // ���̸�ѹ
    bool M60;  // M61ȡ��
    bool M61;  // Բ���ٶȺ㶨����ʱ,�㶨��תӦ�����
    bool M62;  // M64ȡ��
    bool M63;  // ������תM��Ӧ�����
    bool M64;  // ������ת֮���M��Ӧ�����
    bool M65;  // T��Ӧ�����
    bool M66;  // ���ܻ�תλ������
    bool M67;  // ͹�ֳ���ѭ����ͬ������ģʽȡ��
    bool M68;  // ͬ��ģʽA���п�
    bool M69;  // ͬ��ģʽB���п�
    bool M70;  // �ֶ�����ָ��
    bool M71;  //
    bool M72;  // ATC��Ԫ��λ�ڽӽ�λ��
    bool M73;  // ���Ƴ������� 1
    bool M74;  // ���Ƴ������� 2
    bool M75;  // ���Ƴ������� 3
    bool M76;  // �������ֺ���
    bool M77;  // ��������ǰ��
    bool M78;  // ���ļ��ɿ�
    bool M79;  // ���ļܼн�
    bool M80;  // ����ǰ��
    bool M81;  // ���к���
    bool M82;  //
    bool M83;  // ���̼н�
    bool M84;  // �����ɿ�
    bool M85;  // LAP�ֳ�ѭ���󲻷�����ʼλ��
    bool M86;  // �����һ�תָ��
    bool M87;  // M86ȡ��
    bool M88;  // ������
    bool M89;  // ������
    bool M90;  // ����
    bool M91;  // ����
    bool M92;  // ���Ͻ���������
    bool M93;  // ���Ͻ�����ǰ��
    bool M94;  // װ����װ��
    bool M95;  // װ����ж��
    bool M96;  // �����ù������ֺ���
    bool M97;  // �����ù�������ǰ��
    bool M98;  // β�ܵ�ѹ
    bool M99;  // β�ܸ�ѹ
    
    bool  x;
    bool  y;
    bool  z;
    bool  a;
    bool  s;
    bool  f;

    float X_Start;   //ÿ�е� X ����ֵ 
    float X_End;     //ÿ�е� X ����ֵ 
    float Y_Start;   //ÿ�е� Y ����ֵ  
    float Y_End;     //ÿ�е� Y ����ֵ  
    float Z_Start;   //ÿ�е� Z ����ֵ 
    float Z_End;     //ÿ�е� Z ����ֵ 
    float A_Start;
    float A_End;
    float S;     //ÿ�е�ת��ֵ 
    float F;     //ÿ�еĽ���ֵ
    float zHigh; //Z ���Ե��� 
    int   H;     //���Ȳ����� 
    int   T;     //���� 
    float R;     //�뾶 
    float R_Start[2];     //Բ����ʼ���� 
    float R_End[2];       //Բ����ֹ���� 
    float CircularHeartX; //Բ�ĵ� X ���� 
    float CircularHeartY; //Բ�ĵ� Y ���� 
    float I; //
    float J; //
    int   N; //Բ��ֵ 
    int   VertexNumber;               //ʵ��Ҫʹ�õĶ����� 
    float ArcVertexCoordinateX[1024]; //Բ�Ķ������� 
    float ArcVertexCoordinateY[1024]; //Բ�Ķ������� 
    float color[4];   //��ɫ 
    float LineWidth;  //�߿� 
    bool  NullLine;   //��Ч���� 
};
//------------------------------------------------------------------------------
struct CNC_MachineNumber{
   char CncName[14];    
   char  s[12];            //���ת�� 
   char  x[12];            //X ����г�
   char  y[12];            //Y ����г� 
   char  z[12];            //Z ����г� 
   bool ToolMagazine; //�Ƿ��е���
   int  StructLength;
};
//------------------------------------------------------------------------------
#define array(LineNumber, CharNumber)  *(*(G_array+LineNumber)+CharNumber)
//------------------------------------------------------------------------------
/*ȡ��Բ�������� 
  X: Բ����� X ����
  Y: Բ����� Y ����               
  I: Բ����㵽Բ�ĵ� X �����,Բ��λ��Բ����� X ���������Ϊ��,������Ϊ�� 
  J: Բ����㵽Բ�ĵ� Y �����,Բ��λ��Բ����� Y ���������Ϊ��,������Ϊ��
 */
extern float getCoordinateX(float X, float I);
extern float getCoordinateY(float Y, float J);
//------------------------------------------------------------------------------
/*ȡ��Բ���İ뾶 
  I: Բ����㵽Բ�ĵ� X ����� 
  J: Բ����㵽Բ�ĵ� Y ����� 
 */
extern float getRadius(float I, float J);
//------------------------------------------------------------------------------
/*
  X: Բ�ĵ� X ��� 
  Y: Բ�ĵ� Y ���  
  n: Բ��Բ��ֵ 
  R: Բ�İ뾶   
 */
extern void arc(float X, float Y, int n, float R);
//------------------------------------------------------------------------------
extern void drawCNString(const char* str);
extern void selectFont(int size, int charset, const char* face); 
//------------------------------------------------------------------------------
extern void getUseVertexCoordinate(struct G_M_XYZ **G_M_CODE, int LineNumber, float *x, float *y);  //��ȡ����Ҫ�õ����� 
//------------------------------------------------------------------------------
extern void ArcChordLength(float StartX1, float StartY1, float EndX2, float EndY2);
//------------------------------------------------------------------------------
/*���㸺��ת��Ϊ���� 
  TransformationNumber:Ҫת���ĸ����� 
  ����ֵ:��ת������ 
 */
extern float NegativePositiveMutualTransformation(float TransformationNumber);
extern int   getRoundness(float R, float Angle);
extern float ArcChordLength(struct G_M_XYZ **G_M_CODE, int LineNumber);


extern struct G_M_XYZ ** G_M_CODE;
extern void CNC_InterpreterMemoryAllocation(); //�����ļ��ܵ����������ڴ� 
extern void InterpreterMemoryRelease();        //�ͷŷ����ڴ� 
extern void NcFileInterpreter(struct G_M_XYZ **G_M_CODE, char **G_array, int g_v, struct CNC_MachineNumber **cncConfigure, int k);
extern void StructInit(struct G_M_XYZ ** G_M_CODE, int g_v);
//------------------------------------------------------------------------------
//q ǰ׺ �� ����, 1 ��׺ �� ����, s ǰ׺ �� ������ 
#define qdo 262
#define qre 294 
#define qmi 330
#define qfa 349
#define qso 392
#define qla 440
#define qsi 494

#define do 523
#define re 578 
#define mi 659
#define fa 698
#define so 784
#define la 880
#define si 988

#define do1 1046
#define re1 1175 
#define mi1 1318
#define fa1 1480
#define so1 1568
#define la1 1760
#define si1 1976

#define sqdo 277
#define sqre 311
#define sqfa 370
#define sqso 415
#define sqla 466

#define sdo 554
#define sre 622 
#define sfa 740
#define sso 831
#define sla 932

#define sdo1 1046
#define sre1 1245 
#define sfa1 1480
#define sso1 1661
#define sla1 1865

#endif











