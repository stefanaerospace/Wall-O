#ifndef ULTRASONIC_H_
#define ULTRASONIC_H_


class Ultrasonic{//class that controls the ultrasonic sensor and attached servo

  private:
    bool scan_done = false;
    unsigned long last_update = 0;
  
  public:
    int ranges[181];
    
    //methods
    void real_ping(byte, int, int (&)[181], Servo, NewPing);
    void scan(int,int,int (&)[181], Servo, NewPing, int);
};

#endif
