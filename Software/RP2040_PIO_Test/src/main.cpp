#include <RoboKit.hpp>

void setup() {
    Serial.begin();
    robot.init();
}

void Loop() {
    Serial.println("Move Forward!");
    robot.motor(1000, 1000);
    delay(1000);
    robot.motor(0, 0);
    delay(1000);

    Serial.println("Move Backward!");
    robot.motor(-1000, -1000);
    delay(1000);
    robot.motor(0, 0);
    delay(1000);

    Serial.println("Turn Left!");
    robot.motor(-1000, 1000);
    delay(1000);
    robot.motor(0, 0);
    delay(1000);

    Serial.println("Turn Right!");
    robot.motor(1000, -1000);
    delay(1000);
    robot.motor(0, 0);
    delay(1000);
}
