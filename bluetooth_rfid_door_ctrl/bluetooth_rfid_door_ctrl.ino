/*
   --------------------------------------------------------------------------------------------------------------------
   Example sketch/program showing how to read data from a PICC to serial.
   --------------------------------------------------------------------------------------------------------------------
   This is a MFRC522 library example; for further details and other examples see: https://github.com/miguelbalboa/rfid

   Example sketch/program showing how to read data from a PICC (that is: a RFID Tag or Card) using a MFRC522 based RFID
   Reader on the Arduino SPI interface.

   When the Arduino and the MFRC522 module are connected (see the pin layout below), load this sketch into Arduino IDE
   then verify/compile and upload it. To see the output: use Tools, Serial Monitor of the IDE (hit Ctrl+Shft+M). When
   you present a PICC (that is: a RFID Tag or Card) at reading distance of the MFRC522 Reader/PCD, the serial output
   will show the ID/UID, type and any data blocks it can read. Note: you may see "Timeout in communication" messages
   when removing the PICC from reading distance too early.

   If your reader supports it, this sketch/program will read all the PICCs presented (that is: multiple tag reading).
   So if you stack two or more PICCs on top of each other and present them to the reader, it will first output all
   details of the first and then the next PICC. Note that this may take some time as all data blocks are dumped, so
   keep the PICCs at reading distance until complete.

   @license Released into the public domain.

   Typical pin layout used:
   -----------------------------------------------------------------------------------------
               MFRC522      Arduino       Arduino   Arduino    Arduino          Arduino
               Reader/PCD   Uno           Mega      Nano v3    Leonardo/Micro   Pro Micro
   Signal      Pin          Pin           Pin       Pin        Pin              Pin
   -----------------------------------------------------------------------------------------
   RST/Reset   RST          9             5         D9         RESET/ICSP-5     RST
   SPI SS      SDA(SS)      10            53        D10        10               10
   SPI MOSI    MOSI         11 / ICSP-4   51        D11        ICSP-4           16
   SPI MISO    MISO         12 / ICSP-1   50        D12        ICSP-1           14
   SPI SCK     SCK          13 / ICSP-3   52        D13        ICSP-3           15
*/

#include <SPI.h>
#include <MFRC522.h>
#include <EEPROM.h>
#include <SoftwareSerial.h>
#include <Servo.h>

#define RST_PIN         9          // Configurable, see typical pin layout above
#define SS_PIN          10         // Configurable, see typical pin layout above

#define BT_TX           7
#define BT_RX           8
#define SERVO_PIN       3
#define NOTICE_OPENED_LED      5
#define NOTICE_CLOSED_LED      6


MFRC522 mfrc522(SS_PIN, RST_PIN);  // Create MFRC522 instance
SoftwareSerial BT_Serial(BT_TX, BT_RX);
Servo door_servo;
String BT_String;
int door_opening = 1;

void setup() {
  BT_Serial.begin(9600);
  door_servo.attach(SERVO_PIN);
  pinMode(SERVO_PIN, INPUT);
  pinMode(NOTICE_OPENED_LED, OUTPUT);
  pinMode(NOTICE_CLOSED_LED, OUTPUT);
  Serial.println("Begin...");
  Serial.begin(4800);		// Initialize serial communications with the PC
  while (!Serial);		// Do nothing if no serial port is opened (added for Arduinos based on ATMEGA32U4)
  SPI.begin();			// Init SPI bus
  mfrc522.PCD_Init();		// Init MFRC522
  mfrc522.PCD_DumpVersionToSerial();	// Show details of PCD - MFRC522 Card Reader details
  Serial.println(F("Scan PICC to see UID, SAK, type, and data blocks..."));
}

void open_door() {
  door_opening = 1;
  analogWrite(NOTICE_OPENED_LED, 255);
  analogWrite(NOTICE_CLOSED_LED, 0);
  Serial.println("open!");
  door_servo.attach(SERVO_PIN);
  door_servo.write(180);
  for (int j = 0; j <= 4; j++) {
    for (int l = 200; l > 0; l--) {
      analogWrite(NOTICE_OPENED_LED, l);
      delay(5);
    }
    for (int l = 0; l < 200; l++) {
      analogWrite(NOTICE_OPENED_LED, l);
      delay(5);
    }
  }
  // delay(8000);
  door_servo.write(0);
  delay(500);
  digitalWrite(NOTICE_OPENED_LED, HIGH);
  pinMode(SERVO_PIN, INPUT);
}

void close_door() {
  analogWrite(NOTICE_OPENED_LED, 0);
  analogWrite(NOTICE_CLOSED_LED, 255);
  Serial.println("close!");
  door_servo.attach(SERVO_PIN);
  door_servo.write(0);
  for (int j = 0; j <= 2; j++) {
    for (int l = 200; l > 0; l--) {
      analogWrite(NOTICE_CLOSED_LED, l);
      delay(5);
    }
    for (int l = 0; l < 200; l++) {
      analogWrite(NOTICE_CLOSED_LED, l);
      delay(5);
    }
  }
  digitalWrite(NOTICE_CLOSED_LED, HIGH);
  // delay(5000);
  pinMode(SERVO_PIN, INPUT);
  door_opening = 0;
}

