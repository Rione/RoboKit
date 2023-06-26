#ifndef ROBOKIT_H
#define ROBOKIT_H

#include <Arduino.h>
#include <IOWrapper.hpp>
#include <Wire.h>
#include <SPI.h>
#include <VL53L0X.h>
#include <timer.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>

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

/**
 * @brief RoboKitクラスはロボットキットの機能を制御します。
 */
class RoboKit {
    // ...

  public:
    /**
     * @brief RoboKitクラスのコンストラクタです。
     */
    RoboKit();

    /**
     * @brief ロボットキットの初期化を行います。
     */
    void init();

    /**
     * @brief ラインセンサーの閾値を設定します。
     * @param line_ ラインセンサーの閾値を持つLine_t構造体
     */
    void setLineThresold(Line_t &line_);

    /**
     * @brief ラインセンサーの閾値を個別に設定します。
     * @param thr0, thr1,thr2,thr3,thr4,thr5,thr6,thr7 各センサーの閾値(0~1023)
     */
    void setLineThresold(int thr0, int thr1, int thr2, int thr3, int thr4, int thr5, int thr6, int thr7);

    /**
     * @brief 指定されたラインセンサーの値を読み取ります。
     * @param sensor 読み取るセンサーのインデックス
     * @return ラインセンサーの番号(0~7)
     */
    int readLine(uint8_t sensor);

    /**
     * @brief 全てのラインセンサーの値を読み取ります。
     * @return 全てのラインセンサーの値を含むLine_t構造体
     */
    Line_t readLines();

    /**
     * @brief ラインセンサーの値をSerial Monitorに表示します。
     * @param withRead ラインセンサーの値を読み取るかどうか(true: 読み取って表示, false: 事前に読んだ値を表示)
     */
    void printLines(bool withRead = false);

    /**
     * @brief ラインセンサーの値をSerial Monitorに表示します。
     * @param line_ ラインセンサーの値を持つLine_t構造体
     */
    void printLines(Line_t &line_);

    /**
     * @brief モーターの動作を制御します。
     * @param L_Power 左モーターのパワー(0~1023)
     * @param R_Power 右モーターのパワー(0~1023)
     */
    void motor(int L_Power, int R_Power);

    /**
     * @brief モーターの回転方向を設定します。
     * @param CW_R_ 右モーターの回転方向 (true: CW, false: CCW)
     * @param CW_L_ 左モーターの回転方向 (true: CW, false: CCW)
     **/
    void setMotorCW(bool CW_R_, bool CW_L_);

    /**
     * @brief ToFセンサーで距離の値を読み取ります。
     * @return 距離の値(mm)
     */
    int getDistance();

    /**
     * @brief Get the Yaw object
     * @return double() Yaw angle (degree)
     */
    double getYaw();

    /**
     * @brief Get the Pitch object
     *
     * @return double Pitch angle (degree)
     */
    double getPitch();

    /**
     * @brief Get the Roll object
     *
     * @return double Roll angle (degree)
     */
    double getRoll();

    /**
     * @brief スイッチが押されたら状態が変化します。
     * @return 実行状態 (true: 実行中, false: 停止中)
     */
    bool getRun();

    /**
     * @brief ロボットの実行状態を設定します。
     * @param state 設定する実行状態 (true: 実行中, false: 停止中)
     */
    void setRun(bool state);

    DigitalOut led_1;    /**< LEDピン 1 の制御オブジェクト */
    DigitalOut led_2;    /**< LEDピン 2 の制御オブジェクト */
    SwitchObserver sw_1; /**< スイッチピンの状態を監視するオブジェクト */
    VL53L0X tof;         /**< 距離センサーの制御オブジェクト */
    Adafruit_BNO055 bno; /**< IMUの制御オブジェクト */

  private:
    void initVL53L0X();
    void initBNO055();
    bool run; /**< ロボットの実行状態 (true: 実行中, false: 停止中) */
    Line_t line;
    bool CW_R;
    bool CW_L;
};

extern RoboKit robot;
extern DigitalOut led_1;
extern DigitalOut led_2;
extern SwitchObserver sw_1;
// extern DigitalIn sw_1;

extern void Loop();
#endif