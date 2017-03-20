
void setup()
{

   pinMode(2, OUTPUT);   //前置超声波接口引脚
   pinMode(3, INPUT);   
   pinMode(8, OUTPUT);  //左侧置超声波接口引脚 
   pinMode(9, INPUT);   
   pinMode(10, OUTPUT);   //右侧超声波接口引脚
   pinMode(11, INPUT); 
//int PEER = 9;       //蜂鸣器
  // 
  pinMode(INA,OUTPUT);
  pinMode(INB,OUTPUT);
//  pinMode(PEER,OUTPUT);

  pinMode(A0,INPUT);    //红外壁障器1
  pinMode(A5,INPUT);    //红外壁障器2

  pinMode(SensorPin, INPUT_PULLUP);  //触摸模块输入


  // 初始化串口通信及连接SR04的引脚
  Serial.begin(9600); 

  myservo.attach(13);  //定义舵机接口数字接口13

  csb(100);      //舵机带动超声波传感器至正位

}


