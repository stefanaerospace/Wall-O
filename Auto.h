#ifndef AUTO_H_
#define AUTO_H_
#define uint8_t byte
#include "Drive.h"
#include "Ultrasonic.h"
#include <Servo.h>
#include <NewPing.h> //ultrasonic range finder lib


class Auto : public Drive{
  public:
    int control_command = 5; //this will start the vehicle in a stopped position
    unsigned long auto_last_update = millis();
    bool collision_imminent = false;

    int max_element_left(int *,int,int,int);
    int max_element_right(int *,int,int,int);
    int sliding_window(int *);
    void center(int *, bool, Ultrasonic, Servo, NewPing);
};
#endif
