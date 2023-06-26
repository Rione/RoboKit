#ifndef ROBOKIT_H
#define ROBOKIT_H

#include <Arduino.h>
#include <IOWrapper.hpp>
#include <Wire.h>
#include <timer.h>

// Pin Definition
#define INA_LEFT_MOTOR_PWM 0
#define INB_LEFT_MOTOR_PWM 1
#define INA_RIGHT_MOTOR_PWM 2
#define INB_RIGHT_MOTOR_PWM 3

#define LEFT_ENCODER_PIN_A 7
#define LEFT_ENCODER_PIN_B 8
#define RIGHT_ENCODER_PIN_A 21
#define RIGHT_ENCODER_PIN_B 22

#define I2C_SDA_PIN 4
#define I2C_SCL_PIN 5

#define LED_PIN_1 13
#define LED_PIN_2 14
#define SW_PIN 15
#define BUZZER_PIN 20

#define MULTIPLEXER_S0_PIN 16
#define MULTIPLEXER_S1_PIN 17
#define MULTIPLEXER_S2_PIN 18
#define MULTIPLEXER_S3_PIN 19
#define MULTIPLEXER_ANALOGIN_PIN 26

#define ANALOG_IO_PORT_PIN 28

// parameters
#define LINE_SENSOR_QTY 8
#define MOTOR_MAXPOWER 1023

// varuable definition
typedef struct {
    int values[LINE_SENSOR_QTY];
    int thresholds[LINE_SENSOR_QTY];
    bool isWhite[LINE_SENSOR_QTY];
} Line_t;

class RoboKit {

  public:
    RoboKit();
    void init();
    void setLineThresold(Line_t &line_);
    void setLineThresold(int thr0, int thr1, int thr2, int thr3, int thr4, int thr5, int thr6, int thr7);

    Line_t readLine(uint8_t sensor);

    void motor(int L_Power, int R_Power);
    Line_t line;

    bool getRun() {
        return run;
    }

    void setRun(bool state) {
        run = state;
    }

  private:
    bool run;
};

extern RoboKit robot;

extern DigitalOut led_1;
extern DigitalOut led_2;
// extern DigitalIn sw_1;
extern SwitchObserver sw_1;

extern void Loop();

#endif