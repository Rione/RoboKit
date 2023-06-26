#include <Robokit.hpp>

RoboKit::RoboKit() : led_1(LED_PIN_1), led_2(LED_PIN_2),
                     sw_1(SW_PIN, INPUT_PULLUP),
                     tof(), bno(55),
                     run(false), line(),
                     CW_R(true), CW_L(true) {
}

void RoboKit::init() {
    // Line sensor
    pinMode(MULTIPLEXER_S0_PIN, OUTPUT);
    pinMode(MULTIPLEXER_S1_PIN, OUTPUT);
    pinMode(MULTIPLEXER_S2_PIN, OUTPUT);
    pinMode(MULTIPLEXER_S3_PIN, OUTPUT);
    for (int i = 0; i < LINE_SENSOR_QTY; i++) {
        line.values[i] = 0;
        line.thresholds[i] = 0;
        line.isWhite[i] = 0;
    }
    // Motor Encoder
    pinMode(LEFT_ENCODER_PIN_A, OUTPUT);
    pinMode(LEFT_ENCODER_PIN_B, OUTPUT);
    pinMode(RIGHT_ENCODER_PIN_A, OUTPUT);
    pinMode(RIGHT_ENCODER_PIN_B, OUTPUT);

    motor(0, 0);

    Wire.begin();
    initVL53L0X();
    initBNO055();
    run = false;

    SerialUSB.println("RoboKit Init!!");
}

void RoboKit::initVL53L0X() {
    tof.setTimeout(500);
    if (!tof.init()) {
        SerialUSB.println("Failed to detect and initialize sensor!");
        while (1)
            ;
    }
    tof.setSignalRateLimit(0.1);
    tof.setVcselPulsePeriod(VL53L0X::VcselPeriodPreRange, 18);
    tof.setVcselPulsePeriod(VL53L0X::VcselPeriodFinalRange, 14);
}

void RoboKit::initBNO055() {
    // BNO055
    if (!bno.begin()) {
        SerialUSB.print("No BNO055 detected");
        while (1)
            ;
    }
    delay(1000);
    bno.setExtCrystalUse(true);
}

void RoboKit::setLineThresold(Line_t &line_) {
    Serial.println("---------------SET THRESHOLD---------------");
    for (int i = 0; i < LINE_SENSOR_QTY; i++) {
        line.thresholds[i] = line_.thresholds[i];
        Serial.print("LineThr[");
        Serial.print(i);
        Serial.print("]:");
        Serial.println(line.thresholds[i]);
    }
    Serial.println("-------------------------------------------");
}

void RoboKit::setLineThresold(int thr0, int thr1, int thr2, int thr3, int thr4, int thr5, int thr6, int thr7) {
    Serial.println("---------------SET THRESHOLD---------------");
    line.thresholds[0] = thr0;
    line.thresholds[1] = thr1;
    line.thresholds[2] = thr2;
    line.thresholds[3] = thr3;
    line.thresholds[4] = thr4;
    line.thresholds[5] = thr5;
    line.thresholds[6] = thr6;
    line.thresholds[7] = thr7;
    for (int i = 0; i < LINE_SENSOR_QTY; i++) {
        Serial.print("LineThr[");
        Serial.print(i);
        Serial.print("]:");
        Serial.println(line.thresholds[i]);
        Serial.print("\t");
    }
    Serial.println("-------------------------------------------");
}

// sensor must be 0 to 7
int RoboKit::readLine(uint8_t sensor) {
    digitalWrite(MULTIPLEXER_S0_PIN, (sensor & 0b00000001));
    digitalWrite(MULTIPLEXER_S1_PIN, (sensor & 0b00000010));
    digitalWrite(MULTIPLEXER_S2_PIN, (sensor & 0b00000100));
    digitalWrite(MULTIPLEXER_S3_PIN, (sensor & 0b00001000));
    line.values[sensor] = analogRead(MULTIPLEXER_ANALOGIN_PIN);
    if (line.values[sensor] > line.thresholds[sensor]) {
        line.isWhite[sensor] = true;
    } else {
        line.isWhite[sensor] = false;
    }
    return line.values[sensor];
}

