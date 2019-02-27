#ifndef DRIVE_H_
#define DRIVE_H_

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
 
#endif

