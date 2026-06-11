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
#define  BUTTON_PIN PD2

// Servo
#define SERVO_MIN_TICKS 125
#define SERVO_CENTER_TICKS 188
#define SERVO_MAX_TICKS 250
#define SERVO_PERIOD_TICKS 2500

#define FEEDER_GATE_OPEN_ANGLE 180
#define FEEDER_CENTER_ANGLE 180
#define FEEDER_GATE_CLOSED_ANGLE 0

#endif /* CONFIG_H_ */