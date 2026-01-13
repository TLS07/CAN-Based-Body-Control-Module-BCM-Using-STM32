
#include "main.h"
extern unsigned char lcd_buff1[17];
extern unsigned char lcd_buff2[17];
int CAN_receive(uint16_t *id,uint8_t *data,uint8_t *dlc)
{

	//checking if any data present in FIFO 0
	if(CAN1->RF0R & CAN_RF0R_FMP0)
	{
		uint32_t rec=CAN1->sFIFOMailBox[0].RIR;

		//extracting ID
		*id=(rec>>21)& 0x7FF;

		//data lenth
		*dlc=CAN1->sFIFOMailBox[0].RDTR & 0x0F;

		//reading data bytes only 4 bytes
		uint32_t tdlr=CAN1->sFIFOMailBox[0].RDLR;

		for(unsigned char i=0;i<dlc && i<4; i++)
		{
			data[i]=(tdlr>>(8*i)) & 0xFF;
		}

		// Release FIFO 0 output mailbox
		CAN1->RF0R |= CAN_RF0R_RFOM0;

		return 0; // Success
	}
	return -1; // No message
}
//to display sytem parameters
void lcd_display(uint8_t speed, uint8_t left,uint8_t right,uint8_t headlamp,uint8_t temp)
{
    lcd_clear();

    /* First line */
    lcd_set_cursor(0, 0);
    sprintf((char *)lcd_buff1, "SPD:%3d TMP:%2d", speed, temp);
    lcd_print_string(lcd_buff1);

    /* Second line */
    lcd_set_cursor(1, 0);
    sprintf((char *)lcd_buff2,
            "L:%d R:%d HL:%d",
            left, right, headlamp);
    lcd_print_string(lcd_buff2);
}

