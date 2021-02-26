#include "usart.h"

#include <stdio.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/atomic.h>

#include "avr_utils.h"

namespace {
    inline void setBaudRate(uint32_t baud_rate) {
        uint16_t baud_setting = (F_CPU / 4 / baud_rate - 1) / 2;
        UCSR0A = (1 << U2X0);
        UBRR0H = baud_setting >> 8;
        UBRR0L = baud_setting;
    }

    inline void enableUsartReceiveAndTransmit() {
        UCSR0B |= (1 << TXEN0) | (1 << RXEN0);
    }

    inline void enableInterruptsOnReceive() {
        UCSR0B |= (1 << RXCIE0);
        avr::enableInterrupts();
    }

    inline void set8bitCharacterSize() {
        UCSR0C |= (1 << UCSZ01) | (1 << UCSZ00);
    }

    inline void transmitByte(uint8_t data) {
        loop_until_bit_is_set(UCSR0A, UDRE0);
        UDR0 = data;
    }

    volatile uint8_t read_pointer { 0 };
    volatile uint8_t write_pointer { 0 };
    volatile uint8_t elements_to_read { 0 };
    volatile uint8_t receive_buffer[8];

    inline void incrementReadPointer() {
        read_pointer += 1;
        if (read_pointer == 8) {
            read_pointer = 0;
        }
    }

    ISR(USART_RX_vect) {
        receive_buffer[write_pointer] = UDR0;
        write_pointer++;
        if (write_pointer == 8) {
            write_pointer = 0;
        }
        if (elements_to_read < 8) {
            elements_to_read +=  1;
        } else {
            incrementReadPointer();
        }
    }

    inline uint8_t receiveByte() {
        while (elements_to_read == 0) {}
        uint8_t byte;
        ATOMIC_BLOCK(ATOMIC_FORCEON) {
            byte = receive_buffer[read_pointer];
            elements_to_read -= 1;
            incrementReadPointer();
        }
        return byte;
    }
}

namespace avr::usart {
    void init(uint32_t baud_rate) {
        setBaudRate(baud_rate);
        enableUsartReceiveAndTransmit();
        enableInterruptsOnReceive();
        set8bitCharacterSize();
    }

    void printString(const char* str) {
        uint8_t i = 0;
        while (str[i]) {
            transmitByte(str[i]);
            i++;
        }
    }

    void readString(char* str, uint8_t length) {
        for (int i = 0; i < length; i++) {
            str[i] = receiveByte();
        }
    }

    uint8_t readBytes(uint8_t* bytes, uint8_t until_delimiter, uint8_t max_length) {
        for (int bytes_read = 0; bytes_read < max_length; bytes_read++) {
            const uint8_t received_byte { receiveByte() };
            if (received_byte == until_delimiter) {
                return bytes_read;
            }
            bytes[bytes_read] = received_byte;
        }
        return max_length;
    }
}
