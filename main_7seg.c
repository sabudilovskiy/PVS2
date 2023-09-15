#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#define DIOD_A PA0
#define DIOD_B PA1
#define DIOD_C PA2
#define DIOD_D PA3
#define DIOD_E PA4
#define DIOD_F PA5
#define DIOD_G PA6

#define NUM_1 PB3
#define NUM_2 PB2
#define NUM_3 PB1

#define BUTTON_PLUS PB0
#define BUTTON_MINUS PA7

static int CURRENT_NUMBER = 0;
static int DELAY_DISPLAY = 100;


void zero(int NUM_SEG1, int NUM_SEG2) {
    // a b c d e f
    PORTB &= ~((1<<NUM_1) | (1<<NUM_2) | (1<<NUM_3)); // убрали питание со всех сегментов
    PORTA |= ((1<<DIOD_A) | (1<<DIOD_B) | (1<<DIOD_C) | (1<<DIOD_D) | (1<<DIOD_E) | (1<<DIOD_F) | (1<<DIOD_G)); // включили все элементы на сегменте
    PORTA &= ~((1<<DIOD_G)); // выключили элементы для формирования -> 0
    PORTB ^= (1<<NUM_SEG1); // дали питание на сегмент, чтобы погасить
    PORTB ^= (1<<NUM_SEG2); // дали питание на сегмент, чтобы погасить
    _delay_ms(DELAY_DISPLAY);
}
void one(int NUM_SEG1, int NUM_SEG2) {
    // b c
    PORTB &= ~((1<<NUM_1) | (1<<NUM_2) | (1<<NUM_3)); // убрали питание со всех сегментов
    PORTA |= ((1<<DIOD_A) | (1<<DIOD_B) | (1<<DIOD_C) | (1<<DIOD_D) | (1<<DIOD_E) | (1<<DIOD_F) | (1<<DIOD_G)); // включили все элементы на сегменте
    PORTA &= ~((1<<DIOD_A) | (1<<DIOD_F) | (1<<DIOD_G) | (1<<DIOD_E) | (1<<DIOD_D)); // выключили элементы для формирования -> 1
    PORTB ^= (1<<NUM_SEG1); // дали питание на сегмент, чтобы погасить
    PORTB ^= (1<<NUM_SEG2); // дали питание на сегмент, чтобы погасить
    _delay_ms(DELAY_DISPLAY);
}
void two(int NUM_SEG1, int NUM_SEG2) {
    // a b g e d
    PORTB &= ~((1<<NUM_1) | (1<<NUM_2) | (1<<NUM_3)); // убрали питание со всех сегментов
    PORTA |= ((1<<DIOD_A) | (1<<DIOD_B) | (1<<DIOD_C) | (1<<DIOD_D) | (1<<DIOD_E) | (1<<DIOD_F) | (1<<DIOD_G)); // включили все элементы на сегменте
    PORTA &= ~((1<<DIOD_C) | (1<<DIOD_F)); // выключили элементы для формирования -> 2
    PORTB ^= (1<<NUM_SEG1); // дали питание на сегмент, чтобы погасить
    PORTB ^= (1<<NUM_SEG2); // дали питание на сегмент, чтобы погасить
    _delay_ms(DELAY_DISPLAY);
}
void five(int NUM_SEG1, int NUM_SEG2) {
    // a f g c d
    PORTB &= ~((1<<NUM_1) | (1<<NUM_2) | (1<<NUM_3)); // убрали питание со всех сегментов
    PORTA |= ((1<<DIOD_A) | (1<<DIOD_B) | (1<<DIOD_C) | (1<<DIOD_D) | (1<<DIOD_E) | (1<<DIOD_F) | (1<<DIOD_G)); // включили все элементы на сегменте
    PORTA &= ~((1<<DIOD_B) | (1<<DIOD_E)); // выключили элементы для формирования -> 5
    PORTB ^= (1<<NUM_SEG1); // дали питание на сегмент, чтобы погасить
    PORTB ^= (1<<NUM_SEG2); // дали питание на сегмент, чтобы погасить
    _delay_ms(DELAY_DISPLAY);
}
void seven(int NUM_SEG1, int NUM_SEG2) {
    // a b c
    PORTB &= ~((1<<NUM_1) | (1<<NUM_2) | (1<<NUM_3)); // убрали питание со всех сегментов
    PORTA |= ((1<<DIOD_A) | (1<<DIOD_B) | (1<<DIOD_C) | (1<<DIOD_D) | (1<<DIOD_E) | (1<<DIOD_F) | (1<<DIOD_G)); // включили все элементы на сегменте
    PORTA &= ~((1<<DIOD_F) | (1<<DIOD_G) | (1<<DIOD_E) | (1<<DIOD_D)); // выключили элементы для формирования -> 7
    PORTB ^= (1<<NUM_SEG1); // дали питание на сегмент, чтобы погасить
    PORTB ^= (1<<NUM_SEG2); // дали питание на сегмент, чтобы погасить
    _delay_ms(DELAY_DISPLAY);
}

void control_speed() {
    switch (CURRENT_NUMBER) {
        case 0:
            zero(NUM_2, NUM_3); zero(NUM_1, NUM_3); zero(NUM_1, NUM_2);
            break;
        case 25:
            zero(NUM_2, NUM_3); two(NUM_1, NUM_3); five(NUM_1, NUM_2);
            break;
        case 50:
            zero(NUM_2, NUM_3); five(NUM_1, NUM_3); zero(NUM_1, NUM_2);
            break;
        case 75:
            zero(NUM_2, NUM_3); seven(NUM_1, NUM_3); five(NUM_1, NUM_2);
            break;
        case 100:
            one(NUM_2, NUM_3); zero(NUM_1, NUM_3); zero(NUM_1, NUM_2);
            break;
        default:
            break;
    }
}


ISR(PCINT0_vect) // кнопка на уменьшение скорости
{    
    _delay_ms(50); // антидребезг
    if ((PINA & (1<<BUTTON_MINUS)) == 0 && CURRENT_NUMBER > 0) {
        CURRENT_NUMBER -= 25;
    }
    
}

ISR(PCINT1_vect) // кнопка на увеличение скорости
{
    _delay_ms(50); // антидребезг
    if ((PINB & (1<<BUTTON_PLUS)) == 0 && CURRENT_NUMBER < 100) {
        CURRENT_NUMBER += 25;
    }
    
}

int main(void) {
    // Пин сегментов
    DDRA |= (1<<DIOD_A) | (1<<DIOD_B) | (1<<DIOD_C) | (1<<DIOD_D) | (1<<DIOD_E) | (1<<DIOD_F) | (1<<DIOD_G);
    PORTA |= ((1<<DIOD_A) | (1<<DIOD_B) | (1<<DIOD_C) | (1<<DIOD_D) | (1<<DIOD_E) | (1<<DIOD_F));

    // Пины цифр
    DDRB |= (1<<NUM_1) | (1<<NUM_2) | (1<<NUM_3);
    PORTB &= ~((1<<NUM_1) | (1<<NUM_2) | (1<<NUM_3));

    // Пин для повышения скорости
    DDRB |= (1<<BUTTON_PLUS);

    // Пин для понижения скорости
    DDRA |= (1<<BUTTON_MINUS);   

    GIMSK |= (1<<PCIE0) | (1<<PCIE1);
    PCMSK0 |= (1<<BUTTON_MINUS);
    PCMSK1 |= (1<<BUTTON_PLUS);
    sei();
    while (1) {
        control_speed();
    }
}