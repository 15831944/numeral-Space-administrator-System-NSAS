#include <GL/freeglut.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <conio.h>
#include "NcFileInterpreter.h"
#include "main.h"
#include "color.h"

struct G_M_XYZ ** G_M_CODE = NULL;  //�������ָ��ṹָ���ָ����� 
//============================================================================== 
//==============================================================================  
//==============================================================================  
//==============================================================================  
//==============================================================================
//==============================================================================
/* NC �����Ϊ�����ṩ, R �뾶, (I,J ������뾶), ���� XY ����, �յ�� XY ���� 
   �Լ� G02,G03 Բ���ķ��� 
   1:�������Ժ��ʵ� n �������Բ�Ķ������е�����ֵ
   2:���������յ�����,�� G02,G03 ����ָ��,��ȡ��Ҫ�Ķ�������
   3:��������ڻ�е������ԵĹ�������ϵ�� X,Y ����ʵ�ֻ��� 
   
   
   ��һ��:���ļ���������,��ȷ������
   �ڶ���:�ú��ʵĽ�������������ȡ����,H,F,S,G,Mֵ�Ȳ�����ṹ���Ӧ���� 
   ������:��������ļ������ȡ 
   ���Ĳ�:���ṹ���е���Ϣ���л�����ʾ,���� 
   ���岽:���ļ���ʾ�ͱ༭�޸�,�ര�� 
 */
//==========================================================================================================================
//==========================================================================================================================
//==========================================================================================================================
//==========================================================================================================================
//ͨ��Բ�������,�յ���������ҳ��ͽǶ� 
/*
 G_M_CODE:�ṹ��ָ��
 LineNumber:�к� 
 ���� 
 */
extern int xxxx, yyyy, zzzz, nnnn; 
float ArcChordLength(struct G_M_XYZ **G_M_CODE, int LineNumber){
     float SmallAngle  = 0;
     float ChordLength = 0;
     //float BigAngle    = 0;
     ChordLength = sqrt((G_M_CODE[LineNumber]->R_Start[0] - G_M_CODE[LineNumber]->R_End[0])\
                       *(G_M_CODE[LineNumber]->R_Start[0] - G_M_CODE[LineNumber]->R_End[0])\
                       +(G_M_CODE[LineNumber]->R_Start[1] - G_M_CODE[LineNumber]->R_End[1])\
                       *(G_M_CODE[LineNumber]->R_Start[1] - G_M_CODE[LineNumber]->R_End[1]));      //�ҳ� 
     
     SmallAngle = (asin((ChordLength / 2) / G_M_CODE[LineNumber]->R) * 180.0 / 3.1415926536) * 2;  //С��
     //BigAngle   =  360 - SmallAngle;      //��� 
     //if (SmallAngle < 1.0)
        //SmallAngle = 5.1;
     return SmallAngle;
}
//==========================================================================================================================
//==========================================================================================================================
//==========================================================================================================================
//==========================================================================================================================
//����Բ�� R ��С��ȡ���ʵ� N ֵ(Բ��ֵ) 
/*
  
 */
