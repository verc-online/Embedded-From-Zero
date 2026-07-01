/*
 * config.h
 *
 * Created: 10.06.2026 11:27:12
 *  Author: HP
 */ 


#ifndef CONFIG_H_ // Защищает от ситуации, когда файл подключается несколько раз
#define CONFIG_H_

#define F_CPU 1000000UL

#define LED_PIN PB0

// Timer
#define TIMER_TICKS_PER_SECOND 61UL
#define MENU_MAIN_REFRESH_TICKS TIMER_TICKS_PER_SECOND


// Servo
#define SERVO_MIN_TICKS 125
#define SERVO_CENTER_TICKS 188
#define SERVO_MAX_TICKS 250
#define SERVO_PERIOD_TICKS 2500

#define FEEDER_GATE_OPEN_ANGLE 180
#define FEEDER_CENTER_ANGLE 180
#define FEEDER_GATE_CLOSED_ANGLE 0

// У контроллера есть два регистра:
// IR — Instruction Register
// DR — Data Register
//
// Выбор делается через RS:
// RS = 0 ? команда / instruction
// RS = 1 ? данные / символ// Например:
// RS=0 ? очистить экран, поставить курсор
// RS=1 ? вывести букву 'A'
#define LCD_RS_PORT PORTB
#define LCD_RS_DDR  DDRB
#define LCD_RS_PIN  PB2

// E — это строб. LCD считывает данные, когда ты делаешь импульс на E
#define LCD_E_PORT  PORTB
#define LCD_E_DDR   DDRB
#define LCD_E_PIN   PB3

#define LCD_D4_PORT PORTB
#define LCD_D4_DDR  DDRB
#define LCD_D4_PIN  PB4

#define LCD_D5_PORT PORTB
#define LCD_D5_DDR  DDRB
#define LCD_D5_PIN  PB5

#define LCD_D6_PORT PORTB
#define LCD_D6_DDR  DDRB
#define LCD_D6_PIN  PB6

// Busy Flag на D7 читается так:
//
// RS = 0
// RW = 1
// чтение
// DB7 = BF
//
// Если:
// DB7 = 1 ? LCD занят
// DB7 = 0 ? можно слать следующую команду
//
// Но для этого нужно подключать RW к микроконтроллеру и уметь переключать DB4–DB7 между выходом и входом// Для первого драйвера мы сделаем проще:
//
// RW подключаем к GND
// То есть LCD всегда в режиме записи.
// Тогда Busy Flag мы не читаем, а просто ждём задержками:
// после обычной команды ~40 мкс
// после clear/home ~2 мс
#define LCD_D7_PORT PORTB
#define LCD_D7_DDR  DDRB
#define LCD_D7_PIN  PB7

#define BUTTON_OK_PORT    PORTD
#define BUTTON_OK_PINREG  PIND
#define BUTTON_OK_DDR     DDRD
#define BUTTON_OK_PIN     PD2

#define BUTTON_UP_PORT PORTD
#define BUTTON_UP_PINREG PIND
#define BUTTON_UP_DDR DDRD
#define BUTTON_UP_PIN PD3

#define BUTTON_DOWN_PORT PORTD
#define BUTTON_DOWN_PINREG PIND
#define BUTTON_DOWN_DDR DDRD
#define BUTTON_DOWN_PIN PD4

#endif /* CONFIG_H_ */