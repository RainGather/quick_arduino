#define CACHE 100
#define D 20000 //等待间隔，毫秒，必须大于快门时间
#define IFLIGHT true
#define MAX 15000 //最长快门时间
#define S_PIN 7

int s = MAX; //快门时间，毫秒
int light[CACHE];
long int avg_light = 0;
int i = 0;

void setup() {
  pinMode(7, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  if (IFLIGHT) {
    avg_light = 0;
    i++;
    if (i >= CACHE) {
      i = 0;
    }
    light[i] = analogRead(A0);
    for (int j=0; j<CACHE; j++) {
      avg_light += light[j];
    }
    avg_light /= CACHE;
    Serial.println(avg_light);
    s = (1 - avg_light / 1024.0) * MAX;
  }
  digitalWrite(S_PIN, HIGH);
  delay(s);
  digitalWrite(S_PIN, LOW);
  delay(D - s);
}
