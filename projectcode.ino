#include <Servo.h>
#include <SPI.h>
#include <Pixy.h>

#define X_CENTER        ((PIXY_MAX_X-PIXY_MIN_X)/2)       
#define Y_CENTER        ((PIXY_MAX_Y-PIXY_MIN_Y)/2)

#define lightSensor A0
#define tempSensor A1

#define Enable1 9
#define Enable3 10
#define Enable4 5
#define Input1 3
#define Input2 4
#define Input3 6
#define Input4 7
#define Input5 1
#define Input6 2

Pixy pixy;
int angle = 0;
Servo myservo;
float x_pos;
float middle = 156.0;
int current;
float factor = 0.1;
uint16_t blocks;
double lightLVL;
double tempout;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial.print("Starting...\n");
  pinMode(lightSensor,INPUT);
  pinMode(tempSensor, INPUT);
  pinMode(Enable1, OUTPUT);
  pinMode(Enable3, OUTPUT);
  pinMode(Enable4, OUTPUT);
  pinMode(Input1, OUTPUT);
  pinMode(Input2, OUTPUT);
  pinMode(Input3, OUTPUT);
  pinMode(Input4, OUTPUT);
  pinMode(Input5, OUTPUT);
  pinMode(Input6, OUTPUT);
  pixy.init(); 
}

void loop() {
  // put your main code here, to run repeatedly:
  lightLVL=analogRead(lightSensor);
  tempout = analogRead(tempSensor);
  float voltage = tempout * 5.0;
  voltage /= 1024.0;
  float temperatureC = (voltage - 0.5) * 100;
  Serial.print(temperatureC); 
  Serial.println(" degrees C");
  float temperatureF = (temperatureC * 9.0 / 5.0) + 32.0;
  Serial.print(temperatureF); 
  Serial.println(" degrees F");

  int blocks = pixy.getBlocks();
  float wid = pixy.blocks[0].width;
  float x_pos = pixy.blocks[0].x;
  float difference = x_pos -156.0;
  //Serial.println(difference);
  //Serial.println(wid);

  if (lightLVL == 0) {
    analogWrite(Enable1, 0);
    analogWrite(Enable3, 0);
    analogWrite(Enable4, 0);
  }

  if (lightLVL > 0) {
    while (wid < 100 && blocks > 0) {
      //Serial.println("Foward!!!");
      analogWrite(Enable1, 255);
      analogWrite(Enable3, 255);
      analogWrite(Enable4, 255);
      digitalWrite(Input1, HIGH);
      digitalWrite(Input2, LOW);
      digitalWrite(Input3, HIGH);
      digitalWrite(Input4, LOW);
      digitalWrite(Input5, HIGH);
      digitalWrite(Input6, LOW);
    }

    if (blocks == 0 ) {
      //Serial.println("Stopped...");
      analogWrite(Enable1, 0);
      analogWrite(Enable3, 0);
      analogWrite(Enable4, 0);
    }
  
    while (wid > 100 && blocks > 0 ) {
        //Serial.println("Backwarding!!");
        analogWrite(Enable1, 255);
        analogWrite(Enable3, 255);
        analogWrite(Enable4, 150);
        digitalWrite(Input1, LOW);
        digitalWrite(Input2, HIGH);
        digitalWrite(Input3, LOW);
        digitalWrite(Input4, HIGH);
        digitalWrite(Input5, HIGH);
        digitalWrite(Input6, LOW);
    }
  
    while (difference > 60.00 && blocks >0) {
        //Serial.println("turning right!");
        analogWrite(Enable1, 150);
        analogWrite(Enable3, 100);
        digitalWrite(Input1, HIGH);
        digitalWrite(Input2, LOW);
        digitalWrite(Input3, LOW);
        digitalWrite(Input4, HIGH);
    }
  
    while (difference < -60.00 && blocks > 0) {
        //Serial.println("turning left!");
        analogWrite(Enable1, 150);
        analogWrite(Enable3, 100);
        digitalWrite(Input1, LOW);
        digitalWrite(Input2, HIGH);
        digitalWrite(Input3, HIGH);
        digitalWrite(Input4, LOW);
    }
  }
  delay(30);
}
