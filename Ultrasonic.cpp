#include "Ultrasonic.h"

//scan a single degree
void Ultrasonic::real_ping(byte angle, int delay_time, int (&range)[181], Servo servo, NewPing Uranger){

  servo.write(angle);
  bool scan_done = false;
  last_update = millis();
  
  while(scan_done == false){
    if((millis()-last_update)>=delay_time){
      range[angle] = Uranger.ping_cm();
      if(range[angle] == 0){range[angle] = 401;}
      scan_done = true;
    }
  }
}

//scan through a range
void Ultrasonic::scan(int start_angle, int end_angle, int (&range)[181], Servo servo, NewPing Uranger, int delay_time = 3){
  
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

  if(start_angle == end_angle){
    Serial.println("You are using the \"Ultrasonic::scan\" method for a single degree, please use \"Ultrasonic::real_ping\" instead.");
  }
}


