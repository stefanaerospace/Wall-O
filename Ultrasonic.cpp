#include "Ultrasonic.h"
#include<stdio.h>
//scan a single degree
void Ultrasonic::real_ping(int angle, int delay_time, int (&range)[181], Servo servo, NewPing Uranger){

  servo.write(angle);
  delay(delay_time); 
  range[angle] = (int)Uranger.ping_cm();
  if(range[angle] <= 0){range[angle] = 401;}

}

//scan through a range
void Ultrasonic::scan(int start_angle, int end_angle, int (&range)[181], Servo servo, NewPing Uranger, int delay_time){
  
  last_update = millis();  

  if(start_angle < end_angle){
    for(int angle = start_angle; angle<=end_angle; angle++){
      this->real_ping(angle, delay_time, range, servo, Uranger);
    }
  }

  else if(start_angle > end_angle){
  for(int angle = start_angle; angle>=end_angle; angle--){
      this->real_ping(angle, delay_time, range, servo, Uranger);
    }
  }

  else if(start_angle = end_angle){
      this->real_ping(start_angle, delay_time, range, servo, Uranger);
  }
}
