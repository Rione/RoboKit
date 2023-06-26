#include <RoboKit.hpp>

void setup() {
    robot.init();
}

void Loop() {
    // SerialUSB.println("Hello World!!!");

    // ラインセンサの閾値を設定する関数
    robot.setLineThresold(0, 0, 0, 0, 0, 0, 0, 0);

    // ラインセンサの閾値を設定する関数
    Line_t lineThreshold;
    lineThreshold.thresholds[0] = 0;
    lineThreshold.thresholds[1] = 0;
    lineThreshold.thresholds[2] = 0;
    lineThreshold.thresholds[3] = 0;
    lineThreshold.thresholds[4] = 0;
    lineThreshold.thresholds[5] = 0;
    lineThreshold.thresholds[6] = 0;
    lineThreshold.thresholds[7] = 0;
    robot.setLineThresold(lineThreshold);

    // ラインセンサの値を個別に取得するメソッド
    int val0 = robot.readLine(0);
    int val1 = robot.readLine(1);
    int val2 = robot.readLine(2);
    int val3 = robot.readLine(3);
    int val4 = robot.readLine(4);
    int val5 = robot.readLine(5);
    int val6 = robot.readLine(6);
    int val7 = robot.readLine(7);
    SerialUSB.print(val0);
    SerialUSB.print(" ");
    SerialUSB.print(val1);
    SerialUSB.print(" ");
    SerialUSB.print(val2);
    SerialUSB.print(" ");
    SerialUSB.print(val3);
    SerialUSB.print(" ");
    SerialUSB.print(val4);
    SerialUSB.print(" ");
    SerialUSB.print(val5);
    SerialUSB.print(" ");
    SerialUSB.print(val6);
    SerialUSB.print(" ");
    SerialUSB.print(val7);
    SerialUSB.println();

    // ラインセンサの値をまとめて取得するメソッド
    Line_t line = robot.readLines();
    for (size_t i = 0; i < LINE_SENSOR_QTY; i++) {
        Serial.print("val[");
        Serial.print(i);
        Serial.print("]:");
        Serial.println(line.values[i]);
        Serial.print(" ");
    }
    SerialUSB.println();

    // ラインセンサが白か黒かを判定するメソッド

    Line_t lines = robot.readLines();
    bool *isWhite = &lines.isWhite[0];
    for (size_t i = 0; i < LINE_SENSOR_QTY; i++) {
        Serial.print("isWhite[");
        Serial.print(i);
        Serial.print("]:");
        Serial.println(isWhite[i]);
        Serial.print(" ");
    }
    SerialUSB.println();

    // モータを動かすメソッド
    robot.motor(500, 500); // 全身
    delay(1000);
    robot.motor(0, 0);
    delay(500);
    robot.motor(-500, -500); // 後進
    delay(1000);
    robot.motor(0, 0);
    delay(500);
    robot.motor(0, 500);
    delay(1000);
    robot.motor(500, 0);
    delay(1000);
    robot.motor(0, 0);
    delay(500);

    int deg = 0;
    for (size_t i = 0; i < 360; i++) {
        deg++;
        int out = MOTOR_MAXPOWER * sin(deg * 3.1415 / 180);
        robot.motor(-out, out);
        delay(20);
    }

    // led1を光らせる
    led_1 = HIGH;
    delay(1000);

    // led1を消す
    led_1 = LOW;
    delay(1000);

    // led2を光らせる
    led_2 = HIGH;
    delay(1000);

    // led2を消す
    led_2 = LOW;

    // タクトスイッチの状態を取得する
    for (int i = 0; i < 1000; i++) {
        bool sw = sw_1;
        SerialUSB.println(sw);
        delay(1);
    }
}
