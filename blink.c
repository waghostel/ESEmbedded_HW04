#include <stdint.h>
#include "reg.h"
#include "blink.h"
//LED init
void led_init(unsigned int led)
{
	SET_BIT(RCC_BASE + RCC_AHB1ENR_OFFSET, GPIO_EN_BIT(GPIO_PORTD));

	//MODER led pin = 01 => General purpose output mode
	CLEAR_BIT(GPIO_BASE(GPIO_PORTD) + GPIOx_MODER_OFFSET, MODERy_1_BIT(led));
	SET_BIT(GPIO_BASE(GPIO_PORTD) + GPIOx_MODER_OFFSET, MODERy_0_BIT(led));

	//OT led pin = 0 => Output push-pull
	CLEAR_BIT(GPIO_BASE(GPIO_PORTD) + GPIOx_OTYPER_OFFSET, OTy_BIT(led));

	//OSPEEDR led pin = 00 => Low speed
	CLEAR_BIT(GPIO_BASE(GPIO_PORTD) + GPIOx_OSPEEDR_OFFSET, OSPEEDRy_1_BIT(led));
	CLEAR_BIT(GPIO_BASE(GPIO_PORTD) + GPIOx_OSPEEDR_OFFSET, OSPEEDRy_0_BIT(led));

	//PUPDR led pin = 00 => No pull-up, pull-down
	CLEAR_BIT(GPIO_BASE(GPIO_PORTD) + GPIOx_PUPDR_OFFSET, PUPDRy_1_BIT(led));
	CLEAR_BIT(GPIO_BASE(GPIO_PORTD) + GPIOx_PUPDR_OFFSET, PUPDRy_0_BIT(led));
}

//Blink LED forever
void blink(unsigned int led)
{
	led_init(led);

	unsigned int i;

	while (1)
	{
		//set GPIOD led pin
		SET_BIT(GPIO_BASE(GPIO_PORTD) + GPIOx_BSRR_OFFSET, BSy_BIT(led));

		for (i = 0; i < 100000; i++)
			;

		//reset GPIOD led pin
		SET_BIT(GPIO_BASE(GPIO_PORTD) + GPIOx_BSRR_OFFSET, BRy_BIT(led));

		for (i = 0; i < 100000; i++)
			;
	}
}

//Blink LED for count times
void blink_count(unsigned int led, unsigned int count){
	led_init(led);
	unsigned int i;

	while (count--)
	{
		//set GPIOD led pin
		SET_BIT(GPIO_BASE(GPIO_PORTD) + GPIOx_BSRR_OFFSET, BSy_BIT(led));
		for (i = 0; i < 100000; i++)
			;

		//reset GPIOD led pin
		SET_BIT(GPIO_BASE(GPIO_PORTD) + GPIOx_BSRR_OFFSET, BRy_BIT(led));
		for (i = 0; i < 100000; i++)
			;
	}
}


//Initialization the port A for button input
void btn_Init(unsigned int btn){

	//Set PORT A open		
	SET_BIT(RCC_BASE + RCC_AHB1ENR_OFFSET, GPIO_EN_BIT(GPIO_PORTA));
	
	//MODER: led pin = 00 => Input mode
	CLEAR_BIT(GPIO_BASE(GPIO_PORTD) + GPIOx_MODER_OFFSET, MODERy_1_BIT(btn));
	CLEAR_BIT(GPIO_BASE(GPIO_PORTD) + GPIOx_MODER_OFFSET, MODERy_0_BIT(btn));
	
	//PUPDR: led pin = 10=> pull-down
	SET_BIT(GPIO_BASE(GPIO_PORTD) + GPIOx_PUPDR_OFFSET, PUPDRy_1_BIT(btn));
	CLEAR_BIT(GPIO_BASE(GPIO_PORTD) + GPIOx_PUPDR_OFFSET, PUPDRy_0_BIT(btn));
}

//Get Input from button
unsigned int getInput(unsigned int btn){
	//Initialization the button
	btn_Init(BUTTON_USER);

	unsigned int countP=0;
	unsigned int countN=0;
	unsigned int curruntStatus;

	unsigned int threshold=5;
	unsigned int iteration=20;

	for(int i=0;i<iteration;i++){

		curruntStatus=READ_BIT(GPIO_BASE(GPIO_PORTA) + GPIOx_IDR_OFFSET, BGy_BIT(btn));
		
		//Count how many time signal is on
		if(curruntStatus==1)
		{
		countP++;
		}

		else{
		countN ++;
		}
	}

	//If reach the threshold, return 1(button has been clicked)
	if(countP>=countN && countP>=threshold)
		return 1;
	else
		return 0;
}
	



