#include <SoftwareSerial.h>
#include <Servo.h>

const int BT_TX = 9;
const int BT_RX = 10;

SoftwareSerial BT_Serial(BT_TX, BT_RX);
Servo door_servo;

void setup() {
  BT_Serial.begin(9600);
  Serial.begin(4800);
  door_servo.attach(5);
  pinMode(5, INPUT);
  Serial.println("Begin...");
}

void loop() {
  String BT_String; 
  if (BT_Serial.available()) {
    BT_String = BT_Serial.readString();
    Serial.print("BT_String: \t");
    Serial.println(BT_String);
    if (BT_String == "openqwf") {
      Serial.println("open!");
      door_servo.attach(5);
      door_servo.write(180);
      delay(8000);
      pinMode(5, INPUT);
    }
    if (BT_String == "closeqwf") {
      Serial.println("close!");
      door_servo.attach(5);
      door_servo.write(0);
      delay(5000);
      pinMode(5, INPUT);
    }
  }
}
