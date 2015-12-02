#ifndef GPIO_H
#define GPIO_H

#include "stm32f4xx_conf.h"
#include "stm32f4xx.h"
#include "stm32f4xx_gpio.h"
#include "datatypes.h"


#define GPIO_LED_GREEN 		GPIO_Pin_12
#define GPIO_LED_ORANGE		GPIO_Pin_13
#define GPIO_LED_BLUE		GPIO_Pin_15
#define GPIO_LED_RED		GPIO_Pin_14
#define	GPIO_BUTTON			GPIO_Pin_0
#define GPIO_PORT_LEDS		GPIOD
#define GPIO_PORT_BUTTON	GPIOA

typedef enum DISCOVERY_OUTPUT_PINS_
{
	DISCOVERY_LED_GREEN,
	DISCOVERY_LED_ORANGE,
	DISCOVERY_LED_RED,
	DISCOVERY_LED_BLUE
}DISCOVERY_OUTPUT_PINS_T;

extern BOOL button;

void GpioInit();
BOOL GpioGetButtonState();
BOOL GpioToggleLed(DISCOVERY_OUTPUT_PINS_T ledx);

#endif
