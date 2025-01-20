#ifndef LED_H_
#define LED_H_

#include <stdint.h>
#include "Arduino.h"

class LED {
    private:
        uint8_t _pin;
        bool    _state;
    
    public:
        LED();
        void init(uint8_t);
        void set(bool);
};

#endif // LED_H_