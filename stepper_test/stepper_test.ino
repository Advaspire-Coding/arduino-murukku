#include <AccelStepper.h>
#include <MultiStepper.h>
  
AccelStepper stepperT(1, 3, 2);  // 1=Stepper driver option, 3=Step pulse pin, 2=Direction, Axis=tank
AccelStepper stepperX(1, 6, 7);  // 1=Stepper driver option, 7=Step pulse pin, 6=Direction, Axis=x
AccelStepper stepperY(1, 11, 12);  // 1=Stepper driver option, 12=Step pulse pin, 11=Direction, Axis=y
AccelStepper stepperC(1, 13, 14);  // 1=Stepper driver option, 14=Step pulse pin, 13=Direction, Axis=cutter

MultiStepper steppers;

int button1 = 8;                //Button for Jogging X axis
int button2 = 9;                //Button for Jogging Y axis
int button3 = 15;               //Button for controlling motor's direction
int ledPin = 10;                //Pin for LED light

int button1State = 0;           //Variable for button1's state
int button2State = 0;           //Variable for button2's state
int button3State = 0;           //Variable for button3's state

void setup(){

  Serial.begin(9600);
  pinMode(8, INPUT);
  pinMode(9, INPUT);
  pinMode(10, OUTPUT);
  digitalWrite(10, LOW);
  stepperX.setMaxSpeed(300); // Set max speed. Steps per second
  stepperX.setAcceleration(50);  // set motor acceleration
  stepperY.setMaxSpeed(300); // Set max speed. Steps per second
  stepperY.setAcceleration(50);  // set motor acceleration
  stepperC.setMaxSpeed(200); // Set max speed. Steps per second
  stepperC.setAcceleration(50);  // set motor acceleration
  stepperT.setMaxSpeed(200); // Set max speed. Steps per second
  stepperT.setAcceleration(50);  // set motor acceleration
  
  steppers.addStepper(stepperX);
  steppers.addStepper(stepperY);
  steppers.addStepper(stepperT);
  steppers.addStepper(stepperC);
  
//  long positions[2]; // Array of desired stepper position  
//  positions[0] = 500;
//  positions[1] = 500;
//  steppers.moveTo(positions);
//  steppers.runSpeedToPosition(); // Blocks until all are in position
//  delay(1000);
//
//  positions[0] = -1000;
//  steppers.moveTo(positions);
//  steppers.runSpeedToPosition(); // Blocks until all are in position
//  delay(1000);
    
}

void loop(){

    
  button1State = digitalRead(button1);
  button2State = digitalRead(button2);
  button3State = digitalRead(button3);
  
  if (button1State == LOW && button3State == HIGH){
    digitalWrite(ledPin, HIGH);
    stepperX.setSpeed(300);
    stepperX.runSpeed();
  } else if (button1State == LOW && button3State == LOW){
    digitalWrite(ledPin, HIGH);
    stepperX.setSpeed(-300);
    stepperX.runSpeed();
  }
  
  if (button2State == LOW && button3State == HIGH) {
    digitalWrite(ledPin, HIGH);
    stepperY.setSpeed(300);
    stepperY.runSpeed();
  } else if (button2State == LOW && button3State == LOW){
    digitalWrite(ledPin, HIGH);
    stepperY.setSpeed(-300);
    stepperY.runSpeed();
  }

  if (button1State == LOW || button2State == LOW || button3State == LOW){
    digitalWrite(ledPin, LOW);
  }
  

}
