#include "usart.h"
#include <avr/io.h>

namespace {
    inline void setBaudRate(uint32_t baud_rate) {
        uint16_t baud_setting = (F_CPU / 4 / baud_rate - 1) / 2;
        UCSR0A = (1 << U2X0);
        UBRR0H = baud_setting >> 8;
        UBRR0L = baud_setting;
    }

    inline void enableUsartReceiveAndTransmit() {
        UCSR0B = (1 << TXEN0) | (1 << RXEN0);
    }

    inline void set8bitCharacterSize() {
        UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);
    }

    inline void transmitChar(char data) {
        loop_until_bit_is_set(UCSR0A, UDRE0);
        UDR0 = data;
    }

    inline char readChar() {
        loop_until_bit_is_set(UCSR0A, RXC0);
        return UDR0;
    }

}

namespace avr::usart {
    void init(uint32_t baud_rate) {
        setBaudRate(baud_rate);
        enableUsartReceiveAndTransmit();
        set8bitCharacterSize();
    }

    void printString(const char* str) {
        uint8_t i = 0;
        while (str[i]) {
            transmitChar(str[i]);
            i++;
        }
    }

    void readString(char* str, uint8_t length) {
        for (int i = 0; i < length; i++) {
            str[i] = readChar();
        }
    }
}
