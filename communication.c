#include "communication.h"
#include "usbd_cdc_vcp.h"
#include "string.h"
#include "gpio.h"
#include "datatypes.h"
#include "LIS3DSH.h"
#include <stdio.h>
#include "tempsens.h"

void UartGetStatus(char param)
{
	int8_t lbutton;
	int8_t buff[50];
	float xg,yg,zg,ltemp;

	xg = LIS3DSH_Get_X_Out(LIS3DSH_Sense_2g);
	yg = LIS3DSH_Get_Y_Out(LIS3DSH_Sense_2g);
	zg = LIS3DSH_Get_Z_Out(LIS3DSH_Sense_2g);

	lbutton = (button == TRUE) ? 1 : 0;
	ltemp = GetTemp();

	sprintf(buff,"s,x:%.3f,y:%.3f,z:%.3f,t:%.3f,b:%d \r\n",xg,yg,zg,ltemp,lbutton);

	VCP_send_str(buff);

}

void UartToogleLed(char param)
{
	BOOL LedError = FALSE;
	int8_t buff[30];
	int8_t LedString[10];
	DISCOVERY_OUTPUT_PINS_T tled;
	switch (param)
	{
		case 'b':
			tled = DISCOVERY_LED_BLUE;
			strcpy(LedString,"Blue");
			break;
		case 'g':
			tled = DISCOVERY_LED_GREEN;
			strcpy(LedString,"Green");
			break;
		case 'r':
			tled = DISCOVERY_LED_RED;
			strcpy(LedString,"Red");
			break;
		case 'o':
			tled = DISCOVERY_LED_ORANGE;
			strcpy(LedString,"Orange");
			break;
		default:
			LedError = TRUE;
			break;
	}

	if(LedError == FALSE)
	{
		GpioToggleLed(tled);
		sprintf(buff,"t,Toggling Led: %s",LedString);
		VCP_send_str(buff);
	}
	else
	{
		VCP_send_str("t,Error in Toggling Led");
	}

}

/* len is fixed to max 1 byte  */
UART_MSG_T Uart_Messages[UART_MSG_MAX_NUM] =
{
		/*	ID   		len				Cb	  */
		{'s'		,	0		,		(UartMsgCb)&UartGetStatus},
		{'l'		,	1		,		(UartMsgCb)&UartToogleLed}
};

void UartMsgProc()
{
	char i;
	char Msgbuff[20];
	char tparam;
	if(VCP_get_string(Msgbuff))
	{
		for(i=0;i<UART_MSG_MAX_NUM;i++)
		{
			if(Uart_Messages[i].id == Msgbuff[0])
			{
				if(Uart_Messages[i].len !=0 )
				{
					/* note: len is fixed to 1 byte for now */
					tparam = Msgbuff[1];
				}
				else
				{
					tparam = 0;
				}
				Uart_Messages[i].Cb(tparam);
				break;
			}
		}
	}
}

