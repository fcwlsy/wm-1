
int SensorPin = 12;     // 触摸模块输入端口

int hand_start=0;

  int Sensor_L=A0;  //前左红外避障器
  int Sensor_R=A5;  //前右红外避障器

void loop()
{
  int f; 

  hand_start=digitalRead(SensorPin);
  
  if (Serial.available() > 0)
  {
     blue_start=1;
  }
  
  while (blue_start>0 )     //蓝牙通讯启动
  {
      if (digitalRead(Sensor_L) && digitalRead(Sensor_R))    //前方无障碍 
      {
        Blue_Str();
      }
      else 
      {
        stop();     //遇障碍停车，转手动/蓝牙通讯启动
        blue_start=0;
        return;
      }
    }

     
  while (hand_start==1)       //触摸启动
      {
        if (Serial.available() > 0)     //手动过程，可以蓝牙控制停止
        {
          int inChar = Serial.read();
          inString += (char)inChar; 
          if (inString=="stop")
          {
            stop();
            inString = "";
            hand_start=0;
            return;
          }
        }

        if (digitalRead(Sensor_L) && digitalRead(Sensor_R))    //前方无障碍
        {
          CJ();             //超声波测距
          JL_Panduan();     //距离判断并动作
          L_R();
        }
        else 
        {
            stop();
            GoGo_back();  //后退
            delay(300);
            stop();
            LR_panduan(); //左右判断
            L_R();        //左右选择转向
        }
      }

}