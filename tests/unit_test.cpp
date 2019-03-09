// unit_tests.h

#include <gtest/gtest.h>
#include "../Drive.cpp"
#include "../Ultrasonic.cpp"
#include "../Auto.cpp"


TEST(ProperGearing__move_me, move_me) {
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

TEST(ProperUpdateCommand__drive_update, drive_update){
    Drive car;
    car.drive_update(79);
    ASSERT_EQ(3, car.drive_status);
    car.drive_update(80);
    ASSERT_EQ(3, car.drive_status);
    car.drive_update(101);
    ASSERT_EQ(4, car.drive_status);
    car.drive_update(100);
    ASSERT_EQ(4, car.drive_status);
    car.drive_update(90);
    ASSERT_EQ(1, car.drive_status);
}




int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}

