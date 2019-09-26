#include <GL/freeglut.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <conio.h>
#include "NcFileInterpreter.h"
#include "main.h"
#include "color.h"

struct G_M_XYZ ** G_M_CODE = NULL;  //定义二重指向结构指针的指针变量 
//============================================================================== 
//==============================================================================  
//==============================================================================  
//==============================================================================  
//==============================================================================
//==============================================================================
/* NC 程序可为我们提供, R 半径, (I,J 可算出半径), 起点的 XY 坐标, 终点的 XY 坐标 
   以及 G02,G03 圆弧的方向 
   1:首先是以合适的 n 计算出整圆的顶点所有的坐标值
   2:代入起点和终点坐标,及 G02,G03 方向指令,截取需要的顶点坐标
   3:代入相对于机械坐标而言的工件坐标系的 X,Y 坐标实现绘制 
   
   
   第一步:将文件读入数组,并确定行数
   第二步:用合适的解释器将数据提取出来,H,F,S,G,M值等并存入结构体对应的行 
   第三步:顶点坐标的计算和提取 
   第四步:按结构体中的信息进行绘制显示,处理 
   第五步:多文件显示和编辑修改,多窗口 
 */
//==========================================================================================================================
//==========================================================================================================================
//==========================================================================================================================
//==========================================================================================================================
//通过圆弧的起点,终点坐标计算弦长和角度 
/*
 G_M_CODE:结构体指针
 LineNumber:行号 
 返回 
 */
extern int xxxx, yyyy, zzzz, nnnn; 
float ArcChordLength(struct G_M_XYZ **G_M_CODE, int LineNumber){
     float SmallAngle  = 0;
     float ChordLength = 0;
     //float BigAngle    = 0;
     ChordLength = sqrt((G_M_CODE[LineNumber]->R_Start[0] - G_M_CODE[LineNumber]->R_End[0])\
                       *(G_M_CODE[LineNumber]->R_Start[0] - G_M_CODE[LineNumber]->R_End[0])\
                       +(G_M_CODE[LineNumber]->R_Start[1] - G_M_CODE[LineNumber]->R_End[1])\
                       *(G_M_CODE[LineNumber]->R_Start[1] - G_M_CODE[LineNumber]->R_End[1]));      //弦长 
     
     SmallAngle = (asin((ChordLength / 2) / G_M_CODE[LineNumber]->R) * 180.0 / 3.1415926536) * 2;  //小角
     //BigAngle   =  360 - SmallAngle;      //大角 
     //if (SmallAngle < 1.0)
        //SmallAngle = 5.1;
     return SmallAngle;
}
//==========================================================================================================================
//==========================================================================================================================
//==========================================================================================================================
//==========================================================================================================================
//依据圆的 R 大小获取合适的 N 值(圆度值) 
/*
  
 */
int getRoundness(float R, float Angle){
     float ArcLength, Perimeter;     
     int rn = 40, n = 40;
     int dd = 0;
     if (Angle != 0){ //这里角度为 0 时会发生可能是除法溢出错误 
         ArcLength = Angle * 3.1415926536f * R / 180;
         Perimeter = 2 * 3.1415926536f * R;
         rn = (((int)Perimeter) / ((int)ArcLength)) * 10;
         if (rn > 1024)
            rn = 1000;
         if (rn < 10){
            rn = 40;
         } 
     }      
     n  = ((int)R) / 100 * n; 
     if (n > 1024)
       n = 1000;    
     if (n < 10){
       n = 40;
     } 
     
     if (rn > n){     
        dd = rn / 2;  
        rn = dd * 2;
        return rn;
     }       
     else{    
        dd = n / 2;
        n  = dd * 2; 
        return n;
     }   
} 
//==========================================================================================================================
//==========================================================================================================================
//==========================================================================================================================
//==========================================================================================================================
/*浮点负数与正数之间的相互转换 
  TransformationNumber:要转换的浮点数 
  返回值:返回已转换的数
 */
float NegativePositiveMutualTransformation(float TransformationNumber){
      float Temp = 0;
      Temp = TransformationNumber - TransformationNumber - TransformationNumber;
      return Temp;
}
//============================================================================== 
//==============================================================================

int Start(struct G_M_XYZ **G_M_CODE, int LineNumber){      
       if ((G_M_CODE[LineNumber]->R_Start[0] > G_M_CODE[LineNumber]->CircularHeartX)       && (G_M_CODE[LineNumber]->R_Start[1] >= G_M_CODE[LineNumber]->CircularHeartY)) //一象限              
              return 1;
       else if ((G_M_CODE[LineNumber]->R_Start[0] <= G_M_CODE[LineNumber]->CircularHeartX) && (G_M_CODE[LineNumber]->R_Start[1] > G_M_CODE[LineNumber]->CircularHeartY))//二象限
              return 2;    
       else if ((G_M_CODE[LineNumber]->R_Start[0] < G_M_CODE[LineNumber]->CircularHeartX)  && (G_M_CODE[LineNumber]->R_Start[1] <= G_M_CODE[LineNumber]->CircularHeartY)) //三象限
              return 3;
       else if ((G_M_CODE[LineNumber]->R_Start[0] >= G_M_CODE[LineNumber]->CircularHeartX) && (G_M_CODE[LineNumber]->R_Start[1] < G_M_CODE[LineNumber]->CircularHeartY))//四象限
              return 4; 
       else
              return -11;       
}                                                          
int End(struct G_M_XYZ **G_M_CODE, int LineNumber){
       if ((G_M_CODE[LineNumber]->R_End[0] >  G_M_CODE[LineNumber]->CircularHeartX)      && (G_M_CODE[LineNumber]->R_End[1] >= G_M_CODE[LineNumber]->CircularHeartY)) //一象限
            return 1;
       else if ((G_M_CODE[LineNumber]->R_End[0] <= G_M_CODE[LineNumber]->CircularHeartX) && (G_M_CODE[LineNumber]->R_End[1] >  G_M_CODE[LineNumber]->CircularHeartY)) //二象限
            return 2;                   
       else if ((G_M_CODE[LineNumber]->R_End[0] <  G_M_CODE[LineNumber]->CircularHeartX) && (G_M_CODE[LineNumber]->R_End[1] <= G_M_CODE[LineNumber]->CircularHeartY)) //三象限
            return 3;
       else if ((G_M_CODE[LineNumber]->R_End[0] >= G_M_CODE[LineNumber]->CircularHeartX) && (G_M_CODE[LineNumber]->R_End[1] <  G_M_CODE[LineNumber]->CircularHeartY)) //四象限  
            return 4; 
       else 
            return -11;                                        
} 

