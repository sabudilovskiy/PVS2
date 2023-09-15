#include "main.h"
#include <avr/interrupt.h>
#include <avr/io.h>
#include <stdio.h>
#include <string.h>
#include <util/delay.h>

unsigned short int MOTOR_STATUS =
    0; // 0 - двигатель выключен, 1 - двигатель включен
unsigned short int MOTOR_MODE = 0; // 0 - forward, 1 - reverse

unsigned short int CURRENT_SPEED = 0; // текущая скорость

const unsigned short int MIN_SPEED = 0; // минимальная скорость
const unsigned short int MAX_SPEED = 4; // максимальная скорость

#define MAX_LENGTH_COMMAND 15

char buffer_command[MAX_LENGTH_COMMAND + 1];
int cur_end = 0;

enum Command {
  start,
  stop,
  forward,
  reverse,
  zero_speed,
  first_speed,
  second_speed,
  thirst_speed,
  thourth_speed,
  fourth_speed,
  kUnknown
};

void AppendChar(char ch) {
  if (cur_end < MAX_LENGTH_COMMAND) {
    buffer_command[cur_end] = ch;
    cur_end++;
  }
}

void ClearBuffer() { cur_end = 0; }

#define COMMAND_CASE(ENUM_VALUE)                                               \
  if (strcmp(buffer_command, #ENUM_VALUE) == 0)                                \
  return ENUM_VALUE

enum Command GetCommand() {
  buffer_command[cur_end] = '\0';
  COMMAND_CASE(start);
  COMMAND_CASE(stop);
  COMMAND_CASE(forward);
  COMMAND_CASE(reverse);
  COMMAND_CASE(zero_speed);
  COMMAND_CASE(first_speed);
  COMMAND_CASE(second_speed);
  COMMAND_CASE(thirst_speed);
  COMMAND_CASE(thourth_speed);
  COMMAND_CASE(fourth_speed);
  return kUnknown;
}

void signal_to_change_speed(int CHANGE_SPEED) {
  PORTD ^= (1 << CHANGE_SPEED);
  PORTD ^= (1 << CHANGE_SPEED);
}

void switch_speed(int MOTOR_SPEED_ON, int MOTOR_SPEED_OFF1, int MOTOR_SPEED_OFF2,
                  int MOTOR_SPEED_OFF3, int MOTOR_SPEED_OFF4) {
  PORTB &= ~(1 << MOTOR_SPEED_OFF1);
  PORTB &= ~(1 << MOTOR_SPEED_OFF2);
  PORTB &= ~(1 << MOTOR_SPEED_OFF3);
  PORTB &= ~(1 << MOTOR_SPEED_OFF4);
  PORTB |= (1 << MOTOR_SPEED_ON);
}

void choose_speed(int choosen_speed, int MOTOR_SPEED_ON, int MOTOR_SPEED_OFF1,
                  int MOTOR_SPEED_OFF2, int MOTOR_SPEED_OFF3, int MOTOR_SPEED_OFF4) {
  if (CURRENT_SPEED > choosen_speed) {
    while (CURRENT_SPEED > choosen_speed) {
      signal_to_change_speed(SPEED_DOWN_OUT);
      CURRENT_SPEED -= 1;
      _delay_ms(20);
    }
    switch_speed(MOTOR_SPEED_ON, MOTOR_SPEED_OFF1, MOTOR_SPEED_OFF2,
                 MOTOR_SPEED_OFF3, MOTOR_SPEED_OFF4);
  }
  if (CURRENT_SPEED < choosen_speed) {
    while (CURRENT_SPEED < choosen_speed) {
      signal_to_change_speed(SPEED_UP_OUT);
      CURRENT_SPEED += 1;
      _delay_ms(20);
    }
    switch_speed(MOTOR_SPEED_ON, MOTOR_SPEED_OFF1, MOTOR_SPEED_OFF2,
                 MOTOR_SPEED_OFF3, MOTOR_SPEED_OFF4);
  }
}

void start_engine() {
  if (MOTOR_STATUS == 0 && MOTOR_MODE == 0) {
    PORTC ^= (1 << MOTOR_FORWARD);
    PORTB ^= (1 << MOTOR_SPEED_0);
    MOTOR_STATUS = 1;
  }
  if (MOTOR_STATUS == 0 && MOTOR_MODE == 1) {
    PORTC ^= (1 << MOTOR_REVERSE);
    PORTB ^= (1 << MOTOR_SPEED_0);
    MOTOR_STATUS = 1;
  }
}

void stop_engine() {
  if (MOTOR_STATUS == 1 && MOTOR_MODE == 0) {
    PORTC ^= (1 << MOTOR_FORWARD);
    PORTB ^= (1 << MOTOR_SPEED_0);
    MOTOR_STATUS = 0;
  }
  if (MOTOR_STATUS == 1 && MOTOR_MODE == 1) {
    PORTC ^= (1 << MOTOR_REVERSE);
    PORTB ^= (1 << MOTOR_SPEED_0);
    MOTOR_STATUS = 0;
  }
}

void SetZeroSpeed() {
  choose_speed(0, MOTOR_SPEED_0, MOTOR_SPEED_1, MOTOR_SPEED_2, MOTOR_SPEED_3,
               MOTOR_SPEED_4);
}

void SetFirstSpeed() {
  choose_speed(1, MOTOR_SPEED_1, MOTOR_SPEED_0, MOTOR_SPEED_2, MOTOR_SPEED_3,
               MOTOR_SPEED_4);
}

void SetSecondSpeed() {
  choose_speed(2, MOTOR_SPEED_2, MOTOR_SPEED_0, MOTOR_SPEED_1, MOTOR_SPEED_3,
               MOTOR_SPEED_4);
}

void SetThirstSpeed() {
  choose_speed(3, MOTOR_SPEED_3, MOTOR_SPEED_0, MOTOR_SPEED_1, MOTOR_SPEED_2,
               MOTOR_SPEED_4);
}

void SetFourthSpeed() {
  choose_speed(4, MOTOR_SPEED_4, MOTOR_SPEED_0, MOTOR_SPEED_1, MOTOR_SPEED_2,
               MOTOR_SPEED_3);
}

void SetForwardRotation() {
  if (MOTOR_STATUS == 0 && MOTOR_MODE == 1) {
    MOTOR_MODE = 0;
  }
  if (MOTOR_STATUS == 1 && MOTOR_MODE == 1) {
    PORTC &= ~(1 << MOTOR_REVERSE);
    PORTC |= (1 << MOTOR_FORWARD);
    MOTOR_MODE = 0;
  }
}

void SetReverseRotation() {
  if (MOTOR_STATUS == 0 && MOTOR_MODE == 0) {
    MOTOR_MODE = 1;
  }
  if (MOTOR_STATUS == 1 && MOTOR_MODE == 0) {
    PORTC &= ~(1 << MOTOR_FORWARD);
    PORTC |= (1 << MOTOR_REVERSE);
    MOTOR_MODE = 1;
  }
}

ISR(USART_RXC_vect) {
  // Считываем данные из регистра приема данных
  char data = UDR;
  if (data != ';') {
    AppendChar(data);
    return;
  }
  enum Command command = GetCommand();
  ClearBuffer();
  switch (command) {
  case start:
    // старт
    start_engine();
    break;
  case stop:
    // стоп
    SetZeroSpeed();
    stop_engine();
    break;
  case zero_speed:
    SetZeroSpeed();
    break;
  case first_speed:
    SetFirstSpeed();
    break;
  case second_speed:
    SetSecondSpeed();
    break;
  case thirst_speed:
    SetThirstSpeed();
    break;
  case fourth_speed:
    SetFourthSpeed();
    break;
  case forward:
    SetForwardRotation();
    break;
  case reverse:
    SetReverseRotation();
    break;

  default:
    break;
  }
}

void setup_engine() {
  // Пины двигателя
  DDRC |= (1 << MOTOR_FORWARD) | (1 << MOTOR_REVERSE);     // входы
  PORTC &= ~((1 << MOTOR_FORWARD) | (1 << MOTOR_REVERSE)); // выключен
}

void setup_speedometer() {
  // Пины на скорость
  DDRB |= (1 << MOTOR_SPEED_0) | (1 << MOTOR_SPEED_1) | (1 << MOTOR_SPEED_2) |
          (1 << MOTOR_SPEED_3) | (1 << MOTOR_SPEED_4); // выход
  PORTB &=
      ~((1 << MOTOR_SPEED_0) | (1 << MOTOR_SPEED_1) | (1 << MOTOR_SPEED_2) |
        (1 << MOTOR_SPEED_3) | (1 << MOTOR_SPEED_4)); // выключен

  // Пин для отображения скорости
  DDRD |= (1 << SPEED_UP_OUT) | (1 << SPEED_DOWN_OUT);     // выход
  PORTD &= ~((1 << SPEED_UP_OUT) | (1 << SPEED_DOWN_OUT)); // выключен
}

// настройка UART
void setup_usart() {
  // Настраиваем скорость передачи данных
  UBRRH = (uint8_t)(UBRRVAL >> 8);
  UBRRL = (uint8_t)UBRRVAL;
  // Разрешаем прием, передачу, прерывания от приемника USART
  UCSRB = (1 << RXEN) | (1 << TXEN) | (1 << RXCIE);
  UCSRC = (1 << UCSZ0) | (1 << UCSZ1);
}

int main(void) {
  setup_engine();
  setup_speedometer();
  setup_usart();

  // Разрешаем глобальные прерывания
  sei();

  while (1) {
  }
}