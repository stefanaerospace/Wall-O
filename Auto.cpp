#ifndef ARDUINO
#include<cmath>
#define floor std::round
void delay(int i){}
#endif

#include "Auto.h"

void Auto::center(int (&ranges)[181], bool servo_flip, Ultrasonic us, Servo myservo, NewPing ranger){
  //collision avoidance 

  do{
    bool collision_imminent = false;

    for(int i = 45; i<135; i++){
      if(ranges[i]<6){
        collision_imminent = true;
        this->move_me(4); //turn left until no longer about to collide.
      }
    }

    if(collision_imminent = true){
      
      if(servo_flip == false){
        us.scan(0,180,us.ranges,myservo,ranger);
        servo_flip = true;
      }
      else{
        us.scan(180,0,us.ranges,myservo,ranger);
        servo_flip = false;
      }
      
    }

  } while (collision_imminent != false);
  
  
  //determine where to go next
  int direction = this->sliding_window(ranges);
  this->drive_update(direction);
}

int Auto::sliding_window(int (&ranges)[181]){
  /* This finds the best direction to go the farthest given a random set of obstacles and boundaries 
   *  by finding a "window" that the vehicle can go through that has the highest average distance.
   */
  int candidate_window[3]  = {0,1,0}; //candidate window
  int mean = 0;//old mean, used to keep track of candidate window progress
  bool shrink_window = false;//when false the window will continue expanding
  bool new_window    = true;//prevents early termination caused by local minima 
  int right_shift_max_gate = 0;// this is the integer that decides if the window should be shrunk
  int sum = 0;

  for(int i=0; i<180; i++){// find the average for the whole array
    sum = sum + ranges[i];
  }
  sum = (int)floor(sum/181);
 
  int best_window[3] = {(int)floor((sum/180)),180,0}; //keeps track of the window [average, left side, right side]

  //End of variable setup

  while(181 >= candidate_window[1]){

    if(candidate_window[0]>best_window[0]){//save the best window
      best_window[0] = candidate_window[0];
      best_window[1] = candidate_window[1];
      best_window[2] = candidate_window[2];

      //reset the candidate window to continue scanning the range
      candidate_window[0] = 0;
      candidate_window[1] = candidate_window[1] + 1;
      candidate_window[2] = candidate_window[1];
      new_window          = true;
      shrink_window       = false;
      
//      if(180 <= candidate_window[1]){break;}//end state trigger //TODO REMOVE
    }

    else{ 
 
//      if(181 > candidate_window[1]){break;}//end state
//        new_window = true;
//      }    //TODO REMOVE 

      sum = 0;//calculate the new window average

      for(int i=candidate_window[1]; i<=candidate_window[2]; i++){// find the mean distance for the window
        sum = sum + ranges[i];
      }

      right_shift_max_gate = sum/(candidate_window[1]-candidate_window[2]+1);//get the new average

      if(right_shift_max_gate < candidate_window[0]){//see if
	if(false == shrink_window){
          candidate_window[1]--;
	  shrink_window = true;
	}

	if(true == shrink_window){
	  candidate_window[2]--;
          shrink_window = false;
	}
      }
     
      else{
        candidate_window[0] = right_shift_max_gate;
      }

      if(shrink_window ==false){//expand the window towards 180 if the mean is increasing
        candidate_window[1]++;
	new_window = true;
      }

      else{ // shrink the window by compressing from the 0 side so long as the mean is increasing
        candidate_window[2]++;
        new_window = false;
      }
    }

    if(new_window == false && ((candidate_window[2]-candidate_window[1])<=1)){//in case the window shrinks too much, reset it, such that it escapes the local minimum

      candidate_window[1] = candidate_window[1] + 2;
      candidate_window[2] = candidate_window[1] + 1;

  }

  return (int)floor((best_window[1]-best_window[2])/2);
  
}
