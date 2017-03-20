/*小车运动代码*/
//++++++++此为扩展板已有定义，无法更改
int INA = 4;      //电机A正反转控制端
int PWMA = 5;     //电机A调速端
int INB = 7;      //电机B正反转控制端
int PWMB = 6;     //电机B调速端

//++++++++小车运行变量定义
int value_L,value_R;
int L_run,  R_run;    //小车电机驱动运行值

//++++++++蓝牙串口变量定义
String inString = "";  //串口输入字符串：初始化为空
int blue_start=0;      //蓝牙启动标志：若为 1 ，则由蓝牙控制接管。初始化为 0
int xxx;                //串口传送过来的speed数值

//===========  电机动作  ===============//
//高速前进
void Gogo()
{
  value_L = L_run ;    //将电机运行值赋予驱动变量
  value_R = R_run ;    //同上
  Go_run();   //运行电机驱动函数
}

//低速前进
void Go()
{
  value_L = L_run ;
  value_R = R_run ;
  Go_run();
}

//高速后退
void GoGo_back()
{
  value_L = L_run ;
  value_R = R_run ;
  digitalWrite(INA,LOW);          //反转：与驱动函数部分不同，仔细看下哪里不同
  digitalWrite(INB,LOW);          //
  analogWrite(PWMA,value_L);      //PWM调速
  analogWrite(PWMB,value_R);
  delay(80);                      //驱动延时：即为高速后退运行时间
}

//大左转
void Da_left()
{
  value_L = L_run ;
  value_R = R_run ;
  Go_run();
  delay(100);
}

//大右转
void Da_right()
{
  value_L = L_run ;
  value_R = R_run ;
  Go_run();
  delay(100);
}

//左转弯
void left()
{
  value_L = 0 ;
  value_R = R_run ;
  Serial.println("left");
  Go_run();

  delay(550);
}

//右转弯
void right()
{
  value_L = L_run ;
  value_R = 0 ;
  Serial.println("right");
  Go_run();
  delay(550);
}

//左转弯掉头
void Diaotou_left()
{
  value_L = 0 ;
  value_R = 250 ;
  Serial.println("diao-left");
  Go_run();
  delay(1700);
}


//右转弯掉头
void Diaotou_right()
{
  value_L = 250 ;
  value_R = 0 ;
  Serial.println("diao-right");
  Go_run();
  delay(1700);
}

//停止
void stop()
{
  value_L = 0 ;
  value_R = 0 ;
  analogWrite(PWMA,value_L);      //PWM调速
  analogWrite(PWMB,value_R);
  Serial.println("stop");         //此为串口输出stop字符，专为调试时使用，运行该函数即输出stop字符，可帮助判断运行情况
}

//电机驱动
void Go_run()
{
  digitalWrite(INA,HIGH);         //HIGH为前进，LOW为后退
  digitalWrite(INB,HIGH);
  analogWrite(PWMA,value_L);      //PWM调速
  analogWrite(PWMB,value_R);
  //  digitalWrite(PEER,0);
}

//===========  蓝牙通讯控制  ==============//
void Blue_Str()
{
  while (Serial.available() > 0)    //读取串口输入：有输入则读取信息
  {
    int inChar = Serial.read();     //读取信息：读取为单字符，后继需将其连起来成为字符串
    if (inChar == '#') 
    {
      xxx = inString.toInt();
      inString="";
      break;
    }
    else
    {
      if (inChar == '*')   break;
       else    inString += (char)inChar;       //并且将其连起来：如 1+2+3＝123
    }    
      
  }

  if (inString=="go")               //如果输入信息为 go，则低速前行
  {
    L_run = xxx;
    R_run = xxx;
    Go();                           //此为函数：低速前行
    inString = "";                  //将串口信息缓存清空
    blue_start=1;                   //同时将蓝牙标志设置为 1
  }

  if (inString=="gogo")  
  {
    L_run = xxx;
    R_run = xxx;
    Gogo();
    inString = "";
    blue_start=1;
  }


  if (inString=="stop")
  {
    stop();
    inString = "";
    blue_start=0;                   //运行stop指令，则将蓝牙标志初始化，方便后继选择运行模式                 //
  }

  if (inString=="left")
  {
    R_run = xxx;
    left();
    inString = "";
    blue_start=1;
  }

  if (inString=="right")
  {
    L_run = xxx;
    right();
    inString = "";
    blue_start=1;
  } 

  if (inString=="back")
  {
    L_run = xxx;
    R_run = xxx;
    GoGo_back();
    inString = "";
    blue_start=1;
  } 
}

