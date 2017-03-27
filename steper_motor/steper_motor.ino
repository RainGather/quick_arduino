// s1、s2至少大于10比较好

int s1 = 60; // 一圈的时间
int s2 = 60; // 一圈的时间
int n1;
int n2;

void setup() {
  // put your setup code here, to run once:
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(8, OUTPUT);
  pinMode(9, OUTPUT);
  pinMode(10, OUTPUT);
  pinMode(11, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  n1 = int(millis() % s1 / (s1 / 4.0));
  n2 = int(millis() % s2 / (s2 / 4.0));
  if (n1 < 1) {
    digitalWrite(6, LOW);
    digitalWrite(3, HIGH);
  } else if (n1 < 2) {
    digitalWrite(3, LOW);
    digitalWrite(4, HIGH);
  } else if (n1 < 3) {
    digitalWrite(4, LOW);
    digitalWrite(5, HIGH);
  } else if (n1 < 4) {
    digitalWrite(5, LOW);
    digitalWrite(6, HIGH);
  } else {
    Serial.print("Error: ");
    Serial.println(n1);
  }
  
  if (n2 < 1) {
    digitalWrite(11, LOW);
    digitalWrite(8, HIGH);
  } else if (n2 < 2) {
    digitalWrite(8, LOW);
    digitalWrite(9, HIGH);
  } else if (n2 < 3) {
    digitalWrite(9, LOW);
    digitalWrite(10, HIGH);
  } else if (n2 < 4) {
    digitalWrite(10, LOW);
    digitalWrite(11, HIGH);
  } else {
    Serial.print("Error: ");
    Serial.println(n2);
  }
}
