# Wall-O
## Overview
Purchased an Elegoo Smart Car V3, the obstacle avoidance code written for it was very...simple, 
  so I am in the process of improving it. 
  
It scans the environment looking for the farthest direction to go in. The objective was to follow a scheme that 
  would get the agent to explore as much as possible without the ability to store current or previous locations (hardware
  is very limited).

## High Level Design
In arduino code there is a setup() that is called before the main()

setup():
Responsible for capturing the kinematics, and deriving further kinematics. This function also initializes the hardware pin
structure.

main():
Handles the stage of operation for the vehicle.
The vehicle has three stages:
1. Scan the env for best possible direction
2. Set vehicle to move roughly in that direction
3. Scan while moving, stop vehicle if about to collide.
4. Stop vehicle, go back to step 1

## Classes
### Drive:

This class is held over from the original code base, it initialized the pins for the vehicle. The class was augmented with a drive_status, to easily
pull current direction. 

I added a function to it to allow for angles to be translated into commands the robot could follow.

### Ultrasonic:

This class leverages the NewPing library to allow for easy sensor use. 
It contains two methods:

real_ping:

This performs a servo write to the desired angle, and then performs a single ping and inserts the result into the sensor output array.

scan:
Loops real_ping over a range of angles.

### Auto:

Short for Automation, this class holds the logic that directs the robot.
While it has a handful of helper functions for the larger functions, there are only two methodss that need to be understood:

center:

Handles collision avoidance, and calls sliding_window when the robot is not about to crash

sliding_window:

The most labor intensive portion of the code base of O(n) complexity.

Here is the concept:
If there is a door in front of the robot, the normal curve along that portion of the senor array output that is much higher than the rest of the output.
This curve will also have a higher mean than the entire array. So the objective is to create normal curves along the sensor output to find the best one
that the robot can enter.

##Lessons Learned:
Originally the idea was to have the robot scan while moving and drive without stopping. It seems though that the robot moves too quickly for it to be able
assess the environment properly. I could just set it to move away from obstacles as it sees them, but then it would be blindly turning. 

##Todos for a better code base
Reevaluate the servo_flip variable. Is it being properly used in both main() and centering()?

Fix Auto(), I removed the move_me method, this will break Auto()...