//顶点计算,象限,绘制,程序解算,顶点提取 
void getUseVertexCoordinate(struct G_M_XYZ **G_M_CODE, int LineNumber, float *x, float *y){ //提取需要的顶点数据 
     int startA =  0;
     startA = Start(G_M_CODE, LineNumber);
     int endA   =  0;
     endA   =  End(G_M_CODE, LineNumber);
     int v = G_M_CODE[LineNumber]->N / 2; 
     int start_1 = 0, end_1 = 0;
     bool end_bool = false;
     //nnnn = G_M_CODE[LineNumber]->N;
     //zzzz = G_M_CODE[LineNumber]->N;
     //zzzz = (int)G_M_CODE[LineNumber]->R_Start[0];
     //zzzz = (int)G_M_CODE[LineNumber]->CircularHeartX;
     //nnnn = (int)G_M_CODE[LineNumber]->R_Start[1];
     //nnnn = (int)G_M_CODE[LineNumber]->CircularHeartY;
     xxxx = startA;
     yyyy = endA;       
     if (G_M_CODE[LineNumber]-> G02 == true){ //(顺时针)      
         if (startA == 1){ //注意:每个象限是不一样的,不管是什么情况到达象限最大数时必须退出 
              if (endA == 1){ 
                  if (G_M_CODE[LineNumber]-> R_Start[0] < G_M_CODE[LineNumber]->R_End[0]){ //取值不超过象限 1,到达象限最大值时强制退出 
                     for (int i = 0; i < v; i++){
                          if (x[i] > G_M_CODE[LineNumber]->R_Start[0])
                               start_1 = i;                                                      
                     }
                     for (int i = 0; i < v; i++){
                          if (x[i] < G_M_CODE[LineNumber]->R_End[0]){
                               end_1 = i; 
                               break;
                          }                                                        
                     }
                     G_M_CODE[LineNumber]->ArcVertexCoordinateX[0] = G_M_CODE[LineNumber]->R_End[0]; 
                     G_M_CODE[LineNumber]->ArcVertexCoordinateY[0] = G_M_CODE[LineNumber]->R_End[1];   
                     int j = 1;  
                     for(int i = 0; i < v; i++){
                          if ((i >= end_1) && (i <= start_1)){       
                              G_M_CODE[LineNumber]->ArcVertexCoordinateX[j] = x[i];
                              G_M_CODE[LineNumber]->ArcVertexCoordinateY[j] = y[i];
                              j++;
                          }    
                     }
                     G_M_CODE[LineNumber]->ArcVertexCoordinateX[j] = G_M_CODE[LineNumber]->R_Start[0];
                     G_M_CODE[LineNumber]->ArcVertexCoordinateY[j] = G_M_CODE[LineNumber]->R_Start[1];
                     G_M_CODE[LineNumber]->ArcVertexCoordinateX[j+1] = G_M_CODE[LineNumber]->R_Start[0];
                     G_M_CODE[LineNumber]->ArcVertexCoordinateY[j+1] = G_M_CODE[LineNumber]->R_Start[1];
                     G_M_CODE[LineNumber]->VertexNumber = j+1;                       
                  }else if (G_M_CODE[LineNumber]-> R_Start[0] == G_M_CODE[LineNumber]->R_End[0]){   
                     for (int i = 0; i < v; i++){
                          if (x[i] >= G_M_CODE[LineNumber]->R_Start[0])
                               start_1 = i;                                                        
                     }
                     end_1 = start_1;
                     G_M_CODE[LineNumber]->ArcVertexCoordinateX[0] = G_M_CODE[LineNumber]->R_End[0]; 
                     G_M_CODE[LineNumber]->ArcVertexCoordinateY[0] = G_M_CODE[LineNumber]->R_End[1];
                     int j = 1;
                     for (int i = end_1+1; i < G_M_CODE[LineNumber]->N; i++){
                          G_M_CODE[LineNumber]->ArcVertexCoordinateX[j] = x[i];
                          G_M_CODE[LineNumber]->ArcVertexCoordinateY[j] = y[i];
                          j++;
                     }
                     for(int i = 0; i < start_1+1; i++){    
                          G_M_CODE[LineNumber]->ArcVertexCoordinateX[j] = x[i];
                          G_M_CODE[LineNumber]->ArcVertexCoordinateY[j] = y[i];
                          j++;
                     }
                     G_M_CODE[LineNumber]->ArcVertexCoordinateX[j] = G_M_CODE[LineNumber]->R_Start[0];
                     G_M_CODE[LineNumber]->ArcVertexCoordinateY[j] = G_M_CODE[LineNumber]->R_Start[1];
                     G_M_CODE[LineNumber]->VertexNumber = j;
                  }else{   //取整圆,到达最大圆度值时强制退出  
                     for (int i = 0; i < v; i++){
                          if (x[i] > G_M_CODE[LineNumber]->R_Start[0])
                               start_1 = i+1;
                          if (x[i] < G_M_CODE[LineNumber]->R_End[0]){
                               end_1 = i; 
                               break;
                          }                                                        
                     }
                     G_M_CODE[LineNumber]->ArcVertexCoordinateX[0] = G_M_CODE[LineNumber]->R_End[0]; 
                     G_M_CODE[LineNumber]->ArcVertexCoordinateY[0] = G_M_CODE[LineNumber]->R_End[1];   
                     int j = 1;  
                     for(int i = end_1; i < G_M_CODE[LineNumber]->N; i++){    
                          G_M_CODE[LineNumber]->ArcVertexCoordinateX[j] = x[i];
                          G_M_CODE[LineNumber]->ArcVertexCoordinateY[j] = y[i];
                          j++;
                     }
                     for(int i = 0; i < start_1; i++){    
                          G_M_CODE[LineNumber]->ArcVertexCoordinateX[j] = x[i];
                          G_M_CODE[LineNumber]->ArcVertexCoordinateY[j] = y[i];
                          j++;
                     } 
                     G_M_CODE[LineNumber]->ArcVertexCoordinateX[j] = G_M_CODE[LineNumber]->R_Start[0];
                     G_M_CODE[LineNumber]->ArcVertexCoordinateY[j] = G_M_CODE[LineNumber]->R_Start[1];
                     G_M_CODE[LineNumber]->ArcVertexCoordinateX[j+1] = G_M_CODE[LineNumber]->R_Start[0];
                     G_M_CODE[LineNumber]->ArcVertexCoordinateY[j+1] = G_M_CODE[LineNumber]->R_Start[1];
                     G_M_CODE[LineNumber]->VertexNumber = j+1; 
                  }                       
              } 
              if (endA == 2){
                  for (int i = 0; i < v; i++){
                      if (x[i] > G_M_CODE[LineNumber]->R_Start[0])
                           start_1 = i+1;
                      if (x[i] > G_M_CODE[LineNumber]->R_End[0]){
                           end_1 = i+2; 
                           end_bool = true;
                      }                                                        
                  }
                  if (end_bool == false) 
                      end_1 = v;   
                  G_M_CODE[LineNumber]->ArcVertexCoordinateX[0] = G_M_CODE[LineNumber]->R_End[0]; 
                  G_M_CODE[LineNumber]->ArcVertexCoordinateY[0] = G_M_CODE[LineNumber]->R_End[1];   
                  int j = 1;  
                  for(int i = end_1; i < G_M_CODE[LineNumber]->N; i++){    
                      G_M_CODE[LineNumber]->ArcVertexCoordinateX[j] = x[i];
                      G_M_CODE[LineNumber]->ArcVertexCoordinateY[j] = y[i];
                      j++;
                  }
                  for(int i = 0; i < start_1; i++){    
                      G_M_CODE[LineNumber]->ArcVertexCoordinateX[j] = x[i];
                      G_M_CODE[LineNumber]->ArcVertexCoordinateY[j] = y[i];
                      j++;
                  } 
                  G_M_CODE[LineNumber]->ArcVertexCoordinateX[j] = G_M_CODE[LineNumber]->R_Start[0];
                  G_M_CODE[LineNumber]->ArcVertexCoordinateY[j] = G_M_CODE[LineNumber]->R_Start[1];
                  G_M_CODE[LineNumber]->ArcVertexCoordinateX[j+1] = G_M_CODE[LineNumber]->R_Start[0];
                  G_M_CODE[LineNumber]->ArcVertexCoordinateY[j+1] = G_M_CODE[LineNumber]->R_Start[1];
                  G_M_CODE[LineNumber]->VertexNumber = j+1;          
              }                            
              if (endA == 3){
                  for (int i = 0; i < v; i++){
                      if (x[i] > G_M_CODE[LineNumber]->R_Start[0])
                           start_1 = i+1;                                                        
                  }     
                  for (int i = v; i < G_M_CODE[LineNumber]->N; i++){
                      if (x[i] > G_M_CODE[LineNumber]->R_End[0]){
                           end_1 = i; 
                           break;
                      }                                                        
                  }
                  G_M_CODE[LineNumber]->ArcVertexCoordinateX[0] = G_M_CODE[LineNumber]->R_End[0]; 
                  G_M_CODE[LineNumber]->ArcVertexCoordinateY[0] = G_M_CODE[LineNumber]->R_End[1];   
                  int j = 1;  
                  for(int i = end_1; i < G_M_CODE[LineNumber]->N; i++){    
                      G_M_CODE[LineNumber]->ArcVertexCoordinateX[j] = x[i];
                      G_M_CODE[LineNumber]->ArcVertexCoordinateY[j] = y[i];
                      j++;
                  }
                  for(int i = 0; i < start_1; i++){    
                      G_M_CODE[LineNumber]->ArcVertexCoordinateX[j] = x[i];
                      G_M_CODE[LineNumber]->ArcVertexCoordinateY[j] = y[i];
                      j++;
                  } 
                  G_M_CODE[LineNumber]->ArcVertexCoordinateX[j] = G_M_CODE[LineNumber]->R_Start[0];
                  G_M_CODE[LineNumber]->ArcVertexCoordinateY[j] = G_M_CODE[LineNumber]->R_Start[1];
                  G_M_CODE[LineNumber]->ArcVertexCoordinateX[j+1] = G_M_CODE[LineNumber]->R_Start[0];
                  G_M_CODE[LineNumber]->ArcVertexCoordinateY[j+1] = G_M_CODE[LineNumber]->R_Start[1];
                  G_M_CODE[LineNumber]->VertexNumber = j+1;    
              }               
              if (endA == 4){
                  for (int i = 0; i < v; i++){
                      if (x[i] > G_M_CODE[LineNumber]->R_Start[0])
                           start_1 = i+1;                                                        
                  } 
                  for (int i = v; i < G_M_CODE[LineNumber]->N; i++){
                      if (x[i] > G_M_CODE[LineNumber]->R_End[0]){
                           end_1 = i; 
                           end_bool = true;
                           break;
                      }                                                        
                  }
                  if (end_bool == false) 
                      end_1 = G_M_CODE[LineNumber]->N;
                  G_M_CODE[LineNumber]->ArcVertexCoordinateX[0] = G_M_CODE[LineNumber]->R_End[0]; 
                  G_M_CODE[LineNumber]->ArcVertexCoordinateY[0] = G_M_CODE[LineNumber]->R_End[1];   
                  int j = 1;  
                  for(int i = end_1; i < G_M_CODE[LineNumber]->N; i++){    
                      G_M_CODE[LineNumber]->ArcVertexCoordinateX[j] = x[i];
                      G_M_CODE[LineNumber]->ArcVertexCoordinateY[j] = y[i];
                      j++;
                  }
                  for(int i = 0; i < start_1; i++){    
                      G_M_CODE[LineNumber]->ArcVertexCoordinateX[j] = x[i];
                      G_M_CODE[LineNumber]->ArcVertexCoordinateY[j] = y[i];
                      j++;
                  } 
                  G_M_CODE[LineNumber]->ArcVertexCoordinateX[j] = G_M_CODE[LineNumber]->R_Start[0];
                  G_M_CODE[LineNumber]->ArcVertexCoordinateY[j] = G_M_CODE[LineNumber]->R_Start[1];  
                  G_M_CODE[LineNumber]->ArcVertexCoordinateX[j+1] = G_M_CODE[LineNumber]->R_Start[0];
                  G_M_CODE[LineNumber]->ArcVertexCoordinateY[j+1] = G_M_CODE[LineNumber]->R_Start[1];
                  G_M_CODE[LineNumber]->VertexNumber = j+1;         
              }      
         }    
         if (startA == 2){
              if (endA == 1){
                  for (int i = 0; i < v; i++){
                          if (x[i] > G_M_CODE[LineNumber]->R_Start[0])
                               start_1 = i+1;                                                      
                     }
                  for (int i = 0; i < v; i++){
                          if (x[i] < G_M_CODE[LineNumber]->R_End[0]){
                               end_1 = i; 
                               break;
                          }                                                        
                  }
                  G_M_CODE[LineNumber]->ArcVertexCoordinateX[0] = G_M_CODE[LineNumber]->R_End[0]; 
                  G_M_CODE[LineNumber]->ArcVertexCoordinateY[0] = G_M_CODE[LineNumber]->R_End[1];   
                  int j = 1;  
                  for(int i = end_1; i < start_1; i++){    
                      G_M_CODE[LineNumber]->ArcVertexCoordinateX[j] = x[i];
                      G_M_CODE[LineNumber]->ArcVertexCoordinateY[j] = y[i];
                      j++;
                  }
                  G_M_CODE[LineNumber]->ArcVertexCoordinateX[j] = G_M_CODE[LineNumber]->R_Start[0];
                  G_M_CODE[LineNumber]->ArcVertexCoordinateY[j] = G_M_CODE[LineNumber]->R_Start[1];  
                  G_M_CODE[LineNumber]->ArcVertexCoordinateX[j+1] = G_M_CODE[LineNumber]->R_Start[0];
                  G_M_CODE[LineNumber]->ArcVertexCoordinateY[j+1] = G_M_CODE[LineNumber]->R_Start[1];
                  G_M_CODE[LineNumber]->VertexNumber = j+1;     
              } 
              if (endA == 2){
                  if (G_M_CODE[LineNumber]-> R_Start[0] < G_M_CODE[LineNumber]->R_End[0]){ //取值不超过象限 2,到达象限最大值时强制退出 
                     for (int i = 0; i < v; i++){
                          if (x[i] > G_M_CODE[LineNumber]->R_Start[0])
                               start_1 = i;                                                      
                     }
                     for (int i = 0; i < v; i++){
                          if (x[i] < G_M_CODE[LineNumber]->R_End[0]){
                               end_1 = i; 
                               break;
                          }                                                        
                     }
                     G_M_CODE[LineNumber]->ArcVertexCoordinateX[0] = G_M_CODE[LineNumber]->R_End[0]; 
                     G_M_CODE[LineNumber]->ArcVertexCoordinateY[0] = G_M_CODE[LineNumber]->R_End[1];   
                     int j = 1;  
                     for(int i = 0; i < v; i++){
                          if ((i >= end_1) && (i <= start_1)){       
                              G_M_CODE[LineNumber]->ArcVertexCoordinateX[j] = x[i];
                              G_M_CODE[LineNumber]->ArcVertexCoordinateY[j] = y[i];
                              j++;
                          }    
                     }
                     G_M_CODE[LineNumber]->ArcVertexCoordinateX[j] = G_M_CODE[LineNumber]->R_Start[0];
                     G_M_CODE[LineNumber]->ArcVertexCoordinateY[j] = G_M_CODE[LineNumber]->R_Start[1];
                     G_M_CODE[LineNumber]->ArcVertexCoordinateX[j+1] = G_M_CODE[LineNumber]->R_Start[0];
                     G_M_CODE[LineNumber]->ArcVertexCoordinateY[j+1] = G_M_CODE[LineNumber]->R_Start[1];
                     G_M_CODE[LineNumber]->VertexNumber = j+1;    
                  }else if(G_M_CODE[LineNumber]-> R_Start[0] == G_M_CODE[LineNumber]->R_End[0]){
                         for (int i = 0; i < v; i++){
                           if (x[i] >= G_M_CODE[LineNumber]->R_Start[0])
                               start_1 = i;                                                        
                         }
                         end_1 = start_1;
                         G_M_CODE[LineNumber]->ArcVertexCoordinateX[0] = G_M_CODE[LineNumber]->R_End[0]; 
                         G_M_CODE[LineNumber]->ArcVertexCoordinateY[0] = G_M_CODE[LineNumber]->R_End[1];
                         int j = 1;
                         for (int i = end_1+1; i < G_M_CODE[LineNumber]->N; i++){
                              G_M_CODE[LineNumber]->ArcVertexCoordinateX[j] = x[i];
                              G_M_CODE[LineNumber]->ArcVertexCoordinateY[j] = y[i];
                              j++;
                         }
                         for(int i = 0; i < start_1+1; i++){    
                              G_M_CODE[LineNumber]->ArcVertexCoordinateX[j] = x[i];
                              G_M_CODE[LineNumber]->ArcVertexCoordinateY[j] = y[i];
                              j++;
                         }
                         G_M_CODE[LineNumber]->ArcVertexCoordinateX[j] = G_M_CODE[LineNumber]->R_Start[0];
                         G_M_CODE[LineNumber]->ArcVertexCoordinateY[j] = G_M_CODE[LineNumber]->R_Start[1];
                         G_M_CODE[LineNumber]->ArcVertexCoordinateX[j+1] = G_M_CODE[LineNumber]->R_Start[0];
                         G_M_CODE[LineNumber]->ArcVertexCoordinateY[j+1] = G_M_CODE[LineNumber]->R_Start[1];
                         G_M_CODE[LineNumber]->VertexNumber = j+1;
                  } else{ //取整圆,到达最大圆度值时强制退出 
                     for (int i = 0; i < v; i++){
                          if (x[i] > G_M_CODE[LineNumber]->R_Start[0])
                               start_1 = i+1;
                          if (x[i] < G_M_CODE[LineNumber]->R_End[0]){
                               end_1 = i;
                               //end_bool = true; 
                               break;
                          }                                                        
                     }                     
                     G_M_CODE[LineNumber]->ArcVertexCoordinateX[0] = G_M_CODE[LineNumber]->R_End[0]; 
                     G_M_CODE[LineNumber]->ArcVertexCoordinateY[0] = G_M_CODE[LineNumber]->R_End[1];   
                     int j = 1;  
                     for(int i = end_1; i < G_M_CODE[LineNumber]->N; i++){    
                          G_M_CODE[LineNumber]->ArcVertexCoordinateX[j] = x[i];
                          G_M_CODE[LineNumber]->ArcVertexCoordinateY[j] = y[i];
                          j++;
                     }
                     for(int i = 0; i < start_1; i++){    
                          G_M_CODE[LineNumber]->ArcVertexCoordinateX[j] = x[i];
                          G_M_CODE[LineNumber]->ArcVertexCoordinateY[j] = y[i];
                          j++;
                     } 
                     G_M_CODE[LineNumber]->ArcVertexCoordinateX[j] = G_M_CODE[LineNumber]->R_Start[0];
                     G_M_CODE[LineNumber]->ArcVertexCoordinateY[j] = G_M_CODE[LineNumber]->R_Start[1];
                     G_M_CODE[LineNumber]->ArcVertexCoordinateX[j+1] = G_M_CODE[LineNumber]->R_Start[0];
                     G_M_CODE[LineNumber]->ArcVertexCoordinateY[j+1] = G_M_CODE[LineNumber]->R_Start[1];
                     G_M_CODE[LineNumber]->VertexNumber = j+1; 
                  }     
              } 
              if (endA == 3){
                  for (int i = 0; i < v; i++){
                          if (x[i] > G_M_CODE[LineNumber]->R_Start[0])
                               start_1 = i+1;                                                      
                  }
                  for (int i = v; i < G_M_CODE[LineNumber]->N; i++){
                          if (x[i] > G_M_CODE[LineNumber]->R_End[0]){
                               end_1 = i; 
                               break;
                          }                                                        
                  }
                  G_M_CODE[LineNumber]->ArcVertexCoordinateX[0] = G_M_CODE[LineNumber]->R_End[0]; 
                  G_M_CODE[LineNumber]->ArcVertexCoordinateY[0] = G_M_CODE[LineNumber]->R_End[1];   
                  int j = 1;  
                  for(int i = end_1; i < G_M_CODE[LineNumber]->N; i++){    
                      G_M_CODE[LineNumber]->ArcVertexCoordinateX[j] = x[i];
                      G_M_CODE[LineNumber]->ArcVertexCoordinateY[j] = y[i];
                      j++;
                  }
                  for(int i = 0; i < start_1; i++){    
                      G_M_CODE[LineNumber]->ArcVertexCoordinateX[j] = x[i];
                      G_M_CODE[LineNumber]->ArcVertexCoordinateY[j] = y[i];
                      j++;
                  } 
                  G_M_CODE[LineNumber]->ArcVertexCoordinateX[j] = G_M_CODE[LineNumber]->R_Start[0];
                  G_M_CODE[LineNumber]->ArcVertexCoordinateY[j] = G_M_CODE[LineNumber]->R_Start[1];  
                  G_M_CODE[LineNumber]->ArcVertexCoordinateX[j+1] = G_M_CODE[LineNumber]->R_Start[0];
                  G_M_CODE[LineNumber]->ArcVertexCoordinateY[j+1] = G_M_CODE[LineNumber]->R_Start[1];
                  G_M_CODE[LineNumber]->VertexNumber = j+1;     
              }   
              if (endA == 4){
                  for (int i = 0; i < v; i++){
                      if (x[i] > G_M_CODE[LineNumber]->R_Start[0])
                           start_1 = i+1;                                                        
                  } 
                  for (int i = v; i < G_M_CODE[LineNumber]->N; i++){
                      if (x[i] > G_M_CODE[LineNumber]->R_End[0]){
                           end_1 = i; 
                           end_bool = true;
                           break;
                      }                                                        
                  }
                  if (end_bool == false) 
                      end_1 = G_M_CODE[LineNumber]->N;
                  G_M_CODE[LineNumber]->ArcVertexCoordinateX[0] = G_M_CODE[LineNumber]->R_End[0]; 
                  G_M_CODE[LineNumber]->ArcVertexCoordinateY[0] = G_M_CODE[LineNumber]->R_End[1];   
                  int j = 1;  
                  for(int i = end_1; i < G_M_CODE[LineNumber]->N; i++){    
                      G_M_CODE[LineNumber]->ArcVertexCoordinateX[j] = x[i];
                      G_M_CODE[LineNumber]->ArcVertexCoordinateY[j] = y[i];
                      j++;
                  }
                  for(int i = 0; i < start_1; i++){    
                      G_M_CODE[LineNumber]->ArcVertexCoordinateX[j] = x[i];
                      G_M_CODE[LineNumber]->ArcVertexCoordinateY[j] = y[i];
                      j++;
                  } 
                  G_M_CODE[LineNumber]->ArcVertexCoordinateX[j] = G_M_CODE[LineNumber]->R_Start[0];
                  G_M_CODE[LineNumber]->ArcVertexCoordinateY[j] = G_M_CODE[LineNumber]->R_Start[1];  
                  G_M_CODE[LineNumber]->ArcVertexCoordinateX[j+1] = G_M_CODE[LineNumber]->R_Start[0];
                  G_M_CODE[LineNumber]->ArcVertexCoordinateY[j+1] = G_M_CODE[LineNumber]->R_Start[1];
                  G_M_CODE[LineNumber]->VertexNumber = j+1;     
              }      
         } 
         
         if (startA == 3){
              if (endA == 1){
                  for (int i = v; i < G_M_CODE[LineNumber]->N; i++){
                          if (x[i] > G_M_CODE[LineNumber]->R_Start[0]){
                               start_1 = i; 
                               break;
                          }                                                         
                  }
                  for (int i = 0; i < v; i++){
                          if (x[i] < G_M_CODE[LineNumber]->R_End[0]){
                               end_1 = i; 
                               break;
                          }                                                        
                  }
                  G_M_CODE[LineNumber]->ArcVertexCoordinateX[0] = G_M_CODE[LineNumber]->R_End[0]; 
                  G_M_CODE[LineNumber]->ArcVertexCoordinateY[0] = G_M_CODE[LineNumber]->R_End[1];   
                  int j = 1;  
                  for(int i = end_1; i < v; i++){    
                      G_M_CODE[LineNumber]->ArcVertexCoordinateX[j] = x[i];
                      G_M_CODE[LineNumber]->ArcVertexCoordinateY[j] = y[i];
                      j++;
                  }
                  for(int i = v; i < start_1; i++){    
                      G_M_CODE[LineNumber]->ArcVertexCoordinateX[j] = x[i];
                      G_M_CODE[LineNumber]->ArcVertexCoordinateY[j] = y[i];
                      j++;
                  } 
                  G_M_CODE[LineNumber]->ArcVertexCoordinateX[j] = G_M_CODE[LineNumber]->R_Start[0];
                  G_M_CODE[LineNumber]->ArcVertexCoordinateY[j] = G_M_CODE[LineNumber]->R_Start[1];  
                  G_M_CODE[LineNumber]->ArcVertexCoordinateX[j+1] = G_M_CODE[LineNumber]->R_Start[0];
                  G_M_CODE[LineNumber]->ArcVertexCoordinateY[j+1] = G_M_CODE[LineNumber]->R_Start[1];
                  G_M_CODE[LineNumber]->VertexNumber = j+1;     
              } 
              if (endA == 2){
                  for (int i = v; i < G_M_CODE[LineNumber]->N; i++){
                          if (x[i] > G_M_CODE[LineNumber]->R_Start[0]){
                               start_1 = i; 
                               break;
                          }                                                         
                  }
                  for (int i = 0; i < v; i++){
                          if (x[i] < G_M_CODE[LineNumber]->R_End[0]){
                               end_1 = i; 
                               break;
                          }                                                        
                  }
                  G_M_CODE[LineNumber]->ArcVertexCoordinateX[0] = G_M_CODE[LineNumber]->R_End[0]; 
                  G_M_CODE[LineNumber]->ArcVertexCoordinateY[0] = G_M_CODE[LineNumber]->R_End[1];   
                  int j = 1;  
                  for(int i = end_1; i < v; i++){    
                      G_M_CODE[LineNumber]->ArcVertexCoordinateX[j] = x[i];
                      G_M_CODE[LineNumber]->ArcVertexCoordinateY[j] = y[i];
                      j++;
                  }
                  for(int i = v; i < start_1; i++){    
                      G_M_CODE[LineNumber]->ArcVertexCoordinateX[j] = x[i];
                      G_M_CODE[LineNumber]->ArcVertexCoordinateY[j] = y[i];
                      j++;
                  } 
                  G_M_CODE[LineNumber]->ArcVertexCoordinateX[j] = G_M_CODE[LineNumber]->R_Start[0];
                  G_M_CODE[LineNumber]->ArcVertexCoordinateY[j] = G_M_CODE[LineNumber]->R_Start[1];  
                  G_M_CODE[LineNumber]->ArcVertexCoordinateX[j+1] = G_M_CODE[LineNumber]->R_Start[0];
                  G_M_CODE[LineNumber]->ArcVertexCoordinateY[j+1] = G_M_CODE[LineNumber]->R_Start[1];
                  G_M_CODE[LineNumber]->VertexNumber = j+1;                       
              } 
              if (endA == 3){
                  if (G_M_CODE[LineNumber]->R_Start[0] > G_M_CODE[LineNumber]->R_End[0]){ //取值不超过象限 3,到达象限最大值时强制退出 
                         for (int i = v; i < G_M_CODE[LineNumber]->N; i++){
                              if (x[i] > G_M_CODE[LineNumber]->R_Start[0]){
                                   start_1 = i;
                                   break; 
                              }                                                         
                         }
                         for (int i = v; i < G_M_CODE[LineNumber]->N; i++){
                              if (x[i] > G_M_CODE[LineNumber]->R_End[0]){
                                   end_1 = i; 
                                   break;
                              }                                                        
                         }
                         G_M_CODE[LineNumber]->ArcVertexCoordinateX[0] = G_M_CODE[LineNumber]->R_End[0]; 
                         G_M_CODE[LineNumber]->ArcVertexCoordinateY[0] = G_M_CODE[LineNumber]->R_End[1];   
                         int j = 1;  
                         for(int i = end_1; i < start_1; i++){     
                              G_M_CODE[LineNumber]->ArcVertexCoordinateX[j] = x[i];
                              G_M_CODE[LineNumber]->ArcVertexCoordinateY[j] = y[i];
                              j++;   
                         }
                         G_M_CODE[LineNumber]->ArcVertexCoordinateX[j] = G_M_CODE[LineNumber]->R_Start[0];
                         G_M_CODE[LineNumber]->ArcVertexCoordinateY[j] = G_M_CODE[LineNumber]->R_Start[1];
                         G_M_CODE[LineNumber]->ArcVertexCoordinateX[j+1] = G_M_CODE[LineNumber]->R_Start[0];
                         G_M_CODE[LineNumber]->ArcVertexCoordinateY[j+1] = G_M_CODE[LineNumber]->R_Start[1];
                         G_M_CODE[LineNumber]->VertexNumber = j+1;
                  }else if(G_M_CODE[LineNumber]->R_Start[0] == G_M_CODE[LineNumber]->R_End[0]){
                     for (int i = v; i < G_M_CODE[LineNumber]->N; i++){
                          if (x[i] <= G_M_CODE[LineNumber]->R_Start[0])
                               start_1 = i;                                                        
                     }
                     end_1 = start_1;
                     G_M_CODE[LineNumber]->ArcVertexCoordinateX[0] = G_M_CODE[LineNumber]->R_End[0]; 
                     G_M_CODE[LineNumber]->ArcVertexCoordinateY[0] = G_M_CODE[LineNumber]->R_End[1];
                     int j = 1;
                     for (int i = end_1+1; i < G_M_CODE[LineNumber]->N; i++){
                          G_M_CODE[LineNumber]->ArcVertexCoordinateX[j] = x[i];
                          G_M_CODE[LineNumber]->ArcVertexCoordinateY[j] = y[i];
                          j++;
                     }
                     for(int i = 0; i < start_1+1; i++){    
                          G_M_CODE[LineNumber]->ArcVertexCoordinateX[j] = x[i];
                          G_M_CODE[LineNumber]->ArcVertexCoordinateY[j] = y[i];
                          j++;
                     }
                     G_M_CODE[LineNumber]->ArcVertexCoordinateX[j] = G_M_CODE[LineNumber]->R_Start[0];
                     G_M_CODE[LineNumber]->ArcVertexCoordinateY[j] = G_M_CODE[LineNumber]->R_Start[1];
                     G_M_CODE[LineNumber]->ArcVertexCoordinateX[j+1] = G_M_CODE[LineNumber]->R_Start[0];
                     G_M_CODE[LineNumber]->ArcVertexCoordinateY[j+1] = G_M_CODE[LineNumber]->R_Start[1];
                     G_M_CODE[LineNumber]->VertexNumber = j+1;
                  }else{ //取整圆,到达最大圆度值时强制退出  
                      for (int i = v; i < G_M_CODE[LineNumber]->N; i++){
                              if (x[i] > G_M_CODE[LineNumber]->R_Start[0]){
                                   start_1 = i; 
                                   break;
                              }                                                         
                      }
                      for (int i = v; i < G_M_CODE[LineNumber]->N; i++){
                              if (x[i] < G_M_CODE[LineNumber]->R_End[0]){
                                   end_1 = i; 
                              }                                                        
                      }
                      G_M_CODE[LineNumber]->ArcVertexCoordinateX[0] = G_M_CODE[LineNumber]->R_End[0]; 
                      G_M_CODE[LineNumber]->ArcVertexCoordinateY[0] = G_M_CODE[LineNumber]->R_End[1];   
                      int j = 1;  
                      for(int i = end_1+1; i < G_M_CODE[LineNumber]->N; i++){    
                          G_M_CODE[LineNumber]->ArcVertexCoordinateX[j] = x[i];
                          G_M_CODE[LineNumber]->ArcVertexCoordinateY[j] = y[i];
                          j++;
                      }
                      for(int i = 0; i < start_1; i++){    
                          G_M_CODE[LineNumber]->ArcVertexCoordinateX[j] = x[i];
                          G_M_CODE[LineNumber]->ArcVertexCoordinateY[j] = y[i];
                          j++;
                      } 
                      G_M_CODE[LineNumber]->ArcVertexCoordinateX[j] = G_M_CODE[LineNumber]->R_Start[0];
                      G_M_CODE[LineNumber]->ArcVertexCoordinateY[j] = G_M_CODE[LineNumber]->R_Start[1];  
                      G_M_CODE[LineNumber]->ArcVertexCoordinateX[j+1] = G_M_CODE[LineNumber]->R_Start[0];
                      G_M_CODE[LineNumber]->ArcVertexCoordinateY[j+1] = G_M_CODE[LineNumber]->R_Start[1];
                      G_M_CODE[LineNumber]->VertexNumber = j+1;
                  }     
              }   
              if (endA == 4){
                  for (int i = v; i < G_M_CODE[LineNumber]->N; i++){
                          if (x[i] > G_M_CODE[LineNumber]->R_Start[0]){
                               start_1 = i; 
                               break;
                          }                                                         
                  }
                  for (int i = v; i < G_M_CODE[LineNumber]->N; i++){
                          if (x[i] > G_M_CODE[LineNumber]->R_End[0]){
                               end_1 = i; 
                               break;
                          }                                                        
                  }
                  G_M_CODE[LineNumber]->ArcVertexCoordinateX[0] = G_M_CODE[LineNumber]->R_End[0]; 
                  G_M_CODE[LineNumber]->ArcVertexCoordinateY[0] = G_M_CODE[LineNumber]->R_End[1];   
                  int j = 1;  
                  for(int i = end_1; i < G_M_CODE[LineNumber]->N; i++){    
                      G_M_CODE[LineNumber]->ArcVertexCoordinateX[j] = x[i];
                      G_M_CODE[LineNumber]->ArcVertexCoordinateY[j] = y[i];
                      j++;
                  }
                  for(int i = 0; i < start_1; i++){    
                      G_M_CODE[LineNumber]->ArcVertexCoordinateX[j] = x[i];
                      G_M_CODE[LineNumber]->ArcVertexCoordinateY[j] = y[i];
                      j++;
                  } 
                  G_M_CODE[LineNumber]->ArcVertexCoordinateX[j] = G_M_CODE[LineNumber]->R_Start[0];
                  G_M_CODE[LineNumber]->ArcVertexCoordinateY[j] = G_M_CODE[LineNumber]->R_Start[1];  
                  G_M_CODE[LineNumber]->ArcVertexCoordinateX[j+1] = G_M_CODE[LineNumber]->R_Start[0];
                  G_M_CODE[LineNumber]->ArcVertexCoordinateY[j+1] = G_M_CODE[LineNumber]->R_Start[1];
                  G_M_CODE[LineNumber]->VertexNumber = j+1;                       
              }      
         } 
         if (startA == 4){
              if (endA == 1){
                 for (int i = v; i < G_M_CODE[LineNumber]->N; i++){
                      if (x[i] < G_M_CODE[LineNumber]->R_Start[0]){
                           start_1 = i; 
                      }                                                         
                  }
                  for (int i = 0; i < v; i++){
                          if (x[i] < G_M_CODE[LineNumber]->R_End[0]){
                               end_1 = i; 
                               break;
                          }                                                        
                  }
                  G_M_CODE[LineNumber]->ArcVertexCoordinateX[0] = G_M_CODE[LineNumber]->R_End[0]; 
                  G_M_CODE[LineNumber]->ArcVertexCoordinateY[0] = G_M_CODE[LineNumber]->R_End[1];   
                  int j = 1;  
                  for(int i = end_1; i < start_1; i++){    
                      G_M_CODE[LineNumber]->ArcVertexCoordinateX[j] = x[i];
                      G_M_CODE[LineNumber]->ArcVertexCoordinateY[j] = y[i];
                      j++;
                  } 
                  G_M_CODE[LineNumber]->ArcVertexCoordinateX[j] = G_M_CODE[LineNumber]->R_Start[0];
                  G_M_CODE[LineNumber]->ArcVertexCoordinateY[j] = G_M_CODE[LineNumber]->R_Start[1];  
                  G_M_CODE[LineNumber]->ArcVertexCoordinateX[j+1] = G_M_CODE[LineNumber]->R_Start[0];
                  G_M_CODE[LineNumber]->ArcVertexCoordinateY[j+1] = G_M_CODE[LineNumber]->R_Start[1];
                  G_M_CODE[LineNumber]->VertexNumber = j+1; 
              } 
              if (endA == 2){
                 for (int i = v; i < G_M_CODE[LineNumber]->N; i++){
                      if (x[i] < G_M_CODE[LineNumber]->R_Start[0]){
                           start_1 = i+1; 
                      }                                                         
                  }
                  for (int i = 0; i < v; i++){
                          if (x[i] > G_M_CODE[LineNumber]->R_End[0]){
                               end_1 = i+1; 
                          }                                                        
                  }
                  G_M_CODE[LineNumber]->ArcVertexCoordinateX[0] = G_M_CODE[LineNumber]->R_End[0]; 
                  G_M_CODE[LineNumber]->ArcVertexCoordinateY[0] = G_M_CODE[LineNumber]->R_End[1];   
                  int j = 1;  
                  for(int i = end_1; i < start_1; i++){    
                      G_M_CODE[LineNumber]->ArcVertexCoordinateX[j] = x[i];
                      G_M_CODE[LineNumber]->ArcVertexCoordinateY[j] = y[i];
                      j++;
                  } 
                  G_M_CODE[LineNumber]->ArcVertexCoordinateX[j] = G_M_CODE[LineNumber]->R_Start[0];
                  G_M_CODE[LineNumber]->ArcVertexCoordinateY[j] = G_M_CODE[LineNumber]->R_Start[1];  
                  G_M_CODE[LineNumber]->ArcVertexCoordinateX[j+1] = G_M_CODE[LineNumber]->R_Start[0];
                  G_M_CODE[LineNumber]->ArcVertexCoordinateY[j+1] = G_M_CODE[LineNumber]->R_Start[1];
                  G_M_CODE[LineNumber]->VertexNumber = j+1;
              } 
              if (endA == 3){
                  for (int i = v; i < G_M_CODE[LineNumber]->N; i++){
                      if (x[i] < G_M_CODE[LineNumber]->R_Start[0]){
                           start_1 = i+1; 
                      }                                                         
                  }
                  for (int i = v; i < G_M_CODE[LineNumber]->N; i++){
                          if (x[i] > G_M_CODE[LineNumber]->R_End[0]){
                               end_1 = i; 
                               break;
                          }                                                        
                  }
                  G_M_CODE[LineNumber]->ArcVertexCoordinateX[0] = G_M_CODE[LineNumber]->R_End[0]; 
                  G_M_CODE[LineNumber]->ArcVertexCoordinateY[0] = G_M_CODE[LineNumber]->R_End[1];   
                  int j = 1;  
                  for(int i = end_1; i < start_1; i++){    
                      G_M_CODE[LineNumber]->ArcVertexCoordinateX[j] = x[i];
                      G_M_CODE[LineNumber]->ArcVertexCoordinateY[j] = y[i];
                      j++;
                  } 
                  G_M_CODE[LineNumber]->ArcVertexCoordinateX[j] = G_M_CODE[LineNumber]->R_Start[0];
                  G_M_CODE[LineNumber]->ArcVertexCoordinateY[j] = G_M_CODE[LineNumber]->R_Start[1];  
                  G_M_CODE[LineNumber]->ArcVertexCoordinateX[j+1] = G_M_CODE[LineNumber]->R_Start[0];
                  G_M_CODE[LineNumber]->ArcVertexCoordinateY[j+1] = G_M_CODE[LineNumber]->R_Start[1];
                  G_M_CODE[LineNumber]->VertexNumber = j+1;
              }   
              if (endA == 4){
                  if (G_M_CODE[LineNumber]->R_Start[0] > G_M_CODE[LineNumber]->R_End[0]){ //取值不超过象限 4,到达象限最大值时强制退出 
                      for (int i = v; i < G_M_CODE[LineNumber]->N; i++){
                          if (x[i] < G_M_CODE[LineNumber]->R_Start[0]){
                               start_1 = i+1; 
                          }                                                         
                      }
                      for (int i = v; i < G_M_CODE[LineNumber]->N; i++){
                              if (x[i] < G_M_CODE[LineNumber]->R_End[0]){
                                   end_1 = i+1; 
                              }                                                        
                      }
                      G_M_CODE[LineNumber]->ArcVertexCoordinateX[0] = G_M_CODE[LineNumber]->R_End[0]; 
                      G_M_CODE[LineNumber]->ArcVertexCoordinateY[0] = G_M_CODE[LineNumber]->R_End[1];   
                      int j = 1;  
                      for(int i = end_1; i < start_1; i++){    
                          G_M_CODE[LineNumber]->ArcVertexCoordinateX[j] = x[i];
                          G_M_CODE[LineNumber]->ArcVertexCoordinateY[j] = y[i];
                          j++;
                      } 
                      G_M_CODE[LineNumber]->ArcVertexCoordinateX[j] = G_M_CODE[LineNumber]->R_Start[0];
                      G_M_CODE[LineNumber]->ArcVertexCoordinateY[j] = G_M_CODE[LineNumber]->R_Start[1];  
                      G_M_CODE[LineNumber]->ArcVertexCoordinateX[j+1] = G_M_CODE[LineNumber]->R_Start[0];
                      G_M_CODE[LineNumber]->ArcVertexCoordinateY[j+1] = G_M_CODE[LineNumber]->R_Start[1];
                      G_M_CODE[LineNumber]->VertexNumber = j+1;
                  }else if (G_M_CODE[LineNumber]->R_Start[0] == G_M_CODE[LineNumber]->R_End[0]){
                         for (int i = v; i < G_M_CODE[LineNumber]->N; i++){
                            if (x[i] < G_M_CODE[LineNumber]->R_Start[0])
                               start_1 = i;                                                        
                         }
                         end_1 = start_1;
                         G_M_CODE[LineNumber]->ArcVertexCoordinateX[0] = G_M_CODE[LineNumber]->R_End[0]; 
                         G_M_CODE[LineNumber]->ArcVertexCoordinateY[0] = G_M_CODE[LineNumber]->R_End[1];
                         int j = 1;
                         for (int i = end_1+1; i < G_M_CODE[LineNumber]->N; i++){
                              G_M_CODE[LineNumber]->ArcVertexCoordinateX[j] = x[i];
                              G_M_CODE[LineNumber]->ArcVertexCoordinateY[j] = y[i];
                              j++;
                         }
                         for(int i = 0; i < start_1+1; i++){    
                              G_M_CODE[LineNumber]->ArcVertexCoordinateX[j] = x[i];
                              G_M_CODE[LineNumber]->ArcVertexCoordinateY[j] = y[i];
                              j++;
                         }
                         G_M_CODE[LineNumber]->ArcVertexCoordinateX[j] = G_M_CODE[LineNumber]->R_Start[0];
                         G_M_CODE[LineNumber]->ArcVertexCoordinateY[j] = G_M_CODE[LineNumber]->R_Start[1];
                         G_M_CODE[LineNumber]->ArcVertexCoordinateX[j+1] = G_M_CODE[LineNumber]->R_Start[0];
                         G_M_CODE[LineNumber]->ArcVertexCoordinateY[j+1] = G_M_CODE[LineNumber]->R_Start[1];
                         G_M_CODE[LineNumber]->VertexNumber = j+1;
                  }else{ //取整圆,到达最大圆度值时强制退出  
                      for (int i = v; i < G_M_CODE[LineNumber]->N; i++){
                          if (x[i] < G_M_CODE[LineNumber]->R_Start[0]){
                               start_1 = i+1; 
                          }                                                         
                      }
                      for (int i = v; i < G_M_CODE[LineNumber]->N; i++){
                              if (x[i] < G_M_CODE[LineNumber]->R_End[0]){
                                   end_1 = i+1; 
                              }                                                        
                      }
                      G_M_CODE[LineNumber]->ArcVertexCoordinateX[0] = G_M_CODE[LineNumber]->R_End[0]; 
                      G_M_CODE[LineNumber]->ArcVertexCoordinateY[0] = G_M_CODE[LineNumber]->R_End[1];   
                      int j = 1;  
                      for(int i = end_1; i < G_M_CODE[LineNumber]->N; i++){    
                          G_M_CODE[LineNumber]->ArcVertexCoordinateX[j] = x[i];
                          G_M_CODE[LineNumber]->ArcVertexCoordinateY[j] = y[i];
                          j++;
                      } 
                      for(int i = 0; i < start_1; i++){    
                          G_M_CODE[LineNumber]->ArcVertexCoordinateX[j] = x[i];
                          G_M_CODE[LineNumber]->ArcVertexCoordinateY[j] = y[i];
                          j++;
                      }
                      G_M_CODE[LineNumber]->ArcVertexCoordinateX[j] = G_M_CODE[LineNumber]->R_Start[0];
                      G_M_CODE[LineNumber]->ArcVertexCoordinateY[j] = G_M_CODE[LineNumber]->R_Start[1];  
                      G_M_CODE[LineNumber]->ArcVertexCoordinateX[j+1] = G_M_CODE[LineNumber]->R_Start[0];
                      G_M_CODE[LineNumber]->ArcVertexCoordinateY[j+1] = G_M_CODE[LineNumber]->R_Start[1];
                      G_M_CODE[LineNumber]->VertexNumber = j+1;
                  }      
              }      
         }        
     }
     if (G_M_CODE[LineNumber]-> G03 == true){ //(逆时针) 
         if (startA == 1){
              if (endA == 1){
                  if (G_M_CODE[LineNumber]->R_Start[0] > G_M_CODE[LineNumber]->R_End[0]){ //取值不超过象限 1,到达象限最大值时强制退出 
                  }else{ //取整圆,到达最大圆度值时强制退出  
                  }     
              } 
              if (endA == 2){
              } 
              if (endA == 3){
              }   
              if (endA == 4){
              }       
         }  
         if (startA == 2){
              if (endA == 1){
              } 
              if (endA == 2){
                  if (G_M_CODE[LineNumber]->R_Start[0] > G_M_CODE[LineNumber]->R_End[0]){ //取值不超过象限 1,到达象限最大值时强制退出 
                  }else{ //取整圆,到达最大圆度值时强制退出  
                  }     
              } 
              if (endA == 3){
              }   
              if (endA == 4){
              }      
         } 
         if (startA == 3){
              if (endA == 1){
              } 
              if (endA == 2){
              } 
              if (endA == 3){
              }   
              if (endA == 4){
              }      
         } 
         if (startA == 4){
              if (endA == 1){
              } 
              if (endA == 2){
              } 
              if (endA == 3){
              }   
              if (endA == 4){
              }      
         }      
     }
}
//==============================================================================  
//==============================================================================  
//==============================================================================
//==========================================================================================================================
//==========================================================================================================================
//==========================================================================================================================
//==========================================================================================================================
/*                                    |
                      第二象限 X-     |       第一象限 X+
-------------------------------------------------------------------------------- 
                      第三象限        |       第四象限
                                      |
*/
/*void getUseVertexCoordinate(struct G_M_XYZ **G_M_CODE, int LineNumber, float *x, float *y){
     int VertexStart  = 0, VertexEnd = 0; //先通过中心点找出象限,起点 Y 比中心点大则上面,小则下面 
     int VertexNumber = 0;  
     int  Vertex = G_M_CODE[LineNumber]->N / 2;
     if (((G_M_CODE[LineNumber]->R_Start[0] > G_M_CODE[LineNumber]->CircularHeartX) 
       && (G_M_CODE[LineNumber]->R_Start[1] >= G_M_CODE[LineNumber]->CircularHeartY)) 
       ||((G_M_CODE[LineNumber]->R_Start[0] <= G_M_CODE[LineNumber]->CircularHeartX) 
       && (G_M_CODE[LineNumber]->R_Start[1] > G_M_CODE[LineNumber]->CircularHeartY))){
         for (int i = 0; i < Vertex; i++){ //1-2 象限
             if (G_M_CODE[LineNumber]->R_Start[0] >= x[i]){
                 VertexStart = i;
                 break;
             }
         }    
     }else if (((G_M_CODE[LineNumber]->R_Start[0] < G_M_CODE[LineNumber]->CircularHeartX) 
             && (G_M_CODE[LineNumber]->R_Start[1] <= G_M_CODE[LineNumber]->CircularHeartY))
             ||((G_M_CODE[LineNumber]->R_Start[0] >= G_M_CODE[LineNumber]->CircularHeartX) 
             && (G_M_CODE[LineNumber]->R_Start[1] < G_M_CODE[LineNumber]->CircularHeartY))){
         for (int i = Vertex; i < G_M_CODE[LineNumber]->N; i++){ //3-4 象限
             if (G_M_CODE[LineNumber]->R_Start[0] <= x[i]){
                 VertexStart = i;
                 break;
             }
         }                                         
     }     
     
     if (((G_M_CODE[LineNumber]->R_End[0] > G_M_CODE[LineNumber]->CircularHeartX) 
          && (G_M_CODE[LineNumber]->R_End[1] >= G_M_CODE[LineNumber]->CircularHeartY))
          || ((G_M_CODE[LineNumber]->R_End[0] <= G_M_CODE[LineNumber]->CircularHeartX)
          && (G_M_CODE[LineNumber]->R_End[1] > G_M_CODE[LineNumber]->CircularHeartY))){
         for (int i = Vertex; i > 0; i--){
             if (G_M_CODE[LineNumber]->R_End[0] < x[i]){
                 VertexEnd = i;
                 break;                                 
             } else {
                 VertexEnd = i;
             }      
         }                                
     }else if (((G_M_CODE[LineNumber]->R_End[0] < G_M_CODE[LineNumber]->CircularHeartX) 
             && (G_M_CODE[LineNumber]->R_End[1] <= G_M_CODE[LineNumber]->CircularHeartY))
             ||((G_M_CODE[LineNumber]->R_End[0] >= G_M_CODE[LineNumber]->CircularHeartX) 
             && (G_M_CODE[LineNumber]->R_End[1] < G_M_CODE[LineNumber]->CircularHeartY))){
          for (int i = G_M_CODE[LineNumber]->N - 1; i > Vertex; i--){
             if (G_M_CODE[LineNumber]->R_End[0] > x[i]){
                 VertexEnd = i;
                 break;                                 
             } else {
                 VertexEnd = i;
             }    
         }                                   
     }  
     //顶点坐标提取 
     int j = 0;   
     if (G_M_CODE[LineNumber]->G02 == true){
          G_M_CODE[LineNumber]-> ArcVertexCoordinateX[j] = G_M_CODE[LineNumber]->R_End[0]; 
          G_M_CODE[LineNumber]-> ArcVertexCoordinateY[j] = G_M_CODE[LineNumber]->R_End[1];
          j++;                          
          for (int i = VertexEnd + 1; i < G_M_CODE[LineNumber]->N; i++){                          
              G_M_CODE[LineNumber]-> ArcVertexCoordinateX[j] = x[i]; 
              G_M_CODE[LineNumber]-> ArcVertexCoordinateY[j] = y[i];
              j++;
          } 
          for (int i = 0; i < VertexStart; i++){                          
              G_M_CODE[LineNumber]-> ArcVertexCoordinateX[j] = x[i]; 
              G_M_CODE[LineNumber]-> ArcVertexCoordinateY[j] = y[i];
              j++;
          } 
          G_M_CODE[LineNumber]-> ArcVertexCoordinateX[j] = G_M_CODE[LineNumber]->R_Start[0]; 
          G_M_CODE[LineNumber]-> ArcVertexCoordinateY[j] = G_M_CODE[LineNumber]->R_Start[1];
          G_M_CODE[LineNumber]-> VertexNumber = j + 1;                       
     }
     else if (G_M_CODE[LineNumber]->G03 == true){
          G_M_CODE[LineNumber]-> ArcVertexCoordinateX[j] = G_M_CODE[LineNumber]->R_Start[0]; 
          G_M_CODE[LineNumber]-> ArcVertexCoordinateY[j] = G_M_CODE[LineNumber]->R_Start[1]; 
          j++;                      
          for (int i = VertexStart; i < VertexEnd + 1; i++){                          
              G_M_CODE[LineNumber]-> ArcVertexCoordinateX[j] = x[i]; 
              G_M_CODE[LineNumber]-> ArcVertexCoordinateY[j] = y[i];
              j++;
          } 
          G_M_CODE[LineNumber]-> ArcVertexCoordinateX[j] = G_M_CODE[LineNumber]->R_End[0]; 
          G_M_CODE[LineNumber]-> ArcVertexCoordinateY[j] = G_M_CODE[LineNumber]->R_End[1];  
          G_M_CODE[LineNumber]-> VertexNumber = j + 1;                      
     } 
}*/
//==========================================================================================================================
//==========================================================================================================================
//==========================================================================================================================
//==========================================================================================================================
/*取得圆弧的半径 
  I: 圆弧起点到圆心的 X 轴距离 
  J: 圆弧起点到圆心的 Y 轴距离 
 */
