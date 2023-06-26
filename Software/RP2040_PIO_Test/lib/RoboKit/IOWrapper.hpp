#ifndef IO_WRAPPER_HPP
#define IO_WRAPPER_HPP

#include <Arduino.h>

class DigitalOut {
  public:
    DigitalOut(pin_size_t _pinNumber, PinMode _mode = OUTPUT) {
        pinNumber = _pinNumber;
        mode = _mode;
        pinMode(pinNumber, mode);
    }

    void write(bool state) {
        digitalWrite(pinNumber, state);
    }
    void operator=(bool state) {
        write(state);
    }

  private:
    pin_size_t pinNumber;
    PinMode mode;
};

class DigitalIn {
  public:
    DigitalIn(pin_size_t _pinNumber, PinMode _mode = INPUT) {
        pinNumber = _pinNumber;
        mode = _mode;
        pinMode(pinNumber, mode);
    }

    bool read(void) {
        return digitalRead(pinNumber);
    }
    operator bool() {
        return read();
    }

  private:
    pin_size_t pinNumber;
    PinMode mode;
};

#endif