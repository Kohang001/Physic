#include <SPI.h>
#include <RF24.h>

// --- Config Pins ---
// Motor A (เช่น ขวา)
#define ENA 3 // ถ้าต้องการคุมความเร็ว ต่อขา Enable เข้าขา PWM (3,5,6,9,10,11)
#define IN1 10
#define IN2 9

// Motor B (เช่น ซ้าย)
#define ENB 11 // ถ้าต้องการคุมความเร็ว
#define IN3 6
#define IN4 5

// NRF Pins (Arduino Uno/Nano)
#define CE_PIN 8
#define CSN_PIN 7

RF24 radio(CE_PIN, CSN_PIN);
const byte address[6] = "MyA1"; // ต้องตรงกับตัวส่ง

// Variables
float distance = 0.0;
unsigned long lastRecvTime = 0;
const unsigned long TIMEOUT_MS = 500; // หยุดรถถ้าไม่ได้รับค่าเกิน 0.5 วิ

void setup() {
  Serial.begin(9600);
  Serial.println(F("Vehicle Receiver Ready"));

  // Motor Pins Setup
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  stopMotors();

  // NRF Setup
  if (!radio.begin()) {
    Serial.println(F("NRF24 Hardware Error!"));
    while (1); // หยุดการทำงานถ้าวาน NRF ไม่เจอ
  }
  radio.setChannel(90); // ต้องตรงกับตัวส่ง
  radio.setPALevel(RF24_PA_MAX);
  radio.setDataRate(RF24_1MBPS);
  radio.openReadingPipe(0, address);
  radio.startListening(); // เริ่มรับข้อมูล
}

void loop() {
  // 1. รับข้อมูล
  if (radio.available()) {
    radio.read(&distance, sizeof(distance));
    lastRecvTime = millis(); // รีเซ็ตเวลาจับ timeout
    
    Serial.print("Dist: ");
    Serial.print(distance);
    Serial.println(" cm");
    
    processMovement(distance);
  }
  
  // 2. ตรวจสอบ Timeout (Failsafe)
  if (millis() - lastRecvTime > TIMEOUT_MS) {
    if (distance != 0) { // แจ้งเตือนแค่ครั้งเดียวเมื่อหลุด
        Serial.println(F("Signal Lost - Stopping"));
        distance = 0; // Reset state
    }
    stopMotors();
  }
}

void processMovement(float dist) {
  // Logic: 3-8 Left | 8-13 Fwd | 13-18 Back | 18-22 Right
  
  if (dist >= 3 && dist < 8) {
    // --- Turn Left (หมุนซ้าย) ---
    // Motor A เดินหน้า / Motor B ถอยหลัง
    digitalWrite(IN1, HIGH); digitalWrite(IN2, LOW);
    digitalWrite(IN3, LOW);  digitalWrite(IN4, HIGH);
    // Serial.println("CMD: Left");
  }
  else if (dist >= 8 && dist < 13) {
    // --- Forward (เดินหน้า) ---
    // Motor A เดินหน้า / Motor B เดินหน้า
    digitalWrite(IN1, HIGH); digitalWrite(IN2, LOW);
    digitalWrite(IN3, HIGH); digitalWrite(IN4, LOW);
    // Serial.println("CMD: Forward");
  }
  else if (dist >= 13 && dist < 18) {
    // --- Backward (ถอยหลัง) ---
    // Motor A ถอยหลัง / Motor B ถอยหลัง
    digitalWrite(IN1, LOW);  digitalWrite(IN2, HIGH);
    digitalWrite(IN3, LOW);  digitalWrite(IN4, HIGH);
    // Serial.println("CMD: Backward");
  }
  else if (dist >= 18 && dist <= 22) {
    // --- Turn Right (หมุนขวา) ---
    // Motor A ถอยหลัง / Motor B เดินหน้า
    digitalWrite(IN1, LOW);  digitalWrite(IN2, HIGH);
    digitalWrite(IN3, HIGH); digitalWrite(IN4, LOW);
    // Serial.println("CMD: Right");
  }
  else {
    // อยู่นอกระยะกำหนด
    stopMotors();
  }
}

void stopMotors() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
}
