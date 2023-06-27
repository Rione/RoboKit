#include <RoboKit.hpp>

void setup() {
    Serial.begin();
    robot.init();
    robot.motor(0, 0);
}

void Loop() {
    Serial.println("Hello World!");
    delay(100);
}
