#include <Servo.h>

int d = 90;
int v = 50;
String s;
char f;
Servo head;

void nod() {
  for (; d < 100; d++) {
    if (serial_input()) {
      return;
    }
    head.write(d);
    delay(v);
  }
  for (; d > 80; d--) {
    if (serial_input()) {
      return;
    }
    head.write(d);
    delay(v);
  }
}

bool serial_input() {
  if (Serial.available()) {
    s = Serial.readString();
    f = s[0];
    v = s.substring(1).toInt();
    Serial.print(s);
    Serial.print('\t');
    Serial.print(f);
    Serial.print('\t');
    Serial.println(v);
    return true;
  }
  return false;
}

void setup() {
  Serial.begin(9600);
  head.attach(6);
}

void loop() {
  serial_input();
  if (f == 'd') {
    nod();
  } else {
    head.write(90);
  }
}
