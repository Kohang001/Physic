#include <SPI.h>
#include <RF24.h>

#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#define TRIG_PIN 13
#define ECHO_PIN 12

#define k1 32
#define k2 33
#define k3 25
#define k4 14

#define IN1 26
#define IN2 27

// CE, CSN
RF24 radio(4, 5);
// Address ต้องตรงกับตัวรับ (5 byte)
const byte address[6] = "MyA1";

int b1, b2, b3, b4;
long duration;
float distance;

LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup() {
  Serial.begin(9600);

  // LCD
  lcd.begin();
  lcd.backlight();
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("SR04 Ready");

  // SR04
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  // NRF
  radio.begin();
  radio.setChannel(90);
  radio.setPALevel(RF24_PA_HIGH);
  radio.setDataRate(RF24_1MBPS);
  radio.openWritingPipe(address);
  radio.stopListening();  // โหมดส่ง (TX)

  // Button
  pinMode(k1, INPUT_PULLUP);
  pinMode(k2, INPUT_PULLUP);
  pinMode(k3, INPUT_PULLUP);
  pinMode(k4, INPUT_PULLUP);

  //Motor
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
}

void loop() {
  // ส่งคลื่นเสียง
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);

  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  // รับเวลาสะท้อน
  duration = pulseIn(ECHO_PIN, HIGH, 30000);

  if (duration > 0) {
    distance = duration * 0.034 / 2;
    Serial.println(distance);
    lcd.setCursor(0, 1);
    lcd.print(distance);

    // อ่านปุ่ม
    b1 = digitalRead(k1);
    b2 = digitalRead(k2);
    b3 = digitalRead(k3);
    b4 = digitalRead(k4);

    if (b1 == LOW) {
      lcd.setCursor(0,0);
      lcd.print("                ");
      lcd.setCursor(0, 0);
      lcd.print("Left way");
      Serial.println("Left way");
      digitalWrite(IN1, HIGH);
      digitalWrite(IN2, LOW);
    }
    if (b2 == LOW) {
      lcd.setCursor(0,0);
      lcd.print("                ");
      lcd.setCursor(0, 0);
      lcd.print("Send data");
      Serial.println("Send data");
      radio.write(&distance, sizeof(distance));
    }
    if (b3 == LOW) {
      lcd.setCursor(0,0);
      lcd.print("                ");
      lcd.setCursor(0, 0);
      lcd.print("Button 3");
      Serial.println("Button 3");
    }
    if (b4 == LOW) {
      lcd.setCursor(0,0);
      lcd.print("                ");
      lcd.setCursor(0, 0);
      lcd.print("Right way");
      Serial.println("Right way");
      digitalWrite(IN1, LOW);
      digitalWrite(IN2, HIGH);
    }
  }

  delay(150);
}
