#ifndef __NCFILEINTERPRETER_H__
#define __NCFILEINTERPRETER_H__


//------------------------------------------------------------------------------
struct G_M_XYZ{
    bool G00;  //快速定位
    bool G01;  //直线插补
    bool G02;  //顺时针方向圆弧插补
    bool G03;  //逆时针方向圆弧插补
    bool G04;  //定时暂停
    bool G05;  //通过中间点圆弧插补
    bool G06;  //抛物线插补
    bool G07;  //Z 样条曲线插补
    bool G08;  //进给加速
    bool G09;  //进给减速
    bool G10;  //数据设置
    bool G16;  //极坐标编程
    bool G17;  //加工XY平面
    bool G18;  //加工XZ平面
    bool G19;  //加工YZ平面
    bool G20;  //英制尺寸(法兰克系统)
    bool G21;  //公制尺寸(法兰克系统)
    bool G22;  //半径尺寸编程方式
    bool G220; //系统操作界面上使用
    bool G23;  //直径尺寸编程方式
    bool G230; //系统操作界面上使用
    bool G24;  //子程序结束
    bool G25;  //跳转加工
    bool G26;  //循环加工
    bool G30;  //倍率注销
    bool G31;  //倍率定义
    bool G32;  //等螺距螺纹切削,英制
    bool G33;  //等螺距螺纹切削,公制
    bool G34;  //增螺距螺纹切削
    bool G35;  //减螺距螺纹切削
    bool G40;  //刀具补偿/刀具偏置注销
    bool G41;  //刀具补偿——左
    bool G42;  //刀具补偿——右
    bool G43;  //刀具偏置——正
    bool G44;  //刀具偏置——负
    bool G45;  //刀具偏置+/+
    bool G46;  //刀具偏置+/-
    bool G47;  //刀具偏置-/-
    bool G48;  //刀具偏置-/+
    bool G49;  //刀具偏置0/+
    bool G50;  //刀具偏置0/-
    bool G51;  //刀具偏置+/0
    bool G52;  //刀具偏置-/0
    bool G53;  //直线偏移,注销
    bool G54;  //直线偏移 x
    bool G55;  //直线偏移 y
    bool G56;  //直线偏移 z
    bool G57;  //直线偏移 xy
    bool G58;  //直线偏移 xz
    bool G59;  //直线偏移 yz
    bool G60;  //准确路径方式(精)
    bool G61;  //准确路径方式(中)
    bool G62;  //准确路径方式(粗)
    bool G63;  //攻螺纹
    bool G68;  //刀具偏置,内角
    bool G69;  //刀具偏置,外角
    bool G70;  //英制尺寸(这个是西门子的,法兰克的是 G21)
    bool G71;  //公制尺寸 毫米
    bool G74;  //回参考点(机床零点)
    bool G75;  //返回编程坐标零点
    bool G76;  //车螺纹复合循环
    bool G80;  //固定循环注销
    bool G81;  //外圆固定循环
    bool G83;  //
    bool G84;  //
    bool G331; //螺纹固定循环
    bool G90;  //绝对尺寸
    bool G91;  //相对尺寸
    bool G92;  //预制坐标
    bool G93;  //时间倒数,进给率
    bool G94;  //进给率,每分钟进给
    bool G95;  //进给率,每转进给
    bool G96;  //恒线速度控制
    bool G97;  //取消恒线速度控制
    
    bool  Selection;  //被选中 
    
