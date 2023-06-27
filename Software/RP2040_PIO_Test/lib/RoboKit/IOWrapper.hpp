#ifndef IO_WRAPPER_HPP
#define IO_WRAPPER_HPP

#include <Arduino.h>
#include <timer.h>
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

class SwitchObserver {
  public:
    SwitchObserver(pin_size_t _pinNumber, PinMode _mode = OUTPUT) : sw(_pinNumber, _mode),
                                                                    Tim(),
                                                                    state(false),
                                                                    statePrev(false),
                                                                    toggleState(false) {
    }
    bool check() {
        state = sw.read();
        if (Tim.read_ms() > 10) {
            if (state != statePrev && state == LOW) {
                toggleState = !toggleState;
            }
            Tim.reset();
            statePrev = state;
        }
        return state;
    }
    bool getToggleState() {
        return toggleState;
    }
    bool read(void) {
        return sw.read();
    }

    operator bool() {
        return check();
    }

  private:
    DigitalIn sw;
    timer Tim;
    bool state;
    bool statePrev;
    bool toggleState;
};

#endif