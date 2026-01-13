
#include"stm32f1xx.h"

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
	// Enable clocks
	RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;   // GPIOA
	RCC->APB1ENR |= RCC_APB1ENR_CAN1EN;   // CAN1

	//configure PA11  RX
	  GPIOA->CRH &= ~(GPIO_CRH_MODE11 | GPIO_CRH_CNF11);
	  GPIOA->CRH |=  GPIO_CRH_CNF11_1;      // Input pull-up/down
	  GPIOA->ODR |=  GPIO_ODR_ODR11;        // Pull-up

	// configure PA12 TX AF push pull 50mhz
	  GPIOA->CRH &= ~(GPIO_CRH_MODE12 | GPIO_CRH_CNF12);
	  GPIOA->CRH |=  (GPIO_CRH_MODE12 | GPIO_CRH_CNF12_1);


	  // ENter CAN init mode
	  CAN1->MCR |= CAN_MCR_INRQ;
	  while (!(CAN1->MSR & CAN_MSR_INAK));

	  //CAN bit timing (500 Kbps , APB1 =36 MHZ)
	  /*
	   * CAN bit rate= (CAN_clk)/(BRP+1)*(1+TS1+TS2)
	   * Time quanta for stablity 18=1+TS1+TS2
	   * sampling point  (1+TS1)/1+TS1+TS2 =13/18
	   */
	  CAN1->BTR|=(3  << CAN_BTR_BRP_Pos);     // BRP = 4
	  CAN1->BTR|=(12 << CAN_BTR_TS1_Pos);   // TS1 = 13 tq
	  CAN1->BTR|=(5  << CAN_BTR_TS2_Pos);     // TS2 = 6 tq

	  //configuring filter 0(accepts only ID 0x100)
	  CAN1->FMR |= CAN_FMR_FINIT;          // Filter init mode
	  CAN1->FA1R &= ~CAN_FA1R_FACT0;       // Disable filter 0
	  CAN1->FM1R &= ~CAN_FM1R_FBM0;        //  mode =>mask mode
	  CAN1->FS1R |= CAN_FS1R_FSC0;         // 32-bit scale
	  CAN1->FFA1R &= ~CAN_FFA1R_FFA0;      // FIFO 0

	  CAN1->sFilterRegister[0].FR1 = (0x100 << 21);    // ID
	  CAN1->sFilterRegister[0].FR2 = 0xFFFFFFFF;       // MASK = all bits must match

	  CAN1->FA1R |= CAN_FA1R_FACT0;        // Enable filter 0
	  CAN1->FMR &= ~CAN_FMR_FINIT;         // Exit filter init

	  // Exit init mode → normal mode
	  CAN1->MCR &= ~CAN_MCR_INRQ;
	  while (CAN1->MSR & CAN_MSR_INAK);

}



void i2c2_init(void)
{
	/*using I2c2 PB10 SCL, PB11 SDA
	 * SYSCLK=72 mhz
	 * APB1=36mhz
	 * I2c  speed =100khz
	 */

	//step 1 enabling clock for GPIOA and I2c2
	RCC->APB2ENR|=RCC_APB2ENR_IOPBEN;
	RCC->APB1ENR|=RCC_APB1ENR_I2C2EN;

	//STEP 2 configuring PB10 PB11 as alternate function and open drain
    /* Clear configuration first */
    GPIOB->CRH &= ~(GPIO_CRH_MODE10 | GPIO_CRH_CNF10 |
                    GPIO_CRH_MODE11 | GPIO_CRH_CNF11);

    /* MODE = 11 (50 MHz), CNF = 11 (AF Open-Drain) */
	GPIOB->CRH|=GPIO_CRH_MODE10_0|GPIO_CRH_MODE10_1;
	GPIOB->CRH|=GPIO_CRH_CNF10_1|GPIO_CRH_CNF10_0;

	GPIOB->CRH|=GPIO_CRH_MODE11_0|GPIO_CRH_MODE11_1;
	GPIOB->CRH|=GPIO_CRH_CNF11_1|GPIO_CRH_CNF11_0;

	/* step 3. Reset I2C2 */
	I2C2->CR1 |=  I2C_CR1_SWRST;
	I2C2->CR1 &= ~I2C_CR1_SWRST;

	//step  4 Set APB1 frequency (in MHz)
	I2C2->CR2=36;

	//  step 5. Configure clock control for 100 kHz
	//CCR=F_PCLK1/2*F_SCL
	I2C2->CCR=0X140;

	// step 6. Configure maximum rise time
	//TRISE=FPCLK1​(MHz)+1
	I2C2->TRISE = 37;

    //step 7. Enable I2C2
    I2C2->CR1 |= I2C_CR1_PE;

}

void GPIOB_Output_Init(void)
{
    // 1. Enable GPIOB clock
    RCC->APB2ENR |= RCC_APB2ENR_IOPBEN;

    // 2. Clear configuration for PB0, PB1, PB2
    GPIOB->CRL &= ~(
        GPIO_CRL_MODE0 | GPIO_CRL_CNF0 |
        GPIO_CRL_MODE1 | GPIO_CRL_CNF1 |
        GPIO_CRL_MODE2 | GPIO_CRL_CNF2
    );

    //. Configure as output, push-pull, 2 MHz
    GPIOB->CRL |= (
        (0x2 << GPIO_CRL_MODE0_Pos) |   // PB0
        (0x2 << GPIO_CRL_MODE1_Pos) |   // PB1
        (0x2 << GPIO_CRL_MODE2_Pos)     // PB2
    );

    //CNF = 00 → General purpose push-pull

    // Default OFF state */
    GPIOB->BSRR = (GPIO_BSRR_BR0 |GPIO_BSRR_BR1 |GPIO_BSRR_BR2);
}


















