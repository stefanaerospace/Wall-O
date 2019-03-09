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
	using namespace std;
	Servo servo;
	NewPing ranger;
	ranger.ping_cm_return = 100;
	Ultrasonic us;
	int range[181] = {0};
	cout<<"Entering the function"<<endl;
        us.real_ping(101, 1, range, servo, ranger);
	cout<<"exiting the function"<<endl;
	int indexed_result = range[100];
	cout<<"ping return: "<<ranger.ping_cm_return<<endl;

//	for(int i =0; i<181;i++){
//		cout<<i<<": "<<range[i]<<endl;
//	}

	ASSERT_EQ(100,indexed_result);
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS(); 
}

