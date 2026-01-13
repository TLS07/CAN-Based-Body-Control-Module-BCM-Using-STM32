

#ifndef MAIN_H_
#define MAIN_H_
#include <stdint.h>
#include"stm32f1xx.h"
#include"LCD.h"
#include<string.h>

void clk_init(void);
void CAN_init(void);
void i2c2_init(void);

void lcd_display(uint8_t speed,uint8_t left,uint8_t right,uint8_t headlamp,uint8_t temp);
int CAN_receive(uint16_t *id,uint8_t *data,uint8_t *dlc);
void GPIOB_Output_Init(void);
//i2c function protypes
void i2c2_start(void);
void i2c2_send_addr(uint8_t addr,uint8_t rw);
void i2c2_send_byte(uint8_t data);
void i2c2_stop(void);


#endif /* MAIN_H_ */
