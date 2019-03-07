// Drive_test.h
#include <gtest/gtest.h>
#include "../Drive.cpp"

TEST(ProperGearing, move_me) {
    Drive car;
    car.move_me(1);
    ASSERT_EQ(1, car.drive_status);
}