int getRoundness(float R, float Angle){
     float ArcLength, Perimeter;     
     int rn = 40, n = 40;
     int dd = 0;
     if (Angle != 0){ //����Ƕ�Ϊ 0 ʱ�ᷢ�������ǳ���������� 
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
/*���㸺��������֮����໥ת�� 
  TransformationNumber:Ҫת���ĸ����� 
  ����ֵ:������ת������
 */
float NegativePositiveMutualTransformation(float TransformationNumber){
      float Temp = 0;
      Temp = TransformationNumber - TransformationNumber - TransformationNumber;
      return Temp;
}
//============================================================================== 
//==============================================================================

int Start(struct G_M_XYZ **G_M_CODE, int LineNumber){      
       if ((G_M_CODE[LineNumber]->R_Start[0] > G_M_CODE[LineNumber]->CircularHeartX)       && (G_M_CODE[LineNumber]->R_Start[1] >= G_M_CODE[LineNumber]->CircularHeartY)) //һ����              
              return 1;
       else if ((G_M_CODE[LineNumber]->R_Start[0] <= G_M_CODE[LineNumber]->CircularHeartX) && (G_M_CODE[LineNumber]->R_Start[1] > G_M_CODE[LineNumber]->CircularHeartY))//������
              return 2;    
       else if ((G_M_CODE[LineNumber]->R_Start[0] < G_M_CODE[LineNumber]->CircularHeartX)  && (G_M_CODE[LineNumber]->R_Start[1] <= G_M_CODE[LineNumber]->CircularHeartY)) //������
              return 3;
       else if ((G_M_CODE[LineNumber]->R_Start[0] >= G_M_CODE[LineNumber]->CircularHeartX) && (G_M_CODE[LineNumber]->R_Start[1] < G_M_CODE[LineNumber]->CircularHeartY))//������
              return 4; 
       else
              return -11;       
}                                                          
int End(struct G_M_XYZ **G_M_CODE, int LineNumber){
       if ((G_M_CODE[LineNumber]->R_End[0] >  G_M_CODE[LineNumber]->CircularHeartX)      && (G_M_CODE[LineNumber]->R_End[1] >= G_M_CODE[LineNumber]->CircularHeartY)) //һ����
            return 1;
       else if ((G_M_CODE[LineNumber]->R_End[0] <= G_M_CODE[LineNumber]->CircularHeartX) && (G_M_CODE[LineNumber]->R_End[1] >  G_M_CODE[LineNumber]->CircularHeartY)) //������
            return 2;                   
       else if ((G_M_CODE[LineNumber]->R_End[0] <  G_M_CODE[LineNumber]->CircularHeartX) && (G_M_CODE[LineNumber]->R_End[1] <= G_M_CODE[LineNumber]->CircularHeartY)) //������
            return 3;
       else if ((G_M_CODE[LineNumber]->R_End[0] >= G_M_CODE[LineNumber]->CircularHeartX) && (G_M_CODE[LineNumber]->R_End[1] <  G_M_CODE[LineNumber]->CircularHeartY)) //������  
            return 4; 
       else 
            return -11;                                        
} 

//�������,����,����,�������,������ȡ 
void getUseVertexCoordinate(struct G_M_XYZ **G_M_CODE, int LineNumber, float *x, float *y){ //��ȡ��Ҫ�Ķ������� 
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
     if (G_M_CODE[LineNumber]-> G02 == true){ //(˳ʱ��)      
         if (startA == 1){ //ע��:ÿ�������ǲ�һ����,������ʲô����������������ʱ�����˳� 
              if (endA == 1){ 
                  if (G_M_CODE[LineNumber]-> R_Start[0] < G_M_CODE[LineNumber]->R_End[0]){ //ȡֵ���������� 1,�����������ֵʱǿ���˳� 
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
                  }else{   //ȡ��Բ,�������Բ��ֵʱǿ���˳�  
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
                  if (G_M_CODE[LineNumber]-> R_Start[0] < G_M_CODE[LineNumber]->R_End[0]){ //ȡֵ���������� 2,�����������ֵʱǿ���˳� 
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
                  } else{ //ȡ��Բ,�������Բ��ֵʱǿ���˳� 
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
                  if (G_M_CODE[LineNumber]->R_Start[0] > G_M_CODE[LineNumber]->R_End[0]){ //ȡֵ���������� 3,�����������ֵʱǿ���˳� 
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
                  }else{ //ȡ��Բ,�������Բ��ֵʱǿ���˳�  
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
                  if (G_M_CODE[LineNumber]->R_Start[0] > G_M_CODE[LineNumber]->R_End[0]){ //ȡֵ���������� 4,�����������ֵʱǿ���˳� 
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
                  }else{ //ȡ��Բ,�������Բ��ֵʱǿ���˳�  
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
     if (G_M_CODE[LineNumber]-> G03 == true){ //(��ʱ��) 
         if (startA == 1){
              if (endA == 1){
                  if (G_M_CODE[LineNumber]->R_Start[0] > G_M_CODE[LineNumber]->R_End[0]){ //ȡֵ���������� 1,�����������ֵʱǿ���˳� 
                  }else{ //ȡ��Բ,�������Բ��ֵʱǿ���˳�  
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
                  if (G_M_CODE[LineNumber]->R_Start[0] > G_M_CODE[LineNumber]->R_End[0]){ //ȡֵ���������� 1,�����������ֵʱǿ���˳� 
                  }else{ //ȡ��Բ,�������Բ��ֵʱǿ���˳�  
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
                      �ڶ����� X-     |       ��һ���� X+
-------------------------------------------------------------------------------- 
                      ��������        |       ��������
                                      |
*/
/*void getUseVertexCoordinate(struct G_M_XYZ **G_M_CODE, int LineNumber, float *x, float *y){
     int VertexStart  = 0, VertexEnd = 0; //��ͨ�����ĵ��ҳ�����,��� Y �����ĵ��������,С������ 
     int VertexNumber = 0;  
     int  Vertex = G_M_CODE[LineNumber]->N / 2;
     if (((G_M_CODE[LineNumber]->R_Start[0] > G_M_CODE[LineNumber]->CircularHeartX) 
       && (G_M_CODE[LineNumber]->R_Start[1] >= G_M_CODE[LineNumber]->CircularHeartY)) 
       ||((G_M_CODE[LineNumber]->R_Start[0] <= G_M_CODE[LineNumber]->CircularHeartX) 
       && (G_M_CODE[LineNumber]->R_Start[1] > G_M_CODE[LineNumber]->CircularHeartY))){
         for (int i = 0; i < Vertex; i++){ //1-2 ����
             if (G_M_CODE[LineNumber]->R_Start[0] >= x[i]){
                 VertexStart = i;
                 break;
             }
         }    
     }else if (((G_M_CODE[LineNumber]->R_Start[0] < G_M_CODE[LineNumber]->CircularHeartX) 
             && (G_M_CODE[LineNumber]->R_Start[1] <= G_M_CODE[LineNumber]->CircularHeartY))
             ||((G_M_CODE[LineNumber]->R_Start[0] >= G_M_CODE[LineNumber]->CircularHeartX) 
             && (G_M_CODE[LineNumber]->R_Start[1] < G_M_CODE[LineNumber]->CircularHeartY))){
         for (int i = Vertex; i < G_M_CODE[LineNumber]->N; i++){ //3-4 ����
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
     //����������ȡ 
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
/*ȡ��Բ���İ뾶 
  I: Բ����㵽Բ�ĵ� X ����� 
  J: Բ����㵽Բ�ĵ� Y ����� 
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
/*ȡ��Բ�������� 
  X: Բ����� X ����
  Y: Բ����� Y ����               
  I: Բ����㵽Բ�ĵ� X �����,Բ��λ��Բ����� X ���������Ϊ��,������Ϊ�� 
  J: Բ����㵽Բ�ĵ� Y �����,Բ��λ��Բ����� Y ���������Ϊ��,������Ϊ��
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
  G_M_CODE: ָ��ṹ��ָ���ָ����� 
  LineNumber: ����                  
 */
FILE *pF = NULL; 
void VertexCoordinateCalculation(struct G_M_XYZ **G_M_CODE, int LineNumber){//����������� 
    double PI = 3.1415926536f; 
    G_M_CODE[LineNumber]->R = getRadius(G_M_CODE[LineNumber]->I, G_M_CODE[LineNumber]->J); //ȡ�ð뾶 
    G_M_CODE[LineNumber]->N = getRoundness(G_M_CODE[LineNumber]->R, ArcChordLength(G_M_CODE, LineNumber)); //��ȡ���ʵ� N
    if (G_M_CODE[LineNumber]->N < 20)
        G_M_CODE[LineNumber]->N = 40;
    G_M_CODE[LineNumber]->N = (G_M_CODE[LineNumber]->N / 4) * 4; 
    G_M_CODE[LineNumber]->CircularHeartX = getCoordinateX(G_M_CODE[LineNumber]->R_Start[0], G_M_CODE[LineNumber]->I); //Բ������ X 
    G_M_CODE[LineNumber]->CircularHeartY = getCoordinateY(G_M_CODE[LineNumber]->R_Start[1], G_M_CODE[LineNumber]->J); //Բ������ Y 
    float x[G_M_CODE[LineNumber]->N]; 
    float y[G_M_CODE[LineNumber]->N]; //������ʱ���� 
    for (int i = 0; i < G_M_CODE[LineNumber]->N; i++){ //ȫԲ����������� 
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
    //getUseVertexCoordinate(G_M_CODE, LineNumber, x, y); //��ȡ��Ҫ�Ķ������� 
    getUseVertexCoordinate(G_M_CODE, LineNumber, x, y); //��ȡ��Ҫ�Ķ������� 
} 
//==========================================================================================================================
//==========================================================================================================================
//==========================================================================================================================
//========================================================================================================================== 
/*�� atof()�����ַ���ת��Ϊ˫���ȸ�����ֵ
  �� atoi()�����ַ���ת��Ϊ����ֵ
  �� atol()�����ַ���ת��Ϊ������ֵ
  �� strtod()�����ַ���ת��Ϊ˫���ȸ�����ֵ,�����治�ܱ�ת��������ʣ������
  �� strtol()�����ַ���ת��Ϊ����ֵ,�����治�ܱ�ת��������ʣ������
  �� strtoul()�����ַ���ת��Ϊ�޷��ų�����ֵ,�����治�ܱ�ת��������ʣ������
  
  �� itoa()��������ֵת��Ϊ�ַ���
  �� ltoa()����������ֵת��Ϊ�ַ���
  �� ultoa()�����޷��ų�����ֵת��Ϊ�ַ���
  �� gcvt()������������ת��Ϊ�ַ�����ȡ��������
  �� ecvt()����˫���ȸ�����ֵת��Ϊ�ַ�����ת������в�����ʮ����С����
  �� fcvt()��ָ��λ��Ϊת�����ȣ�����ͬecvt()
  //printf("=======> %f\n",atof("-34.889"));    
 */
void G_CodeHandle(struct G_M_XYZ **G_M_CODE, int i, int Number){//G���봦�� 
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
void M_CodeHandle(struct G_M_XYZ **G_M_CODE, int i, int Number){//M���봦�� 
     switch (Number){
          case 0:
                 break;  
     }
}
//==========================================================================================================================
//==========================================================================================================================
//==========================================================================================================================
//==========================================================================================================================
//ָ��������ȡ 
/*
  TempCh    : �������ݵ���ʱ���� 
  G_array   : ������ 
  CharNumber: ���ַ���  
  i         : �к�                      
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
//NC �ļ������� 
/*
  G_M_CODE:�ṹ����� 
  G_array :�ļ���ȡ���� 
  g_v     :������ 
  cncConfigure:������̨�����ļ��ṹ�� 
  k:�û�ѡ�еĻ�̨��             
 */ 
void NcFileInterpreter(struct G_M_XYZ **G_M_CODE, char **G_array, int g_v, struct CNC_MachineNumber **cncConfigure, int k){
     //long LineNumber  = 0; //�к�
     int  CharNumber  = 0;   //�ַ��� 
     char TempCh[128];       //ע��ÿһ�ж�Ӧ�� XYZ ������
     char c;
     bool NullLine = false;
     for (int i = 0; i < g_v; i++){ 
         NullLine = false;        
         while (*(*(G_array + i) + CharNumber) != 10){
             c = *(*(G_array + i) + CharNumber);  
             if (c == '(' || c == '%'){  //ע�ͺ��ļ�ͷ����ȥ�� 
                 break; 
             }    
             else if (((c > 64) && (c < 91))  || ((c > 96) && (c < 123))){ //����ĸ��ͷ                
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
             else if (c > 47 && c < 58){ //�����ֿ�ͷ 
                  if (CharNumber >= 254)
                     break; 
             } 
             else if (c == 32){ //�Կո�ͷ 
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
             if (G_M_CODE[i]->x == false)  //���һ����û�и�����ָ��,û�и����Ľ���һ�е�����д�뵱ǰ��,��� X �� Y,G ֮��û����������� 0  
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
         if (G_M_CODE[i]->X_End < 0){ //�ҳ����ı߽� 
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
void CNC_InterpreterMemoryAllocation(){  //�����ļ��ܵ����������ڴ� 
    int n = 0;
    G_M_CODE = (struct G_M_XYZ **)malloc(g_v * sizeof(struct G_M_XYZ *));
    for (int i = 0; i < g_v; i++)
       G_M_CODE[i] = (struct G_M_XYZ*)malloc((sizeof(struct G_M_XYZ)));  
}
//==========================================================================================================================
//==========================================================================================================================
//==========================================================================================================================
//==========================================================================================================================
void InterpreterMemoryRelease(){ //�ͷŷ����ڴ� 
    for (int i = 0; i < g_v; i++)
       free(G_M_CODE[i]);  /*�ͷ���*/
    free(G_M_CODE);        /*�ͷ���*/ 
    //g_v = 0;  
}
//==========================================================================================================================
//==========================================================================================================================
//==========================================================================================================================
//========================================================================================================================== 
/*  //--------------------------------------------------------------------------
    struct G_M_XYZ ** G_M_CODE = NULL;  //�������ָ��ṹָ���ָ����� 
    G_M_CODE = (struct G_M_XYZ**)malloc((g_m * sizeof(struct G_M_XYZ*))); //Ϊ��һ��ָ������ڴ� 
    for (int i = 0; i < g_m; i++)
       G_M_CODE[i] = (struct G_M_XYZ*)malloc((sizeof(struct G_M_XYZ)));   //Ϊ�ڶ���ָ������ڴ� 
    //--------------------------------------------------------------------------    
    G_M_CODE[0][0].F = 100.0f;
    G_M_CODE[1]->F   = 200;
    //--------------------------------------------------------------------------
    for (int i = 0; i < g_m; i++) //�ͷŷ�����ڴ� 
       free(G_M_CODE[i]);         //�ͷ���     
    free(G_M_CODE);               //�ͷ���
    //--------------------------------------------------------------------------
*/    
//==========================================================================================================================
//==========================================================================================================================
//==========================================================================================================================
//==========================================================================================================================                
void StructInit(struct G_M_XYZ ** G_M_CODE, int g_v){
     for (int i = 0; i < g_v; i++){
            G_M_CODE[i] -> G00 = false;  //���ٶ�λ
            G_M_CODE[i] -> G01 = false;  //ֱ�߲岹
            G_M_CODE[i] -> G02 = false;  //˳ʱ�뷽��Բ���岹
            G_M_CODE[i] -> G03 = false;  //��ʱ�뷽��Բ���岹
            G_M_CODE[i] -> G04 = false;  //��ʱ��ͣ
            G_M_CODE[i] -> G05 = false;  //ͨ���м��Բ���岹
            G_M_CODE[i] -> G06 = false;  //�����߲岹
            G_M_CODE[i] -> G07 = false;  //Z �������߲岹
            G_M_CODE[i] -> G08 = false;  //��������
            G_M_CODE[i] -> G09 = false;  //��������
            G_M_CODE[i] -> G10 = false;  //��������
            G_M_CODE[i] -> G16 = false;  //��������
            G_M_CODE[i] -> G17 = false;  //�ӹ�XYƽ��
            G_M_CODE[i] -> G18 = false;  //�ӹ�XZƽ��
            G_M_CODE[i] -> G19 = false;  //�ӹ�YZƽ��
            G_M_CODE[i] -> G20 = false;  //Ӣ�Ƴߴ�(������ϵͳ)
            G_M_CODE[i] -> G21 = false;  //���Ƴߴ�(������ϵͳ)
            G_M_CODE[i] -> G22 = false;  //�뾶�ߴ��̷�ʽ
            G_M_CODE[i] -> G220 = false; //ϵͳ����������ʹ��
            G_M_CODE[i] -> G23  = false; //ֱ���ߴ��̷�ʽ
            G_M_CODE[i] -> G230 = false; //ϵͳ����������ʹ��
            G_M_CODE[i] -> G24 = false;  //�ӳ������
            G_M_CODE[i] -> G25 = false;  //��ת�ӹ�
            G_M_CODE[i] -> G26 = false;  //ѭ���ӹ�
            G_M_CODE[i] -> G30 = false;  //����ע��
            G_M_CODE[i] -> G31 = false;  //���ʶ���
            G_M_CODE[i] -> G32 = false;  //���ݾ���������,Ӣ��
            G_M_CODE[i] -> G33 = false;  //���ݾ���������,����
            G_M_CODE[i] -> G34 = false;  //���ݾ���������
            G_M_CODE[i] -> G35 = false;  //���ݾ���������
            G_M_CODE[i] -> G40 = false;  //���߲���/����ƫ��ע��
            G_M_CODE[i] -> G41 = false;  //���߲���������
            G_M_CODE[i] -> G42 = false;  //���߲���������
            G_M_CODE[i] -> G43 = false;  //����ƫ�á�����
            G_M_CODE[i] -> G44 = false;  //����ƫ�á�����
            G_M_CODE[i] -> G45 = false;  //����ƫ��+/+
            G_M_CODE[i] -> G46 = false;  //����ƫ��+/-
            G_M_CODE[i] -> G47 = false;  //����ƫ��-/-
            G_M_CODE[i] -> G48 = false;  //����ƫ��-/+
            G_M_CODE[i] -> G49 = false;  //����ƫ��0/+
            G_M_CODE[i] -> G50 = false;  //����ƫ��0/-
            G_M_CODE[i] -> G51 = false;  //����ƫ��+/0
            G_M_CODE[i] -> G52 = false;  //����ƫ��-/0
            G_M_CODE[i] -> G53 = false;  //ֱ��ƫ��,ע��
            G_M_CODE[i] -> G54 = false;  //ֱ��ƫ�� x
            G_M_CODE[i] -> G55 = false;  //ֱ��ƫ�� y
            G_M_CODE[i] -> G56 = false;  //ֱ��ƫ�� z
            G_M_CODE[i] -> G57 = false;  //ֱ��ƫ�� xy
            G_M_CODE[i] -> G58 = false;  //ֱ��ƫ�� xz
            G_M_CODE[i] -> G59 = false;  //ֱ��ƫ�� yz
            G_M_CODE[i] -> G60 = false;  //׼ȷ·����ʽ(��)
            G_M_CODE[i] -> G61 = false;  //׼ȷ·����ʽ(��)
            G_M_CODE[i] -> G62 = false;  //׼ȷ·����ʽ(��)
            G_M_CODE[i] -> G63 = false;  //������
            G_M_CODE[i] -> G68 = false;  //����ƫ��,�ڽ�
            G_M_CODE[i] -> G69 = false;  //����ƫ��,���
            G_M_CODE[i] -> G70 = false;  //Ӣ�Ƴߴ�(����������ӵ�,�����˵��� G21)
            G_M_CODE[i] -> G71 = false;  //���Ƴߴ� ����
            G_M_CODE[i] -> G74 = false;  //�زο���(�������)
            G_M_CODE[i] -> G75 = false;  //���ر���������
            G_M_CODE[i] -> G76 = false;  //�����Ƹ���ѭ��
            G_M_CODE[i] -> G80 = false;  //�̶�ѭ��ע��
            G_M_CODE[i] -> G81 = false;  //��Բ�̶�ѭ��
            G_M_CODE[i] -> G83 = false;  //
            G_M_CODE[i] -> G84 = false;  //
            G_M_CODE[i] -> G331 = false; //���ƹ̶�ѭ��
            G_M_CODE[i] -> G90 = false;  //���Գߴ�
            G_M_CODE[i] -> G91 = false;  //��Գߴ�
            G_M_CODE[i] -> G92 = false;  //Ԥ������
            G_M_CODE[i] -> G93 = false;  //ʱ�䵹��,������
            G_M_CODE[i] -> G94 = false;  //������,ÿ���ӽ���
            G_M_CODE[i] -> G95 = false;  //������,ÿת����
            G_M_CODE[i] -> G96 = false;  //�����ٶȿ���
            G_M_CODE[i] -> G97 = false;  //ȡ�������ٶȿ���
            
            G_M_CODE[i] -> Selection = false; //��ѡ�� 
            
            G_M_CODE[i] -> M00 = false;  // ����ֹͣ
            G_M_CODE[i] -> M01 = false;  // ��ѡֹͣ
            G_M_CODE[i] -> M02 = false;  // �������
            G_M_CODE[i] -> M03 = false;  // ���������� (��ת)
            G_M_CODE[i] -> M04 = false;  // ���������� (��ת)
            G_M_CODE[i] -> M05 = false;  // ����ֹͣ
            G_M_CODE[i] -> M06 = false;  // ���߽���
            G_M_CODE[i] -> M07 = false;  //
            G_M_CODE[i] -> M08 = false;  // ��ȴҺ��
            G_M_CODE[i] -> M09 = false;  // ��ȴҺ��
            G_M_CODE[i] -> M10 = false;  // ����㶯��
            G_M_CODE[i] -> M11 = false;  // ����㶯��
            G_M_CODE[i] -> M12 = false;  // ����������ֹͣ
            G_M_CODE[i] -> M13 = false;  // ������������ת
            G_M_CODE[i] -> M14 = false;  // ���������ᷴת
            G_M_CODE[i] -> M15 = false;  // C������λ
            G_M_CODE[i] -> M16 = false;  // C�ᷴ��λ
            G_M_CODE[i] -> M17 = false;  // �����������ͨ��RS232C��������
            G_M_CODE[i] -> M18 = false;  // ���ᶨ��ȡ��
            G_M_CODE[i] -> M19 = false;  // ���ᶨ��
            G_M_CODE[i] -> M20 = false;  // β�ܸ����������������ӹ�(����˫������)
            G_M_CODE[i] -> M21 = false;  // β�ܸ����������������ӿ�(����˫������)
            G_M_CODE[i] -> M22 = false;  // ���ǹ�
            G_M_CODE[i] -> M23 = false;  // ���ǿ�
            G_M_CODE[i] -> M24 = false;  // ���̸�������,���߸�������
            G_M_CODE[i] -> M25 = false;  // ���̸�������,���߸�������
            G_M_CODE[i] -> M26 = false;  // ���Ƶ�����Ч��Z��ָ��
            G_M_CODE[i] -> M27 = false;  // ���Ƶ�����Ч��X��ָ��
            G_M_CODE[i] -> M28 = false;  // ���߸����鹦�ܹ�
            G_M_CODE[i] -> M29 = false;  // ���߸����鹦�ܿ�
            G_M_CODE[i] -> M30 = false;  // �������
            G_M_CODE[i] -> M31 = false;  //
            G_M_CODE[i] -> M32 = false;  // ���Ƴ�����������ģʽ
            G_M_CODE[i] -> M33 = false;  // ���Ƴ���ʱ��������ģʽ
            G_M_CODE[i] -> M34 = false;  // ���Ƴ�������������ģʽ
            G_M_CODE[i] -> M35 = false;  // װ�����г���Z�򻬶�����
            G_M_CODE[i] -> M36 = false;  // װ�����г���Z�򻬶�ǰ��
            G_M_CODE[i] -> M37 = false;  // װ�����ۺ���
            G_M_CODE[i] -> M38 = false;  // װ������ǰ����ж��λ��
            G_M_CODE[i] -> M39 = false;  // װ������ǰ��������λ��
            G_M_CODE[i] -> M40 = false;  // ������ֿյ�
            G_M_CODE[i] -> M41 = false;  // �������1���������Ȧ
            G_M_CODE[i] -> M42 = false;  // �������2���������Ȧ
            G_M_CODE[i] -> M43 = false;  // �������3��
            G_M_CODE[i] -> M44 = false;  // �������4��
            G_M_CODE[i] -> M45 = false;  //
            G_M_CODE[i] -> M46 = false;  //
            G_M_CODE[i] -> M47 = false;  //
            G_M_CODE[i] -> M48 = false;  // ����ת�ٱ�����Чȡ��
            G_M_CODE[i] -> M49 = false;  // ����ת�ٱ�����Ч
            G_M_CODE[i] -> M50 = false;  // ���Ӵ�����1��
            G_M_CODE[i] -> M51 = false;  // ���Ӵ�����1��
            G_M_CODE[i] -> M52 = false;  //
            G_M_CODE[i] -> M53 = false;  //
            G_M_CODE[i] -> M54 = false;  // �ֶȿ����Զ��ֶ�
            G_M_CODE[i] -> M55 = false;  // β�ܺ���
            G_M_CODE[i] -> M56 = false;  // β��ǰ��
            G_M_CODE[i] -> M57 = false;  // M63ȡ��
            G_M_CODE[i] -> M58 = false;  // ���̵�ѹ
            G_M_CODE[i] -> M59 = false;  // ���̸�ѹ
            G_M_CODE[i] -> M60 = false;  // M61ȡ��
            G_M_CODE[i] -> M61 = false;  // Բ���ٶȺ㶨����ʱ,�㶨��תӦ�����
            G_M_CODE[i] -> M62 = false;  // M64ȡ��
            G_M_CODE[i] -> M63 = false;  // ������תM��Ӧ�����
            G_M_CODE[i] -> M64 = false;  // ������ת֮���M��Ӧ�����
            G_M_CODE[i] -> M65 = false;  // T��Ӧ�����
            G_M_CODE[i] -> M66 = false;  // ���ܻ�תλ������
            G_M_CODE[i] -> M67 = false;  // ͹�ֳ���ѭ����ͬ������ģʽȡ��
            G_M_CODE[i] -> M68 = false;  // ͬ��ģʽA���п�
            G_M_CODE[i] -> M69 = false;  // ͬ��ģʽB���п�
            G_M_CODE[i] -> M70 = false;  // �ֶ�����ָ��
            G_M_CODE[i] -> M71 = false;  //
            G_M_CODE[i] -> M72 = false;  // ATC��Ԫ��λ�ڽӽ�λ��
            G_M_CODE[i] -> M73 = false;  // ���Ƴ������� 1
            G_M_CODE[i] -> M74 = false;  // ���Ƴ������� 2
            G_M_CODE[i] -> M75 = false;  // ���Ƴ������� 3
            G_M_CODE[i] -> M76 = false;  // �������ֺ���
            G_M_CODE[i] -> M77 = false;  // ��������ǰ��
            G_M_CODE[i] -> M78 = false;  // ���ļ��ɿ�
            G_M_CODE[i] -> M79 = false;  // ���ļܼн�
            G_M_CODE[i] -> M80 = false;  // ����ǰ��
            G_M_CODE[i] -> M81 = false;  // ���к���
            G_M_CODE[i] -> M82 = false;  //
            G_M_CODE[i] -> M83 = false;  // ���̼н�
            G_M_CODE[i] -> M84 = false;  // �����ɿ�
            G_M_CODE[i] -> M85 = false;  // LAP�ֳ�ѭ���󲻷�����ʼλ��
            G_M_CODE[i] -> M86 = false;  // �����һ�תָ��
            G_M_CODE[i] -> M87 = false;  // M86ȡ��
            G_M_CODE[i] -> M88 = false;  // ������
            G_M_CODE[i] -> M89 = false;  // ������
            G_M_CODE[i] -> M90 = false;  // ����
            G_M_CODE[i] -> M91 = false;  // ����
            G_M_CODE[i] -> M92 = false;  // ���Ͻ���������
            G_M_CODE[i] -> M93 = false;  // ���Ͻ�����ǰ��
            G_M_CODE[i] -> M94 = false;  // װ����װ��
            G_M_CODE[i] -> M95 = false;  // װ����ж��
            G_M_CODE[i] -> M96 = false;  // �����ù������ֺ���
            G_M_CODE[i] -> M97 = false;  // �����ù�������ǰ��
            G_M_CODE[i] -> M98 = false;  // β�ܵ�ѹ
            G_M_CODE[i] -> M99 = false;  // β�ܸ�ѹ
            
            G_M_CODE[i] -> x = false;;
            G_M_CODE[i] -> y = false;;
            G_M_CODE[i] -> z = false;;
            G_M_CODE[i] -> a = false;;
            G_M_CODE[i] -> s = false;;
            G_M_CODE[i] -> f = false;;
            
            G_M_CODE[i] -> X_Start = 0;     //ÿ�е� X ����ֵ 
            G_M_CODE[i] -> X_End   = 0;     //ÿ�е� X ����ֵ 
            G_M_CODE[i] -> Y_Start = 0;     //ÿ�е� Y ����ֵ  
            G_M_CODE[i] -> Y_End   = 0;     //ÿ�е� Y ����ֵ  
            G_M_CODE[i] -> Z_Start = 0;     //ÿ�е� Z ����ֵ 
            G_M_CODE[i] -> Z_End   = 0;     //ÿ�е� Z ����ֵ 
            G_M_CODE[i] -> S = 0;     //ÿ�е�ת��ֵ 
            G_M_CODE[i] -> F = 0;     //ÿ�еĽ���ֵ
            G_M_CODE[i] -> zHigh = 0; //Z ���Ե��� 
            G_M_CODE[i] -> H = 0;     //���Ȳ����� 
            G_M_CODE[i] -> T = 0;     //���� 
            G_M_CODE[i] -> R = 0;     //�뾶 
            G_M_CODE[i] -> R_Start[0] = 0;   //Բ����ʼ���� 
            G_M_CODE[i] -> R_End[0]   = 0;   //Բ����ֹ���� 
            G_M_CODE[i] -> R_Start[1] = 0;   //Բ����ʼ���� 
            G_M_CODE[i] -> R_End[1]   = 0;   //Բ����ֹ���� 
            G_M_CODE[i] -> CircularHeartX = 0; //Բ�ĵ� X ���� 
            G_M_CODE[i] -> CircularHeartY = 0; //Բ�ĵ� Y ���� 
            G_M_CODE[i] -> I = 0; //
            G_M_CODE[i] -> J = 0; //
            G_M_CODE[i] -> N = 0; //Բ��ֵ 
            G_M_CODE[i] -> VertexNumber = 0;   //ʵ��Ҫʹ�õĶ����� 
            for (int j = 0; j < 1024; j++){
               G_M_CODE[i] -> ArcVertexCoordinateX[j] = 0; //Բ�Ķ������� 
               G_M_CODE[i] -> ArcVertexCoordinateY[j] = 0; //Բ�Ķ������� 
            }
            G_M_CODE[i] -> color[0]  = 0;     //��ɫ 
            G_M_CODE[i] -> color[1]  = 0;     //��ɫ 
            G_M_CODE[i] -> color[2]  = 0;     //��ɫ 
            G_M_CODE[i] -> color[3]  = 0;     //��ɫ 
            G_M_CODE[i] -> LineWidth = 0;     //�߿� 
            G_M_CODE[i] -> NullLine = false;
     }
}
//==========================================================================================================================
//==========================================================================================================================
//==========================================================================================================================
//==========================================================================================================================

