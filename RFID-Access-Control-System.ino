
#include <SPI.h>
#include <MFRC522.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// RFID
#define SS_PIN 10
#define RST_PIN 9
MFRC522 rfid(SS_PIN, RST_PIN);

// LCD
LiquidCrystal_I2C lcd(0x27, 16, 2);

// LEDs
#define GREEN1 2
#define GREEN2 3
#define RED1 4
#define RED2 5

// Buzzer
#define BUZZER 6

void setup() {
  Serial.begin(9600);

  // LEDs and buzzer
  pinMode(GREEN1, OUTPUT);
  pinMode(GREEN2, OUTPUT);
  pinMode(RED1, OUTPUT);
  pinMode(RED2, OUTPUT);
  pinMode(BUZZER, OUTPUT);

  // RFID
  SPI.begin();
  rfid.PCD_Init();

  // LCD
  lcd.init();
  lcd.backlight();

  // Initial state
  standbyMode();

  Serial.println("RFID SYSTEM READY");
}

void loop() {

  // Wait for any RFID card/key
  if (!rfid.PICC_IsNewCardPresent()) {
    return;
  }

  if (!rfid.PICC_ReadCardSerial()) {
    return;
  }

  // Any card is accepted
  Serial.println("CARD DETECTED - ACCESS GRANTED");

  // SCANNING
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("SCANNING...");
  
  digitalWrite(RED1, LOW);
  digitalWrite(RED2, LOW);

  digitalWrite(GREEN1, HIGH);
  digitalWrite(GREEN2, HIGH);

  tone(BUZZER, 1000);
  delay(300);
  noTone(BUZZER);

  delay(1000);

  // DOOR OPEN
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("DOOR OPEN");

  lcd.setCursor(0, 1);
  lcd.print("WELCOME!");

  // Green LEDs stay ON for 6 seconds
  delay(6000);

  // Finish RFID reading
  rfid.PICC_HaltA();
  rfid.PCD_StopCrypto1();

  // Return to standby
  standbyMode();

  delay(500);
}

// Standby mode
void standbyMode() {

  digitalWrite(GREEN1, LOW);
  digitalWrite(GREEN2, LOW);

  digitalWrite(RED1, HIGH);
  digitalWrite(RED2, HIGH);

  noTone(BUZZER);

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("WELCOME");

  lcd.setCursor(0, 1);
  lcd.print("PUT YOUR KEY");
}
