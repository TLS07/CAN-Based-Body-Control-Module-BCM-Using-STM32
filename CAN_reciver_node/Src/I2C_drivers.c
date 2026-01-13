/*
 * I2C_drivers.c
 *
 *  Created on: Jan 2, 2026
 *      Author: Admin
 */
/*
 * IMPORTANT to rember
 * START  → SB set → clear by DR write
 * ADDR   → ADDR set → clear by SR1 + SR2 read
 * TXE    → write data
 * BTF    → write next data / STOP
 * STOP   → bus released
*/
#include "main.h"

void i2c2_start(void)
{
	// 1. Wait until I2C bus is free
	while (I2C2->SR2 & I2C_SR2_BUSY);

	//2. Generate START condition
	I2C2->CR1 |= I2C_CR1_START;

	// 3. Wait for START condition generated (SB flag set) */
	while (!(I2C2->SR1 & I2C_SR1_SB));

	// SB flag will be clearing while sending address


}
void i2c2_send_addr(uint8_t addr,uint8_t rw)
{
	/*
	 * address 7bits
	 * rw 0/1 , 0=>write,1=>read
	 */
	I2C2->DR=(addr<<1)|rw ;   //sending slave address , also clears SR1 flags
	while(!(I2C2->SR1 & I2C_SR1_ADDR)); //wait till address is tranfered

	//to clear SR1, SR2 flag
	volatile uint32_t temp;
	temp = I2C2->SR1;
	temp = I2C2->SR2;
}

void i2c2_send_byte(uint8_t data)
{
	// 1. Wait until DR is empty (TXE set)
	while (!(I2C2->SR1 & I2C_SR1_TXE));

	// 2. Write data to DR
	I2C2->DR = data;

	// 3. Wait until byte transfer finished (BTF set)
	while (!(I2C2->SR1 & I2C_SR1_BTF));
}

void i2c2_stop(void)
{
    // 1. Generate STOP condition
    I2C2->CR1 |= I2C_CR1_STOP;

}