Line_t RoboKit::readLines() {
    for (int i = 0; i < LINE_SENSOR_QTY; i++) {
        readLine(i);
    }
    return line;
}

void RoboKit::printLines(bool withRead = false) {
    if (withRead) readLines();
    for (size_t i = 0; i < LINE_SENSOR_QTY; i++) {
        Serial.print("val[");
        Serial.print(i);
        Serial.print("]:");
        Serial.println(line.values[i]);
        Serial.print("\t");
    }
    SerialUSB.println();
}

void RoboKit::printLines(Line_t &line_) {
    for (size_t i = 0; i < LINE_SENSOR_QTY; i++) {
        Serial.print("val[");
        Serial.print(i);
        Serial.print("]:");
        Serial.println(line_.values[i]);
        Serial.print("\t");
    }
    SerialUSB.println();
}

// -1023 to 1023
void RoboKit::motor(int L_Power, int R_Power) {
    int outA1, outB1, outA2, outB2;

    if (CW_R == false) R_Power = -R_Power;
    if (CW_L == false) L_Power = -L_Power;
    // Left Motor
    if (L_Power == 0) {
        // BLAKE
        outA1 = 255;
        outB1 = 255;
    } else {
        if (L_Power > 0) {
            // CCW
            outA1 = abs(L_Power) > MOTOR_MAXPOWER ? MOTOR_MAXPOWER : abs(L_Power);
            outB1 = 0;
        } else {
            // CW
            outA1 = 0;
            outB1 = abs(L_Power) > MOTOR_MAXPOWER ? MOTOR_MAXPOWER : abs(L_Power);
        }
    }
    // Right Motor
    if (R_Power == 0) {
        // BLAKE
        outA2 = 255;
        outB2 = 255;
    } else {
        if (R_Power > 0) {
            // CCW
            outA2 = abs(R_Power) > MOTOR_MAXPOWER ? MOTOR_MAXPOWER : abs(R_Power);
            outB2 = 0;
        } else {
            // CW
            outA2 = 0;
            outB2 = abs(R_Power) > MOTOR_MAXPOWER ? MOTOR_MAXPOWER : abs(R_Power);
        }
    }
    analogWrite(INA_LEFT_MOTOR_PWM, outA1);
    analogWrite(INB_LEFT_MOTOR_PWM, outB1);
    analogWrite(INA_RIGHT_MOTOR_PWM, outA2);
    analogWrite(INB_RIGHT_MOTOR_PWM, outB2);
}

void RoboKit::setMotorCW(bool CW_R_, bool CW_L_) {
    CW_R = CW_R_;
    CW_L = CW_L_;
}

int RoboKit::getDistance() {
    if (tof.timeoutOccurred()) {
        Serial.print(" TIMEOUT");
    }
    return tof.readRangeSingleMillimeters();
}

double RoboKit::getYaw() {
    imu::Vector<3> euler = bno.getVector(Adafruit_BNO055::VECTOR_EULER);
    return euler.x();
}

double RoboKit::getPitch() {
    imu::Vector<3> euler = bno.getVector(Adafruit_BNO055::VECTOR_EULER);
    return euler.y();
}

double RoboKit::getRoll() {
    imu::Vector<3> euler = bno.getVector(Adafruit_BNO055::VECTOR_EULER);
    return euler.z();
}

bool RoboKit::getRun() {
    return run;
}

void RoboKit::setRun(bool state) {
    run = state;
}

RoboKit robot;
DigitalOut led_1(LED_PIN_1);
DigitalOut led_2(LED_PIN_1);
// DigitalIn sw_1(SW_PIN, INPUT_PULLUP);
SwitchObserver sw_1(SW_PIN, INPUT_PULLUP);

void loop() {
    if (robot.getRun()) {
        Loop();
    } else {
        robot.motor(0, 0);
        Serial.println("Press sw_1...");
        delay(100);
    }
}

void setup1() {
}
void loop1() {
    sw_1.check();
    robot.setRun(sw_1.getToggleState());
}