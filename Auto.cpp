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


int average_range(int (&range)[181],int start, int end){
  int sum = 0;

  for(int i=start; i<end; i++){// find the average for the whole array
    sum = sum + range[i];
  }
  return (int)floor(sum/(end-start+1));
} 


int Auto::sliding_window(int (&ranges)[181]){
  /* This finds the best direction to go the farthest given a random set of obstacles and boundaries 
   *  by finding a "window" that the vehicle can go through that has the highest average distance.
   *  
   *  The Servo has 0 degrees for the right, and 180 for the left
   */
  int candidate_window[3]  = {0,1,0}; //candidate window
  int mean = 0;//old mean, used to keep track of candidate window progress
  bool shrink_window = false;//when false the window will continue expanding
  bool new_window    = true;//prevents early termination caused by local minima 
  int right_shift_max_gate = 0;// this is the integer that decides if the window should be shrunk
  int best_window[3] = {average_range(ranges,0,180),91,89}; //keeps track of the window [average, left side, right side]
  
  //End of variable setup

  while(180 >= candidate_window[1]){

     if(shrink_window ==false){//expand the window towards 180 if the mean is increasing
       candidate_window[1]++;
       new_window = true;
     }

     else{ // shrink the window by compressing from the 0 side so long as the mean is increasing
       candidate_window[2]++;
       new_window = false;
     }

    if((candidate_window[0]>=best_window[0]) && 
	    best_window[2]-best_window[1]<candidate_window[2]-candidate_window[1]){
		    
      best_window[0] = candidate_window[0];
      best_window[1] = candidate_window[1];
      best_window[2] = candidate_window[2];

      //reset the candidate window to continue scanning the range
      candidate_window[0] = 0;
      candidate_window[1] = candidate_window[1] + 1;
      candidate_window[2] = candidate_window[1];
      new_window          = true;
      shrink_window       = false;

    }
    

    else{ 

      right_shift_max_gate = average_range(ranges,candidate_window[2],candidate_window[1]);//get the new average 

      if(right_shift_max_gate < candidate_window[0]){//see if the window has gone too far
	if(false == shrink_window){
          candidate_window[1]--;
	  shrink_window = true;
	}

	if(true == shrink_window){//see if the window needs to be shrunk
	  candidate_window[2]++;
          shrink_window = false;
	}
      }
     
      else{
        candidate_window[0] = right_shift_max_gate;
      }
    }

    if(new_window == false && ((candidate_window[1]-candidate_window[2])<2)){//in case the window shrinks too much, reset it, such that it escapes the local minimum

      candidate_window[1] = candidate_window[1] + 2;
      candidate_window[2] = candidate_window[1] + 1;
      new_window = true;

    }

  }

//TODO remove after debug code block between
    std::cout<<"\nCandidate: "<<candidate_window[0]<<","<<candidate_window[1]<<","<<candidate_window[2];
    std::cout<<"\nBest: "<<best_window[0]<<","<<best_window[1]<<","<<best_window[2]<<"\n";
//TODO

  return ((int)floor((best_window[1]-best_window[2])/2))+best_window[2]-1;
  
}
