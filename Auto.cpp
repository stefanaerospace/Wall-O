#include "Auto.h"

int Auto::max_element_left(int * arr, int size, int start_, int end_){
 int maxIndex = 0;
 int max = arr[maxIndex];
 for (int i=1; i<size; i++){
   if (max<arr[i]){
     max = arr[i];
     maxIndex = i;
   }
 }
 return maxIndex;
}

int Auto::max_element_right(int * arr, int size, int start_, int end_){
 int maxIndex = 0;
 int max = arr[maxIndex];
 for (int i=180; i<size; i--){
   if (max<arr[i]){
     max = arr[i];
     maxIndex = i;
   }
 }
 return maxIndex;
}

void Auto::center(int * ranges, bool servo_flip, Ultrasonic us, Servo myservo, NewPing ranger){
  //collision avoidance
  do{
    for(int i = 45; i<135; i++){
      if(ranges[i]<6){
        collision_imminent = true;
        this->move_me(4); //turn left until no longer about to collide.
        delay(300);
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
      
      for(int i = 45; i<135; i++){
        collision_imminent = false;
        if(ranges[i]<6){
          collision_imminent = true;
        }
      }
      this->move_me(5);
    }
    
  } while (collision_imminent != false);
  
  
  //determine where to go next
  int direction = 0;
  direction = this->sliding_window(ranges);
  if(direction = 0){
    this->move_me(2);
  }
  this->drive_update(direction);
}

int Auto::sliding_window(int * ranges){
  /* This finds the best direction to go the farthest given a random set of obstacles and boundaries 
   *  by finding a "window" that the vehicle can go through that has the highest average distance.
   */
  
  int sum = 0;

  for(int i=0; i<180; i++){// find the average for the whole array
    sum = sum + ranges[i];
  }
  
  
  int best_window[3] = {(int)floor((sum/180)),180,0}; //keeps track of the window [average, left side, right side]

  int candidate_window[3] = {0,0,0}; //candidate window
  int mean = 0;//old mean, used to keep track of candidate window progress

  bool shrink_window = false;//when false the window will 
  bool tried = false;
  bool new_window = true;

  while(candidate_window[1] != 181){

    if(candidate_window[0]>best_window[0]){//save the best window
      best_window[0] = candidate_window[0];
      best_window[1] = candidate_window[1];
      best_window[2] = candidate_window[2];

      //reset the candidate window to continue scanning the range
      candidate_window[0] = 0;
      candidate_window[1] = candidate_window[2];
      candidate_window[2] = candidate_window[2] + 1;
      new_window = true;
      shrink_window = false;
      
      if(candidate_window[2]>=180){break;}//edge case protection
    }

    else{ 
      
      sum = 0;
      for(int i=candidate_window[1]; i<=candidate_window[2]; i++){// find the mean distance for the window
        sum = sum + ranges[i];
      }

      candidate_window[0] = sum;

      
      if(shrink_window ==false){//expand the window to the right if the mean is increasing

        candidate_window[2]++;
      }

      else{ // shrink the window by compressing from the left so long as the mean is increasing

        candidate_window[1]++;
        
        new_window = false;
      }
    }

    if(new_window = false && ((candidate_window[2]-candidate_window[1])<=1)){//incase the window shrinks too much, reset it

      candidate_window[1] = candidate_window[1] + 1;
      candidate_window[2] = candidate_window[1] + 2;

      if(candidate_window[2]>=180){break;}//edge case protection

      new_window = true;
      
    }
  }


  if((int)floor((best_window[2]-best_window[1])/2)==0){
    while(true){
      this->move_me(2);
    }
  }
  return (int)floor((best_window[2]-best_window[1])/2);
  
}


