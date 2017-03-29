#include <SoftwareSerial.h>

#define PIN11 8
#define PIN12 9
#define PIN13 10
#define PIN14 11

#define PIN21 3
#define PIN22 4
#define PIN23 5
#define PIN24 6

int scale = 100;
int v = 20 * scale;
int s1 = v; // 一圈的时间
// int s2 = 60000; // 一圈的时间
bool start = false;
float r = 100; // cm
float leng = 27.5; // cm
long int s2 = v + (leng / r) * v;
int side = 2; // 2 left 3 right
float n1;
float n2;

SoftwareSerial bluetooth(12, 13);

void setup() {
  // put your setup code here, to run once:
  pinMode(PIN11, OUTPUT);
  pinMode(PIN12, OUTPUT);
  pinMode(PIN13, OUTPUT);
  pinMode(PIN14, OUTPUT);
  pinMode(PIN21, OUTPUT);
  pinMode(PIN22, OUTPUT);
  pinMode(PIN23, OUTPUT);
  pinMode(PIN24, OUTPUT);
  bluetooth.begin(9600);
  Serial.begin(115200);
  Serial.println(s1);
  Serial.println(s2);
}

void loop() {
  if (bluetooth.available()) {
    String input = bluetooth.readString();
    int value = input.substring(1).toInt();
    Serial.println(value);
    char ctrl = input[0];
    if (ctrl == 'v') {
      v = value * scale;
    } else if (ctrl == 'r') {
      if (value) {
        r = value;
      } else {
        side = 3;
      }
    } else if (ctrl == 'c') {
      side = 1;
    } else if (ctrl == 'l') {
      if (value) {
        leng = value;
      } else {
        side = 2;
      }
    } else if (ctrl == 's') {
      start = !start;
    }
    if (side == 1) {
      s1 = v;
      s2 = v;
    } else if (side == 2) {
      s2 = v;
      s1 = v + (leng / r) * v;
    } else if (side == 3) {
      s1 = v;
      s2 = v + (leng / r) * v;
    }
    bluetooth.print("Done");
  }
  // put your main code here, to run repeatedly:
  // Serial.println(millis() * 100);
  if (start) {
    n1 = millis() * scale % s1 / (s1 / 4.0);
    n2 = millis() * scale % s2 / (s2 / 4.0);
    if (n1 < 1) {
      digitalWrite(PIN14, LOW);
      digitalWrite(PIN11, HIGH);
    } else if (n1 < 2) {
      digitalWrite(PIN11, LOW);
      digitalWrite(PIN12, HIGH);
    } else if (n1 < 3) {
      digitalWrite(PIN12, LOW);
      digitalWrite(PIN13, HIGH);
    } else if (n1 < 4) {
      digitalWrite(PIN13, LOW);
      digitalWrite(PIN14, HIGH);
    } else {
      Serial.print("Error: ");
      Serial.println(n1);
    }

    if (n2 < 1) {
      digitalWrite(PIN24, LOW);
      digitalWrite(PIN21, HIGH);
    } else if (n2 < 2) {
      digitalWrite(PIN21, LOW);
      digitalWrite(PIN22, HIGH);
    } else if (n2 < 3) {
      digitalWrite(PIN22, LOW);
      digitalWrite(PIN23, HIGH);
    } else if (n2 < 4) {
      digitalWrite(PIN23, LOW);
      digitalWrite(PIN24, HIGH);
    } else {
      Serial.print("Error: ");
      Serial.println(n2);
    }
  }
}

