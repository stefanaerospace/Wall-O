//set motors to move the vehicle forward
#ifndef ARDUINO
#include <math.h>
#endif
#include "Drive.h"


void Drive::forward(){
  analogWrite(ENA, carSpeed);
  analogWrite(ENB, carSpeed);
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
  this->drive_status = 1;
}

void Drive::back() {
  analogWrite(ENA, carSpeed);
  analogWrite(ENB, carSpeed);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  this->drive_status = 2;
}

void Drive::right() {
  analogWrite(ENA, carSpeed);
  analogWrite(ENB, carSpeed);
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  this->drive_status = 3;
}

//set motors to pivot the vehicle to the left like a tank
void Drive::left() {
  analogWrite(ENA, carSpeed);
  analogWrite(ENB, carSpeed);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
  this->drive_status = 4;
}

//set motors to stop moving
void Drive::stop() {
  digitalWrite(ENA, LOW);
  digitalWrite(ENB, LOW);
  this->drive_status = 5;
}

void Drive::drive_update(int control_param){
  /*This function handles the driving, it is designed to be used in the loop(), use move_me for situations where the method call will not be revisited.
      control_param = angle (degrees)you would like to go in the control direction
  */
  
  using namespace std;

  if(control_param < 120 && control_param > 60){
    this->forward();
    this->command_time = (int)floor(this->distance/this->forward_rate);
  }

  else if(control_param <= 60){
   this->left();
   this->command_time = (int)floor((90-this->command_time)/this->turn_rate); //the vehicle is always pointed at 90 degrees
  }

  else if(control_param >= 120){
    this->right();  
    this->command_time = (int)floor((this->command_time-90)/this->turn_rate);
  }

}
 