void loop() {
  if (BT_Serial.available()) {
    BT_String = BT_Serial.readString();
    Serial.print("BT_String: \t");
    Serial.println(BT_String);
    if (BT_String == "openqwf") {
      open_door();
    }
    if (BT_String == "closeqwf") {
      close_door();
    }
    if (BT_String == "changeqwf") {
      if (door_opening == 1) {
        close_door();
      } else {
        open_door();
      }
    }
    if (BT_String == "delcardqwf") {
      del_saved_rfid();
    }
    if (BT_String == "regcardqwf") {
      BT_Serial.println("regcard begin!");
      Serial.println("Scan a rfid card and save it to eeprom...");
      while (1) {
        if (BT_Serial.available()) {
          BT_String = BT_Serial.readString();
          if (BT_String == "stopreg") {
            Serial.println("stop reg card!");
            return;
          }
        }
        if (mfrc522.PICC_IsNewCardPresent() and mfrc522.PICC_ReadCardSerial()) {
          print_rfid_uid(mfrc522.uid.uidByte, mfrc522.uid.size);
          int save_addr = get_eeprom_none_pointer(mfrc522.uid.size);
          save_rfid_uid(mfrc522.uid.uidByte, mfrc522.uid.size, save_addr);
          Serial.println("Saved!");
          BT_Serial.println("Saved!");
          return;
        }
      }
    }
  }

  // Look for new cards
  if ( ! mfrc522.PICC_IsNewCardPresent()) {
    return;
  }

  // Select one of the cards
  if ( ! mfrc522.PICC_ReadCardSerial()) {
    return;
  }

  // Dump debug info about the card; PICC_HaltA() is automatically called
  Serial.print(F("Card UID:"));
  print_rfid_uid(mfrc522.uid.uidByte, mfrc522.uid.size);
  if (loop_check_rfid_same(mfrc522.uid.uidByte, mfrc522.uid.size)) {
    Serial.println("same!");
    if (door_opening == 1) {
      close_door();
    } else {
      open_door();
    }
  } else {
    Serial.println("not same!");
  }

  // save_rfid_uid(mfrc522.uid.uidByte, mfrc522.uid.size, 0);
}

void print_rfid_uid(byte* uid, int uid_size) {
  for (byte i = 0; i < uid_size; i++) {
    byte b = uid[i];
    Serial.print(b < 0x10 ? " 0" : " ");
    Serial.print(b, HEX);
  }
  Serial.println();
}

int get_eeprom_none_pointer(int rfid_size) {
  Serial.println("get pointer: ");
  Serial.println(rfid_size);

  for (int index = 0; index < EEPROM.length(); index += rfid_size) {
    Serial.print(EEPROM[ index ]);
    Serial.print("\t");
    Serial.print(EEPROM[ index + 1 ]);
    Serial.print("\t");
    Serial.print(EEPROM[ index + 2 ]);
    Serial.print("\t");
    Serial.println(EEPROM[ index + 3 ]);
    if (EEPROM[ index ] == 0 or EEPROM[ index ] == 255) {
      return index;
    }
  }
}

void del_saved_rfid() {
  for (int i = 0 ; i < EEPROM.length() ; i++) {
    EEPROM.write(i, 0);
  }
  Serial.println("Deleted!");
  BT_Serial.println("Deleted!");
}

void show_saved_rfid() {
  int addr_len = get_eeprom_none_pointer(4);
  for (int index = 0; index < addr_len; index += 4) {
    BT_Serial.print(index);
    BT_Serial.print("\t");
    BT_Serial.print(EEPROM[ index ]);
    BT_Serial.print("\t");
    BT_Serial.print(EEPROM[ index + 1 ]);
    BT_Serial.print("\t");
    BT_Serial.print(EEPROM[ index + 2 ]);
    BT_Serial.print("\t");
    BT_Serial.println(EEPROM[ index + 3 ]);
  }
}

void save_rfid_uid(byte* uid, int uid_size, int begin_addr) {
  for (byte i = 0; i < uid_size; i++) {
    byte b = uid[i];
    Serial.println(b);
    EEPROM.write(begin_addr + i, b);
  }
}

int loop_check_rfid_same(byte* uid, int uid_size) {
  int addr_len = get_eeprom_none_pointer(mfrc522.uid.size);
  for (int addr = 0; addr < addr_len; addr += mfrc522.uid.size) {
    if (rfid_uid_same(uid, uid_size, addr)) {
      return 1;
    }
  }
  return 0;
}

int rfid_uid_same(byte* uid, int uid_size, int begin_addr) {
  for (byte i = 0; i < uid_size; i++) {
    byte b = uid[i];
    if (b != EEPROM.read(i + begin_addr)) {
      return 0;
    }
  }
  return 1;
}

