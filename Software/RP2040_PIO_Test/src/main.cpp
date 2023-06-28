#include <RoboKit.hpp>

#include <U8g2lib.h>

U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R1);

void setup() {
    robot.init();

    u8g2.begin(); // OLED初期化
    u8g2.setContrast(1);

    u8g2.clearBuffer();
    u8g2.setFont(u8g2_font_crox1hb_tf);
    u8g2.drawStr(0, 17, "Hello world");
    u8g2.sendBuffer();
}

void Loop() {
}
