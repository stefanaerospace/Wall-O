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
    int carSpeed = 200;
    long unsigned last_update= 0;

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

void Drive::drive_update(byte control, int control_time = 1000, bool force = false){
  if(force == true){
    this->move_me(control);
  }

  if((millis()-last_update)>control_time){
    last_update = millis();
    this->stop();
  }
    
  if((millis()-last_update)<control_time){
    this->move_me(control);
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
  Serial.println(angle);
  bool scan_done = false;
  
  while(scan_done == false){
    if((millis()-last_update)>=delay_time){
      range[angle] = Uranger.ping_cm();
      scan_done = true;
    }
  }
  Serial.println(scan_done);
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

  //Go in a circle scanning 90 degree quadrants, keep track of the quadrant with the most room TODO: use gating function to 
  int starting_position [3] = {0, 0, 0}; // where the vehicle should start { which quadrant the vehicle is in, angle of sensor, distance recorded at angle}
  int current_position = 0;
  int start_time = 0;

  for(int i = 0; i!=4; i++){
    //turns the vehicle 90 degrees 4 times. so that way that direction can be scanned
    myservo.write(45);
    delay(300);
    
    us.scan(45,135,us.ranges,myservo,ranger);
      

    if(us.ranges[brain.max_element(us.ranges,181)] > starting_position[2]){
        starting_position[0] = i;
        starting_position[1] = brain.max_element(us.ranges,181);
        starting_position[2] = us.ranges[brain.max_element(us.ranges,181)];
    } 

    drive.drive_update(4,840);
  
  }


  ////establish direction to go in
  //equation based on emperical data, following equation converts position and angle into turning milliseconds
  starting_position[0] = floor(((starting_position[0]+ .5 )*840)+ ((starting_position[1]-1)/.10588));

  drive.drive_update(4, starting_position[0]);


  //refine the starting position
  current_position = 0;
  starting_position[0] = 0;// TODO, look up what is the proper way of zeroing out the array, I forgot. 
  starting_position[1] = 0;
  starting_position[2] = 0;
  
  myservo.write(0);
  delay(300);
  us.scan(0,180,us.ranges,myservo,ranger);
        
  if( current_position > starting_position[2]){
    starting_position[1] = brain.max_element(us.ranges,181);
    starting_position[2] = us.ranges[brain.max_element(us.ranges,181)];
  }
  

  starting_position[0] = abs(starting_position[1]-90)/.10588;
  drive.drive_update(4,starting_position[0]);
  
}










////Globals
//Servo myservo;
//Ultrasonic us;
//Drive drive;
//NewPing ranger(A5, A4, 400);
//bool servo_flip = false;







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

  
  
}
