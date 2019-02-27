#ifndef ULTRASONIC_H_
#define ULTRASONIC_H_
#include <Servo.h>
#include <NewPing.h> //ultrasonic range finder lib


class Ultrasonic{//class that controls the ultrasonic sensor and attached servo

  private:
    bool scan_done = false;
    unsigned long last_update = 0;
  
  public:
    int ranges[181];
    
    //methods
    void real_ping(int, int, int (&)[181], Servo, NewPing);
    void scan(int,int,int (&)[181], Servo, NewPing, int delay_time = 3);
};

#endif
