#include <RoboKit.hpp>

void setup() {
    robot.init();
}

void Loop() {
    robot.led_1 = HIGH;
    robot.led_2 = LOW;
    delay(1000);

    robot.led_1 = LOW;
    robot.led_2 = HIGH;
    delay(1000);
}