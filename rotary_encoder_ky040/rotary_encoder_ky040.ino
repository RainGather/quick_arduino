int pin_clk = 3;  // Connected to CLK on KY-040
int pin_dt = 4;  // Connected to DT on KY-040
int encoder_pos_count = 0;
int pin_clk_last;
int clk_val;
boolean b_cw; // clockwise or counterclockwise

void rotary_setup() {
  pinMode(pin_clk, INPUT);
  pinMode(pin_dt, INPUT);
  /* Read Pin A
    Whatever state it's in will reflect the last position
  */
  pin_clk_last = digitalRead(pin_clk);
}

int rotary() {
  clk_val = digitalRead(pin_clk);
  if (clk_val != pin_clk_last) { // Means the knob is rotating
    // if the knob is rotating, we need to determine direction
    // We do that by reading pin B.
    if (digitalRead(pin_dt) == clk_val) {  // Means pin A Changed first - We're Rotating Clockwise
      encoder_pos_count ++;
      b_cw = true;
    } else {// Otherwise B changed first and we're moving CCW
      b_cw = false;
      encoder_pos_count --;
    }
    Serial.print("Rotated: ");
    if (b_cw) {
      Serial.println("clockwise");
    } else {
      Serial.println("counterclockwise");
    }
    Serial.print("Encoder Position: ");
    Serial.println(encoder_pos_count);

  }
  pin_clk_last = clk_val;
}

void setup() {
  rotary_setup();
  Serial.begin(9600);
}

void loop() {
  rotary();
}
