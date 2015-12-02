#include "gpio.h"
#include "datatypes.h"
#include "stm32f4xx_rcc.h"

BOOL button = FALSE;

void GpioInit()
{

	/* STM32F4 discovery LEDs */
	GPIO_InitTypeDef LED_Config;


	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
	LED_Config.GPIO_Pin = GPIO_LED_GREEN | GPIO_LED_ORANGE| GPIO_LED_BLUE| GPIO_LED_RED;
	LED_Config.GPIO_Mode = GPIO_Mode_OUT;
	LED_Config.GPIO_OType = GPIO_OType_PP;
	LED_Config.GPIO_Speed = GPIO_Speed_25MHz;
	LED_Config.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIO_PORT_LEDS, &LED_Config);

	/* STM32F4 discovery btn */
	GPIO_InitTypeDef BTN_Config;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);

	//Pin 0
	BTN_Config.GPIO_Pin = GPIO_BUTTON;
	//Mode output
	BTN_Config.GPIO_Mode = GPIO_Mode_IN;
	//Output type push-pull
	BTN_Config.GPIO_OType = GPIO_OType_PP;
	//With pull down resistor
	BTN_Config.GPIO_PuPd = GPIO_PuPd_DOWN;
	//50MHz pin speed
	BTN_Config.GPIO_Speed = GPIO_Speed_2MHz;

	//Initialize pin on GPIOA port
	GPIO_Init(GPIO_PORT_BUTTON, &BTN_Config);
}


BOOL GpioGetButtonState()
{
    BOOL retval;
    uint32_t delay;

	if (GPIO_ReadInputDataBit(GPIO_PORT_BUTTON, GPIO_BUTTON) != Bit_RESET)
	{
		for(delay = 0;delay < 150; delay++) asm volatile("nop");
		if (GPIO_ReadInputDataBit(GPIO_PORT_BUTTON, GPIO_BUTTON) != Bit_RESET)
		{
			retval = TRUE;
		}

    }
	else
    {
		//for(delay = 0;delay < 1000000; delay++) asm volatile("nop");
		if (GPIO_ReadInputDataBit(GPIO_PORT_BUTTON, GPIO_BUTTON) == Bit_RESET)
		{
			retval = FALSE;
		}
    }

	return retval;
}

BOOL GpioToggleLed(DISCOVERY_OUTPUT_PINS_T ledx)
{
	BOOL retval = TRUE;
	switch (ledx) {
		case DISCOVERY_LED_GREEN:
			if(GPIO_ReadOutputDataBit(GPIO_PORT_LEDS,GPIO_LED_GREEN) == Bit_SET)
			{
				GPIO_ResetBits(GPIO_PORT_LEDS,GPIO_LED_GREEN);
			}
			else
			{
				GPIO_SetBits(GPIO_PORT_LEDS,GPIO_LED_GREEN);
			}
			break;
		case DISCOVERY_LED_BLUE:
			if(GPIO_ReadOutputDataBit(GPIO_PORT_LEDS,GPIO_LED_BLUE) == Bit_SET)
			{
				GPIO_ResetBits(GPIO_PORT_LEDS,GPIO_LED_BLUE);
			}
			else
			{
				GPIO_SetBits(GPIO_PORT_LEDS,GPIO_LED_BLUE);
			}
			break;
		case DISCOVERY_LED_ORANGE:
			if(GPIO_ReadOutputDataBit(GPIO_PORT_LEDS,GPIO_LED_ORANGE) == Bit_SET)
			{
				GPIO_ResetBits(GPIO_PORT_LEDS,GPIO_LED_ORANGE);
			}
			else
			{
				GPIO_SetBits(GPIO_PORT_LEDS,GPIO_LED_ORANGE);
			}
			break;
		case DISCOVERY_LED_RED:
			if(GPIO_ReadOutputDataBit(GPIO_PORT_LEDS,GPIO_LED_RED) == Bit_SET)
			{
				GPIO_ResetBits(GPIO_PORT_LEDS,GPIO_LED_RED);
			}
			else
			{
				GPIO_SetBits(GPIO_PORT_LEDS,GPIO_LED_RED);
			}
			break;
		default:
			/* bad input value */
			retval = FALSE;
			break;
	}
	return retval;
}

