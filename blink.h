#ifndef BLINK_H
#define BLINK_H
#include "reg.h"

#define LED_GREEN 12
#define LED_ORANGE 13
#define LED_RED 14
#define LED_BLUE 15
#define BUTTON_USER 0

void blink(unsigned int led);
void blink_count(unsigned int led, unsigned int count);
unsigned int getInput(unsigned int btn);
void btn_Init(unsigned int btn);
#endif