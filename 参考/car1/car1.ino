/*
SR04超声波传感器驱动
 串口显示检测距离
 */
// 设定SR04连接的Arduino引脚
 // const int TrigPin1 = 2;    //定义前置超声波传感器的连接，同时为宏定义，后面直接用 TrigPin1 代表引脚 2
 // const int EchoPin1 = 3; 

 // const int TrigPin2 = 8;    //定义左侧超声波传感器的连接
 // const int EchoPin2 = 9; 

 // const int TrigPin3 = 10;   //定义右侧超声波传感器的连接
 // const int EchoPin3 = 11; 


float a[6]={};    //前置超声波传感器数据缓存：存取6个测距数据：分别为a[0]、a[1]、a[2]、a[3]、a[4]、a[5]
float b[6]={};    //左侧超声波传感器数据缓存：存取6个测距数据
float c[6]={};    //右侧超声波传感器数据缓存：存取6个测距数据

float CJ_data_a,CJ_data_b,CJ_data_c;    //筛选后的测距数据

int gg_back;    //后退次数：此为进入死胡同通道时，需后退的次数，现暂时定义为2次，2次后即后退时间加长，保证能退出死胡同

//读取前置超声波测距数据
void read_csb(int TrigPin,int EchoPin)
{
  unsigned int s,t,f;   
  // 产生一个10us的高脉冲去触发TrigPin
  for (s=0;s<6;s++)         //连续6次，读取前置超声波测距数值
  {
    digitalWrite(TrigPin, LOW);    //引脚 2 为 低
    delayMicroseconds(2);           //延时
    digitalWrite(TrigPin, HIGH);   //引脚 3 为 高
    delayMicroseconds(10);          
    digitalWrite(TrigPin, LOW); 
          // 检测脉冲宽度，并计算出距离
    
    switch (TrigPin)
    {
      case 2:   a[s] = pulseIn(EchoPin, HIGH) / 58.00;   //计算出测距数值，存放于前置超声波数组中
              break;
      case 8:   b[s] = pulseIn(EchoPin, HIGH) / 58.00;
              break;
      case 10:  c[s] = pulseIn(EchoPin, HIGH) / 58.00;
              break;
    }
                                            //s 即为上面的0～6的数值。分别将数值存入a[0]～a[5]中
    delay(10);
  }
}

//对超声波数组进行排序（冒泡法：小额数值往上走，大的数值往下走）
void Cj_data()
{
  unsigned int i,j,s; 
  float temp1,temp2,temp3;

  for(s=0;s<6;s++)
  {
    for(j=0;j<=5;j++) 
    {
      for (i=0;i<5-j;i++) 
        if (a[i]>a[i+1]) 
        {
          temp1=a[i];         //前置数值
          a[i]=a[i+1]; 
          a[i+1]=temp1;
        } 
    }
  }


    for(s=0;s<6;s++)
    {
      for(j=0;j<=5;j++) 
      {
        for (i=0;i<5-j;i++) 
          if (b[i]>b[i+1]) 
          {
            temp2=b[i];       //左侧数值
            b[i]=b[i+1]; 
            b[i+1]=temp2;
          }
      }
    }

      for(s=0;s<6;s++)
      {
        for(j=0;j<=5;j++) 
        {
          for (i=0;i<5-j;i++) 
            if (c[i]>c[i+1]) 
            {
              temp3=c[i];       //右侧数值
              c[i]=c[i+1]; 
              c[i+1]=temp3;
            } 
        }
      }
}

//对超声波数组排序结果，掐头去尾取平均，得到测距值
void JL()
 {
    unsigned int i;
    float CJ_temp1,CJ_temp2,CJ_temp3;

    for (i=1;i<5;i++)
    {
       CJ_temp1+=a[i];    //累加
    }
      
    CJ_data_a=CJ_temp1*100/400;      //取4个数据取平均

    for (i=1;i<5;i++)
    {
      CJ_temp2+=b[i];    //累加
    }
      
    CJ_data_b=CJ_temp2*100/400;      //取4个数据取平均
    

    for (i=1;i<5;i++)
    {
      CJ_temp3+=c[i];    //累加
    }
      
    CJ_data_c=CJ_temp3*100/400;      //取4个数据取平均
 }

//测定当前距离
void CJ()
 {
  read_csb(2,3);       //读前置超声波
  read_csb(8,9);       //读左侧超声波
  read_csb(10,11);       //读右侧超声波
  Cj_data();      //对数据排序
  JL();           //掐头去尾取平均，得到测距值
}

