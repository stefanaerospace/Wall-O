// Drive_test.h
#include <gtest/gtest.h>
#include "../Drive.cpp"

TEST(ProperGearing, move_me) {
    Drive car;
    car.move_me(1);
    ASSERT_EQ(1, car.drive_status);
}


int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
