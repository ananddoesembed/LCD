/*
 * lcd.c
 *
 *  Created on: 13-Feb-2018
 *
 */
#include "main.h"
#include "stm32f1xx_hal.h"
#include "lcd.h"
#include "Timer.h"
#include "string.h"
void lcdInitCmd(void)
{
	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_9,GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_8,GPIO_PIN_SET);
	//////////////////
	putLcdCmd(0x33);									//only msb is considered so 3332 is equavalent to 30 30 30 20
	putLcdCmd(0x32);
	putLcdCmd(0x28);							// this is to ensure that 4 bit mode is selected
	////////////////////////////
	putLcdCmd(0x02);
	putLcdCmd(0x01);
	putLcdCmd(0x0F);
	putLcdCmd(0x80);

}


void lcdWrite(uint8_t reg, uint8_t data)//this funtion cannot be used in main() in 4 bit mode(it does not split data to nibbles)
{
	if(reg == 0)
		HAL_GPIO_WritePin(GPIOC, LCD_RS, GPIO_PIN_RESET);

	else if(reg == 1)
		HAL_GPIO_WritePin(GPIOC, LCD_RS, GPIO_PIN_SET);

	HAL_GPIO_WritePin(GPIOC,LCD_EN,GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_12,(data& 0x01));
	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_13,(data& 0x02));
	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_14,(data& 0x04));
	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_15,(data& 0x08));
	timer_sleep(5);
	HAL_GPIO_WritePin(GPIOC,LCD_EN,GPIO_PIN_RESET);
}


void putLcdData(uint8_t data)
 {
	   lcdWrite(DATASEL,(data >> 0x04) & 0x0F);		//Send higher nibble
	   timer_sleep(5);
	   lcdWrite(DATASEL,(data & 0x0F));		        //Send Lower nibble

 }

void putLcdCmd(uint8_t cmd)
{

	lcdWrite(CMDSEL,(cmd >> 0x04) & 0x0F);		//Send higher nibble
	timer_sleep(5);
	lcdWrite(CMDSEL,(cmd & 0x0F));		//Send Lower nibble

}

void putLcdString(char *ptr)
 {
	 while((*ptr) != '\0')
    putLcdData(*ptr++);
 }

void Myprintf(char* format,...)
{
	char *traverse;
	signed int i;
	char *s;
	uint8_t floater[16];

	//Module 1: Initializing Myprintf's arguments
	va_list arg;
	va_start(arg, format);

	for(traverse = format; *traverse != '\0';traverse++)
	{
		if( *traverse != '%' )
		{
			putLcdData(*traverse);
		}
		else
		{
		traverse++;

			//Module 2: Fetching and executing arguments
			switch(*traverse)
			{
				case 'c' : i = va_arg(arg,int);		//Fetch char argument
							putLcdData(i);
							break;

				case 'd' : i = va_arg(arg,int); 		//Fetch Decimal/Integer argument
							if(i<0)
							{
								i = -i;
								putLcdData('-');
							}
							putLcdString(convert(i,10));
							break;

				case 'o': i = va_arg(arg,unsigned int); //Fetch Octal representation
							putLcdString((char *)convert(i,8));
							break;

				case 's': s = va_arg(arg,char *); 		//Fetch string
							putLcdString((char *)s);
							break;

				case 'x': i = va_arg(arg,unsigned int); //Fetch Hexadecimal representation
							putLcdString((char *)convert(i,16));
							break;

				case 'f': /*s = va_arg(arg,char *);*/
							sprintf((char *)floater,"%ld",s);
							putLcdString((char *)floater);
							break;

			}
	  }
	}

	//Module 3: Closing argument list to necessary clean-up
	va_end(arg);
}

char *convert(unsigned int num, int base)
{
	static char Representation[]= "0123456789ABCDEF";
	static char buffer[50];
	char *ptr;

	ptr = &buffer[49];
	*ptr = '\0';

	do
	{
		*--ptr = Representation[num%base];
		num /= base;
	}while(num != 0);

	return(ptr);
}

void xypos(char x, char y)
{
	if(x==0)
		putLcdCmd(0x80+(y));
	else if(x==1)
	 putLcdCmd(0xC0+(y));

}












