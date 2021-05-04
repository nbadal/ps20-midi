
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

uint8_t n1Blocks = 0;
uint8_t n2Blocks = 0;
uint8_t n3Blocks = 0;
uint8_t n4Blocks = 0;
uint8_t n5Blocks = 0;
uint8_t n6Blocks = 0;
uint8_t n7Blocks = 0;

int main(void) {

    // Set up pins.
    // B: 01 noteblock outputs, 2 nc, 345 ISP, 67 xtal
    DDRB = (1 << DD0) | (1 << DD1); //0b11000000;
    PORTB;
    PINB;

    // C: 012345 noteblock outputs, 6 reset
    DDRC = (1 << DD0) | (1 << DD1) | (1 << DD2)
           | (1 << DD3) | (1 << DD4) | (1 << DD5);
    PORTC;
    PINC;

    // D: 0 uart midi rx, 1234567 notescan interrupts.
    DDRD = 0;
    PORTD;
    PIND;

    // === UART Midi RX ===

    // Baud rate (31250):
    // https://trolsoft.ru/en/uart-calc
    // 0x001F @ U2X0 = 0
    // 0x003F @ U2X0 = 1
    UBRR0H = 0x00;
    UBRR0L = 0x1F;

    // No doubling, no multi-processor
    UCSR0A = (0 << U2X0) | (0 << MPCM0);
    // Enable RX only, and RX interrupt
    UCSR0B = (1 << RXEN0) | (1 << RXCIE0);
    // Frame format: async, 1 start, 8 data, no parity, 1 stop
    UCSR0C = (0 << UCSZ02) | (1 << UCSZ00) | (1 << UCSZ01);


    // === Notescan Interrupts: ===
    // Enable interrupts for 17-23 (not incl 16)
    PCICR = (1 << PCIE2);
    PCMSK2 = (1 << PCINT17) | (1 << PCINT18) | (1 << PCINT19)
             | (1 << PCINT20) | (1 << PCINT21) | (1 << PCINT22) | (1 << PCINT23);

    while (1) {
    }
}

ISR(USART_RX_vect) {
    // Midi message recieved.
    // TODO: see if this actually gets triggered.

    // 1. Parse the message type
    // 2. Filter out anything that isn't a note ON/OFF
    // 3.
}

ISR(PCINT2_vect) {
    // Notescan change triggered.
    // TODO: verify this is triggered.

    // 1. Read notescan inputs
    // 2. If none or more than one is set, set outputs to zero
    // 3. If only one is set, output the indicated bank.

    PORTB &= ~(0x3); // unset first two bits
    switch (PIND) {
        case (1 << PIND1): // N1
            PORTC = n1Blocks & 0xFC; // Put first 6 bites into PORTC
            PORTB |= (n1Blocks << 6) & 0x03;
            break;
        case (1 << PIND2): // N2
            PORTC = n1Blocks & 0xFC;
            PORTB |= 0x00;
            break;
        case (1 << PIND3): // N3
            PORTC = n1Blocks & 0xFC;
            PORTB |= n1Blocks & ;
            break;
        case (1 << PIND4): // N4
            PORTC = n1Blocks & 0xFC;
            PORTB |= 0x00;
            break;
        case (1 << PIND5): // N5
            PORTC = n1Blocks & 0xFC;
            PORTB |= 0x00;
            break;
        case (1 << PIND6): // N6
            PORTC = n1Blocks & 0xFC;
            PORTB |= 0x00;
            break;
        case (1 << PIND7): // N7
            PORTC = n1Blocks & 0xFC;
            PORTB |= 0x00;
            break;
        default:
            PORTC = 0x00;
            break;
    }
}