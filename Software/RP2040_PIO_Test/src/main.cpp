#include <RoboKit.hpp>

void setup() {
    robot.init();
    // ラインセンサのしきい値を設定
    robot.setLineThresold(250, 250, 250, 250, 250, 250, 250, 250);
}

void Loop() {
    robot.readLines();  // 読み取り
    robot.printLines(); // プリント
    delay(1);
}
