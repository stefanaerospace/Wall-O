#include <NewPing.h> //ultrasonic range finder lib
#include <Servo.h>
#include <math.h> 
#include <Arduino.h>

#include "Drive.h"
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
  myservo.write(90);
  delay(300);
  Serial.begin(9600);
  pinMode(brain.IN1, OUTPUT);
  pinMode(brain.IN2, OUTPUT);
  pinMode(brain.IN3, OUTPUT);
  pinMode(brain.IN4, OUTPUT);
  pinMode(brain.ENA, OUTPUT);
  pinMode(brain.ENB, OUTPUT);
  brain.stop();


  //calibration code
  int start_time = millis();

  us.real_ping(90,300,us.ranges,myservo,ranger);//get sensor in position and get a quick ping
  int start_range = us.ranges[90];

  while(us.ranges[90]>(start_range-10)){//ping until 10 cm of distance is covered.
    brain.forward();
    us.real_ping(90,1,us.ranges,myservo,ranger);
    
  }
  int travel_time = millis()-start_time;
  brain.stop();

  //establish kinematic properties
  brain.forward_rate = 10.0000000/travel_time; //the division operator only produces the number of significant digits that are present in the arguements
  
  brain.turn_rate = ((brain.forward_rate/6.3500000))*(PI/180); //radius from center of vehicle to center of tire is 6.35 cm
}


void loop() {
  
  //This if-else causes the scanner to continuously scan
  //  and populate the different angles efficiently

  if(servo_flip == false){
    us.scan(0,180,us.ranges,myservo,ranger);
    servo_flip = true;
  }
  else{
    us.scan(180,0,us.ranges,myservo,ranger);
    servo_flip = false;
  }

  //guidance for the vehicle
  Serial.print("Should be next");
  Serial.print('\n');
  
  brain.control_command = brain.sliding_window(us.ranges);
  
  Serial.print(brain.control_command,7);
  Serial.print('\n');

  //enact direction
  //brain.drive_update(brain.control_command);

//TODO REMOVE BELOW LINE
brain.command_time = 0;
  
  //while moving, scan for obstacles
  brain.collision_avoidance(servo_flip, us, myservo, ranger);

  brain.stop();
  

}
