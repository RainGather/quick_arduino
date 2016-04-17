int LED = 3;

void setup() {
    pinMode(LED, OUTPUT); 
}

void loop() {
    blink(LED, 1000, 3);
    while (1) {
        ;
    }
}

void blink(int pin, int dtime, int times) {
    if (times == 0) {
        times = 65535;
    }
    for (int i=0; i<times; i++) {
        digitalWrite(pin, HIGH);
        delay(dtime);
        digitalWrite(pin, LOW);
        delay(dtime);
    }
}