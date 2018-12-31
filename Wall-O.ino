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
    bool finished = false; // since the methods will be only checked once quickly we need a way to determine if the method has completely finished
    float left_turn_rate = 0.0;//this is the degrees/millisecond turn rate
    float right_turn_rate = 0.0;//this is the degrees/millisecond turn rate
    float forward_rate = 0.0;// cm/millisecond


    void forward();
    void back();
    void left();
    void right();
    void stop();
    void move_me(byte);
    void drive_update(byte, int, bool);
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

void Drive::drive_update(byte control, int control_param, bool force = false){
  /*This function handles the driving, it is designed to be used in the loop(), use move_me for situations where the method call will not be revisited.
      control = direction that you wish to go, based off Drive::move_me
      control_param = angle (degrees) or centimeters you would like to go in the control direction
      force = override the current state and force a new control immediately. (used to avoid collisions)
  */
    
  if(control==1 || control == 2){
    //calculate milliseconds needed of turning for the angle to be met
  }
  
  if(control==4 || control == 3){
    //calculate milliseconds needed of turning for the angle to be met
  }

  finished = false;
  
  if(force == true){
    this->move_me(control);
  }
  
  if((millis()-last_update)<control_param){
    this->move_me(control);
    drive_status = control;
  }

  if((millis()-last_update)>control_param){
    last_update = millis();
    this->move_me(5);
    drive_status = 5;
    finished = true;
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

void Ultrasonic::scan(int start_angle, int end_angle, int (&range)[181], Servo servo, NewPing Uranger, int delay_time = 100){
  
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
    Serial.println("You are using the \"Ultrasonic::scan\" for a single degree, please use \"Ultrasonic::real_ping\" instead.");
  }
}








class Auto : public Drive{
  public:
    int control_command = 5; //this will start the vehicle in a stopped position
    unsigned long auto_last_update = millis();

    max_element(int *,int);
};

int Auto::max_element(int * arr, int size){
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






//Globals
Servo myservo;
Auto brain;
Ultrasonic us;
Drive drive;
NewPing ranger(A5, A4, 400);

bool servo_flip = false;




//initialization. Sets pins and figures out which way to start. 
//TODO: Include a calibration phase, currently dynamic constants are 
void setup() {
    
  myservo.attach(3);  // attach servo on pin 3 to servo object
  myservo.write(45);
  delay(300);
  Serial.begin(9600);
  pinMode(drive.IN1, OUTPUT);
  pinMode(drive.IN2, OUTPUT);
  pinMode(drive.IN3, OUTPUT);
  pinMode(drive.IN4, OUTPUT);
  pinMode(drive.ENA, OUTPUT);
  pinMode(drive.ENB, OUTPUT);
  drive.stop();

  //calibration code
  long edges[3] = {10000,0,10000};// distance at 0,90, 180
  int left_turn = 0;// keeps track of time needed to complete a left turn
  int right_turn = 0;// similar to left_turn
  int straight = 0;// similar to left_turn
  us.real_ping(90,300, us.ranges, myservo, ranger);
  edges[1] = us.ranges[90];
  
  //calibrate left turn
  drive.move_me(4);
  
  left_turn=millis();// begin timer
  
  while((edges[0]+20) > edges[1]){
    us.real_ping(0,300, us.ranges, myservo, ranger);
    edges[0] = us.ranges[0];
  }

  left_turn = millis()-left_turn;// end timer, save difference
  drive.left_turn_rate = 80/left_turn;

  //recenter vehicle
  drive.move_me(5);
  drive.move_me(3);
  int temp_var = 10000;
  myservo.write(90);
  delay(300);
  
  while((temp_var+1) > edges[1]){
      Serial.println("Working");
      us.real_ping(90,1, us.ranges, myservo, ranger);
      temp_var = us.ranges[90];
  }
  
  //calibrate right turn
  myservo.write(180);
  delay(300);
  
  right_turn=millis();// begin timer
  
  while((edges[2]+20) > edges[1]){
    us.real_ping(180,1, us.ranges, myservo, ranger);
    edges[2] = us.ranges[180];
  }

  right_turn = millis()-right_turn;// end timer,save difference
  drive.right_turn_rate= 80/right_turn;

  drive.move_me(5);

  myservo.write(90);
  while(true){
  Serial.print(edges[0]); Serial.print("\t");
  Serial.print(edges[1]); Serial.print("\t");
  Serial.println(edges[2]);
  }  
}










////Globals
//Servo myservo;
//Ultrasonic us;
//Drive drive;
//NewPing ranger(A5, A4, 400);
//bool servo_flip = false;







void loop() {
  
//  //This if-else causes the scanner to continuously scan
//  //  and populate the different angles
//  if(servo_flip == false){
//    us.scan(0,180,us.ranges,myservo,ranger);
//    servo_flip = true;
//  }
//  else{
//    us.scan(180,0,us.ranges,myservo,ranger);
//    servo_flip = false;
//  }

  ;//remove line
  
}
