#include <AccelStepper.h>
#include <MultiStepper.h>
  
AccelStepper stepperX(1, 2, 53);  // 1=Stepper driver option, 2=Step pulse pin, 3=Direction, X Axis
AccelStepper stepperY(1, 4, 5);  // 1=Stepper driver option, 4=Step pulse pin, 5=Direction, Y Axis
AccelStepper stepperZ(1, 6, 7);  // 1=Stepper driver option, 6=Step pulse pin, 7=Direction, Lifting Axis
AccelStepper stepperT(1, 12, 13);  // 1=Stepper driver option, 8=Step pulse pin, 9=Direction, Tank Axis
AccelStepper stepperC(1, 10, 11);  // 1=Stepper driver option, 10=Step pulse pin, 11=Direction, Cutter Axis

MultiStepper steppers;
MultiStepper steppers_2;

//home Switch for each of the steppers

const int homeSwitchX = 21;  
const int homeSwitchY = 22;
const int homeSwitchZ = 14;
const int homeSwitchT = 15;
const int homeSwitchC = 27;

//Input pin

const int selectionSwitch = 17;
const int pushStartButton = 18;
const int manual_up = 28;
const int manual_down = 29;
const int manual_switch = 30;

//Output Pin
const int buzzer = 20;
const int greenLight = 24;
const int redLight = 25;

byte hsvalx;  
byte hsvaly; 
byte hsvalz;  
byte hsvalt; 
byte hsvalc;

int startButtonState = 0;
int stopButtonState = 0;

float x_ratio = 1876 / 19;
float y_ratio = 1876 / 19;
float t_ratio = 3000 / 110;
int tankMove = 0;
int cutter_dir = 1;

long positions[2];
long positions_2[1];

