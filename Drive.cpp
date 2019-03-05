#include "Drive.h"

 
//set motors to move the vehicle forward
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
  this->drive_status = 4;
}

//set motors to pivot the vehicle to the left like a tank
void Drive::left() {
  analogWrite(ENA, carSpeed);
  analogWrite(ENB, carSpeed);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
  this->drive_status = 3;
}

//set motors to stop moving
void Drive::stop() {
  digitalWrite(ENA, LOW);
  digitalWrite(ENB, LOW);
  this->drive_status = 5;
}

//simpler way of managing direction
void Drive::move_me(int control){
  switch(control){
    case 1: this->forward(); break;
    case 2: this->back(); break;
    case 3: this->right(); break;
    case 4: this->left(); break;
    case 5: this->stop(); break;
  }
}

void Drive::drive_update(int control_param){
  /*This function handles the driving, it is designed to be used in the loop(), use move_me for situations where the method call will not be revisited.
      control_param = angle (degrees)you would like to go in the control direction
  */
  int control = 0;

  if(control_param < 100 || control_param > 80){
    control = 1;
  }

  else if(control_param <= 80){
    control = 3;
    command_time = turn_rate*control_param;
  }

  else if(control_param >= 100){
    control = 4;
    command_time = turn_rate*control_param;
  }
  
  if((millis()-last_update)<command_time){//if statment for when action is in process
    this->move_me(control);
    drive_status = control;
    in_progress = true;
  }

  if((millis()-last_update)>command_time){//kills the process
    last_update = millis();
    this->move_me(5);
    drive_status = 5;
    in_progress = false;
  }
}
 
