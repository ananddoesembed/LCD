/*
 * lcd.h
 *
 *  Created on: 13-Feb-2018
 *
 */

#ifndef LCD_H_
#define LCD_H_

#ifndef __LCD_H
#define __LCD_H
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include "stm32f1xx_hal.h"

#define LCD_DB4 GPIO_PIN_12
#define LCD_DB5 GPIO_PIN_13
#define LCD_DB6 GPIO_PIN_14
#define LCD_DB7 GPIO_PIN_15

#define LCD_RS GPIO_PIN_7
//#define LCD_RW GPIO_Pin_8
#define LCD_EN GPIO_PIN_6



#define CMDSEL 0
#define DATASEL 1

void lcdInitCmd(void);
void lcdWrite(uint8_t, uint8_t);
void putLcdData(uint8_t);
void putLcdCmd(uint8_t);
void putLcdString(char *);
char *convert(unsigned int num, int base);
void Myprintf(char* format,...);
void delay(uint32_t x);
void xypos(char x, char y);
#endif




#endif /* LCD_H_ */
