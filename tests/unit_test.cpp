// unit_tests.h

#include <gtest/gtest.h>
#include <stdio.h>
#include "../Drive.cpp"
#include "../Ultrasonic.cpp"
#include "../Auto.cpp"

TEST(Proper_Gearing, move_me) {
    Drive car;
    car.move_me(1);
    ASSERT_EQ(1, car.drive_status);
    car.move_me(2);
    ASSERT_EQ(2, car.drive_status);
    car.move_me(3);
    ASSERT_EQ(3, car.drive_status);
    car.move_me(4);
    ASSERT_EQ(4, car.drive_status);
    car.move_me(5);
    ASSERT_EQ(5, car.drive_status);
}

TEST(Proper_Update_Command, drive_update){
    Drive car;
    car.drive_update(79);
    ASSERT_EQ(3, car.drive_status);
    car.drive_update(80);
    ASSERT_EQ(3, car.drive_status);
    car.drive_update(101);
    ASSERT_EQ(4, car.drive_status);
    car.drive_update(101);
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

TEST(Centering_the_path, center){
	Servo servo;
	NewPing ranger;
	Drive car;
	Auto brain;
	Ultrasonic us;
	int range[181] = {7};
	range[100] = 100;
	
	//this test needs to be modified, low fidelity 
	brain.center(range, false, us, servo, ranger, car);

	ASSERT_EQ(3, car.drive_status);

}

TEST(Does_the_window_slide_Auto, sliding_window){
	Servo servo;
	NewPing ranger;
	Auto brain;
	int range[181] = {0};
	
	//This should return a 90 degree result
	range[100] = 100;
	int result = brain.sliding_window(range);

	using namespace std;
	cout<<"                 RESULT:  "<<result<<endl;
	ASSERT_EQ(100, result);



}


int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS(); 
}

