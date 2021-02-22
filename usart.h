#ifndef USART_H
#define USART_H

#include <stdint.h>

namespace avr {
    void initUsart(uint32_t baud_rate);
    void printString(const char* str);
    void readString(char* str, uint8_t length);
}

#endif //USART_H