void setup(){

  Serial.begin(9600);
  pinMode(pushStartButton, INPUT_PULLUP);
  pinMode(selectionSwitch, INPUT_PULLUP);
  pinMode(manual_up, INPUT_PULLUP);
  pinMode(manual_down, INPUT_PULLUP);
  pinMode(manual_switch, INPUT_PULLUP);
  pinMode(homeSwitchX, INPUT_PULLUP);
  pinMode(homeSwitchY, INPUT_PULLUP);
  pinMode(homeSwitchZ, INPUT_PULLUP);
  pinMode(homeSwitchT, INPUT_PULLUP);
  pinMode(homeSwitchC, INPUT_PULLUP);
  pinMode(buzzer, OUTPUT);
  pinMode(greenLight, OUTPUT);
  pinMode(redLight, OUTPUT);
  
  stepperX.setMaxSpeed(1500); // Set max speed. Steps per second
  stepperX.setAcceleration(1000);  // set motor acceleration
  stepperY.setMaxSpeed(1500); // Set max speed. Steps per second
  stepperY.setAcceleration(1000);  // set motor acceleration
  stepperT.setMaxSpeed(1500); // Set max speed. Steps per second
  stepperT.setAcceleration(1000);  // set motor acceleration
  stepperZ.setMaxSpeed(1500); // Set max speed. Steps per second
  stepperZ.setAcceleration(1000);  // set motor acceleration
  stepperC.setMaxSpeed(3000); // Set max speed. Steps per second
  stepperC.setAcceleration(1000);  // set motor acceleration

  while(digitalRead(manual_switch) == LOW){

    stepperT.setMaxSpeed(600); // Set max speed. Steps per second
    stepperT.setAcceleration(50);  // set motor acceleration
     
     if(digitalRead(manual_up) == LOW){
          digitalWrite(redLight, LOW);
          digitalWrite(greenLight, HIGH);
          stepperT.setSpeed(-600); 
          stepperT.runSpeed(); 
          
     } else if(digitalRead(manual_down) == LOW){
          digitalWrite(redLight, LOW);
          digitalWrite(greenLight, HIGH);
          stepperT.setSpeed(600); 
          stepperT.runSpeed(); 
     }
       
  }
  
  steppers.addStepper(stepperX);
  steppers.addStepper(stepperY);
  steppers.addStepper(stepperT);
  
  steppers_2.addStepper(stepperC);
  steppers_2.addStepper(stepperZ);
    
  Serial.print("Home Pos Setup");
  Serial.print("\n");
  //delay(100);         

  digitalWrite(redLight, LOW);
  digitalWrite(greenLight, HIGH);
  
  stepperHomeX(); //intialize run routine to home motor

  stepperX.setCurrentPosition(0); 
  stepperY.setCurrentPosition(0); 
  stepperT.setCurrentPosition(0); 

  positions[0] = -3752;
  positions[1] = 0;
  positions[2] = 0;
  steppers.moveTo(positions);
  steppers.runSpeedToPosition();

  stepperX.setCurrentPosition(0); 

  stepperHomeY(); //intialize run routine to home motor
  stepperHomeZ(); //intialize run routine to home motor
  stepperHomeT(); //intialize run routine to home motor
  stepperHomeC(); //intialize run routine to home motor

  positions[0] = 0;
  positions[1] = 3752;
  positions[2] = -6950;
  steppers.moveTo(positions);
  steppers.runSpeedToPosition();

  stepperY.setCurrentPosition(0);
  stepperT.setCurrentPosition(0); 

  //Hold motors' torque
  
  positions[0] = 0;
  positions[1] = 0;
  positions[2] = 0;
  steppers.moveTo(positions);
  steppers.runSpeedToPosition();
  
  positions_2[0] = 10500;
  positions_2[1] = 16000;
  steppers_2.moveTo(positions_2);
  steppers_2.runSpeedToPosition();

  //set current positions
  
  stepperZ.setCurrentPosition(0); 
  stepperC.setCurrentPosition(0); 

  //indicate ready for running

  digitalWrite(redLight, HIGH);
  digitalWrite(greenLight, LOW);
    
  //Manually Jog Tank motor to make sure the tank is ready to push out murukku
 
  while(digitalRead(manual_switch) == HIGH){

     stepperT.setMaxSpeed(600); // Set max speed. Steps per second
     stepperT.setAcceleration(50);  // set motor acceleration
     
       if(digitalRead(manual_up) == LOW){
          digitalWrite(redLight, LOW);
          digitalWrite(greenLight, HIGH);
          stepperT.setSpeed(-600); 
          stepperT.runSpeed(); 
          
       } else if(digitalRead(manual_down) == LOW){
          digitalWrite(redLight, LOW);
          digitalWrite(greenLight, HIGH);
          stepperT.setSpeed(600); 
          stepperT.runSpeed(); 
       }
       
    }

    digitalWrite(redLight, HIGH);
    digitalWrite(greenLight, LOW);

    positions_2[0] = -10000;        //cutter run with direction
    positions_2[1] = 0;                         //Lifter lower down
    steppers_2.moveTo(positions_2);
    steppers_2.runSpeedToPosition();
    
    //Set All Currrent Position to zero
    stepperT.setCurrentPosition(0);
    stepperX.setCurrentPosition(0);
    stepperY.setCurrentPosition(0);
    stepperZ.setCurrentPosition(0);
    stepperC.setCurrentPosition(0);
    
    stepperT.setMaxSpeed(1500); // Set max speed. Steps per second
    stepperT.setAcceleration(50);  // set motor acceleration

}

