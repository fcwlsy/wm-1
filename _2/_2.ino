    char getstr; 
    int in1=8;   
    int in2=9; 
    int in3=10;  
    int in4=11;
    int val=255;  
    //上面定义了板上的4个控制端，12一组，34一组
       
    void _mRight(int pin1,int pin2,int pwm)//当in1和in2中有一个接高位时，电机会向某一方向转动
    { 
       
      digitalWrite(pin1,LOW);
      digitalWrite(pin2,HIGH);
      analogWrite(5,pwm); 
      analogWrite(6,pwm);
    }   
    void _mLeft(int pin1,int pin2,int pwm)
    {   
      digitalWrite(pin1,HIGH);     
      digitalWrite(pin2,LOW);   
      analogWrite(5,pwm); 
      analogWrite(6,pwm);  
    }     
    void _mStop(int pin1,int pin2)//紧急制动，实际就是将电机两个端短接了 
    {
      digitalWrite(pin1,HIGH);     
      digitalWrite(pin2,HIGH);   
    }
  
    void setup()     
    {   
      Serial.begin(9600);   
      pinMode(in1,OUTPUT);     
      pinMode(in2,OUTPUT);   
      pinMode(in3,OUTPUT);  
      pinMode(in4,OUTPUT); 
   
      digitalWrite(in1,HIGH);  
      digitalWrite(in2,HIGH);   
      digitalWrite(in3,HIGH);   
      digitalWrite(in4,HIGH); 
    } 
    void loop() 
    {   
      getstr=Serial.read();
      if (getstr=='+') {
        if (val>=255){
          val=255;  
        }else{
          val=val+10;
        }
      }  
      if (getstr=='-') {
        if (val<=0){
          val=0;  
        }else{
          val=val-10;
        }
      }  
      if(getstr=='f')  
      { 
        Serial.println("go forward!");
        _mStop(in1,in2); 
        _mStop(in3,in4);  
        _mRight(in1,in2,val);  
        _mRight(in3,in4,val); 
      }  
      else if(getstr=='b'){ 
        Serial.println("go back!");
        _mStop(in1,in2); 
        _mStop(in3,in4);
        _mLeft(in1,in2,val);
        _mLeft(in3,in4,val);
      }
      else if(getstr=='l'){

        Serial.println("go left!");
        _mStop(in1,in2);
        _mStop(in3,in4); 
        _mRight(in3,in4,val);
      } 
      else if(getstr=='r'){
        Serial.println("go right!"); 
        _mStop(in1,in2);  
        _mStop(in3,in4); 
         _mRight(in1,in2,val);   
      } 
       
      else if(getstr=='s'){
        Serial.println("Stop!");
        _mStop(in1,in2);
        _mStop(in3,in4);   
      }   
    }
