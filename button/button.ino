int BTN = 3;

void setup() {
    pinMode(BTN, INPUT);
    Serial.begin(9600);
}

void loop() {
    Serial.println(get_btn(BTN));
}

int get_btn(int pin) {
    return digitalRead(pin);
}

// 用于按钮按一次改变一次状态。
int btn_state = -1;
int DELTA_PRESS_TIMES = 5;
int btn_unpressed = DELTA_PRESS_TIMES;
int click_switch(int pin) {
    if (btn_unpressed >= DELTA_PRESS_TIMES) {
        if (digitalRead(pin)) {
            btn_state *= -1;
            btn_unpressed = 0;
        } 
    } else if (digitalRead(pin)) {
        btn_unpressed = 0;
    } else if (btn_unpressed < DELTA_PRESS_TIMES) {
        btn_unpressed ++; 
    }
    return btn_state;
}