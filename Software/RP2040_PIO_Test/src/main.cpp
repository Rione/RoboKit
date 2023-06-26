#include <RoboKit.hpp>

void setup() {
    robot.init();
}

void Loop() {
    SerialUSB.println("Hello World!!!");
}