//距离判断之后的动作
void JL_Panduan()
{
  Serial.println(CJ_data_a);      //串口输出测距值：可加 // 注释（不执行）
  Serial.println(CJ_data_b);
  Serial.println(CJ_data_c);

  if (CJ_data_a<25)   //前方距离小于25cm
  {
    //停止
    stop();
  }
  else
  {
    if (CJ_data_a<35)   //前方距离小于35cm
    {
      //停止并左右判断，左转或右转
      stop();
      delay(200);
      LR_panduan();
    }
    else
    {
      if (CJ_data_a<40)   //前方距离小于40cm
      {
        //低速前进
       L_run=150;  
       R_run=150;
       Go();
      }
      else 
      {
        //高速前进
        L_run=240;  
        R_run=240;
        Gogo();
      }
    }
  }
}

//左右距离判断，并进行动作
void LR_panduan()   
{
  float M1, M2, M3, M4;    //M1右侧距离；M2左侧距离

  csb(40);      //舵机转右40°
  CJ();         //测定当前距离
  M1=CJ_data_a;   //40°右侧距离

  csb(160);     //舵机转左160°
  CJ();
  M2=CJ_data_a;   //160°左侧距离

  M3=CJ_data_b;   //左侧超声波距离
  M4=CJ_data_c;   //右侧超声波距离

  Serial.println(M1);
  Serial.println(M2);
  Serial.println(M3);
  Serial.println(M4);
  csb(100);     //舵机归正位

//  Serial.println(M1);       //可输出40°与16°角的测距值，现在已经注释掉了（不执行）
//  Serial.println(M2);

  if (((M3+M4)<80)  && (M3<40) && (M4<40) && (M1<40) && (M2<40))   //左右测距值相加＜80cm
                                              //同时左右间距也＜40cm，后退并左右判断距离再动作
  {
    L_run = 240 ;
    R_run = 240 ;
    GoGo_back();

    gg_back++;              //后退标志值加1次
// Serial.println(gg_back);
    if (gg_back>=2)         //如果后退标志值等于2次，则加长后退运行时间
    {
      gg_back=0;
      delay(400);
      LR_panduan();
    }
    else    delay(50);
  }
    else
    {
      if ((M3>M4) && (M3>40)) //M3左侧距离＞M4右侧距离，同时左侧距离＞40cm
      {
        if ((M2>40) && (M3<60))   //40°前方＞40cm，同时左侧距离＜60cm，则转向偏左40°位置
        {
          L_run=120;  R_run=200;
          Da_right();
          delay(200);
        }
         else 
          if ((M2<40) && (M3<60))  //40°前方＜40cm，同时左侧距离＜60cm，则掉头左转
          {
            Diaotou_left();   
            gg_back=0;
          }
            else 
            {
              L_run=0;  R_run=250;
              left();       //M3左侧距离＞60cm，90°左转
              gg_back=0;
            }
      }
        else
        {
           if ((M4>M3) && (M4>40)) //M4右侧距离＞M3左侧距离
           {
              if ((M1>40)  && (M4<60))  //160°前方＞40cm，同时右侧距离＜60cm，则转向偏右40°位置
              {
                L_run=200;  R_run=120;
                Da_right();
                delay(200);
              }
              else
              {
                if ((M1<40) && (M4<60)) //160°前方＜40cm，同时右侧距离＜60cm，则掉头右转
                {
                  Diaotou_right();   
                  gg_back=0;
                }   
                else 
                {
                  L_run=250;  R_run=0;
                  right();       //M4右侧距离＞60cm，90°右转
                  gg_back=0;
                }
              }

              
            }
        }
  }
}

//左右侧超声波判断
void L_R()
{
  if ((CJ_data_b>10) && (CJ_data_c>10))     //如果左右测距值都大于10cm，则高速运行
  {
    L_run=240;  R_run=240;
  }  
    else 
    {
      if ((CJ_data_b<10)  && (CJ_data_c>10))    //如果存在一侧的测距值小于10cm，则逐步调整小车进行转向
      {
        L_run=120;  R_run=200;
        Da_left();
        delay(10);
      }  
        else
        {
          if ((CJ_data_b>10)  &&  (CJ_data_c<10))  
            {
              L_run=200;  R_run=120;
              Da_right();
              delay(10);
            } 
             else
             {
               if ((CJ_data_b<10)  &&  (CJ_data_c<10))
                {
                  L_run=120;  R_run=120;         //否则即为进入窄通道，需减速行驶
                }
                
             }     
        }
    }
}