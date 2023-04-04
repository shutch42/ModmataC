#include <ModmataC.h>
#include <math.h>
#include <string.h>
#include "signal.h"

#define THERMO_ADDR 0x4D 

#define IRQ 8
#define CE 9
#define CS 12

#define LCD_SETCURSOR 20
#define LCD_PRINT 21
#define LCD_CLEAR 22

void* blink();
void* button();
void* digital_analog();
void* fade();
void* servo(void* angle);
void* servo_control();
void* temperature();
void* radio(void* str);
void* lcd(void* str);

