#include <AFMotor.h>  
#include <NewPing.h>
#include <Servo.h> 

#define TRIG_PIN A0 
#define ECHO_PIN A1 
#define MAX_DISTANCE 200 
#define MAX_SPEED 255 // sets speed of DC  motors  //190->90
#define MAX_SPEED_OFFSET 20

NewPing sonar(TRIG_PIN, ECHO_PIN, MAX_DISTANCE); 

AF_DCMotor motor1(1, MOTOR12_1KHZ); 
AF_DCMotor motor2(2, MOTOR12_1KHZ);
AF_DCMotor motor3(3, MOTOR34_1KHZ);
AF_DCMotor motor4(4, MOTOR34_1KHZ);
Servo myservo;   

boolean goesForward=false;
int distance = 100;
int speedSet = 0;

void setup() {

  myservo.attach(10);  
  myservo.write(115); 
  delay(2000);
  distance = readPing();
  delay(100);
  distance = readPing();
  delay(100);
  distance = readPing();
  delay(100);
  distance = readPing();
  delay(100);
}

void loop() {
 int distanceR = 0;
 int distanceL =  0;
 delay(40);
 
 if(distance<=20)//15->20
 {
  moveStop();
  delay(100);
  moveBackward();
  delay(300);
  moveStop();
  delay(200);
  distanceR = lookRight();
  delay(200);
  distanceL = lookLeft();
  delay(200);

  if(distanceR>=distanceL)
  {
    turnRight();
    moveStop();
  }else
  {
    turnLeft();
    moveStop();
  }
 }else
 {
  moveForward();
 }
 distance = readPing();
}

int lookRight()
{
    myservo.write(50); 
    delay(500);
    int distance = readPing();
    delay(100);
    myservo.write(115); 
    return distance;
}

int lookLeft()
{
    myservo.write(170); 
    delay(500);
    int distance = readPing();
    delay(100);
    myservo.write(115); 
    return distance;
    delay(100);
}

int readPing() { 
  delay(70);
  int cm = sonar.ping_cm();
  if(cm==0)
  {
    cm = 250;
  }
  return cm;
}

void moveStop() {
  motor1.run(RELEASE); 
  motor2.run(RELEASE);
  motor3.run(RELEASE);
  motor4.run(RELEASE);
  } 
  
void moveForward() {

 if(!goesForward)
  {
    goesForward=true;
    motor1.run(FORWARD);      
    motor2.run(FORWARD);
    motor3.run(FORWARD); 
    motor4.run(FORWARD);     
   for (speedSet = 0; speedSet < MAX_SPEED; speedSet +=2) // slowly bring the speed up to avoid loading down the batteries too quickly
   {
    motor1.setSpeed(speedSet);
    motor2.setSpeed(speedSet);
    motor3.setSpeed(speedSet);
    motor4.setSpeed(speedSet);
    delay(5);
   }
  }
}

void moveBackward() {
    goesForward=false;
    motor1.run(BACKWARD);      
    motor2.run(BACKWARD);
    motor3.run(BACKWARD);
    motor4.run(BACKWARD);  
  for (speedSet = 0; speedSet < MAX_SPEED; speedSet +=2) // slowly bring the speed up to avoid loading down the batteries too quickly
  {
    motor1.setSpeed(speedSet);
    motor2.setSpeed(speedSet);
    motor3.setSpeed(speedSet);
    motor4.setSpeed(speedSet);
    delay(5);
  }
}  

void turnRight() {
  motor1.run(FORWARD);
  motor2.run(FORWARD);
  motor3.run(BACKWARD);
  motor4.run(BACKWARD);     
  delay(1000);
  motor1.run(FORWARD);      
  motor2.run(FORWARD);
  motor3.run(FORWARD);
  motor4.run(FORWARD);      
} 
 
void turnLeft() {
  motor1.run(BACKWARD);     
  motor2.run(BACKWARD);  
  motor3.run(FORWARD);
  motor4.run(FORWARD);   
  delay(1000);
  motor1.run(FORWARD);     
  motor2.run(FORWARD);
  motor3.run(FORWARD);
  motor4.run(FORWARD);
}



Bluetooth controlled robot arm

#include <Servo.h>
#define SERVO_BASE            2
#define SERVO_SHOULDER        3
#define SERVO_ELBOW           4
#define SERVO_GRIPPER         5

Servo myservo_1;  // create servo object to control a servo
Servo myservo_2;
Servo myservo_3;
Servo myservo_4;

unsigned char Data_String[25],Data_Index = 0,New_Data_Rec_Flag = 0;
unsigned int Received_Servo_Value[4],Final_Servo_Val[4];
unsigned char Index_i = 0,Index_j = 0,Counter_to_Refresh = 0;