float getRadius(float I, float J){
      float R = 0;
      R = sqrt(I * I + J * J);
      return R;
} 
//==========================================================================================================================
//==========================================================================================================================
//==========================================================================================================================
//==========================================================================================================================
/*取得圆弧的坐标 
  X: 圆弧起点 X 坐标
  Y: 圆弧起点 Y 坐标               
  I: 圆弧起点到圆心的 X 轴距离,圆心位于圆弧起点 X 轴的正方向为正,负方向为负 
  J: 圆弧起点到圆心的 Y 轴距离,圆心位于圆弧起点 Y 轴的正方向为正,负方向为负
 */
float getCoordinateX(float X, float I){
      float C = 0;
      if ((I > 0) || (I < 0))
          C = X + I;
      else
          C = X;    
      return C;
} 

float getCoordinateY(float Y, float J){
      float C = 0;
      if ((J > 0) || (J < 0))
          C = Y + J;
      else
          C = Y; 
      return C;
} 
//==========================================================================================================================
//==========================================================================================================================
//==========================================================================================================================
//==========================================================================================================================
/*
  G_M_CODE: 指向结构体指针的指针变量 
  LineNumber: 行数                  
 */
FILE *pF = NULL; 
void VertexCoordinateCalculation(struct G_M_XYZ **G_M_CODE, int LineNumber){//顶点坐标计算 
    double PI = 3.1415926536f; 
    G_M_CODE[LineNumber]->R = getRadius(G_M_CODE[LineNumber]->I, G_M_CODE[LineNumber]->J); //取得半径 
    G_M_CODE[LineNumber]->N = getRoundness(G_M_CODE[LineNumber]->R, ArcChordLength(G_M_CODE, LineNumber)); //获取合适的 N
    if (G_M_CODE[LineNumber]->N < 20)
        G_M_CODE[LineNumber]->N = 40;
    G_M_CODE[LineNumber]->N = (G_M_CODE[LineNumber]->N / 4) * 4; 
    G_M_CODE[LineNumber]->CircularHeartX = getCoordinateX(G_M_CODE[LineNumber]->R_Start[0], G_M_CODE[LineNumber]->I); //圆心坐标 X 
    G_M_CODE[LineNumber]->CircularHeartY = getCoordinateY(G_M_CODE[LineNumber]->R_Start[1], G_M_CODE[LineNumber]->J); //圆心坐标 Y 
    float x[G_M_CODE[LineNumber]->N]; 
    float y[G_M_CODE[LineNumber]->N]; //两个临时数组 
    for (int i = 0; i < G_M_CODE[LineNumber]->N; i++){ //全圆顶点坐标计算 
        x[i] = G_M_CODE[LineNumber]->CircularHeartX + G_M_CODE[LineNumber]->R * cos(2 * PI / G_M_CODE[LineNumber]->N * i);
        y[i] = G_M_CODE[LineNumber]->CircularHeartY + G_M_CODE[LineNumber]->R * sin(2 * PI / G_M_CODE[LineNumber]->N * i);
    }
    /*char xx[25];
    xx[24] = 10;
    pF = fopen("C:\\Users\\Administrator\\Desktop\\dds.NC","ab+");
    for (int i = 0; i < G_M_CODE[LineNumber]->N; i++){
         gcvt(x[i],7,xx);
         fputs(xx,pF);
         fputs(",",pF);
         if (i == 9)
            fputs("\n",pF);
         if (i == 19)
            fputs("\n",pF);
         if (i == 29)
            fputs("\n",pF);
         if (i == 39)
            fputs("\n",pF);
    }*/
    //getUseVertexCoordinate(G_M_CODE, LineNumber, x, y); //提取需要的顶点数据 
    getUseVertexCoordinate(G_M_CODE, LineNumber, x, y); //提取需要的顶点数据 
} 
//==========================================================================================================================
//==========================================================================================================================
//==========================================================================================================================
//========================================================================================================================== 
/*● atof()：将字符串转换为双精度浮点型值
  ● atoi()：将字符串转换为整型值
  ● atol()：将字符串转换为长整型值
  ● strtod()：将字符串转换为双精度浮点型值,并报告不能被转换的所有剩余数字
  ● strtol()：将字符串转换为长整值,并报告不能被转换的所有剩余数字
  ● strtoul()：将字符串转换为无符号长整型值,并报告不能被转换的所有剩余数字
  
  ● itoa()：将整型值转换为字符串
  ● ltoa()：将长整型值转换为字符串
  ● ultoa()：将无符号长整型值转换为字符串
  ● gcvt()：将浮点型数转换为字符串，取四舍五入
  ● ecvt()：将双精度浮点型值转换为字符串，转换结果中不包含十进制小数点
  ● fcvt()：指定位数为转换精度，其余同ecvt()
  //printf("=======> %f\n",atof("-34.889"));    
 */
