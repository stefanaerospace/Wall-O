#include <NewPing.h> //ultrasonic range finder lib
#include <Servo.h>
#include <math.h> 

#include "Dive.h"
#include "Ultrasonic.h"
#include "Auto.h"

/*
 * Numbers to remember:
 * 50 cm/sec
 * .05 cm/millisecond subtract 3 cm for acceleration--needs 200 milliseconds to accel
 * .10588 degrees per millisecond or 3400 milsec for a 360
 */


//Globals
Servo myservo;
Auto brain;
Ultrasonic us;
NewPing ranger(A5, A4, 400);
bool servo_flip = false;


//initialization. Sets pins and figures out which way to start. 
void setup() {
    
  myservo.attach(3);  // attach servo on pin 3 to servo object
  myservo.write(45);
  delay(300);
  Serial.begin(9600);
  pinMode(brain.IN1, OUTPUT);
  pinMode(brain.IN2, OUTPUT);
  pinMode(brain.IN3, OUTPUT);
  pinMode(brain.IN4, OUTPUT);
  pinMode(brain.ENA, OUTPUT);
  pinMode(brain.ENB, OUTPUT);
  brain.move_me(5);



  //calibration code
  int start_time = millis();

  us.real_ping(90,300,us.ranges,myservo,ranger);//get sensor in position and get a quick ping
  int start_range = us.ranges[90];

  while(us.ranges[90]!=(start_range-10)){//ping until 10 cm of distance is covered.
    brain.move_me(1);
    us.real_ping(90,1,us.ranges,myservo,ranger);
  }
  int velocity = millis();
  brain.move_me(5);//stop the vehicle

  //establish kinematic properties
  brain.forward_rate = 10/velocity;
  brain.turn_rate = atan(6/(10/velocity))*180;

}


void loop() {
  
  //This if-else causes the scanner to continuously scan
  //  and populate the different angles
  
  if(servo_flip == false){
    us.scan(0,180,us.ranges,myservo,ranger);
    servo_flip = true;
  }
  else{
    us.scan(180,0,us.ranges,myservo,ranger);
    servo_flip = false;
  }

  //guidance for the vehicle
  brain.center(us.ranges, servo_flip, us, myservo, ranger);
  
}
