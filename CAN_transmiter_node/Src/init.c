

#include<stm32f1xx.h>

//clock configuration
void clk_init(void)
{
	//step 1 choosing clock source
	RCC->CR|=RCC_CR_HSEON;    //choosing hse
	while(!(RCC->CR & RCC_CR_HSERDY));

	//step 2 configure flash prefetch and latency
	FLASH->ACR|=FLASH_ACR_PRFTBE|FLASH_ACR_LATENCY_2;

	//step 3 configurinh PLL
	RCC->CFGR|=RCC_CFGR_PLLSRC;            //hse as pll source
	RCC->CFGR&=~RCC_CFGR_PLLXTPRE_HSE;     //hse before pll not divided
	RCC->CFGR|=RCC_CFGR_PLLMULL9;         //pll multipiler

	// configuring speed of AHB,APB1,APB2
	RCC->CFGR|=RCC_CFGR_HPRE_DIV1;       //AHB =SYSCLK(72)
	RCC->CFGR|=RCC_CFGR_PPRE2_DIV1;      //APB2=72mhz
	RCC->CFGR|=RCC_CFGR_PPRE1_DIV2;      //APB1=36mhz


	//step 4 enable PLL and wait to get ready
	RCC->CR|=RCC_CR_PLLON;   //enable the PLL
	while(!(RCC->CR & RCC_CR_PLLRDY));


	//step 5 enabling the systems clock
	RCC->CFGR|=RCC_CFGR_SW_PLL;
	while(!(RCC->CFGR & RCC_CFGR_SWS_PLL));
}


void CAN_init(void)
{
	/*PA11 => CAN_RX  (input pull -up
	 * PA12 =>CAN_TX   (AF push pull)
	*/

	//step 1 enable clocks
	  RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;   // GPIOA
	  RCC->APB1ENR |= RCC_APB1ENR_CAN1EN;   // CAN1

	//configure PA11  RX
	  GPIOA->CRH &= ~(GPIO_CRH_MODE11 | GPIO_CRH_CNF11);
	  GPIOA->CRH |=  GPIO_CRH_CNF11_1;      // Input pull-up/down
	  GPIOA->ODR |=  GPIO_ODR_ODR11;        // Pull-up

	// configure PA12 TX AF push pull 50mhz
	  GPIOA->CRH &= ~(GPIO_CRH_MODE12 | GPIO_CRH_CNF12);
	  GPIOA->CRH |=  (GPIO_CRH_MODE12 | GPIO_CRH_CNF12_1);

	// CAN init mode
	  CAN1->MCR |= CAN_MCR_INRQ;            //request for initilization mode
	  while (!(CAN1->MSR & CAN_MSR_INAK));   //wait till ack by hardware

	  //CAN bit timing (500 Kbps , APB1 =36 MHZ)
	  /*
	   * CAN bit rate= (CAN_clk)/(BRP+1)*(1+TS1+TS2)
	   * Time quanta for stablity 18=1+TS1+TS2
	   * sampling point  (1+TS1)/1+TS1+TS2 =13/18
	   */
	  CAN1->BTR|=(3  << CAN_BTR_BRP_Pos);     // BRP = 4
	  CAN1->BTR|=(12 << CAN_BTR_TS1_Pos);   // TS1 = 13 tq
	  CAN1->BTR|=(5  << CAN_BTR_TS2_Pos);     // TS2 = 6 tq



	  /* 5. Configure filter 0 (MASK MODE) */
	  CAN1->FMR |= CAN_FMR_FINIT;

	  CAN1->FA1R &= ~CAN_FA1R_FACT0;   // Disable filter 0  (CAN_filter_activation)
	  CAN1->FM1R &= ~CAN_FM1R_FBM0;    // Mask mode         (CAN_FILTER_MODE_REGISTER)
	  CAN1->FS1R |=  CAN_FS1R_FSC0;    // 32-bit            (CAN_FILTER_SCALE_REG
	  CAN1->FFA1R &= ~CAN_FFA1R_FFA0;  //filter assigned to  FIFO 0 (CAN_FILTER_FIFO assignment register)

	  CAN1->sFilterRegister[0].FR1 = (0x100 << 21);  // ID  (21 bcz 32-11)
	  CAN1->sFilterRegister[0].FR2 = (0x7FC << 21);  // MASK

	  CAN1->FA1R |= CAN_FA1R_FACT0;    // enable filter
	  CAN1->FMR &= ~CAN_FMR_FINIT;      //exit filter init

	  /* comming out of init mode to normal mode*/
	  CAN1->MCR &= ~CAN_MCR_INRQ;
	  while (CAN1->MSR & CAN_MSR_INAK);

}
void ADC1_init(void)
{
	// enable clock
	 RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;   // Enable GPIOA clock
	 RCC->APB2ENR |= RCC_APB2ENR_ADC1EN;   // Enable ADC1 clock

	 //Configure PA0 and PA1 as analog inputs
	  GPIOA->CRL &= ~(GPIO_CRL_MODE0 | GPIO_CRL_CNF0);  // PA0
	  GPIOA->CRL &= ~(GPIO_CRL_MODE1 | GPIO_CRL_CNF1);  //PA1


	  ADC1->CR2 |= ADC_CR2_ADON;           //power on module
	  for (volatile int i=0; i<1000; i++);
	  ADC1->CR2 |= ADC_CR2_CAL;            // adc calibration //
	  while (ADC1->CR2 & ADC_CR2_CAL);

	  ADC1->CR1 = 0;       //  no scan , we will slect the channel
	  ADC1->SQR1 = 0;      // one channel conversion
	  ADC1->SMPR2 = 0;     // 1.5 cycles for channel 0–9 (PA0 = CH0, PA1 = CH1)

	  // Enable ADC
	  ADC1->CR2 |= ADC_CR2_ADON;

}

void GPIO_init(void)
{
	// enable  clock for PORTA
	RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;

	/*PA2 left indicator, PA3 right indicator, Pa4 head lamp
	 * configure as input pull up
	 */
	 GPIOA->CRL &= ~(GPIO_CRL_MODE2 | GPIO_CRL_CNF2);
	 GPIOA->CRL &= ~(GPIO_CRL_MODE3 | GPIO_CRL_CNF3);
	 GPIOA->CRL &= ~(GPIO_CRL_MODE4 | GPIO_CRL_CNF4);

	 // Set CNF = Input Pull-up / Pull-down (10), MODE = 00 (Input)
	 GPIOA->CRL |=  (0x2 << GPIO_CRL_CNF2_Pos);  // PA2 input pull-up
	 GPIOA->CRL |=  (0x2 << GPIO_CRL_CNF3_Pos);  // PA3 input pull-up
	 GPIOA->CRL |=  (0x2 << GPIO_CRL_CNF4_Pos);  // PA4 input pull-up

	 // Enable pull-ups
	 GPIOA->ODR |= (1 << 2) | (1 << 3) | (1 << 4);
}
