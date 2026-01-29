### ตัวส่ง(ส่งค่าปุ่ม)



\#include <SPI.h>

\#include <RF24.h>



\#define K1 32

\#define K2 33

\#define K3 25

\#define K4 14



// CE, CSN

RF24 radio(4, 5);



// Address ต้องยาว 5 byte

const byte address\[6] = "MyA1";



char message\[3];  // "B1" + '\\0'



void setup() {

&nbsp; Serial.begin(115200);



&nbsp; pinMode(K1, INPUT\_PULLUP);

&nbsp; pinMode(K2, INPUT\_PULLUP);

&nbsp; pinMode(K3, INPUT\_PULLUP);

&nbsp; pinMode(K4, INPUT\_PULLUP);



&nbsp; radio.begin();



&nbsp; radio.setChannel(90);          // ช่องสัญญาณ

&nbsp; radio.setPALevel(RF24\_PA\_HIGH); // แรงส่ง (เหมาะกับ PA+LNA)

&nbsp; radio.setDataRate(RF24\_1MBPS);  // เสถียรสุด

&nbsp; radio.openWritingPipe(address);

&nbsp; radio.stopListening();          // โหมดส่ง (TX)



&nbsp; Serial.println("NRF24 Transmitter Ready");

}



void loop() {

&nbsp; strcpy(message, "");   // เคลียร์ค่า



&nbsp; if (digitalRead(K1) == LOW) strcpy(message, "B1");

&nbsp; else if (digitalRead(K2) == LOW) strcpy(message, "B2");

&nbsp; else if (digitalRead(K3) == LOW) strcpy(message, "B3");

&nbsp; else if (digitalRead(K4) == LOW) strcpy(message, "B4");



&nbsp; if (message\[0] != '\\0') {

&nbsp;   Serial.print("Send: ");

&nbsp;   Serial.println(message);



&nbsp;   radio.write(\&message, sizeof(message));

&nbsp;   delay(200);

&nbsp; }

}



### ตัวรับ(รับค่าปุ่ม)



\#include <SPI.h>

\#include <RF24.h>



// CE, CSN

RF24 radio(8, 7);



// Address ต้องตรงกับตัวส่ง และยาว 5 byte

const byte address\[6] = "MyA1";



char message\[3];   // รับ "B1" "B2" "B3" "B4"



void setup() {

&nbsp; Serial.begin(115200);



&nbsp; radio.begin();



&nbsp; radio.setChannel(90);           // ต้องตรงกับ TX

&nbsp; radio.setPALevel(RF24\_PA\_HIGH); // เหมาะกับ PA+LNA

&nbsp; radio.setDataRate(RF24\_1MBPS);  // เสถียรสุด



&nbsp; radio.openReadingPipe(0, address);

&nbsp; radio.startListening();         // โหมดรับ (RX)



&nbsp; Serial.println("NRF24 Receiver Ready");

}



void loop() {

&nbsp; if (radio.available()) {

&nbsp;   radio.read(\&message, sizeof(message));



&nbsp;   Serial.print("Receive: ");

&nbsp;   Serial.println(message);

&nbsp; }

}



### ตัวส่ง(ส่งSensor SR04)



\#include <SPI.h>

\#include <RF24.h>



\#define TRIG\_PIN 13

\#define ECHO\_PIN 12



// CE, CSN

RF24 radio(4, 5);



// Address ต้องตรงกับตัวรับ (5 byte)

const byte address\[6] = "MyA1";



long duration;

float distance;



void setup() {

&nbsp; Serial.begin(9600);



&nbsp; pinMode(TRIG\_PIN, OUTPUT);

&nbsp; pinMode(ECHO\_PIN, INPUT);



&nbsp; radio.begin();

&nbsp; radio.setChannel(90);

&nbsp; radio.setPALevel(RF24\_PA\_HIGH);

&nbsp; radio.setDataRate(RF24\_1MBPS);

&nbsp; radio.openWritingPipe(address);

&nbsp; radio.stopListening();   // โหมดส่ง (TX)



&nbsp; Serial.println("SR04 Transmitter Ready");

}



void loop() {

&nbsp; // ส่งคลื่นเสียง

&nbsp; digitalWrite(TRIG\_PIN, LOW);

&nbsp; delayMicroseconds(2);



&nbsp; digitalWrite(TRIG\_PIN, HIGH);

&nbsp; delayMicroseconds(10);

&nbsp; digitalWrite(TRIG\_PIN, LOW);



&nbsp; // รับเวลาสะท้อน

&nbsp; duration = pulseIn(ECHO\_PIN, HIGH, 30000); // timeout 30ms (~5m)



&nbsp; if (duration > 0) {

&nbsp;   distance = duration \* 0.034 / 2;



&nbsp;   Serial.print("Send distance: ");

&nbsp;   Serial.print(distance);

&nbsp;   Serial.println(" cm");



&nbsp;   // ส่งค่า distance (float)

&nbsp;   radio.write(\&distance, sizeof(distance));

&nbsp; }



&nbsp; delay(100);

}



### ตัวรับ(รับค่าSensor SR04)



\#include <SPI.h>

\#include <RF24.h>



// CE, CSN

RF24 radio(8, 7);



// Address ต้องตรงกับ TX

