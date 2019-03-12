#ifndef AUTO_H_
#define AUTO_H_
#define uint8_t byte

#ifdef ARDUINO
#include "Arduino.h"
#include <Servo.h>
#include <NewPing.h> //ultrasonic range finder lib
#endif

#ifndef ARDUINO
#include "tests/Arduino.cpp"
#include "tests/Servo.cpp"
#include "tests/NewPing.cpp" //ultrasonic range finder lib
#endif


#include "Drive.h"
#include "Ultrasonic.h"

class Auto : public Drive{
  public:
    int control_command = 5; //this will start the vehicle in a stopped position
    unsigned long auto_last_update = millis();
    bool collision_imminent = false;

    int sliding_window(int (&)[181]);
    void center(int (&)[181], bool, Ultrasonic, Servo, NewPing);
};
#endif
