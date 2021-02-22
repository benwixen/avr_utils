#ifndef AVRUTILS_H
#define AVRUTILS_H

#include <stdint.h>
#include <avr/io.h>
#include <avr/interrupt.h>

namespace {
    inline void setBit(volatile uint8_t& reg, uint8_t bit) {
        reg |= (1 << bit);
    }

    inline void clearBit(volatile uint8_t& reg, uint8_t bit) {
        reg &= ~(1 << bit);
    }
}

namespace avr {
    inline void setPinToOutput(volatile uint8_t& ddr, uint8_t pin) {
        setBit(ddr, pin);
    }

    inline void setOutputPinHigh(volatile uint8_t& ddr, uint8_t pin) {
        setBit(ddr, pin);
    }

    inline void setOutputPinLow(volatile uint8_t& ddr, uint8_t pin) {
        clearBit(ddr, pin);
    }

    namespace timer0 {
        inline void enable8bitPhaseCorrectPwmMode() {
            TCCR0A |= (1 << WGM00);
        }

        inline void enableOutputOnPD5andPD6() {
            setPinToOutput(DDRD, PD5);
            setPinToOutput(DDRD, PD6);
            TCCR0A |= (1 << COM0A1) | (1 << COM0B1);
        }

        inline void setPrescaleBy64() {
            TCCR0B |= (1 << CS01) | (1 << CS00);
        }

        inline void setPwmValueForPD5(uint8_t value) {
            OCR0B = value;
        }

        inline void setPwmValueForPD6(uint8_t value) {
            OCR0A = value;
        }
    }

    namespace timer1 {
        inline void enable8bitPhaseCorrectPwmMode() {
            TCCR1A |= (1 << WGM10);
        }

        inline void enableOutputOnPB1andPB2() {
            setPinToOutput(DDRB, PB1);
            setPinToOutput(DDRB, PB2);
            TCCR1A |= (1 << COM1A1) | (1 << COM1B1);
        }

        inline void setPrescaleBy64() {
            TCCR1B |= (1 << CS11) | (1 << CS10);
        }

        inline void setPrescaleBy256() {
            TCCR1B |= (1 << CS12);
        }

        inline void setPwmValueForPB1(uint8_t value) {
            OCR1A = value;
        }

        inline void setPwmValueForPB2(uint8_t value) {
            OCR1B = value;
        }

        inline uint16_t getCount() {
            return TCNT1;
        }
    }

    inline void setPwmValue(uint8_t pin, uint8_t pwm_value) {
        switch (pin) {
            case PD5:
                timer0::setPwmValueForPD5(pwm_value);
                break;
            case PD6:
                timer0::setPwmValueForPD6(pwm_value);
                break;
            case PB1:
                timer1::setPwmValueForPB1(pwm_value);
                break;
            case PB2:
                timer1::setPwmValueForPB2(pwm_value);
                break;
            default:
                return;
        }
    }

    inline void configureInterruptsForINT0() {
        EIMSK |= (1 << INT0);
    }

    inline void triggerInterruptsOnVoltageChanges() {
        EICRA |= (1 << ISC00);
    }

    inline void enableInterrupts() {
        sei();
    }
}

#endif //AVR_UTILS_H
