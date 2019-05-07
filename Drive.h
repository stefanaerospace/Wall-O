#ifndef DRIVE_H_
#define DRIVE_H_

#ifdef ARDUINO
#include "Arduino.h"
#endif
#ifndef ARDUINO
#include "tests/Arduino.cpp"
#define HIGH 1
#define LOW 0
#endif

class Drive{
  public: 
    const int ENB = 5;
    const int IN1 = 7;
    const int IN2 = 8;
    const int IN3 = 9;
    const int IN4 = 11;
    const int ENA = 6;
    const int carSpeed = 200; //controls vehicle speed. Didn't see any documentation for this, so I will be experimenting with this later
    int drive_status   = 5; //this will allow the main loop to read the current status of vehicle motion
    int command_time   = 0;//amount of time a drive command should go on for
    int distance       = 0; //this stores the forward distance to be traveled
    float turn_rate;//this is the degrees/millisecond turn rate
    float forward_rate;// cm/millisecond
   

    void forward();
    void back();
    void left();
    void right();
    void stop();
    void drive_update(int);
};



#endif
