#include <Robokit.hpp>

RoboKit::RoboKit() : led_1(LED_PIN_1), led_2(LED_PIN_2),
                     sw_1(SW_PIN, INPUT_PULLUP),
                     tof(), bno(55),
                     run(false), line(),
                     CW_R(true), CW_L(true), u8g2(U8G2_R1) {
}

void RoboKit::init() {
    Serial.begin();
    // Motor Driver
    pinMode(INA_LEFT_MOTOR_PWM, OUTPUT);
    pinMode(INB_LEFT_MOTOR_PWM, OUTPUT);
    pinMode(INA_RIGHT_MOTOR_PWM, OUTPUT);
    pinMode(INB_RIGHT_MOTOR_PWM, OUTPUT);
    // analogWriteRange(1024);
    analogWriteFreq(30000);
    // Motor Encoder
    pinMode(LEFT_ENCODER_PIN_A, OUTPUT);
    pinMode(LEFT_ENCODER_PIN_B, OUTPUT);
    pinMode(RIGHT_ENCODER_PIN_A, OUTPUT);
    pinMode(RIGHT_ENCODER_PIN_B, OUTPUT);

    motor(0, 0);
    setMotorCW(false, true);
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

    Wire.begin();
    Wire.setClock(400000);
    // initVL53L0X();
    initBNO055();
    initDisplay();
    run = false;

    Serial.println("RoboKit Init!!");
}

void RoboKit::initVL53L0X() {
    tof.setTimeout(TOF_TIMEOUT);
    if (!tof.init())
        while (1) {
            Serial.println("Failed to detect and initialize sensor!");
            delay(100);
        }

    tof.setSignalRateLimit(0.1);
    tof.setVcselPulsePeriod(VL53L0X::VcselPeriodPreRange, 18);
    tof.setVcselPulsePeriod(VL53L0X::VcselPeriodFinalRange, 14);
}

void RoboKit::initBNO055() {
    // BNO055
    if (!bno.begin())
        while (1) {
            Serial.print("No BNO055 detected");
            delay(100);
        }

    delay(1000);
    bno.setExtCrystalUse(true);
}

void RoboKit::initDisplay() {
    u8g2.begin();
    u8g2.setContrast(1);

    u8g2.clearBuffer();
    u8g2.setFont(u8g2_font_crox1hb_tf);
    u8g2.drawStr(0, 8, "Hello world");
    u8g2.sendBuffer();
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
    digitalWrite(MULTIPLEXER_S0_PIN, bitRead(sensor, 0));
    digitalWrite(MULTIPLEXER_S1_PIN, bitRead(sensor, 1));
    digitalWrite(MULTIPLEXER_S2_PIN, bitRead(sensor, 2));
    digitalWrite(MULTIPLEXER_S3_PIN, bitRead(sensor, 3));
    line.values[sensor] = analogRead(MULTIPLEXER_ANALOGIN_PIN);
    if (line.values[sensor] < line.thresholds[sensor]) {
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

void RoboKit::printLines(bool withRead) {
    if (withRead) readLines();
    for (size_t i = 0; i < LINE_SENSOR_QTY; i++) {
        Serial.print("val[");
        Serial.print(i);
        Serial.print("]:");
        Serial.print(line.values[i]);
        Serial.print("\t");
    }
    Serial.println();
}

void RoboKit::printLines(Line_t &line_) {
    for (size_t i = 0; i < LINE_SENSOR_QTY; i++) {
        Serial.print("val[");
        Serial.print(i);
        Serial.print("]:");
        Serial.println(line_.values[i]);
        Serial.print("\t");
    }
    Serial.println();
}

// -255 to 255
void RoboKit::motor(int L_Power, int R_Power) {
    int outA1, outB1, outA2, outB2;

    if (CW_R == false) R_Power = -R_Power;
    if (CW_L == false) L_Power = -L_Power;
    // Left Motor
    if (L_Power == 0) {
        // BLAKE
        outA1 = MOTOR_MAXPOWER;
        outB1 = MOTOR_MAXPOWER;
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
        outA2 = MOTOR_MAXPOWER;
        outB2 = MOTOR_MAXPOWER;
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

void RoboKit::setLCD(int row, int col, const char *str) {
    u8g2.setFont(u8g2_font_crox1hb_tf);
    u8g2.drawStr(col, row * 8, str);
    u8g2.sendBuffer();
}

void RoboKit::clearLCD() {
    u8g2.clearBuffer();
    u8g2.sendBuffer();
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
SwitchObserver sw_1(SW_PIN, INPUT_PULLUP);

void setup1() {
}
void loop1() {
    sw_1.check();
    robot.setRun(sw_1.getToggleState());
    delay(10);
}
void loop() {
    if (robot.getRun()) {
        Loop();
    } else {
        robot.motor(0, 0);
        Serial.print("Press sw_1...");
        Serial.println(robot.sw_1.read());
        robot.clearLCD();
        robot.setLCD(8, 8, "wait ");
        delay(100);
    }
}