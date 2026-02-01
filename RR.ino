#include <SPI.h>
#include <RF24.h>

#define IN1 10
#define IN2 9
#define IN3 6
#define IN4 5

// CE, CSN
RF24 radio(8, 7);

// Address ต้องตรงกับ TX
const byte address[6] = "MyA1";

float distance;

void setup() {
  Serial.begin(9600);
  Serial.println("SR04 Receiver Ready");


  //NRF
  radio.begin();
  radio.setChannel(90);
  radio.setPALevel(RF24_PA_HIGH);
  radio.setDataRate(RF24_1MBPS);
  radio.openReadingPipe(0, address);
  radio.startListening();   // โหมดรับ (RX)


  //Motor
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
}

void loop() {
  if (radio.available()) {
    radio.read(&distance, sizeof(distance));

    if (distance >= 3 && distance <= 7) {
      Serial.print("Distance: ");
      Serial.print(distance);
      Serial.print(" cm");
      Serial.println(" Turn left");
      digitalWrite(IN1, HIGH);
      digitalWrite(IN2, LOW);
      digitalWrite(IN3, LOW);
      digitalWrite(IN4, HIGH);
    }
    if (distance >= 8 && distance <= 12) {
      Serial.print("Distance: ");
      Serial.print(distance);
      Serial.print(" cm");
      Serial.println(" Front way");
      digitalWrite(IN1, HIGH);
      digitalWrite(IN2, LOW);
      digitalWrite(IN3, HIGH);
      digitalWrite(IN4, LOW);
    }
    if (distance >= 13 && distance <= 17) {
      Serial.print("Distance: ");
      Serial.print(distance);
      Serial.print(" cm");
      Serial.println(" Back way");
      digitalWrite(IN1, LOW);
      digitalWrite(IN2, HIGH);
      digitalWrite(IN3, LOW);
      digitalWrite(IN4, HIGH);
    }
    if (distance >= 18 && distance <= 22) {
      Serial.print("Distance: ");
      Serial.print(distance);
      Serial.print(" cm");
      Serial.println(" Turn right");
      digitalWrite(IN1, LOW);
      digitalWrite(IN2, HIGH);
      digitalWrite(IN3, HIGH);
      digitalWrite(IN4, LOW);
    }

    delay(150);
  }
}