void loop(){

    int murukku_interval = 0;
    int murukku_layer_count = 0;
    int chippi_ort = 1;
    int chippi_layer = 0;
    
    while(digitalRead(selectionSwitch) == HIGH){
          
    //Selection Switch on Murukku Recipe, Push Start to run the machine

        if(digitalRead(pushStartButton) == LOW){
          
          digitalWrite(redLight, LOW);
          digitalWrite(greenLight, HIGH); 
                  
          for(int i = 0; i < 3; i++){

            tankMove = tankMove + round(2.5 * t_ratio);
            positions[0] = round(0 * x_ratio);
            positions[1] = round((0 - murukku_interval) * y_ratio ) ;
            positions[2] = tankMove;
            steppers.moveTo(positions);
            steppers.runSpeedToPosition();
            
            tankMove = tankMove + round(0.5 * t_ratio);
            positions[0] = round(0 * x_ratio);
            positions[1] = round((0 - murukku_interval) * y_ratio);
            positions[2] = tankMove;
            steppers.moveTo(positions);
            steppers.runSpeedToPosition();
            
            tankMove = tankMove + round(0.5 * t_ratio);
            positions[0] = round(-2 * x_ratio);
            positions[1] = round((-5 - murukku_interval) * y_ratio);
            positions[2] = tankMove;
            steppers.moveTo(positions);
            steppers.runSpeedToPosition();
            
            tankMove = tankMove + round(0.5 * t_ratio);
            positions[0] = round(5 * x_ratio);
            positions[1] = round((-9 - murukku_interval) * y_ratio);
            positions[2] = tankMove;
            steppers.moveTo(positions);
            steppers.runSpeedToPosition();
            
            tankMove = tankMove + round(0.5 * t_ratio);
            positions[0] = round(16 * x_ratio);
            positions[1] = round((-1 - murukku_interval) * y_ratio);
            positions[2] = tankMove;
            steppers.moveTo(positions);
            steppers.runSpeedToPosition();
            
            tankMove = tankMove + round(0.5 * t_ratio);
            positions[0] = round(14 * x_ratio);
            positions[1] = round((15 - murukku_interval) * y_ratio);
            positions[2] = tankMove;
            steppers.moveTo(positions);
            steppers.runSpeedToPosition();
            
            tankMove = tankMove + round(0.5 * t_ratio);
            positions[0] = round(-1 * x_ratio);
            positions[1] = round((23 - murukku_interval) * y_ratio) ;
            positions[2] = tankMove;
            steppers.moveTo(positions);
            steppers.runSpeedToPosition();
            
            tankMove = tankMove + round(0.5 * t_ratio);
            positions[0] = round(-18 * x_ratio);
            positions[1] = round((14 - murukku_interval) * y_ratio);
            positions[2] = tankMove;
            steppers.moveTo(positions);
            steppers.runSpeedToPosition();
            
            tankMove = tankMove + round(0.5 * t_ratio);
            positions[0] = round(-26 * x_ratio);
            positions[1] = round((-3 - murukku_interval) * y_ratio);
            positions[2] = tankMove;
            steppers.moveTo(positions);
            steppers.runSpeedToPosition();
            
            tankMove = tankMove + round(0.5 * t_ratio);
            positions[0] = round(-19 * x_ratio);
            positions[1] = round((-18 - murukku_interval) * y_ratio);
            positions[2] = tankMove;
            steppers.moveTo(positions);
            steppers.runSpeedToPosition();
            
            tankMove = tankMove + round(0.5 * t_ratio);
            positions[0] = round(-1 * x_ratio);
            positions[1] = round((-24 - murukku_interval) * y_ratio);
            positions[2] = tankMove;
            steppers.moveTo(positions);
            steppers.runSpeedToPosition();
            
            tankMove = tankMove + round(0.5 * t_ratio);
            positions[0] = round(17 * x_ratio);
            positions[1] = round((-18 - murukku_interval) * y_ratio);
            positions[2] = tankMove;
            steppers.moveTo(positions);
            steppers.runSpeedToPosition();
            
            tankMove = tankMove + round(0.5 * t_ratio);
            positions[0] = round(24 * x_ratio);
            positions[1] = round((-4 - murukku_interval) * y_ratio);
            positions[2] = tankMove;
            steppers.moveTo(positions);
            steppers.runSpeedToPosition();
            
            tankMove = tankMove + round(0.5 * t_ratio);
            positions[0] = round(20 * x_ratio);
            positions[1] = round((8 - murukku_interval) * y_ratio);
            positions[2] = tankMove;
            steppers.moveTo(positions);
            steppers.runSpeedToPosition();
            
            tankMove = tankMove + round(0.5 * t_ratio);
            positions[0] = round(9 * x_ratio);
            positions[1] = round((5 - murukku_interval) * y_ratio);
            positions[2] = tankMove;
            steppers.moveTo(positions);
            steppers.runSpeedToPosition();
            
            tankMove = tankMove + round(0.5 * t_ratio);
            positions[0] = 0;
            positions[1] = round((0 - murukku_interval) * y_ratio);
            positions[2] = tankMove;
            steppers.moveTo(positions);
            steppers.runSpeedToPosition();
        
            //Cut off murukku
            
            positions_2[0] = 10000 * cutter_dir;            //cutter run with direction
            positions_2[1] = -3800 * murukku_layer_count;   //Lifter lower down
            steppers_2.moveTo(positions_2);
            steppers_2.runSpeedToPosition();

            if(murukku_layer_count % 2 == 0){
            
              murukku_interval = murukku_interval - 70;
                  
            } else {
                  
              murukku_interval = murukku_interval + 70;
                  
            }
            
            if(murukku_interval <= -140){

              murukku_interval = -140;
                  
            } else if(murukku_interval >= 140){
              
              murukku_interval = 140;
              
            }
           
            if(cutter_dir == 1){
              cutter_dir = 0;
            } else {
              cutter_dir = 1;
            }

          }

          //After a layer done, put a paper on it and press start button to continue next layer
          
          murukku_layer_count = murukku_layer_count + 1;
          
          positions_2[0] = 0;                                
          positions_2[1] = -3800 * murukku_layer_count;             //Lifter lower down
          steppers_2.moveTo(positions_2);
          steppers_2.runSpeedToPosition();
        }

        
    }

    while(digitalRead(selectionSwitch) == LOW){

        if(digitalRead(pushStartButton) == LOW){
          if(chippi_ort == 1){                    //right to left
            
            tankMove = tankMove + round(5 * t_ratio);
            positions[0] = 0;
            positions[1] = 0;
            positions[2] = tankMove;
            steppers.moveTo(positions);
            steppers.runSpeedToPosition();
  
            tankMove = tankMove + round(15 * t_ratio);
            positions[0] = 0;
            positions[1] = 18751;
            positions[2] = tankMove;
            steppers.moveTo(positions);
            steppers.runSpeedToPosition();
            
          } else {
            tankMove = tankMove + round(5 * t_ratio);
            positions[0] = 0;
            positions[1] = 18751;
            positions[2] = tankMove;
            steppers.moveTo(positions);
            steppers.runSpeedToPosition();
  
            tankMove = tankMove + round(15 * t_ratio);
            positions[0] = 0;
            positions[1] = 0;
            positions[2] = tankMove;
            steppers.moveTo(positions);
            steppers.runSpeedToPosition();
          }

          if(chippi_ort == 1){
              chippi_ort = 0;
          } else {
              chippi_ort = 1;
          }

          //Chippis Done

          if(cutter_dir == 1){
              cutter_dir = 0;
          } else {
              cutter_dir = 1;
          }

          positions_2[0] = 10000 * cutter_dir;                                
          positions_2[1] = -3800 * chippi_layer;             //Lifter lower down
          steppers_2.moveTo(positions_2);
          steppers_2.runSpeedToPosition();
          
        }
    }

}

