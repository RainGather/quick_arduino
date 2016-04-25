// 红色5V，黑色GND，蓝色信号。数值越大越黑，越小越白。白纸是90左右，黑皮是200+左右。

void setup()
{
    Serial.begin(9600); // open serial port, set the baud rate to 9600 bps
}
void loop()
{
    int val;
    val=analogRead(0);   //connect grayscale sensor to Analog 0
    Serial.println(val,DEC);//print the value to serial        
    delay(100);
}
