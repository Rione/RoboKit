#include <RoboKit.hpp>

void setup() {
    robot.init();
}

void Loop() {
    robot.motor(200, 200);
    wait(1.0);
}
