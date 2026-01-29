### RC Sensor SR04



\#define TRIG\_PIN 13

\#define ECHO\_PIN 12



long duration;

float distance;



void setup() {

  Serial.begin(9600);



  pinMode(TRIG\_PIN, OUTPUT);

  pinMode(ECHO\_PIN, INPUT);

}



void loop() {

  // ส่งคลื่นเสียง

  digitalWrite(TRIG\_PIN, LOW);

  delayMicroseconds(2);



  digitalWrite(TRIG\_PIN, HIGH);

  delayMicroseconds(10);

  digitalWrite(TRIG\_PIN, LOW);



  // รับเวลาที่คลื่นสะท้อนกลับ

  duration = pulseIn(ECHO\_PIN, HIGH);



  // คำนวณระยะทาง (cm)

  distance = duration \* 0.034 / 2;



  Serial.print("Distance: ");

  Serial.print(distance);

  Serial.println(" cm");



  delay(100);

}



### RC Motor servo



\#define IN1 26

\#define IN2 27



void setup() {

  Serial.begin(9600);

  pinMode(IN1, OUTPUT);

  pinMode(IN2, OUTPUT);

}



void loop(){

  Serial.print("Way 1");

  digitalWrite(IN1, HIGH);

  digitalWrite(IN2, LOW);

  delay(3000);



  Serial.print("Way 2");

  digitalWrite(IN1, LOW);

  digitalWrite(IN2, HIGH);

  delay(3000);

}



### RC Button



\#define K1 32

\#define K2 33

\#define K3 25

\#define K4 14



int b1, b2, b3, b4;



void setup() {

  Serial.begin(9600);

  pinMode(K1, INPUT\_PULLUP);

  pinMode(K2, INPUT\_PULLUP);

  pinMode(K3, INPUT\_PULLUP);

  pinMode(K4, INPUT\_PULLUP);

}



void loop() {

  b1 = digitalRead(K1);

  b2 = digitalRead(K2);

  b3 = digitalRead(K3);

  b4 = digitalRead(K4);



  if (b1 == LOW) Serial.println("Button 1 Pressed");

  if (b2 == LOW) Serial.println("Button 2 Pressed");

  if (b3 == LOW) Serial.println("Button 3 Pressed");

  if (b4 == LOW) Serial.println("Button 4 Pressed");



  delay(100);

}



### RC Buzzer



\#define buzzer 15



void setup() {

pinMode(buzzer, OUTPUT);

Serial.begin(9600);

}



void loop() {

  Serial.println("Buzzer on");

  digitalWrite(buzzer, LOW);

  delay(500);



  Serial.println("Buzzer off");

  digitalWrite(buzzer, HIGH);

  delay(500);

}



### RR Motor servo



\#define IN1 10

\#define IN2 9

\#define IN3 6

\#define IN4 5



void setup() {

  Serial.begin(9600);

  pinMode(IN1, OUTPUT);

  pinMode(IN2, OUTPUT);

  pinMode(IN3, OUTPUT);

  pinMode(IN4, OUTPUT);

}



void loop() {

  digitalWrite(IN1, LOW);

  digitalWrite(IN2, LOW);

  digitalWrite(IN3, LOW);

  digitalWrite(IN4, LOW);

  delay(5000);



  Serial.println("ตรง");

  digitalWrite(IN1, HIGH);

  digitalWrite(IN2, LOW);

  digitalWrite(IN3, HIGH);

  digitalWrite(IN4, LOW);

  delay(2000);



  digitalWrite(IN1, LOW);

  digitalWrite(IN2, LOW);

  digitalWrite(IN3, LOW);

  digitalWrite(IN4, LOW);

  delay(1000);



  Serial.println("หลัง");

  digitalWrite(IN1, LOW);

  digitalWrite(IN2, HIGH);

  digitalWrite(IN3, LOW);

  digitalWrite(IN4, HIGH);

  delay(2000);



  digitalWrite(IN1, LOW);

  digitalWrite(IN2, LOW);

  digitalWrite(IN3, LOW);

  digitalWrite(IN4, LOW);

  delay(1000);

 

  Serial.println("ซ้าย");

  digitalWrite(IN1, HIGH);

  digitalWrite(IN2, LOW);

  digitalWrite(IN3, LOW);

  digitalWrite(IN4, HIGH);

  delay(2000);



  digitalWrite(IN1, LOW);

  digitalWrite(IN2, LOW);

  digitalWrite(IN3, LOW);

  digitalWrite(IN4, LOW);

  delay(1000);



  Serial.println("ขวา");

  digitalWrite(IN1, LOW);

  digitalWrite(IN2, HIGH);

  digitalWrite(IN3, HIGH);

  digitalWrite(IN4, LOW);

  delay(2000);



  digitalWrite(IN1, LOW);

  digitalWrite(IN2, LOW);

  digitalWrite(IN3, LOW);

  digitalWrite(IN4, LOW);

  delay(1000);

}



### 

