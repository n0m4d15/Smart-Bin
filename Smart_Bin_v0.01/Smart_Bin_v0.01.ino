// Program to Print Sensor Data from Smart Bin into the Serial Monitor
// This program is meant to be uploaded onto an Arduino Nano, as it is the central node of the project
// Interface sensors and actuator into the appropriate pins

#include<Servo.h>

const int MAINLED = 13;
const int echo = 2;
const int trig = 3;
const int bin_lid = 4;
const int mq5 = A0;
const int mq6 = A1;

int dist, mq5_val, mq6_val;
int thresh = 6, mq5_thresh = 700, mq6_thresh = 700;
long dur;
Servo s;

void led_blink(int);
void lid_chk(void);
void fill_chk(void);
void gas_chk(void);

void setup() {

  pinMode(MAINLED, OUTPUT);
  pinMode(echo, INPUT);
  pinMode(trig, OUTPUT);
  pinMode(bin_lid, INPUT);
  pinMode(mq5, INPUT);
  pinMode(mq6, INPUT);
  s.attach(5);

  Serial.begin(9600);

  Serial.println("_______________________________");
  Serial.println("Warming up Sensors, Please Wait");
  Serial.println("_______________________________");
  s.write(0);
  delay(3000);                                      // Warm up time (3 seconds)
  Serial.println("Starting SMART BIN v0.01");
  digitalWrite(MAINLED, HIGH);
}

void loop() {
  lid_chk();
  fill_lvl();
  gas_chk();
}

void led_blink(int a) {
  digitalWrite(MAINLED, LOW);
  delay(a);
  digitalWrite(MAINLED, HIGH);
  delay(a);
}

void lid_chk() {
  if (digitalRead(bin_lid) == LOW) {
    Serial.println("Bin Lid is OPEN!");
    led_blink(1000);
    led_blink(1000);
    led_blink(1000);
    led_blink(1000);
    Serial.println("Closing the Bin LID Automatically");
    s.write(0);                                      // Close Lid of the Bin
    Serial.println("Bin Lid is CLOSED");
  }
  else {
    Serial.println("Bin Lid is CLOSED");
  }
}

void fill_lvl() {
  digitalWrite(trig, LOW);
  delayMicroseconds(2);
  digitalWrite(trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig, LOW);
  dur = pulseIn(echo, HIGH);
  dist = dur * 0.034 / 2;
  Serial.print("Remaining Fill Level : ");
  Serial.print(dist);
  Serial.println(" cm");

  if (dist < thresh) {                              // 'thresh' is a variable storing min. fill val
    Serial.println("Alert : Bin is FULL!");         // Alert : Bin is Full
    led_blink(200);
    led_blink(200);
    led_blink(200);
    led_blink(200);
  }
}

void gas_chk() {
  mq5_val = analogRead(mq5);
  mq6_val = analogRead(mq6);

  Serial.print("MQ5 Value : ");
  Serial.println(mq5_val);
  Serial.print("MQ6 Value : ");
  Serial.println(mq6_val);

  if (mq5_val > mq5_thresh) {                       // mq5_thresh and mq6_thresh is a variable storing min. toxicity/flammable val
    Serial.println("Alert : Toxic/Flammable Entity Detected!");
  }
  if (mq6_val > mq6_thresh) {
    Serial.println("Alert : Toxic/Flammable Entity Detected!");
  }
}
