#include <SPI.h>
#include <RF24.h>

// Motor Pins
#define IN1 10
#define IN2 9
#define IN3 6
#define IN4 5

// NRF
RF24 radio(8, 7); // CE, CSN
const byte address[6] = "MyA1"; // Address

// Variables
float distance = 0.0;

// Variables for Safety Timeout
unsigned long lastRecvTime = 0;
const unsigned long TIMEOUT_MS = 500;

void setup() {
  Serial.begin(9600);
  Serial.println("Vehicle Receiver Ready");

  // Motor Setup
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  stopMotors();

  // NRF Setup
  if (!radio.begin()) {
    Serial.println("NRF24 Hardware Error!");
    while(1);
  }
  radio.setChannel(90);
  radio.setPALevel(RF24_PA_MAX);
  radio.setDataRate(RF24_1MBPS);
  radio.openReadingPipe(0, address);
  radio.startListening();
}

void loop() {
  if (radio.available()) {
    radio.read(&distance, sizeof(distance));
    lastRecvTime = millis();
    
    Serial.print("Received: ");
    Serial.println(distance);
    
    processMovement(distance);
  }
  
  if (millis() - lastRecvTime > TIMEOUT_MS) {
    stopMotors();
  }
}

void processMovement(float dist) {
  if (dist >= 3 && dist < 8) {
    // Turn Left
    digitalWrite(IN1, HIGH); digitalWrite(IN2, LOW);
    digitalWrite(IN3, LOW);  digitalWrite(IN4, HIGH);
    Serial.println("Left");
  }
  else if (dist >= 8 && dist < 13) {
    // Forward
    digitalWrite(IN1, HIGH); digitalWrite(IN2, LOW);
    digitalWrite(IN3, HIGH); digitalWrite(IN4, LOW);
    Serial.println("Forward");
  }
  else if (dist >= 13 && dist < 18) {
    // Backward
    digitalWrite(IN1, LOW);  digitalWrite(IN2, HIGH);
    digitalWrite(IN3, LOW);  digitalWrite(IN4, HIGH);
    Serial.println("Backward");
  }
  else if (dist >= 18 && dist <= 22) {
    // Turn Right
    digitalWrite(IN1, LOW);  digitalWrite(IN2, HIGH);
    digitalWrite(IN3, HIGH); digitalWrite(IN4, LOW);
    Serial.println("Right");
  }
  else {
    stopMotors();
  }
}

void stopMotors() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
}