const byte address\[6] = "MyA1";



float distance;



void setup() {

&nbsp; Serial.begin(9600);



&nbsp; radio.begin();

&nbsp; radio.setChannel(90);

&nbsp; radio.setPALevel(RF24\_PA\_HIGH);

&nbsp; radio.setDataRate(RF24\_1MBPS);



&nbsp; radio.openReadingPipe(0, address);

&nbsp; radio.startListening();   // โหมดรับ (RX)



&nbsp; Serial.println("SR04 Receiver Ready");

}



void loop() {

&nbsp; if (radio.available()) {

&nbsp;   radio.read(\&distance, sizeof(distance));



&nbsp;   Serial.print("Distance: ");

&nbsp;   Serial.print(distance);

&nbsp;   Serial.println(" cm");

&nbsp;   delay(200);

&nbsp; }

}



### ตัวส่ง(ส่งค่าปุ่มไปคุม)



\#include <SPI.h>

\#include <RF24.h>



\#define K1 32

\#define K2 33

\#define K3 25

\#define K4 14



// CE, CSN

RF24 radio(4, 5);

const byte address\[6] = "MyA1";



char command = 'S'; // S = Stop



void setup() {

&nbsp; Serial.begin(9600);



&nbsp; pinMode(K1, INPUT\_PULLUP);

&nbsp; pinMode(K2, INPUT\_PULLUP);

&nbsp; pinMode(K3, INPUT\_PULLUP);

&nbsp; pinMode(K4, INPUT\_PULLUP);



&nbsp; radio.begin();

&nbsp; radio.setChannel(90);

&nbsp; radio.setPALevel(RF24\_PA\_HIGH);

&nbsp; radio.setDataRate(RF24\_1MBPS);

&nbsp; radio.openWritingPipe(address);

&nbsp; radio.stopListening();   // TX mode



&nbsp; Serial.println("Controller Ready");

}



void loop() {

&nbsp; command = 'S'; // ค่าเริ่มต้น = หยุด



&nbsp; if (digitalRead(K1) == LOW) command = 'L';

&nbsp; else if (digitalRead(K2) == LOW) command = 'F';

&nbsp; else if (digitalRead(K3) == LOW) command = 'B';

&nbsp; else if (digitalRead(K4) == LOW) command = 'R';



&nbsp; radio.write(\&command, sizeof(command));



&nbsp; Serial.print("Send: ");

&nbsp; Serial.println(command);



&nbsp; delay(100);

}



### ตัวรับ(รับค่าปุ่มไปคุมรถ)



\#include <SPI.h>

\#include <RF24.h>



\#define IN1 10

\#define IN2 9

\#define IN3 6

\#define IN4 5



// CE, CSN

RF24 radio(8, 7);

const byte address\[6] = "MyA1";



char command;



void stopMotor() {

&nbsp; digitalWrite(IN1, LOW);

&nbsp; digitalWrite(IN2, LOW);

&nbsp; digitalWrite(IN3, LOW);

&nbsp; digitalWrite(IN4, LOW);

}



void forward() {

&nbsp; digitalWrite(IN1, HIGH);

&nbsp; digitalWrite(IN2, LOW);

&nbsp; digitalWrite(IN3, HIGH);

&nbsp; digitalWrite(IN4, LOW);

}



void backward() {

&nbsp; digitalWrite(IN1, LOW);

&nbsp; digitalWrite(IN2, HIGH);

&nbsp; digitalWrite(IN3, LOW);

&nbsp; digitalWrite(IN4, HIGH);

}



void left() {

&nbsp; digitalWrite(IN1, LOW);

&nbsp; digitalWrite(IN2, HIGH);

&nbsp; digitalWrite(IN3, HIGH);

&nbsp; digitalWrite(IN4, LOW);

}



void right() {

&nbsp; digitalWrite(IN1, HIGH);

&nbsp; digitalWrite(IN2, LOW);

&nbsp; digitalWrite(IN3, LOW);

&nbsp; digitalWrite(IN4, HIGH);

}



void setup() {

&nbsp; Serial.begin(9600);



&nbsp; pinMode(IN1, OUTPUT);

&nbsp; pinMode(IN2, OUTPUT);

&nbsp; pinMode(IN3, OUTPUT);

&nbsp; pinMode(IN4, OUTPUT);



&nbsp; stopMotor();



&nbsp; radio.begin();

&nbsp; radio.setChannel(90);

&nbsp; radio.setPALevel(RF24\_PA\_HIGH);

&nbsp; radio.setDataRate(RF24\_1MBPS);

&nbsp; radio.openReadingPipe(0, address);

&nbsp; radio.startListening();  // RX mode



&nbsp; Serial.println("Robot Ready");

}



void loop() {

&nbsp; if (radio.available()) {

&nbsp;   radio.read(\&command, sizeof(command));



&nbsp;   Serial.print("Receive: ");

&nbsp;   Serial.println(command);



&nbsp;   switch (command) {

&nbsp;     case 'F': forward();  break;

&nbsp;     case 'B': backward(); break;

&nbsp;     case 'L': left();     break;

&nbsp;     case 'R': right();    break;

&nbsp;     default:  stopMotor();

&nbsp;   }

&nbsp; }

}



