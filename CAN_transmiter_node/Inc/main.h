

#ifndef MAIN_H_
#define MAIN_H_

void clk_init(void);
void CAN_init(void);
void ADC1_init(void);
void GPIO_init(void);


int CAN_transmit(uint16_t std_id,uint8_t *data,uint8_t dlc);
uint16_t adc1_read_channel(uint8_t channel);
uint8_t read_switches(void);

#endif /* MAIN_H_ */