void G_CodeHandle(struct G_M_XYZ **G_M_CODE, int i, int Number){//G代码处理 
     switch (Number){
         case 0:G_M_CODE[i]->G00 = true;break; 
         case 1:G_M_CODE[i]->G01 = true;break; 
         case 2:G_M_CODE[i]->G02 = true;break; 
         case 3:G_M_CODE[i]->G03 = true;break; 
         case 4:G_M_CODE[i]->G04 = true;break; 
         case 5:G_M_CODE[i]->G05 = true;break; 
         case 6:G_M_CODE[i]->G06 = true;break;  
     }
}
//==========================================================================================================================
//==========================================================================================================================
//==========================================================================================================================
//==========================================================================================================================
void M_CodeHandle(struct G_M_XYZ **G_M_CODE, int i, int Number){//M代码处理 
     switch (Number){
          case 0:
                 break;  
     }
}
//==========================================================================================================================
//==========================================================================================================================
//==========================================================================================================================
//==========================================================================================================================
//指令数据提取 
/*
  TempCh    : 保存数据的临时数组 
  G_array   : 行数据 
  CharNumber: 行字符号  
  i         : 行号                      
 */ 
int InstructionsDataExtract(char *TempCh, char **G_array, int CharNumber, int i){
    int j = 0;
    for (int s = 0; s < sizeof(TempCh)/sizeof(TempCh[0]); s++) // 
          TempCh[s] = 0;
    CharNumber = CharNumber + 1;    
    while (((*(*(G_array + i) + CharNumber) > 47) 
         && (*(*(G_array + i) + CharNumber) < 58))   //'0'~'9'
         || (*(*(G_array + i) + CharNumber) == 46)   //'.'
         || (*(*(G_array + i) + CharNumber) == 45)   //'-'
         || (*(*(G_array + i) + CharNumber) == 32)){ //' '
            TempCh[j] = *(*(G_array + i) + CharNumber);
            j++;
            CharNumber++;
    }
    return CharNumber - 1;
}
//==========================================================================================================================
//==========================================================================================================================
//==========================================================================================================================
//==========================================================================================================================
//NC 文件解释器 
/*
  G_M_CODE:结构体变量 
  G_array :文件读取数组 
  g_v     :总行数 
  cncConfigure:工厂机台配置文件结构体 
  k:用户选中的机台号             
 */ 
