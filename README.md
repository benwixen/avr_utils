# avr_utils

Low-level utils for interfacing with AVR microcontroller.

Contains a collection of zero-cost inline functions to manipulate registers in a readable manner.

Instead of:

````c
TCCR1B |= (1 << CS11) | (1 << CS10);
````

You can write:

````c
avr::timer1::setPrescaleBy64();
````

Also contains an interrupt-driven and read-buffered
library for serial communication over the USART. 