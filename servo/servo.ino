// 棕线电源负，红线电源正，黄线控制信号。
// MG996R是180度舵机，实际转动角度:30-180度，信号端可以用数值表示转动到的角度。
// MG995是360度舵机，信号端85以下开始顺时针旋转（85还是静止），越小速度越快。90以上开始逆时针旋转（90还是静止），越大速度越快。


#include <Servo.h>


Servo myservo;  //创建一个舵机控制对象，使用Servo类最多可以控制8个舵机
int pos = 0;    // 该变量用与存储舵机角度位置
String input;

void setup()
{
  myservo.attach(9);  // 该舵机由arduino第九脚控制
  Serial.begin(9600);
}


void loop()
{
  if (Serial.available() > 0) {
    input = Serial.readString();
    pos = input.toInt();
  }
  Serial.println(pos);
  myservo.write(pos);        // 指定舵机转向的角度
}

