#include <SoftwareSerial.h>
#include <EEPROM.h>

#define MAC_LEN 15
#define BT_TX 10
#define BT_RX 11
#define BT_KEY 7
#define AUTH_PIN 8
#define LED 13

SoftwareSerial BTSerial(BT_TX, BT_RX); // Arduino_RX, Arduino_TX


void bt_init() {
  BTSerial.write("AT+INIT\r\n");
  delay(1000);
  while (BTSerial.available()) {
    Serial.write(BTSerial.read());
  }
  BTSerial.write("AT+CLASS=0\r\n");
  delay(1000);
  while (BTSerial.available()) {
    Serial.write(BTSerial.read());
  }
  BTSerial.write("AT+INQM=1,9,48\r\n");
  delay(1000);
  while (BTSerial.available()) {
    Serial.write(BTSerial.read());
  }
  //  BTSerial.write("AT+INQ\r\n");
  //  while (! BTSerial.available()) {}
  //  delay(3000);
  //  while (BTSerial.available()) {
  //    mac = BTSerial.readString();
  //    Serial.println(mac);
  //    Serial.println("kala");
  //  }
  Serial.println("bt_init done!");
}


void setup() {
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  Serial.println("Goodnight moon!");

  // set the data rate for the SoftwareSerial port
  BTSerial.begin(38400);
  BTSerial.println("Hello, world?");
  pinMode(BT_KEY, OUTPUT);
  digitalWrite(BT_KEY, HIGH);
  pinMode(AUTH_PIN, INPUT);
  bt_init();
//  del_saved_macs();
}

void save_mac(String mac) {
  int begin_addr = get_eeprom_none_pointer();
  for (int i = 0; i < MAC_LEN; i++) {
    byte b = 0;
    if (i < mac.length()) {
      b = mac[i];
    }
    Serial.print(b);
    EEPROM.write(begin_addr + i, b);
  }
}

int mac_same(String mac, int begin_addr) {
  for (int i = 0; i < MAC_LEN; i++) {
    byte b = mac[i];
    byte m = EEPROM.read(i + begin_addr);
    Serial.print(b);
    Serial.print('\t');
    Serial.println(m);
    if (b != m && m) {
      Serial.println("----------------");
      return 0;
    }
  }
  Serial.println("----------------");
  return 1;
}

int loop_check_mac_same(String mac) {
  int addr_len = get_eeprom_none_pointer();
  for (int addr = 0; addr < addr_len; addr += MAC_LEN) {
    if (mac_same(mac, addr)) {
      return 1;
    }
  }
  return 0;
}

void del_saved_macs() {
  for (int i = 0 ; i < EEPROM.length() ; i++) {
    EEPROM.write(i, 0);
  }
  Serial.println("Deleted!");
  BTSerial.println("Deleted!");
}

int get_eeprom_none_pointer() {
  //  Serial.println("get pointer: ");
  //  Serial.println(MAC_LEN);

  for (int index = 0; index < EEPROM.length(); index += MAC_LEN) {
    //    Serial.print(EEPROM[ index ]);
    //    Serial.print("\t");
    //    Serial.print(EEPROM[ index + 1 ]);
    //    Serial.print("\t");
    //    Serial.print(EEPROM[ index + 2 ]);
    //    Serial.print("\t");
    //    Serial.println(EEPROM[ index + 3 ]);
    if (EEPROM[ index ] == 0 or EEPROM[ index ] == 255) {
      return index;
    }
  }
}

String get_mac() {
  //  BTSerial.write("AT+INQ\r\n");
  String drop = BTSerial.readStringUntil(':');
  String mac = BTSerial.readStringUntil(',');
  mac.replace(':', ',');
  mac.replace("+INQ,", "");
  return mac;
}

void loop() { // run over and over
  if (digitalRead(AUTH_PIN)) {
    Serial.println("Add Auth...");
    BTSerial.write("AT+INQ\r\n");
    delay(3000);
    while (BTSerial.available()) {
      String mac = get_mac();
      if (mac.length() != 14) {
        continue ;
      }
      if (! loop_check_mac_same(mac)) {
        save_mac(mac);
        Serial.print("saved: ");
        Serial.println(mac);
      }
    }
  } else {
    BTSerial.write("AT+INQ\r\n");
    delay(3000);
    if (! BTSerial.available()) {
      Serial.println("AUTH FAILED");
    }
    //    Serial.println(BTSerial.readString());
    while (BTSerial.available()) {
      String mac = get_mac();
      if (mac.length() != 14) {
        continue ;
      }
      Serial.println("mac begin:");
      Serial.println(mac);
      Serial.println("mac end.");
      // Serial.println(mac);
      if (loop_check_mac_same(mac)) {
        digitalWrite(LED, HIGH);
        Serial.println("AUTH SUCCESS!");
      } else {
        digitalWrite(LED, LOW);
        Serial.println("AUTH FAILED!");
      }
    }
    //    if (BTSerial.available()) {
    //      Serial.write(BTSerial.read());
    //    }
    //    if (Serial.available()) {
    //      BTSerial.write(Serial.read());
    //    }
  }
  delay(5000);
  Serial.println("loop");
}

