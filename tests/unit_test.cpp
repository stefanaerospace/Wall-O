// unit_tests.h

#include <gtest/gtest.h>
#include <stdio.h>
#include "../Drive.cpp"
#include "../Ultrasonic.cpp"
#include "../Auto.cpp"

TEST(Proper_Gearing, direction_changes) {
    Drive car;
    car.forward();
    ASSERT_EQ(1, car.drive_status);
    car.back();
    ASSERT_EQ(2, car.drive_status);
    car.right();
    ASSERT_EQ(3, car.drive_status);
    car.left();
    ASSERT_EQ(4, car.drive_status);
    car.stop();
    ASSERT_EQ(5, car.drive_status);
}

TEST(Proper_Update_Command, drive_update){
    Drive car;
    car.drive_update(120);
    ASSERT_EQ(3, car.drive_status);
    car.drive_update(60);
    ASSERT_EQ(4, car.drive_status);
    car.drive_update(90);
    ASSERT_EQ(1, car.drive_status);
}

TEST(Ping_Method, real_ping){
	Servo servo;
	NewPing ranger;
	Ultrasonic us;
	int range[181] = {0};
	
	int test_angle = 100;
	/*do not let "test_angle" confuse you
	 * it is the value the function should return
	 * and the angle to index
	 */
	ranger.ping_cm_return = test_angle;

        us.real_ping(test_angle, 1, range, servo, ranger);
	
	ASSERT_EQ(test_angle,range[test_angle]);
}

TEST(Scanning, scan){
	Servo servo;
	NewPing ranger;
	Ultrasonic us;

	ranger.ping_cm_return = 100;
	int range[181] = {0};

	//this should return a 100 on an earlier angle
	int test_angle = 0;
	us.scan(test_angle, 180, range, servo, ranger, 3);
	ASSERT_EQ(100, range[test_angle]);
	
	
	//this should return a 100 on a later angle
        test_angle = 180;
        us.scan(0, test_angle, range, servo, ranger, 3);
	ASSERT_EQ(100, range[test_angle]);
}

TEST(Collision_Avoidance, collision_avoidance){
	
    Servo servo;
	NewPing ranger;
	ranger.ping_cm_return = 7;
	Drive car;
	Auto brain;
	Ultrasonic us;

    bool servo_flip = false;

	int range[181] = {0};
	for(int x=0; x<=181; x++){
	  range[x] = 7;
	}
	range[30] = 100;
	
	//this test needs to be modified, low fidelity 
	brain.collision_avoidance(servo_flip, us, servo, ranger);

	ASSERT_EQ(5, brain.drive_status);

}


TEST(Pathfinding_Test_Zero_case, sliding_window){
	Servo servo;
	NewPing ranger;
	Auto brain;
	//This should return a 90 degree result
	int range[181] = {0};
	int result = brain.sliding_window(range);
	ASSERT_EQ(90, result);
}

TEST(Pathfinding_Test_D_Forward, sliding_window){
	Servo servo;
	NewPing ranger;
	Auto brain;

	//This should return a 90 degree result
  
  int range[181];

  for(int i=0; i<=180; ++i){
    range[i] = 0;
  }

	range[90] = 190;
	int result = brain.sliding_window(range);
	ASSERT_EQ(88, result);
}

TEST(Pathfinding_Test_D_Left, sliding_window){
	Servo servo;
	NewPing ranger;
	Auto brain;
	//This should return a 90 degree result
  int range[181];

  for(int i=0; i<=180; ++i){
    range[i] = 0;
  }

	range[170] = 190;
	int result = brain.sliding_window(range);

	ASSERT_EQ(178, result);
}

TEST(Pathfinding_Test_D_Right, sliding_window){
	Servo servo;
	NewPing ranger;
	Auto brain;
	//This should return a 90 degree result
  int range[181];

  for(int i=0; i<=180; ++i){
    range[i] = 0;
  }

	range[10] = 190;
	int result = brain.sliding_window(range);

	ASSERT_EQ(7, result);
}

int main(int argc, char **argv){
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS(); 
}

