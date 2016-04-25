// Pin connected to latch pin (ST_CP) of 74HC595
const int latchPin = 8;
// Pin connected to clock pin (SH_CP) of 74HC595
const int clockPin = 3;
// Pin connected to Data in (DS) of 74HC595
const int dataPin = 9;
byte Tab[] = {0xc0, 0xf9, 0xa4, 0xb0, 0x99, 0x92, 0x82, 0xf8, 0x80, 0x90, 0xff};

void set_digital_led(String led_show) {
  int led_len = led_show.length();
  // Serial.println(led_show);
  digitalWrite(latchPin, LOW);
  if (led_len <= 8) {
    for (int j = led_len - 1; j >= 0; j--) {
      int led_i = led_show[j] - 48;
      shiftOut(dataPin, clockPin, MSBFIRST, Tab[led_i]);
    }
    for (int j = 0; j < 8 - led_len; j++) {
      shiftOut(dataPin, clockPin, MSBFIRST, Tab[0]);
    }
  }
  digitalWrite(latchPin, HIGH);
}

void setup() {
  // set pins to output because they are addressed in the main loop
  pinMode(latchPin, OUTPUT);
  pinMode(dataPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  // Serial.begin(9600);
}

void loop() {
  long input = 12345678;
  set_digital_led(String(input));
}