void NcFileInterpreter(struct G_M_XYZ **G_M_CODE, char **G_array, int g_v, struct CNC_MachineNumber **cncConfigure, int k){
     //long LineNumber  = 0; //行号
     int  CharNumber  = 0;   //字符号 
     char TempCh[128];       //注意每一行都应有 XYZ 等数据
     char c;
     bool NullLine = false;
     for (int i = 0; i < g_v; i++){ 
         NullLine = false;        
         while (*(*(G_array + i) + CharNumber) != 10){
             c = *(*(G_array + i) + CharNumber);  
             if (c == '(' || c == '%'){  //注释和文件头的行去掉 
                 break; 
             }    
             else if (((c > 64) && (c < 91))  || ((c > 96) && (c < 123))){ //以字母开头                
                     switch(c){        
                        case 'G':CharNumber = InstructionsDataExtract(TempCh, G_array, CharNumber, i);
                                 G_CodeHandle(G_M_CODE, i, atoi(TempCh));
                                 break;
                        case 'M':CharNumber = InstructionsDataExtract(TempCh, G_array, CharNumber, i);
                                 M_CodeHandle(G_M_CODE, i, atoi(TempCh));
                                 break;  
                        case 'X':CharNumber = InstructionsDataExtract(TempCh, G_array, CharNumber, i);
                                 G_M_CODE[i]->X_End = atof(TempCh);
                                 G_M_CODE[i] -> x = true;
                                 NullLine = true;
                                 break;
                        case 'Y':CharNumber = InstructionsDataExtract(TempCh, G_array, CharNumber, i);
                                 G_M_CODE[i]->Y_End = atof(TempCh);
                                 G_M_CODE[i] -> y = true;
                                 NullLine = true;
                                 break;
                        case 'Z':CharNumber = InstructionsDataExtract(TempCh, G_array, CharNumber, i);
                                 G_M_CODE[i]->Z_End = atof(TempCh);
                                 G_M_CODE[i] -> z = true;
                                 NullLine = true;
                                 break;
                        case 'A':CharNumber = InstructionsDataExtract(TempCh, G_array, CharNumber, i);
                                 G_M_CODE[i]->A_End  = atof(TempCh);
                                 G_M_CODE[i]->a = true;
                                 NullLine = true;
                                 break;
                        case 'I':CharNumber = InstructionsDataExtract(TempCh, G_array, CharNumber, i);
                                 G_M_CODE[i]->I = atof(TempCh);
                                 break; 
                        case 'J':CharNumber = InstructionsDataExtract(TempCh, G_array, CharNumber, i);
                                 G_M_CODE[i]->J = atof(TempCh);
                                 break;
                        case 'R':CharNumber = InstructionsDataExtract(TempCh, G_array, CharNumber, i);
                                 G_M_CODE[i]->R = atof(TempCh);
                                 break; 
                        case 'T':CharNumber = InstructionsDataExtract(TempCh, G_array, CharNumber, i);
                                 G_M_CODE[i]->T = (int)atof(TempCh);
                                 break;                              
                        case 'H':CharNumber = InstructionsDataExtract(TempCh, G_array, CharNumber, i);
                                 G_M_CODE[i]->H = (int)atof(TempCh);
                                 break;
                        case 'F':CharNumber = InstructionsDataExtract(TempCh, G_array, CharNumber, i);
                                 G_M_CODE[i]->F = atof(TempCh);
                                 G_M_CODE[i]->f = true;
                                 break;  
                        case 'S':CharNumber = InstructionsDataExtract(TempCh, G_array, CharNumber, i);
                                 G_M_CODE[i]->S = atof(TempCh);
                                 G_M_CODE[i]->s = true;
                                 break; 
                        //default: break;                         
                     }
             }
             else if (c > 47 && c < 58){ //以数字开头 
                  if (CharNumber >= 254)
                     break; 
             } 
             else if (c == 32){ //以空格开头 
                  if (CharNumber >= 254)
                     break; 
             }
             CharNumber++;
         } //while
         CharNumber = 0;

         if (i == 0){
             G_M_CODE[i]->X_Start = 0;
             G_M_CODE[i]->Y_Start = 0;             
             G_M_CODE[i]->A_Start = 0;
             G_M_CODE[i]->X_End   = 0;
             G_M_CODE[i]->Y_End   = 0;       
             G_M_CODE[i]->A_End   = 0; 
             if (CncConfigureFile_YES_NO == true){
                  G_M_CODE[i]->Z_Start = atof(cncConfigure[k]->z);
                  G_M_CODE[i]->Z_End   = atof(cncConfigure[k]->z);
             }else{
                  G_M_CODE[i]->Z_Start = 500;
                  G_M_CODE[i]->Z_End   = 500; 
             }     
             G_M_CODE[i]->G00 = true;
         }else{
             if (G_M_CODE[i]->x == false)  //检查一行中没有给出的指令,没有给出的将上一行的数据写入当前行,如果 X 或 Y,G 之后没有数据则会是 0  
                 G_M_CODE[i]->X_End = G_M_CODE[i-1]->X_End;
             if (G_M_CODE[i]->y == false)   
                 G_M_CODE[i]->Y_End = G_M_CODE[i-1]->Y_End;
             if (G_M_CODE[i]->z == false)   
                 G_M_CODE[i]->Z_End = G_M_CODE[i-1]->Z_End;
             if (G_M_CODE[i]->a == false)  
                 G_M_CODE[i]->A_End = G_M_CODE[i-1]->A_End;
             G_M_CODE[i]->X_Start = G_M_CODE[i-1]->X_End;
             G_M_CODE[i]->Y_Start = G_M_CODE[i-1]->Y_End;
             G_M_CODE[i]->Z_Start = G_M_CODE[i-1]->Z_End;
             G_M_CODE[i]->A_Start = G_M_CODE[i-1]->A_End; 
             if ((G_M_CODE[i]->G00==false)&&(G_M_CODE[i]->G01==false)&&(G_M_CODE[i]->G02==false)&&(G_M_CODE[i]->G03==false)){
                 if (G_M_CODE[i-1]->G00 == true) 
                      G_M_CODE[i]->G00 = G_M_CODE[i-1]->G00;  
                 if (G_M_CODE[i-1]->G01 == true) 
                      G_M_CODE[i]->G01 = G_M_CODE[i-1]->G01;
                 if (G_M_CODE[i-1]->G02 == true) 
                      G_M_CODE[i]->G02 = G_M_CODE[i-1]->G02;
                 if (G_M_CODE[i-1]->G03 == true) 
                      G_M_CODE[i]->G03 = G_M_CODE[i-1]->G03;                                                                                                                  
             }        
         }          
         if (G_M_CODE[i]->X_End < 0){ //找出最大的边界 
           if (windowsSize < (NegativePositiveMutualTransformation(G_M_CODE[i]->X_End)))
              windowsSize = (int)NegativePositiveMutualTransformation(G_M_CODE[i]->X_End);
         }else{
           if (windowsSize < G_M_CODE[i]->X_End)
              windowsSize = (int)G_M_CODE[i]->X_End;    
         }  
         if (G_M_CODE[i]->Y_End < 0){ 
           if (windowsSize < (NegativePositiveMutualTransformation(G_M_CODE[i]->Y_End)))
              windowsSize = (int)NegativePositiveMutualTransformation(G_M_CODE[i]->Y_End);
         }else{
           if (windowsSize < G_M_CODE[i]->Y_End)
              windowsSize = (int)G_M_CODE[i]->Y_End;
         } 
         if (NullLine == true)
             G_M_CODE[i] -> NullLine = true;
         else 
             G_M_CODE[i] -> NullLine = false; 
         if ((G_M_CODE[i] ->G02 == true) || (G_M_CODE[i] -> G03 == true)){            
             G_M_CODE[i] ->R_Start[0] = G_M_CODE[i-1]->X_End; 
             G_M_CODE[i] ->R_Start[1] = G_M_CODE[i-1]->Y_End; 
             G_M_CODE[i] ->R_End[0] = G_M_CODE[i]->X_End; 
             G_M_CODE[i] ->R_End[1] = G_M_CODE[i]->Y_End;               
             VertexCoordinateCalculation(G_M_CODE, i);  
         }      
     } //for    
}
//==========================================================================================================================
//==========================================================================================================================
//==========================================================================================================================
//==========================================================================================================================
void CNC_InterpreterMemoryAllocation(){  //依据文件总的行数分配内存 
    int n = 0;
    G_M_CODE = (struct G_M_XYZ **)malloc(g_v * sizeof(struct G_M_XYZ *));
    for (int i = 0; i < g_v; i++)
       G_M_CODE[i] = (struct G_M_XYZ*)malloc((sizeof(struct G_M_XYZ)));  
}
//==========================================================================================================================
//==========================================================================================================================
//==========================================================================================================================
//==========================================================================================================================
void InterpreterMemoryRelease(){ //释放分配内存 
    for (int i = 0; i < g_v; i++)
       free(G_M_CODE[i]);  /*释放列*/
    free(G_M_CODE);        /*释放行*/ 
    //g_v = 0;  
}
//==========================================================================================================================
//==========================================================================================================================
//==========================================================================================================================
//========================================================================================================================== 
/*  //--------------------------------------------------------------------------
    struct G_M_XYZ ** G_M_CODE = NULL;  //定义二重指向结构指针的指针变量 
    G_M_CODE = (struct G_M_XYZ**)malloc((g_m * sizeof(struct G_M_XYZ*))); //为第一重指针分配内存 
    for (int i = 0; i < g_m; i++)
       G_M_CODE[i] = (struct G_M_XYZ*)malloc((sizeof(struct G_M_XYZ)));   //为第二重指针分配内存 
    //--------------------------------------------------------------------------    
    G_M_CODE[0][0].F = 100.0f;
    G_M_CODE[1]->F   = 200;
    //--------------------------------------------------------------------------
    for (int i = 0; i < g_m; i++) //释放分配的内存 
       free(G_M_CODE[i]);         //释放列     
    free(G_M_CODE);               //释放行
    //--------------------------------------------------------------------------
*/    
//==========================================================================================================================
//==========================================================================================================================
//==========================================================================================================================
//==========================================================================================================================                
void StructInit(struct G_M_XYZ ** G_M_CODE, int g_v){
     for (int i = 0; i < g_v; i++){
            G_M_CODE[i] -> G00 = false;  //快速定位
            G_M_CODE[i] -> G01 = false;  //直线插补
            G_M_CODE[i] -> G02 = false;  //顺时针方向圆弧插补
            G_M_CODE[i] -> G03 = false;  //逆时针方向圆弧插补
            G_M_CODE[i] -> G04 = false;  //定时暂停
            G_M_CODE[i] -> G05 = false;  //通过中间点圆弧插补
            G_M_CODE[i] -> G06 = false;  //抛物线插补
            G_M_CODE[i] -> G07 = false;  //Z 样条曲线插补
            G_M_CODE[i] -> G08 = false;  //进给加速
            G_M_CODE[i] -> G09 = false;  //进给减速
            G_M_CODE[i] -> G10 = false;  //数据设置
            G_M_CODE[i] -> G16 = false;  //极坐标编程
            G_M_CODE[i] -> G17 = false;  //加工XY平面
            G_M_CODE[i] -> G18 = false;  //加工XZ平面
            G_M_CODE[i] -> G19 = false;  //加工YZ平面
            G_M_CODE[i] -> G20 = false;  //英制尺寸(法兰克系统)
            G_M_CODE[i] -> G21 = false;  //公制尺寸(法兰克系统)
            G_M_CODE[i] -> G22 = false;  //半径尺寸编程方式
            G_M_CODE[i] -> G220 = false; //系统操作界面上使用
            G_M_CODE[i] -> G23  = false; //直径尺寸编程方式
            G_M_CODE[i] -> G230 = false; //系统操作界面上使用
            G_M_CODE[i] -> G24 = false;  //子程序结束
            G_M_CODE[i] -> G25 = false;  //跳转加工
            G_M_CODE[i] -> G26 = false;  //循环加工
            G_M_CODE[i] -> G30 = false;  //倍率注销
            G_M_CODE[i] -> G31 = false;  //倍率定义
            G_M_CODE[i] -> G32 = false;  //等螺距螺纹切削,英制
            G_M_CODE[i] -> G33 = false;  //等螺距螺纹切削,公制
            G_M_CODE[i] -> G34 = false;  //增螺距螺纹切削
            G_M_CODE[i] -> G35 = false;  //减螺距螺纹切削
            G_M_CODE[i] -> G40 = false;  //刀具补偿/刀具偏置注销
            G_M_CODE[i] -> G41 = false;  //刀具补偿——左
            G_M_CODE[i] -> G42 = false;  //刀具补偿——右
            G_M_CODE[i] -> G43 = false;  //刀具偏置——正
            G_M_CODE[i] -> G44 = false;  //刀具偏置——负
            G_M_CODE[i] -> G45 = false;  //刀具偏置+/+
            G_M_CODE[i] -> G46 = false;  //刀具偏置+/-
            G_M_CODE[i] -> G47 = false;  //刀具偏置-/-
            G_M_CODE[i] -> G48 = false;  //刀具偏置-/+
            G_M_CODE[i] -> G49 = false;  //刀具偏置0/+
            G_M_CODE[i] -> G50 = false;  //刀具偏置0/-
            G_M_CODE[i] -> G51 = false;  //刀具偏置+/0
            G_M_CODE[i] -> G52 = false;  //刀具偏置-/0
            G_M_CODE[i] -> G53 = false;  //直线偏移,注销
            G_M_CODE[i] -> G54 = false;  //直线偏移 x
            G_M_CODE[i] -> G55 = false;  //直线偏移 y
            G_M_CODE[i] -> G56 = false;  //直线偏移 z
            G_M_CODE[i] -> G57 = false;  //直线偏移 xy
            G_M_CODE[i] -> G58 = false;  //直线偏移 xz
            G_M_CODE[i] -> G59 = false;  //直线偏移 yz
            G_M_CODE[i] -> G60 = false;  //准确路径方式(精)
            G_M_CODE[i] -> G61 = false;  //准确路径方式(中)
            G_M_CODE[i] -> G62 = false;  //准确路径方式(粗)
            G_M_CODE[i] -> G63 = false;  //攻螺纹
            G_M_CODE[i] -> G68 = false;  //刀具偏置,内角
            G_M_CODE[i] -> G69 = false;  //刀具偏置,外角
            G_M_CODE[i] -> G70 = false;  //英制尺寸(这个是西门子的,法兰克的是 G21)
            G_M_CODE[i] -> G71 = false;  //公制尺寸 毫米
            G_M_CODE[i] -> G74 = false;  //回参考点(机床零点)
            G_M_CODE[i] -> G75 = false;  //返回编程坐标零点
            G_M_CODE[i] -> G76 = false;  //车螺纹复合循环
            G_M_CODE[i] -> G80 = false;  //固定循环注销
            G_M_CODE[i] -> G81 = false;  //外圆固定循环
            G_M_CODE[i] -> G83 = false;  //
            G_M_CODE[i] -> G84 = false;  //
            G_M_CODE[i] -> G331 = false; //螺纹固定循环
            G_M_CODE[i] -> G90 = false;  //绝对尺寸
            G_M_CODE[i] -> G91 = false;  //相对尺寸
            G_M_CODE[i] -> G92 = false;  //预制坐标
            G_M_CODE[i] -> G93 = false;  //时间倒数,进给率
            G_M_CODE[i] -> G94 = false;  //进给率,每分钟进给
            G_M_CODE[i] -> G95 = false;  //进给率,每转进给
            G_M_CODE[i] -> G96 = false;  //恒线速度控制
            G_M_CODE[i] -> G97 = false;  //取消恒线速度控制
            
            G_M_CODE[i] -> Selection = false; //被选中 
            
            G_M_CODE[i] -> M00 = false;  // 程序停止
            G_M_CODE[i] -> M01 = false;  // 任选停止
            G_M_CODE[i] -> M02 = false;  // 程序结束
            G_M_CODE[i] -> M03 = false;  // 工作主轴起动 (正转)
            G_M_CODE[i] -> M04 = false;  // 工作主轴起动 (反转)
            G_M_CODE[i] -> M05 = false;  // 主轴停止
            G_M_CODE[i] -> M06 = false;  // 刀具交换
            G_M_CODE[i] -> M07 = false;  //
            G_M_CODE[i] -> M08 = false;  // 冷却液开
            G_M_CODE[i] -> M09 = false;  // 冷却液关
            G_M_CODE[i] -> M10 = false;  // 主轴点动关
            G_M_CODE[i] -> M11 = false;  // 主轴点动开
            G_M_CODE[i] -> M12 = false;  // 动力刀具轴停止
            G_M_CODE[i] -> M13 = false;  // 动力刀具轴正转
            G_M_CODE[i] -> M14 = false;  // 动力刀具轴反转
            G_M_CODE[i] -> M15 = false;  // C轴正向定位
            G_M_CODE[i] -> M16 = false;  // C轴反向定位
            G_M_CODE[i] -> M17 = false;  // 机外测量数据通过RS232C传送请求
            G_M_CODE[i] -> M18 = false;  // 主轴定向取消
            G_M_CODE[i] -> M19 = false;  // 主轴定向
            G_M_CODE[i] -> M20 = false;  // 尾架干涉区或主轴干涉监视关(对面双主轴规格)
            G_M_CODE[i] -> M21 = false;  // 尾架干涉区或主轴干涉监视开(对面双主轴规格)
            G_M_CODE[i] -> M22 = false;  // 倒角关
            G_M_CODE[i] -> M23 = false;  // 倒角开
            G_M_CODE[i] -> M24 = false;  // 卡盘干涉区关,刀具干涉区关
            G_M_CODE[i] -> M25 = false;  // 卡盘干涉区开,刀具干涉区开
            G_M_CODE[i] -> M26 = false;  // 螺纹导程有效轴Z轴指定
            G_M_CODE[i] -> M27 = false;  // 螺纹导程有效轴X轴指定
            G_M_CODE[i] -> M28 = false;  // 刀具干涉检查功能关
            G_M_CODE[i] -> M29 = false;  // 刀具干涉检查功能开
            G_M_CODE[i] -> M30 = false;  // 程序结束
            G_M_CODE[i] -> M31 = false;  //
            G_M_CODE[i] -> M32 = false;  // 螺纹车削单面切削模式
            G_M_CODE[i] -> M33 = false;  // 螺纹车削时交叉切削模式
            G_M_CODE[i] -> M34 = false;  // 螺纹车削逆向单面切削模式
            G_M_CODE[i] -> M35 = false;  // 装料器夹持器Z向滑动后退
            G_M_CODE[i] -> M36 = false;  // 装料器夹持器Z向滑动前进
            G_M_CODE[i] -> M37 = false;  // 装料器臂后退
            G_M_CODE[i] -> M38 = false;  // 装料器臂前进到卸载位置
            G_M_CODE[i] -> M39 = false;  // 装料器臂前进到卡盘位置
            G_M_CODE[i] -> M40 = false;  // 主轴齿轮空档
            G_M_CODE[i] -> M41 = false;  // 主轴齿轮1档或底速线圈
            G_M_CODE[i] -> M42 = false;  // 主轴齿轮2档或高速线圈
            G_M_CODE[i] -> M43 = false;  // 主轴齿轮3档
            G_M_CODE[i] -> M44 = false;  // 主轴齿轮4档
            G_M_CODE[i] -> M45 = false;  //
            G_M_CODE[i] -> M46 = false;  //
            G_M_CODE[i] -> M47 = false;  //
            G_M_CODE[i] -> M48 = false;  // 主轴转速倍率无效取消
            G_M_CODE[i] -> M49 = false;  // 主轴转速倍率无效
            G_M_CODE[i] -> M50 = false;  // 附加吹气口1关
            G_M_CODE[i] -> M51 = false;  // 附加吹气口1开
            G_M_CODE[i] -> M52 = false;  //
            G_M_CODE[i] -> M53 = false;  //
            G_M_CODE[i] -> M54 = false;  // 分度卡盘自动分度
            G_M_CODE[i] -> M55 = false;  // 尾架后退
            G_M_CODE[i] -> M56 = false;  // 尾架前进
            G_M_CODE[i] -> M57 = false;  // M63取消
            G_M_CODE[i] -> M58 = false;  // 卡盘底压
            G_M_CODE[i] -> M59 = false;  // 卡盘高压
            G_M_CODE[i] -> M60 = false;  // M61取消
            G_M_CODE[i] -> M61 = false;  // 圆周速度恒定切削时,恒定旋转应答忽视
            G_M_CODE[i] -> M62 = false;  // M64取消
            G_M_CODE[i] -> M63 = false;  // 主轴旋转M码应答忽视
            G_M_CODE[i] -> M64 = false;  // 主轴旋转之外的M码应答忽视
            G_M_CODE[i] -> M65 = false;  // T码应答忽视
            G_M_CODE[i] -> M66 = false;  // 刀架回转位置自由
            G_M_CODE[i] -> M67 = false;  // 凸轮车削循环中同步运行模式取消
            G_M_CODE[i] -> M68 = false;  // 同步模式A运行开
            G_M_CODE[i] -> M69 = false;  // 同步模式B运行开
            G_M_CODE[i] -> M70 = false;  // 手动换到指令
            G_M_CODE[i] -> M71 = false;  //
            G_M_CODE[i] -> M72 = false;  // ATC单元定位在接近位置
            G_M_CODE[i] -> M73 = false;  // 螺纹车削类型 1
            G_M_CODE[i] -> M74 = false;  // 螺纹车削类型 2
            G_M_CODE[i] -> M75 = false;  // 螺纹车削类型 3
            G_M_CODE[i] -> M76 = false;  // 工件捕手后退
            G_M_CODE[i] -> M77 = false;  // 工件捕手前进
            G_M_CODE[i] -> M78 = false;  // 中心架松开
            G_M_CODE[i] -> M79 = false;  // 中心架夹紧
            G_M_CODE[i] -> M80 = false;  // 过切前进
            G_M_CODE[i] -> M81 = false;  // 过切后退
            G_M_CODE[i] -> M82 = false;  //
            G_M_CODE[i] -> M83 = false;  // 卡盘夹紧
            G_M_CODE[i] -> M84 = false;  // 卡盘松开
            G_M_CODE[i] -> M85 = false;  // LAP粗车循环后不返回起始位置
            G_M_CODE[i] -> M86 = false;  // 刀架右回转指定
            G_M_CODE[i] -> M87 = false;  // M86取消
            G_M_CODE[i] -> M88 = false;  // 吹气关
            G_M_CODE[i] -> M89 = false;  // 吹气开
            G_M_CODE[i] -> M90 = false;  // 关门
            G_M_CODE[i] -> M91 = false;  // 开门
            G_M_CODE[i] -> M92 = false;  // 棒料进给器后退
            G_M_CODE[i] -> M93 = false;  // 棒料进给器前进
            G_M_CODE[i] -> M94 = false;  // 装料器装料
            G_M_CODE[i] -> M95 = false;  // 装料器卸料
            G_M_CODE[i] -> M96 = false;  // 副轴用工件捕手后退
            G_M_CODE[i] -> M97 = false;  // 副轴用工件捕手前进
            G_M_CODE[i] -> M98 = false;  // 尾架低压
            G_M_CODE[i] -> M99 = false;  // 尾架高压
            
            G_M_CODE[i] -> x = false;;
            G_M_CODE[i] -> y = false;;
            G_M_CODE[i] -> z = false;;
            G_M_CODE[i] -> a = false;;
            G_M_CODE[i] -> s = false;;
            G_M_CODE[i] -> f = false;;
            
            G_M_CODE[i] -> X_Start = 0;     //每行的 X 坐标值 
            G_M_CODE[i] -> X_End   = 0;     //每行的 X 坐标值 
            G_M_CODE[i] -> Y_Start = 0;     //每行的 Y 坐标值  
            G_M_CODE[i] -> Y_End   = 0;     //每行的 Y 坐标值  
            G_M_CODE[i] -> Z_Start = 0;     //每行的 Z 坐标值 
            G_M_CODE[i] -> Z_End   = 0;     //每行的 Z 坐标值 
            G_M_CODE[i] -> S = 0;     //每行的转速值 
            G_M_CODE[i] -> F = 0;     //每行的进给值
            G_M_CODE[i] -> zHigh = 0; //Z 背吃刀量 
            G_M_CODE[i] -> H = 0;     //长度补正号 
            G_M_CODE[i] -> T = 0;     //刀号 
            G_M_CODE[i] -> R = 0;     //半径 
            G_M_CODE[i] -> R_Start[0] = 0;   //圆的起始坐标 
            G_M_CODE[i] -> R_End[0]   = 0;   //圆的终止坐标 
            G_M_CODE[i] -> R_Start[1] = 0;   //圆的起始坐标 
            G_M_CODE[i] -> R_End[1]   = 0;   //圆的终止坐标 
            G_M_CODE[i] -> CircularHeartX = 0; //圆心的 X 坐标 
            G_M_CODE[i] -> CircularHeartY = 0; //圆心的 Y 坐标 
            G_M_CODE[i] -> I = 0; //
            G_M_CODE[i] -> J = 0; //
            G_M_CODE[i] -> N = 0; //圆度值 
            G_M_CODE[i] -> VertexNumber = 0;   //实际要使用的顶点数 
            for (int j = 0; j < 1024; j++){
               G_M_CODE[i] -> ArcVertexCoordinateX[j] = 0; //圆的顶点坐标 
               G_M_CODE[i] -> ArcVertexCoordinateY[j] = 0; //圆的顶点坐标 
            }
            G_M_CODE[i] -> color[0]  = 0;     //颜色 
            G_M_CODE[i] -> color[1]  = 0;     //颜色 
            G_M_CODE[i] -> color[2]  = 0;     //颜色 
            G_M_CODE[i] -> color[3]  = 0;     //颜色 
            G_M_CODE[i] -> LineWidth = 0;     //线宽 
            G_M_CODE[i] -> NullLine = false;
     }
}
//==========================================================================================================================
//==========================================================================================================================
//==========================================================================================================================
//==========================================================================================================================

