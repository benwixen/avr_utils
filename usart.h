#ifndef USART_H
#define USART_H

#include <stdint.h>

namespace avr::usart {
    void init(uint32_t baud_rate);
    void printString(const char* str);
    void readString(char* str, uint8_t length);
}

#endif //USART_H
