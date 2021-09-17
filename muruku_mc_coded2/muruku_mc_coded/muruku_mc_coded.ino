#include <AccelStepper.h>
#include <MultiStepper.h>
  
AccelStepper stepperZ(1, 3, 2);  // 1=Stepper driver option, 3=Step pulse pin, 2=Direction
AccelStepper stepperX(1, 6, 7);  // 1=Stepper driver option, 6=Step pulse pin, 7=Direction

MultiStepper steppers;

const int homeSwitchZ = 5;  
const int homeSwitchX = 8;  
const int ledPinZ = 13;  
const int ledPinX = 12;
const int pushStart = 11;
byte hsvalz;  
byte hsvalx; 
int muruku_diameter;
int muruku_interval;
int run_speed;
float expand_cof;
float radius;
String command;

void setup(){

  Serial.begin(9600);
  stepperZ.setMaxSpeed(500); // Set max speed. Steps per second
  stepperZ.setAcceleration(100);  // set motor acceleration
  stepperX.setMaxSpeed(500); // Set max speed. Steps per second
  stepperX.setAcceleration(100);  // set motor acceleration

  steppers.addStepper(stepperX);
  steppers.addStepper(stepperZ);
    
  Serial.print("Home Pos Setup");
  Serial.print("\n");
  delay(100);  
  pinMode(homeSwitchZ, INPUT);  
  pinMode(homeSwitchX, INPUT);  
  pinMode(ledPinZ, OUTPUT);             
  pinMode(ledPinX, OUTPUT);      
  digitalWrite(ledPinZ, LOW); 
  digitalWrite(ledPinX, LOW); 

  stepperHomeX(); //intialize run routine to home motor

  stepperHomeZ(); //intialize run routine to home motor

  long positions[2]; // Array of desired stepper position  
  positions[0] = -2500;
  positions[1] = 2500;
  steppers.moveTo(positions);
  steppers.runSpeedToPosition(); // Blocks until all are in position
  delay(1000);
    /*
  positions[0] = -2500-210;
  positions[1] = 2500+115;
  steppers.moveTo(positions);
  steppers.runSpeedToPosition(); // Blocks until all are in position

  positions[0] = -2500-210-140;
  positions[1] = 2500+115+145;
  steppers.moveTo(positions);
  steppers.runSpeedToPosition(); // Blocks until all are in position

  positions[0] = -2500-210-140-20;
  positions[1] = 2500+115+145+75;
  steppers.moveTo(positions);
  steppers.runSpeedToPosition(); // Blocks until all are in position  

  positions[0] = -2500-210-140-20+20;
  positions[1] = 2500+115+145+75+100;
  steppers.moveTo(positions);
  steppers.runSpeedToPosition(); // Blocks until all are in position   

  positions[0] = -2500-210-140-20+20+70;
  positions[1] = 2500+115+145+75+100+235;
  steppers.moveTo(positions);
  steppers.runSpeedToPosition(); // Blocks until all are in position   
  
  positions[0] = -2500-210-140-20+20+70+105;
  positions[1] = 2500+115+145+75+100+235+175;
  steppers.moveTo(positions);
  steppers.runSpeedToPosition(); // Blocks until all are in position    

  positions[0] = -2500-210-140-20+20+70+105+125;
  positions[1] = 2500+115+145+75+100+235+175+140;
  steppers.moveTo(positions);
  steppers.runSpeedToPosition(); // Blocks until all are in position   

  positions[0] = -2500-210-140-20+20+70+105+125+125;
  positions[1] = 2500+115+145+75+100+235+175+140+70;
  steppers.moveTo(positions);
  steppers.runSpeedToPosition(); // Blocks until all are in position   

  positions[0] = -2500-210-140-20+20+70+105+125+125+190;
  positions[1] = 2500+115+145+75+100+235+175+140+70+35;
  steppers.moveTo(positions);
  steppers.runSpeedToPosition(); // Blocks until all are in position  

   */
   
  Serial.print("Ready, Press 1");
  Serial.print("\n");
  delay(100);  
}


