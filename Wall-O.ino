#include <NewPing.h> //ultrasonic range finder lib
#include <Servo.h>
#include <math.h> 

/*
 * Numbers to remember:
 * 50 cm/sec
 * .05 cm/millisecond subtract 3 cm for acceleration--needs 200 milliseconds to accel
 * .10588 degrees per millisecond or 3400 milsec for a 360
 */


class Drive{
  public:
  
    const byte ENB = 5;
    const byte IN1 = 7;
    const byte IN2 = 8;
    const byte IN3 = 9;
    const byte IN4 = 11;
    const byte ENA = 6;
    const byte carSpeed = 200; //controls vehicle speed. Didn't see any documentation for this, so I will be experimenting with this later
    byte drive_status = 5; //this will allow the main loop to read the current status of vehicle motion
    long unsigned last_update= 0;
    bool in_progress = false; // since the methods will be only checked once quickly we need a way to determine if the method has completely finished
    float turn_rate = 0.0;//this is the degrees/millisecond turn rate
    float forward_rate = 0.0;// cm/millisecond
    int command_time = 0;//amount of time a drive command should go on for


    void forward();
    void back();
    void left();
    void right();
    void stop();
    void move_me(byte);
    void drive_update(int);
};
  
void Drive::forward(){
  analogWrite(ENA, carSpeed);
  analogWrite(ENB, carSpeed);
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}

void Drive::back() {
  analogWrite(ENA, carSpeed);
  analogWrite(ENB, carSpeed);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}

void Drive::left() {
  analogWrite(ENA, carSpeed);
  analogWrite(ENB, carSpeed);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}

void Drive::right() {
  analogWrite(ENA, carSpeed);
  analogWrite(ENB, carSpeed);
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}

void Drive::stop() {
  digitalWrite(ENA, LOW);
  digitalWrite(ENB, LOW);
}

void Drive::move_me(byte control){
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
      control_param = angle (degrees) or centimeters you would like to go in the control direction
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
  







class Ultrasonic{//class that controls the ultrasonic sensor and attached servo

  private:
    bool scan_done = false;
    unsigned long last_update = 0;
  
  public:
    int ranges[181];
    
    //methods
    void real_ping(byte, int, int (&)[181], Servo, NewPing);
    void Ultrasonic::scan(int,int,int (&)[181], Servo, NewPing, int);
};

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








class Auto : public Drive{
  public:
    int control_command = 5; //this will start the vehicle in a stopped position
    unsigned long auto_last_update = millis();
    bool collision_imminent = false;

    int max_element_left(int *,int,int,int);
    int max_element_right(int *,int,int,int);
    int sliding_window(int *);
    void center(int *);
};

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

void Auto::center(int * ranges){
  
  //collision avoidance
  for(int i = 45; i<135; i++){
    if(ranges[i]<6){
      collision_imminent = true;
      this->move_me(4); //turn left until no longer about to collide.
      delay(300);
    }
  }
  
  //centering
  int direction = 0;
  
  if(collision_imminent == false){
    direction = this->sliding_window(ranges);
  }
}

int Auto::sliding_window(int * ranges){
  /* This finds the best direction to go the farthest given a random set of obstacles and boundaries 
   *  by finding a "window" that the vehicle can go through that has the highest average distance.
   */
  
  int sum = 0;

  for(int i=0; i<180; i++){// find the average for the whole array
    sum = sum + ranges[i];
  }
  
  int best_window[3] = {floor((sum/180)),180,0}; //keeps track of the window [average, left side, right side]

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
      candidate_window[0] = candidate_window[0];
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


  return floor((best_window[2]-best_window[1])/2);
  
}



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
  brain.center(us.ranges);
  
}
