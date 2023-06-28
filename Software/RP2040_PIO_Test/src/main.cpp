#include <RoboKit.hpp>

void setup() {
    robot.init();
}

void Loop() {
    robot.motor(200, 200);
    robot.clearLCD();
    robot.setLCD(8, 8, "Running");
    delay(100);
}
