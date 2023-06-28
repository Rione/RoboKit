#include <RoboKit.hpp>

void setup() {
    robot.init();
}

void Loop() {
    robot.motor(100, 100);
    wait(1);
    robot.motor(0, 0);
    wait(1);
}
