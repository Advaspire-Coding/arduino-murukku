int redLight = 25;                //Pin for LED light
int greenLight = 24;   
int pushStart = 18;   
int buzzer = 20;
int select_switch = 17;
int testSwitch = 22;              //The switch that you want to test


void setup() {
  // put your setup code here, to run once:
  pinMode(greenLight, OUTPUT);
  pinMode(redLight, OUTPUT);
  pinMode(pushStart, INPUT_PULLUP);
  pinMode(select_switch, INPUT_PULLUP);
  pinMode(xHomeSwitch, INPUT_PULLUP);
  pinMode(testSwitch, INPUT_PULLUP);
  pinMode(buzzer, OUTPUT);

  digitalWrite(greenLight, HIGH);
  digitalWrite(redLight, LOW);
  digitalWrite(buzzer, LOW);
  delay(500);
}

void loop() {
  // put your main code here, to run repeatedly:

  if(digitalRead(testSwitch) == LOW){

    digitalWrite(greenLight, LOW);  //green light ON
    digitalWrite(redLight, HIGH);   //red light OFF
    
  } else {
    
    digitalWrite(greenLight, HIGH);
    digitalWrite(redLight, LOW);
   
  }


}
