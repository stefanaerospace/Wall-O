#ifndef ARDUINO_CPP
#define ARDUINO_CPP

//Arduino.cpp

//This is a library of fake functions to substitute for Arduino.h functions
#include<chrono>

//chrono snippet pulled from:
//https://stackoverflow.com/questions/11122569/c-fastest-way-to-get-milliseconds-since-midnight

//int duration_since_midnight() {
//    auto now = std::chrono::system_clock::now();
//
//    time_t tnow = std::chrono::system_clock::to_time_t(now);
//    tm *date = std::localtime(&tnow);
//    date->tm_hour = 0;
//    date->tm_min = 0;
//    date->tm_sec = 0;
//    auto midnight = std::chrono::system_clock::from_time_t(std::mktime(date));
//    auto now1 = (std::chrono::milliseconds) now;
//    auto midnight1 = (std::chrono::milliseconds) midnight;
//    return (int)((now1.count() -midnight1.count()));
//}

int millis(){
	using namespace std::chrono;
	auto now = system_clock::now();
	auto now_ms = time_point_cast<milliseconds>(now);
        auto epoch = now_ms.time_since_epoch();	
	int now_long = (int)epoch.count();
	return now_long;
}

void analogWrite(int a, int b){;}//just a dummy function, should eventually write as a function
//that affects a motor class status

void digitalWrite(int a, int b){;}//similar to analogWrite

#endif