    bool M00;  // 程序停止
    bool M01;  // 任选停止
    bool M02;  // 程序结束
    bool M03;  // 工作主轴起动 (正转)
    bool M04;  // 工作主轴起动 (反转)
    bool M05;  // 主轴停止
    bool M06;  // 刀具交换
    bool M07;  //
    bool M08;  // 冷却液开
    bool M09;  // 冷却液关
    bool M10;  // 主轴点动关
    bool M11;  // 主轴点动开
    bool M12;  // 动力刀具轴停止
    bool M13;  // 动力刀具轴正转
    bool M14;  // 动力刀具轴反转
    bool M15;  // C轴正向定位
    bool M16;  // C轴反向定位
    bool M17;  // 机外测量数据通过RS232C传送请求
    bool M18;  // 主轴定向取消
    bool M19;  // 主轴定向
    bool M20;  // 尾架干涉区或主轴干涉监视关(对面双主轴规格)
    bool M21;  // 尾架干涉区或主轴干涉监视开(对面双主轴规格)
    bool M22;  // 倒角关
    bool M23;  // 倒角开
    bool M24;  // 卡盘干涉区关,刀具干涉区关
    bool M25;  // 卡盘干涉区开,刀具干涉区开
    bool M26;  // 螺纹导程有效轴Z轴指定
    bool M27;  // 螺纹导程有效轴X轴指定
    bool M28;  // 刀具干涉检查功能关
    bool M29;  // 刀具干涉检查功能开
    bool M30;  // 程序结束
    bool M31;  //
    bool M32;  // 螺纹车削单面切削模式
    bool M33;  // 螺纹车削时交叉切削模式
    bool M34;  // 螺纹车削逆向单面切削模式
    bool M35;  // 装料器夹持器Z向滑动后退
    bool M36;  // 装料器夹持器Z向滑动前进
    bool M37;  // 装料器臂后退
    bool M38;  // 装料器臂前进到卸载位置
    bool M39;  // 装料器臂前进到卡盘位置
    bool M40;  // 主轴齿轮空档
    bool M41;  // 主轴齿轮1档或底速线圈
    bool M42;  // 主轴齿轮2档或高速线圈
    bool M43;  // 主轴齿轮3档
    bool M44;  // 主轴齿轮4档
    bool M45;  //
    bool M46;  //
    bool M47;  //
    bool M48;  // 主轴转速倍率无效取消
    bool M49;  // 主轴转速倍率无效
    bool M50;  // 附加吹气口1关
    bool M51;  // 附加吹气口1开
    bool M52;  //
    bool M53;  //
    bool M54;  // 分度卡盘自动分度
    bool M55;  // 尾架后退
    bool M56;  // 尾架前进
    bool M57;  // M63取消
    bool M58;  // 卡盘底压
    bool M59;  // 卡盘高压
    bool M60;  // M61取消
    bool M61;  // 圆周速度恒定切削时,恒定旋转应答忽视
    bool M62;  // M64取消
    bool M63;  // 主轴旋转M码应答忽视
    bool M64;  // 主轴旋转之外的M码应答忽视
    bool M65;  // T码应答忽视
    bool M66;  // 刀架回转位置自由
    bool M67;  // 凸轮车削循环中同步运行模式取消
    bool M68;  // 同步模式A运行开
    bool M69;  // 同步模式B运行开
    bool M70;  // 手动换到指令
    bool M71;  //
    bool M72;  // ATC单元定位在接近位置
    bool M73;  // 螺纹车削类型 1
    bool M74;  // 螺纹车削类型 2
    bool M75;  // 螺纹车削类型 3
    bool M76;  // 工件捕手后退
    bool M77;  // 工件捕手前进
    bool M78;  // 中心架松开
    bool M79;  // 中心架夹紧
    bool M80;  // 过切前进
    bool M81;  // 过切后退
    bool M82;  //
    bool M83;  // 卡盘夹紧
    bool M84;  // 卡盘松开
    bool M85;  // LAP粗车循环后不返回起始位置
    bool M86;  // 刀架右回转指定
    bool M87;  // M86取消
    bool M88;  // 吹气关
    bool M89;  // 吹气开
    bool M90;  // 关门
    bool M91;  // 开门
    bool M92;  // 棒料进给器后退
    bool M93;  // 棒料进给器前进
    bool M94;  // 装料器装料
    bool M95;  // 装料器卸料
    bool M96;  // 副轴用工件捕手后退
    bool M97;  // 副轴用工件捕手前进
    bool M98;  // 尾架低压
    bool M99;  // 尾架高压
    
    bool  x;
    bool  y;
    bool  z;
    bool  a;
    bool  s;
    bool  f;

