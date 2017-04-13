#define D 20000 //等待间隔，毫秒，必须大于快门时间
#define S 15000 //快门时间，毫秒

void setup() {
  pinMode(7, OUTPUT);
}

void loop() {
  digitalWrite(7, HIGH);
  delay(S);
  digitalWrite(7, LOW);
  delay(D - S);
}