void loop(){

  if (Serial.available()){
      command = Serial.readStringUntil('\n');
   
      if(command.equals("1")){
        Serial.print("Program Start");
        Serial.print("\n");
        delay(100);  
        int muruku_dia = 600;                       //set muruku diameter to 600
        int muruku_interval = muruku_dia / 2 + 20;  //set tray's position for motor to locate at top of the tray
        int run_speed = 30;                         //set motor running speed to 10
        float x_centre = (-2500) / 2;               //set centre of the tray for position[0] control
        float y_centre = 2500;                      //set offset for y
        float expand_cof = 5;                       //set the radius expansion coefficient to 0.3

        for (int i = 0; i < 4; i++){

        //Make 4 murukus from bottom to top position of the tray
      
        float radius = 5;                                   //set initial radius, starting to spit out muruku from the center position
        int x_pos = round((radius * (-1)) + x_centre);      //set x position of the motor to the left of the circle, our circle loop starting point is at left with offsetting 2500
        y_centre = y_centre - muruku_interval;
        int y_pos = round(y_centre);             //set y position of the motor to draw at the height of the circle with muruku_interval
                
        Serial.print("x_start: ");
        Serial.print(x_pos);
        Serial.print(", ");
        Serial.print("y_start: ");
        Serial.print(y_pos);
        Serial.print(", ");
        Serial.print("Radius_start: ");
        Serial.println(round(radius));

        long positions[2];                            //Array of desired stepper position        
        positions[0] = x_pos;                         //locate x_position for motor X-axis (centre of X-axis)
        positions[1] = y_pos;                         //locate y_position for motor Y-axis (bottom of Y-axis)
        steppers.moveTo(positions);
        steppers.runSpeedToPosition();                //Blocks until all are in position
        Serial.print("first position done!");
        Serial.print("\n");
        Serial.println("Moving to second position");
        
          while (round(radius) <= muruku_dia / 2){  
          
           //repeat to draw the circle until outer diameter reaches muruku diameter

             while ( (x_pos - x_centre) < radius ){
  
                //keep drawing arc until x_pos reach to the right of the circle
  
                x_pos = x_pos + run_speed;                                                        //keep increase x_pos with run_speed

                if(round(radius) <= round(x_pos - x_centre)){
                  y_pos = round(y_centre); //calculate corresponding y_pos based on x_pos movement with circle formula
                } else {
                  y_pos = round(sqrt(sq(radius) - sq(x_pos - x_centre)) + y_centre); //calculate corresponding y_pos based on x_pos movement with circle formula
                }
                
                
                positions[0] = x_pos;                                                             //locate motor X-axis based on calculated x_pos
                positions[1] = y_pos;                         //locate motor y-axis based on calculated y_pos, off-setting muruku interval
                
                Serial.print("x+: ");
                Serial.print(x_pos);
                Serial.print(", ");
                Serial.print("y+: ");
                Serial.print(y_pos);
                Serial.print(", ");
                Serial.print("Radius+: ");
                Serial.println(round(radius));
                
                steppers.moveTo(positions);
                steppers.runSpeedToPosition();                                                    // Blocks until all are in position
                             
                if(radius >= muruku_dia / 2){
                    break;                                                                     //expand the circle for every point of time to make whirl shape                  
                } else {
                    radius = radius + expand_cof;
                }
                
             }

             while (round(x_pos - x_centre) > round(radius * -1)){
  
                //keep drawing arc until x_pos reach to the left of the circle
  
                x_pos = x_pos - run_speed;                          //keep increase x_pos with run_speed
                
                if(round(x_pos - x_centre) <= round(radius * -1)){
                  y_pos = round(y_centre);  //calculate corresponding y_pos based on x_pos movement with circle formula
                } else {
                  y_pos = round(sqrt(sq(radius) - sq(x_pos - x_centre)) * (-1) + y_centre);
                }
                
                positions[0] = x_pos;                               //locate motor X-axis based on calculated x_pos
                positions[1] = y_pos;                               //locate motor y-axis based on calculated y_pos, off-setting muruku interval
                
                Serial.print("x-: ");
                Serial.print(x_pos);
                Serial.print(", ");
                Serial.print("y-: ");
                Serial.print(y_pos);
                Serial.print(", ");
                Serial.print("Radius-: ");
                Serial.println(round(radius));
                
                steppers.moveTo(positions);
                steppers.runSpeedToPosition();                      // Blocks until all are in position
                
                if(radius >= muruku_dia / 2){
                    break;                                                                     //expand the circle for every point of time to make whirl shape                  
                } else {
                    radius = radius + expand_cof;
                }
                
             }
        
            }

           muruku_interval = muruku_interval + muruku_dia + 20;
           y_centre = y_centre - muruku_interval;
        }
      }
  }
}

void stepperHomeZ(){ 
  hsvalz = digitalRead(homeSwitchZ);  
  while (hsvalz == HIGH)  
  {
    // drive stepper slowly until it makes the home switch 
    stepperZ.setSpeed(-500); 
    stepperZ.runSpeed(); 
    stepperZ.moveTo(10000); 
    stepperZ.run();  
    hsvalz = digitalRead(homeSwitchZ);  
  }
  digitalWrite(ledPinZ, HIGH); 
  stepperZ.setCurrentPosition(0); 
  delay(1000);  
}

void stepperHomeX(){ 
  hsvalx = digitalRead(homeSwitchX);  
  while (hsvalx == HIGH) 
  {
   
    stepperX.setSpeed(500); 
    stepperX.runSpeed(); 
    stepperX.moveTo(10000); 
    stepperX.run(); 
    hsvalx = digitalRead(homeSwitchX);  
  } 
  digitalWrite(ledPinX, HIGH); 
  stepperX.setCurrentPosition(0); 
  delay(1000);  
}
