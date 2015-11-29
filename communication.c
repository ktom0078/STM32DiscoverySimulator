#include "communication.h"
#include "usbd_cdc_vcp.h"
#include "string.h"
#include "gpio.h"
#include "datatypes.h"

void UartGetStatus(char param)
{
	VCP_send_str("In UsartGetStatus! \r\n");
}

void UartToogleLed(char param)
{
	BOOL LedError = FALSE;
	DISCOVERY_OUTPUT_PINS_T tled;
	switch (param)
	{
		case 'b':
			tled = DISCOVERY_LED_BLUE;
			break;
		case 'g':
			tled = DISCOVERY_LED_GREEN;
			break;
		case 'r':
			tled = DISCOVERY_LED_RED;
			break;
		case 'o':
			tled = DISCOVERY_LED_ORANGE;
			break;
		default:
			LedError = TRUE;
			break;
	}

	if(LedError == FALSE)
	{
		GpioToggleLed(tled);
		VCP_send_str("Toggling Led");
	}
	else
	{
		VCP_send_str("Error Toggling Led");
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
