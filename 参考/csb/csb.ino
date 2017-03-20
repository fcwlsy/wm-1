#include <Servo.h>
 
Servo myservo;  // 定义舵机变量

void csb(int pos) 
{
  myservo.write(pos);  
  delay(250); 			//舵机动作时间
}

