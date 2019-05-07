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
    int control_command = 0; //this will save the desired angle
    int command_start = 0; //this will save when a command is started
    unsigned long auto_last_update = millis();

    int sliding_window(int (&)[181]);
    void collision_avoidance(bool &, Ultrasonic, Servo, NewPing);
};
#endif
