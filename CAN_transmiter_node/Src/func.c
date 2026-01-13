
#include<stm32f1xx.h>
 int CAN_transmit(uint16_t std_id,uint8_t *data,uint8_t dlc)
 {
	 uint8_t tx_mailbox;

	 // finding the empty mailbox
	  if (CAN1->TSR & CAN_TSR_TME0)       //  mailbox 0 empty
	        tx_mailbox = 0;
	  else if (CAN1->TSR & CAN_TSR_TME1)  // mailbox 1 empty
	        tx_mailbox = 1;
	  else if (CAN1->TSR & CAN_TSR_TME2)  // mailbox 2 empty
	        tx_mailbox = 2;
	  else
	        return -1;                       // None are free


	  // loading id
	  /*
	   * id 11 bits [31:21]
	   * IDE=0 standard
	   * RTR=0 ddata frame
	   */
	  CAN1->sTxMailBox[tx_mailbox].TIR=(std_id<<21);

	  //setting CAN data lenght
	  /*
	   * DLC 4 bits => o bytes to 8 bytes
	   */
	  CAN1->sTxMailBox[tx_mailbox].TDTR=dlc & 0x0F;

	  // First 4 bytes go to TDLR
	  // entering data bytes to data low register
	  CAN1->sTxMailBox[tx_mailbox].TDLR=  (data[0])
										  |(data[1] << 8)
										  |(data[2] << 16);

	  // data trnasmit request
	  CAN1->sTxMailBox[tx_mailbox].TIR |= CAN_TI0R_TXRQ;
	  return 0; // frame in queue for transmsiion
 }

 uint16_t adc1_read_channel(uint8_t channel)
 {
     ADC1->SQR3 = channel;             // Select channel (0 = PA0, 1 = PA1)
     ADC1->CR2 |= ADC_CR2_SWSTART;     // Start conversion
     while (!(ADC1->SR & ADC_SR_EOC)); // Wait for end
     return (uint16_t)ADC1->DR;
 }
 uint8_t read_switches(void)
 {
     // Mask PA2-PA4 and shift down to bit 0-2
     return (~(GPIOA->IDR >> 2)) & 0x07;  // active-low switches
 }
