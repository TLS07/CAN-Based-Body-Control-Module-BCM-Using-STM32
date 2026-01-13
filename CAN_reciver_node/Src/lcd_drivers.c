/*
 * lcd_drivers.c
 *
 *  Created on: Jan 2, 2026
 *      Author: Admin
 */

/*
 * LCD driver for LCD->HD44780(4bit mode) using i2c lcd module(PCF8574)
 * This driver interfaces an HD44780-compatible character LCD using
 * a PCF8574 I2C I/O expander. Since the LCD is parallel and the MCU
 * uses I2C, the PCF8574 acts as a bridge between the two
 *
 * Typical PCF8574 → LCD mapping:
 *   P0–P3 : LCD D4–D7 (data lines)
 *   P4    : Backlight control
 *   P5    : EN (Enable)
 *   P7    : RS (Register Select)
 *
 * Reading and writing to lcd
 * R/w =>1 reading
 * R/w =>0 writing , we use in writing mode (GND pin)
 *
 *
 * Lcd operating modes
 * RS=0 =>instruction
 * RS=1 =>data
 *
 * data transfer
 * data is tranfsered in two steps
 * 1) High nibble(D7-D4)
 * 2) Low nibble (D3-D0)
 */
#include "main.h"
//#include"LCD.h"

//lcd intialsiation
void lcd_init(void)
{
	/* for step of initialsation refer datasheet of lcd hd44780*/

	 // 1. Wait after power on 15ms
	for (volatile int i = 0; i < 50000; i++);

	// 2. 4-bit initialization sequence
	lcd_send_nibble(0x03, INST);
	for (volatile int i = 0; i < 10000; i++);
	lcd_send_nibble(0x03, INST);
	for (volatile int i = 0; i < 1000; i++);
	lcd_send_nibble(0x03, INST);
	lcd_send_nibble(0x02, INST); // 4-bit mode

	// 3. Function set: 4-bit, 2 lines, 5x8 dots
	lcd_send_cmd(0x28);

	// 4. Display ON, cursor OFF
	lcd_send_cmd(0x0C);

	// 5. Entry mode: increment, no shift
	lcd_send_cmd(0x06);

	// 6. Clear display
	lcd_send_cmd(0x01);
	for (volatile int i = 0; i < 5000; i++);

}
// to send instruction
void lcd_send_cmd(uint8_t cmd)
{
	lcd_send_byte(cmd,INST);   //RS =0 for command
}
// to send data
void lcd_send_data(uint8_t data)
{
	lcd_send_byte(data,DATA); //RS=1 for data
}

void lcd_send_byte(uint8_t byte,uint8_t rs)
{
	lcd_send_nibble(byte>>4,rs); // high nibble
	lcd_send_nibble(byte & 0x0F,rs); //low nibble
}

// to send 4 bits
void lcd_send_nibble(uint8_t nibble,uint8_t rs)
{
	uint8_t data=(nibble & 0x0F);   //P0-P3
	if(rs)data|=(1<<7);    //RS=>Pin 7
	lcd_pulse_en(data);
}
//to clear the display
void lcd_clear(void)
{
    lcd_send_cmd(0x01);                 // Clear display command
    for (volatile int i = 0; i < 5000; i++);
}

//to send data
void lcd_pulse_en(uint8_t data)
{
	i2c2_start();                                 //start i2c
    i2c2_send_addr(PCF8574_ADDR, 0);              // send PCF8574 address
    i2c2_send_byte((data | LCD_EN) |LCD_BL);                // EN high
    for(volatile unsigned short i=0;i<500;i++);   //small delay
    i2c2_send_byte((data & ~LCD_EN) |LCD_BL);               // EN low
    for(volatile unsigned short i=0;i<500;i++);   //small delay
    i2c2_stop();

}

// main user functions
//to set cursor on lcd
void lcd_set_cursor(uint8_t row,uint8_t col)
{
	uint8_t address;
	if(row==0)        //first row
	{
		address=0x80+col;
	}
	else if(row==1)  // second row
	{
		address=0xC0 +col;
	}

	lcd_send_cmd(address);
}

//to print charecter
void lcd_print_char(char ch)
{
	lcd_send_data((uint8_t)ch);
}

// to print string
void lcd_print_string(const char* str)
{
	while(*str!='\0')
	{
		lcd_send_data(*str++);
	}

}

