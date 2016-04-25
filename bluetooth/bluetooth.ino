#include <SoftwareSerial.h>

const int BT_TX = 9;
const int BT_RX = 10;

SoftwareSerial BT_Serial(BT_TX, BT_RX);

void setup() {
  BT_Serial.begin(9600);
  Serial.begin(4800);
  Serial.println("Begin...");
}

void loop() {
  String BT_String;
  if (BT_Serial.available()) {
    BT_String = BT_Serial.readString();
    Serial.print("BT_String: \t");
    Serial.println(BT_String);
  }
}