    float X_Start;   //每行的 X 坐标值 
    float X_End;     //每行的 X 坐标值 
    float Y_Start;   //每行的 Y 坐标值  
    float Y_End;     //每行的 Y 坐标值  
    float Z_Start;   //每行的 Z 坐标值 
    float Z_End;     //每行的 Z 坐标值 
    float A_Start;
    float A_End;
    float S;     //每行的转速值 
    float F;     //每行的进给值
    float zHigh; //Z 背吃刀量 
    int   H;     //长度补正号 
    int   T;     //刀号 
    float R;     //半径 
    float R_Start[2];     //圆的起始坐标 
    float R_End[2];       //圆的终止坐标 
    float CircularHeartX; //圆心的 X 坐标 
    float CircularHeartY; //圆心的 Y 坐标 
    float I; //
    float J; //
    int   N; //圆度值 
    int   VertexNumber;               //实际要使用的顶点数 
    float ArcVertexCoordinateX[1024]; //圆的顶点坐标 
    float ArcVertexCoordinateY[1024]; //圆的顶点坐标 
    float color[4];   //颜色 
    float LineWidth;  //线宽 
    bool  NullLine;   //无效的行 
};
//------------------------------------------------------------------------------
struct CNC_MachineNumber{
   char CncName[14];    
   char  s[12];            //最大转速 
   char  x[12];            //X 最大行程
   char  y[12];            //Y 最大行程 
   char  z[12];            //Z 最大行程 
   bool ToolMagazine; //是否有刀库
   int  StructLength;
};
//------------------------------------------------------------------------------
#define array(LineNumber, CharNumber)  *(*(G_array+LineNumber)+CharNumber)
//------------------------------------------------------------------------------
/*取得圆弧的坐标 
  X: 圆弧起点 X 坐标
  Y: 圆弧起点 Y 坐标               
  I: 圆弧起点到圆心的 X 轴距离,圆心位于圆弧起点 X 轴的正方向为正,负方向为负 
  J: 圆弧起点到圆心的 Y 轴距离,圆心位于圆弧起点 Y 轴的正方向为正,负方向为负
 */
extern float getCoordinateX(float X, float I);
extern float getCoordinateY(float Y, float J);
//------------------------------------------------------------------------------
/*取得圆弧的半径 
  I: 圆弧起点到圆心的 X 轴距离 
  J: 圆弧起点到圆心的 Y 轴距离 
 */
extern float getRadius(float I, float J);
//------------------------------------------------------------------------------
/*
  X: 圆心的 X 起点 
  Y: 圆心的 Y 起点  
  n: 圆的圆度值 
  R: 圆的半径   
 */
extern void arc(float X, float Y, int n, float R);
//------------------------------------------------------------------------------
extern void drawCNString(const char* str);
extern void selectFont(int size, int charset, const char* face); 
//------------------------------------------------------------------------------
extern void getUseVertexCoordinate(struct G_M_XYZ **G_M_CODE, int LineNumber, float *x, float *y);  //提取顶点要用的坐标 
//------------------------------------------------------------------------------
extern void ArcChordLength(float StartX1, float StartY1, float EndX2, float EndY2);
//------------------------------------------------------------------------------
/*浮点负数转换为正数 
  TransformationNumber:要转换的浮点数 
  返回值:已转换的数 
 */
extern float NegativePositiveMutualTransformation(float TransformationNumber);
extern int   getRoundness(float R, float Angle);
extern float ArcChordLength(struct G_M_XYZ **G_M_CODE, int LineNumber);


extern struct G_M_XYZ ** G_M_CODE;
extern void CNC_InterpreterMemoryAllocation(); //依据文件总的行数分配内存 
extern void InterpreterMemoryRelease();        //释放分配内存 
extern void NcFileInterpreter(struct G_M_XYZ **G_M_CODE, char **G_array, int g_v, struct CNC_MachineNumber **cncConfigure, int k);
extern void StructInit(struct G_M_XYZ ** G_M_CODE, int g_v);
//------------------------------------------------------------------------------
//q 前缀 ＝ 低音, 1 后缀 ＝ 高音, s 前缀 ＝ 半音阶 
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