void setup() {
  Serial.begin(9600);
  myservo_1.attach(SERVO_BASE);       // attaches the servo on pin 2 to the servo object
  myservo_2.attach(SERVO_SHOULDER);   // attaches the servo on pin 3 to the servo object
  myservo_3.attach(SERVO_ELBOW);      // attaches the servo on pin 4 to the servo object
  myservo_4.attach(SERVO_GRIPPER);    // attaches the servo on pin 5 to the servo object

  myservo_1.write(90);
  delay(200);
  myservo_2.write(90);
  delay(200);
  myservo_3.write(90);
  delay(200);
  myservo_4.write(90);
  delay(200);

  Received_Servo_Value[0] = 90;     // Default values
  Received_Servo_Value[1] = 90;
  Received_Servo_Value[2] = 90;
  Received_Servo_Value[3] = 45;
  
  Final_Servo_Val[0] = 90;          // Default values
  Final_Servo_Val[1] = 90;
  Final_Servo_Val[2] = 90;
  Final_Servo_Val[3] = 45;
}

void loop()
{
  if(New_Data_Rec_Flag==0)
  {
    if (Serial.available())     // check whether bluetooth data is available
    {
      // read incoming serial data:
      char inChar = Serial.read();    // read bluetooth data one by one
      //Serial.print(inChar);
      if(inChar==0x0A)                // End character of bluetooth data
      {
          Data_String[Data_Index] = inChar;
          Data_Index = 0;
          New_Data_Rec_Flag = 1;
      }
      else
      {
        if(inChar!=0x2C)              // removing ascii value, except for 0x2C which is ascii for ,
        {
            inChar = inChar - 0x30;   // removing Ascii value of 0, so that we can get exat value
        }
        Data_String[Data_Index] = inChar;
        Data_Index++;
      }
    }
  }

  if(New_Data_Rec_Flag==1)
  {
      Received_Servo_Value[0] = 0;
      Received_Servo_Value[1] = 0;
      Received_Servo_Value[2] = 0;
      Received_Servo_Value[3] = 0;
      
      for(Index_i = 0,Index_j = 0;;)
      {
            if(Data_String[Index_j]==0x2C)
            {
                Index_j++;
                Index_i++;
                Serial.print("A ");
            }
            else if(Data_String[Index_j]==0x0A)
            {
                  New_Data_Rec_Flag = 0;
                  Serial.print("B ");
                  break;
            }
            else
            {
                Received_Servo_Value[Index_i] = Received_Servo_Value[Index_i] * 10 + Data_String[Index_j];
                Index_j++;
                Serial.print("C ");
            }
      }
      Serial.print(Received_Servo_Value[0]);
      Serial.print("  ");
      Serial.print(Received_Servo_Value[1]);
      Serial.print("  ");
      Serial.print(Received_Servo_Value[2]);
      Serial.print("  ");
      Serial.println(2*Received_Servo_Value[3]);
  }
  
  Counter_to_Refresh++;
  delay(1);
  if(Counter_to_Refresh >= 10)    // delay of 10 msec = 1 msec * 10, this will allow smooth movement of servos
  {
      Counter_to_Refresh = 0;
      if(Received_Servo_Value[0]!=Final_Servo_Val[0])
      {
          if(Received_Servo_Value[0]>Final_Servo_Val[0])
          {
              Final_Servo_Val[0]++;
          }

          if(Received_Servo_Value[0]<Final_Servo_Val[0])
          {
              Final_Servo_Val[0]--;
          }
          myservo_1.write(180 - Final_Servo_Val[0]);      // adjuted as per app
      }

      if(Received_Servo_Value[1]!=Final_Servo_Val[1])
      {
          if(Received_Servo_Value[1]>Final_Servo_Val[1])
          {
              Final_Servo_Val[1]++;
          }

          if(Received_Servo_Value[1]<Final_Servo_Val[1])
          {
              Final_Servo_Val[1]--;
          }
          myservo_2.write(Final_Servo_Val[1]);
      }

      if(Received_Servo_Value[2]!=Final_Servo_Val[2])
      {
          if(Received_Servo_Value[2]>Final_Servo_Val[2])
          {
              Final_Servo_Val[2]++;
          }

          if(Received_Servo_Value[2]<Final_Servo_Val[2])
          {
              Final_Servo_Val[2]--;
          }
          myservo_3.write(Final_Servo_Val[2]);
      }

      if(Received_Servo_Value[3]!=Final_Servo_Val[3])
      {
          if(Received_Servo_Value[3]>Final_Servo_Val[3])
          {
              Final_Servo_Val[3]++;
          }

          if(Received_Servo_Value[3]<Final_Servo_Val[3])
          {
              Final_Servo_Val[3]--;
          }
          myservo_4.write(180 - (2*Final_Servo_Val[3]));  // adjuted as per app
      }
  }
}