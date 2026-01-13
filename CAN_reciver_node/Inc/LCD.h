/*
 * LCD.h
 *
 *  Created on: Jan 3, 2026
 *      Author: Admin
 */

#ifndef LCD_H_
#define LCD_H_

#include <stdint.h>


#define INST   0   // RS = 0 → Instruction
#define DATA   1   // RS = 1 → Data
#define PCF8574_ADDR   0x27    //I2c module address,

/* PCF8574 → LCD pin mapping */
#define LCD_EN         (1 << 5)  //enable
#define LCD_BL         (1 << 4)  // Blacklight

//funciton protypes
/* Initialize LCD in 4-bit mode */
void lcd_init(void);
void lcd_send_cmd(uint8_t cmd);
void lcd_send_data(uint8_t data);
void lcd_send_byte(uint8_t byte, uint8_t rs);
void lcd_send_nibble(uint8_t nibble, uint8_t rs);
void lcd_pulse_en(uint8_t data);
void lcd_display_rpm(uint32_t rpm,uint32_t target);
void lcd_print_string(const char* str);
void lcd_print_char(char ch);
void lcd_set_cursor(uint8_t row,uint8_t col);
void lcd_clear(void);

#endif /* LCD_H_ */





