/*
 * 此程序用于通过简单的GND和INPUT信号PIN产生0信号和VCC(或5V等正极)和INPUT信号PIN 产生1信号来控制延时摄影的间隔时间长短。
 */

#define PIN_PLUS 8
#define PIN_MINUS 7
#define SHUTTER 500 //快门时间
#define S_PIN 12 // 继电器信号针脚
#define CHECK_LED 13

#define MIN_DELAY_TIME 1000
#define MAX_DELAY_TIME 30000 //最长间隔时间
#define DELAY_UNIT 1000 // 最小改变delay时间单位，1000为1秒

unsigned long delay_time;
unsigned long now;
unsigned long pin_plus_last_time = 0;
unsigned long pin_minus_last_time = 0;
unsigned long next_shutter_open;
unsigned long next_shutter_close;
int minus_count = 0;
int plus_count = 0;

void minus_delay_time() {
  Serial.print(pin_minus_last_time);
  Serial.print("\t");
  if ((millis() - pin_minus_last_time > 300) && digitalRead(PIN_MINUS)) {
    plus_count = 0;
    minus_count = 0;
    pin_minus_last_time = millis();
  }
  if (digitalRead(PIN_MINUS) && (millis() - pin_minus_last_time > 100)) {
    minus_count ++;
    plus_count = 0;
    pin_minus_last_time = millis();
  }
  if (minus_count >= 20) {
    delay_time -= DELAY_UNIT;
    if (delay_time < MIN_DELAY_TIME) {
      delay_time = MAX_DELAY_TIME;
    }
    digitalWrite(CHECK_LED, LOW);
    for (int i = 0; i < int(delay_time / 1000) / 5; i ++ ) {
      digitalWrite(CHECK_LED, HIGH);
      delay(300);
      digitalWrite(CHECK_LED, LOW);
      delay(300);
    }
    delay(500);
    for (int i = 0; i < int(delay_time / 1000) % 5; i ++ ) {
      digitalWrite(CHECK_LED, HIGH);
      delay(150);
      digitalWrite(CHECK_LED, LOW);
      delay(150);
    }
    plus_count = 0;
    minus_count = 0;
    next_shutter_open = millis();
    next_shutter_close = next_shutter_open + SHUTTER;
  }
}

void plus_delay_time() {
  Serial.print(pin_plus_last_time);
  Serial.print("\t");
  if ((millis() - pin_plus_last_time > 300) && digitalRead(PIN_PLUS)) {
    plus_count = 0;
    minus_count = 0;
    pin_plus_last_time = millis();
  }
  if (digitalRead(PIN_PLUS) && (millis() - pin_plus_last_time > 100)) {
    plus_count ++;
    minus_count = 0;
    pin_plus_last_time = millis();
  }
  if (plus_count >= 20) {
    delay_time += DELAY_UNIT;
    if (delay_time > MAX_DELAY_TIME) {
      delay_time = MIN_DELAY_TIME;
    }
    digitalWrite(CHECK_LED, LOW);
    for (int i = 0; i < int(delay_time / 1000) / 5; i ++ ) {
      digitalWrite(CHECK_LED, HIGH);
      delay(300);
      digitalWrite(CHECK_LED, LOW);
      delay(300);
    }
    delay(500);
    for (int i = 0; i < int(delay_time / 1000) % 5; i ++ ) {
      digitalWrite(CHECK_LED, HIGH);
      delay(150);
      digitalWrite(CHECK_LED, LOW);
      delay(150);
    }
    plus_count = 0;
    minus_count = 0;
    next_shutter_open = millis();
    next_shutter_close = next_shutter_open + SHUTTER;
  }
}


void setup() {
  delay_time = 15000;
  next_shutter_open = millis();
  next_shutter_close = next_shutter_open + SHUTTER;
  Serial.begin(9600);
  pinMode(PIN_PLUS, INPUT);
  pinMode(PIN_MINUS, INPUT);
  pinMode(S_PIN, OUTPUT);
  pinMode(CHECK_LED, OUTPUT);
  digitalWrite(CHECK_LED, LOW);
}


void loop() {
  plus_delay_time();
  minus_delay_time();
  Serial.print(next_shutter_open);
  Serial.print("\t");
  Serial.print(next_shutter_close);
  Serial.print("\t");
  now = millis();
  Serial.print(now);
  Serial.print("\t");
  if (now >= next_shutter_open) {
    digitalWrite(S_PIN, HIGH);
    next_shutter_open = next_shutter_open + delay_time;
  }
  if (now >= next_shutter_close) {
    digitalWrite(S_PIN, LOW);
    next_shutter_close = next_shutter_open + SHUTTER;
  }
  Serial.println(delay_time);
}

