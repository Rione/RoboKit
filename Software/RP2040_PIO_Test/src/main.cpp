#include <RoboKit.hpp>

void setup() {
    robot.init();
}

void Loop() {
    robot.motor(245, 255);
    delay(1900);
    robot.motor(0, 0);
    while (1) {
    }
}
