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

int prevFanSpeed;
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
  prevFanSpeed = 255;
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

//  Serial.print("Current Temperature = ");
//  Serial.println(tempout);

  int blocks = pixy.getBlocks();
  float wid = pixy.blocks[0].width;
  float x_pos = pixy.blocks[0].x;
  float difference = x_pos -156.0;
  //Serial.println(difference);
  //Serial.println(wid);

  if (temperatureC < 20.0) {
    analogWrite(Enable4, 0);
  }

  if (lightLVL < 300) {
    analogWrite(Enable1, 0);
    analogWrite(Enable3, 0);
    analogWrite(Enable4, 0);
  }

  if (lightLVL > 300) {
    if (wid < 100 && blocks > 0) {
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
      prevFanSpeed = 255;
    }

    if (wid < 150 && wid > 100 && blocks > 0) {
      analogWrite(Enable1, 0);
      analogWrite(Enable3, 0);
      analogWrite(Enable4, prevFanSpeed);
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
      analogWrite(Enable4, prevFanSpeed);
    }
  
    if (wid > 150 && blocks > 0 ) {
        //Serial.println("Backwards");
        analogWrite(Enable1, 255);
        analogWrite(Enable3, 255);
        analogWrite(Enable4, 50);
        digitalWrite(Input1, LOW);
        digitalWrite(Input2, HIGH);
        digitalWrite(Input3, LOW);
        digitalWrite(Input4, HIGH);
        digitalWrite(Input5, HIGH);
        digitalWrite(Input6, LOW);
        prevFanSpeed = 100;
     }
  
    if (difference > 70.00 && blocks >0) {
        //Serial.println("turning forward right!");
        analogWrite(Enable1, 150);
        analogWrite(Enable3, 0);
        digitalWrite(Input1, HIGH);
        digitalWrite(Input2, LOW);
        digitalWrite(Input3, LOW);
        digitalWrite(Input4, HIGH);
    }
  
    if (difference < -70.00 && blocks > 0) {
        //Serial.println("turning forward left!");
        analogWrite(Enable1, 0);
        analogWrite(Enable3, 150);
        digitalWrite(Input1, LOW);
        digitalWrite(Input2, HIGH);
        digitalWrite(Input3, HIGH);
        digitalWrite(Input4, LOW);
    }
  }
  delay(30);
}

int getSpeed(int width, int color, int temp) {
  // two temp thresholds, two distance thresholds (close far), color high low
  // high medium-high, medium-low, low fan settings
  //color 1 is fast, 2 is slow
  if (color == 1 || temp > 70) {
    //fast
    return 255;
  } else {

    if (width < 20) {
      return 255;
    } else if (width < 50) {
      return 200;
    } else if (width < 100) {
      return 150;
    } else if (width < 200) {
      return 75;
    } else {
      return 50;
    }
    //slow
    if (width < 100) {
      return 100;
    }
    return 50;
  }
}
