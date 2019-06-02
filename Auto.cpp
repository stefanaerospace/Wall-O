#ifndef ARDUINO
#include<cmath>
#define floor std::round
#endif

#include "Auto.h"

int average_range(int (&range)[181],int start, int end){
  int sum = 0;
  int counter = 0;

  if(end > 180){end = 180;}//prevents error

  for(int i=start; i<=end; i++){// find the average for the sub array
    counter++;
    sum = sum + range[i];
  }

  sum = sum/counter;

  return (int)floor(sum);
} 



void Auto::collision_avoidance(bool &servo_flip, Ultrasonic us, Servo myservo, NewPing ranger){
  //collision avoidance 
   
  this->command_start = millis();//start timer
 
  while ((millis()-(this->command_start)) < this->command_time){

    if(servo_flip == false){
      us.scan(0,180,us.ranges,myservo,ranger,1);
      servo_flip = true;
    }
    else{
      us.scan(180,0,us.ranges,myservo,ranger,1);
      servo_flip = false;
    }

    for(int i = 0; i<180; i++){
      //40 centimeters is about the distance it can travel during a scan
      if(us.ranges[i]<40){

        if(us.ranges[i]<6){
            this->back(); //back up
            delay(150);
            this->stop();
            break;
        }
        else{
            if(average_range(us.ranges,0,89)<average_range(us.ranges,90,180)){this->left();}
            else{this->right(); }
            delay(250);
            this->stop();
            break;
        } 
      }
    }
  } 
}


void force_new_window(int (&range)[181],int (&window)[3]){//this resets the window to be farther to the right

        window[1] = window[2];
        window[2] = window[2]+2;
        if(window[2]>180){window[0]=0;}

        else{window[0] = average_range(range,window[1],window[2]);}
}


int Auto::sliding_window(int (&ranges)[181]){
  /* This finds the best direction to go the farthest given a random set of obstacles and boundaries 
   *  by finding a "window" that the vehicle can go through that has the highest average distance.
   *  
   *  The Servo has 0 degrees for the right, and 180 for the left
   */

    int best[3]      = {average_range(ranges,0,180),89,91}; //REMEMBER THAT THE SERVO IS 180 ON THE LEFT AND 0 ON THE RIGHT
    int candidate[3] = {average_range(ranges,0,3),0,3};
    int proposed_shift = 0;
    int proposed_mean  = 0;
    bool left_increment = false;

    while(true){//break conditions inside the loop

      if(candidate[0] >= best[0] && 
         candidate[2]-candidate[1] >= best[2]-best[1]){

           //make the candidate the new best

           best[0] = candidate[0];
           best[1] = candidate[1];
           best[2] = candidate[2];
           
           //force the candidate window to a new position to continue exploring the range 
           force_new_window(ranges,candidate);
           left_increment = false;

           //exit criteria
           if(candidate[2]>=180){break;} 

      }

      //propose incrementing the window

      //if the right side is being incremented...expanding the window
      if(false == left_increment){
        proposed_shift = candidate[2] + 1; 
        proposed_mean  = average_range(ranges,candidate[1],proposed_shift);

        if(proposed_mean >= candidate[0]){
                candidate[0] = proposed_mean;
                candidate[2] = proposed_shift;
        }

        //if shifting to the right no longer yields positive results, start shrinking the window
        else{left_increment = true;}

        if(candidate[2]>180){break;}

      }


      //if the left side is being incremented...shrinking the window
      if(true == left_increment){

        proposed_shift = candidate[1] + 1; 
        proposed_mean  = average_range(ranges,proposed_shift,candidate[2]);

        if(proposed_mean >= candidate[0]){
          candidate[0] = proposed_mean;
          candidate[1] = proposed_shift;
        }

        else{
          left_increment = false;

          //reject the local minima
          force_new_window(ranges, candidate);
          if(candidate[2]>=180){break;} 
        }

      }

      //prevent the window from getting too small
      if(candidate[2]-candidate[1]<=2){
        force_new_window(ranges, candidate);
        if(candidate[2]>=180){break;} 
      }
  }

  this->distance = ranges[(int)floor(((best[2]-best[1])/2)+best[1])];

  return((int)floor(((best[2]-best[1])/2)+best[1]));
}
