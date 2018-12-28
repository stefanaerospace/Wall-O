#include <NewPing.h> //ultrasonic range finder lib
#include <Servo.h>  //servo library

/*
 * Numbers to remember:
 * 50 cm/sec
 * .05 cm/millisecond subtract 3 cm for acceleration--needs 200 milliseconds to accel
 * .10588 degrees per millisecond or 3400 milsec for a 360
 */


class Drive{
  public:
  
    const byte ENB = 5;
    const byte IN1 = 7;
    const byte IN2 = 8;
    const byte IN3 = 9;
    const byte IN4 = 11;
    const byte ENA = 6;
    int carSpeed = 200;
    
  forward(){
    analogWrite(ENA, carSpeed);
    analogWrite(ENB, carSpeed);
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, HIGH);
  }
  
  back() {
    analogWrite(ENA, carSpeed);
    analogWrite(ENB, carSpeed);
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, HIGH);
    digitalWrite(IN3, HIGH);
    digitalWrite(IN4, LOW);
  }
  
  left() {
    analogWrite(ENA, carSpeed);
    analogWrite(ENB, carSpeed);
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, HIGH);
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, HIGH);
  }
  
  right() {
    analogWrite(ENA, carSpeed);
    analogWrite(ENB, carSpeed);
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);
    digitalWrite(IN3, HIGH);
    digitalWrite(IN4, LOW);
  }

  stop() {
    digitalWrite(ENA, LOW);
    digitalWrite(ENB, LOW);
  }
  
};

//classes used in Ultrasonic
Drive drive;
NewPing ranger(A5, A4, 400);

class Ultrasonic{

  public:
    int rightDistance = 0, leftDistance = 0, middleDistance = 0;
      
  
};


//Declaring remaining classes for main loop
Servo myservo;
Ultrasonic us;


void setup() {
    
  myservo.attach(3);  // attach servo on pin 3 to servo object
  myservo.write(45);
  delay(300);
  Serial.begin(9600);
  pinMode(drive.IN1, OUTPUT);
  pinMode(drive.IN2, OUTPUT);
  pinMode(drive.IN3, OUTPUT);
  pinMode(drive.IN4, OUTPUT);
  pinMode(drive.ENA, OUTPUT);
  pinMode(drive.ENB, OUTPUT);
  drive.stop();

  int starting_position [3] = {0, 0, 0}; // where the vehicle should start { which quadrant the vehicle is in, angle of sensor, distance recorded at angle}
  int current_position = 0;
  int start_time = 0;

  for(int i = 0; i!=4; i++){
    //turns the vehicle 90 degrees 4 times. so that way that direction can be scanned
    Serial.println(i);
    myservo.write(45);
    delay(300);
    for(int j = 45; j!=135; j++){
      //turns the range finder through a 90 degree sweep looking for the greatest distance. 
      myservo.write(j);
      current_position = ranger.ping_cm();
      delay(10);

      if(current_position > starting_position[2]){
        starting_position[0] = i;
        starting_position[1] = j;
        starting_position[2] = current_position;

        
      }

    } 

    start_time = millis();

    while((millis()-start_time)<850){
      drive.left();
    }
    drive.stop();
  
  }


  ////establish direction to go in
  //equation based on emperical data, following equation converts position and angle into turning milliseconds
  starting_position[0] = floor(((starting_position[0]+ .5 )*840)+ ((starting_position[1]-1)/.10588));

  start_time = millis();

  while((millis()-start_time)<starting_position[0]){
    drive.left();
  }
  drive.stop();


  //refine the starting position
  current_position = 0;
  starting_position[0] = 0;// TODO, look up what is the proper way of zeroing out the array, I forgot. 
  starting_position[1] = 0;
  starting_position[2] = 0;
  
  myservo.write(0);
  delay(300);
  
  for(int i = 0; i!=181; i++){
    myservo.write(i);
    current_position = ranger.ping_cm();
    delay(4);
        
    if( current_position > starting_position[2]){
      starting_position[2] = current_position;
      starting_position[1] = i;
    }
  }

  starting_position[0] = abs(starting_position[1]-90)/.10588;
  start_time = millis();
  
  while((millis()-start_time)<starting_position[0]){
    if(starting_position[1] > 90){
      drive.left();
    }
    
    if(starting_position[1] < 90){
      drive.right();
    }    
  }
  drive.stop();
  
}

//this value needs to be outside of the main

void loop() {
  
  int end_time = 0;
  myservo.write(0);
  while(myservo.read()!=0){;}
  delay(1000);
  int start_time = millis();
  myservo.write(180);

  while(ranger.ping_cm()!=12){myservo.read();}
  end_time = millis();
  
  while(1){
    Serial.println(end_time-start_time);
    Serial.println(ranger.ping_cm());
  }


}