void stepperHomeX(){ 
  hsvalx = digitalRead(homeSwitchX);  
  while (hsvalx == HIGH)  
  {
    // drive stepper slowly until it makes the home switch 
    stepperX.setSpeed(563); 
    stepperX.runSpeed(); 
    stepperX.moveTo(1876); 
    stepperX.run();  
    hsvalx = digitalRead(homeSwitchX);  
  } 
  
  stepperX.setCurrentPosition(0); 
  delay(500); 
  
}


void stepperHomeY(){ 
  hsvaly = digitalRead(homeSwitchY);  
  while (hsvaly == HIGH)  
  {
    // drive stepper slowly until it makes the home switch 
    stepperY.setSpeed(-563); 
    stepperY.runSpeed(); 
    stepperY.moveTo(1876); 
    stepperY.run();  
    hsvaly = digitalRead(homeSwitchY);  
  } 
  
  stepperY.setCurrentPosition(0); 
  delay(500); 
  
}

void stepperHomeT(){ 
  hsvalt = digitalRead(homeSwitchT);  
  while (hsvalt == HIGH)  
  {
    // drive stepper slowly until it makes the home switch 
    stepperT.setSpeed(1000); 
    stepperT.runSpeed(); 
    stepperT.moveTo(10000); 
    stepperT.run();  
    hsvalt = digitalRead(homeSwitchT);  
  } 

  stepperT.setCurrentPosition(0); 
  delay(500); 
  
}

void stepperHomeZ(){ 
  hsvalz = digitalRead(homeSwitchZ);  
  while (hsvalz == HIGH)  
  {
    // drive stepper slowly until it makes the home switch 
    stepperZ.setSpeed(-1500); 
    stepperZ.runSpeed(); 
    stepperZ.moveTo(10000); 
    stepperZ.run();  
    hsvalz = digitalRead(homeSwitchZ);  
  } 

  stepperZ.setCurrentPosition(0); 
  delay(500); 
  
}

void stepperHomeC(){ 
  hsvalc = digitalRead(homeSwitchC);  
  while (hsvalc == HIGH)  
  {
    // drive stepper slowly until it makes the home switch 
    stepperC.setSpeed(-1500); 
    stepperC.runSpeed(); 
    stepperC.moveTo(10000); 
    stepperC.run();  
    hsvalc = digitalRead(homeSwitchC);  
  } 
  
  stepperC.setCurrentPosition(0); 
  delay(500); 
  
}
