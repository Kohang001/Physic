#include <SPI.h>
#include <RF24.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// --- Config Pins ---
#define TRIG_PIN 13
#define ECHO_PIN 12

// Button Pins
#define k1 32
#define k2 33
#define k3 25
#define k4 14

// Local Motor Pins
#define IN1 26
#define IN2 27

// NRF
RF24 radio(4, 5); // CE, CSN
const byte address[6] = "MyA1"; // Address

// Variables
long duration;
float distance = 0.0;

// Timer Variables
unsigned long timerEndTime = 0;
bool isTimerRunning = false;
const int countdownMinutes = 5;

LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup() {
  Serial.begin(9600);
  Wire.begin();

  // LCD Setup
  lcd.begin(); 
  lcd.backlight();
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("System Ready...");

  // Sensor Setup
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  // Buttons Setup
  pinMode(k1, INPUT_PULLUP);
  pinMode(k2, INPUT_PULLUP);
  pinMode(k3, INPUT_PULLUP);
  pinMode(k4, INPUT_PULLUP);

  // Local Motor Setup
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);

  // NRF24 Setup
  if (!radio.begin()) {
    Serial.println("NRF24 Hardware Error!");
    lcd.setCursor(0, 0);
    lcd.print("NRF Error!    ");
  }
  radio.setChannel(90);
  radio.setPALevel(RF24_PA_MAX);
  radio.setDataRate(RF24_1MBPS);
  radio.openWritingPipe(address);
  radio.stopListening();
  
  delay(1000);
  lcd.clear();
}

void loop() {
  // Ultrasonic
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);
  duration = pulseIn(ECHO_PIN, HIGH, 30000);
  if (duration > 0) {
    distance = duration * 0.034 / 2;
  } else {
    distance = 0;
  }
  

  lcd.setCursor(0, 1);
  lcd.print("Dist: ");
  lcd.print(distance, 1);
  lcd.print(" cm    ");

  // --- อ่านปุ่มกด ---
  int b1 = digitalRead(k1);
  int b2 = digitalRead(k2);
  int b3 = digitalRead(k3);
  int b4 = digitalRead(k4);

  // --- Logic Timer ---
  if (b3 == LOW && !isTimerRunning) {
    timerEndTime = millis() + (countdownMinutes * 60 * 1000);
    isTimerRunning = true;
    Serial.println("Timer Started!");
    delay(200);
  }

  lcd.setCursor(0, 0);
  if (isTimerRunning) {
    long remainingTime = timerEndTime - millis();
    if (remainingTime > 0) {
      int secondsTotal = remainingTime / 1000;
      int mins = secondsTotal / 60;
      int secs = secondsTotal % 60;

      lcd.print("Time: ");
      lcd.print(mins);
      lcd.print(":");
      if (secs < 10) lcd.print("0");
      lcd.print(secs);
      lcd.print("     ");
    } else {
      isTimerRunning = false;
      lcd.print("Time's Up!      ");
      Serial.println("Time Finished");
    }
  } else {
    lcd.print("Timer: STOP     ");
  }

  // --- Local Motor Control ---
  if (b1 == LOW) {
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);
  } else if (b4 == LOW) {
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, HIGH);
  } else {
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, LOW);
  }

  // --- NRF Transmit Logic ---
  if (b2 == LOW) {
    Serial.print("Sending: ");
    Serial.println(distance);

    bool report = radio.write(&distance, sizeof(distance));
    
    if (!report) {
       Serial.println("Send Failed (Check Connection)");
    }
  }

  delay(50);
}
