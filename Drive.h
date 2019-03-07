#ifndef DRIVE_H_
#define DRIVE_H_

#ifdef ARDUINO_VERSION
#include "Arduino.h"
#endif
#ifndef ARDUINO_VERSION
#include "tests/Arduino.cpp"
#endif

class Drive{
  public:
    const int HIGH = 1;
    const int LOW = 0; 
    const int ENB = 5;
    const int IN1 = 7;
    const int IN2 = 8;
    const int IN3 = 9;
    const int IN4 = 11;
    const int ENA = 6;
    const int carSpeed = 200; //controls vehicle speed. Didn't see any documentation for this, so I will be experimenting with this later
    int drive_status   = 5; //this will allow the main loop to read the current status of vehicle motion
    int command_time   = 0;//amount of time a drive command should go on for
    long unsigned last_update = 0;
    bool in_progress          = false; // since the methods will be only checked once quickly we need a way to determine if the method has completely finished
    float turn_rate    = 0.0;//this is the degrees/millisecond turn rate
    float forward_rate = 0.0;// cm/millisecond


    void forward();
    void back();
    void left();
    void right();
    void stop();
    void move_me(int);
    void drive_update(int);
};



#endif

