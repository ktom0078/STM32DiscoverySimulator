#ifndef COMM_H
#define COMM_H

#define UART_MSG_MAX_NUM	2
#define COMM_BUFFER_LEN		70

typedef void (*UartMsgCb)(char param);

typedef struct UART_MSG_
{
	char id;
	char len;
	UartMsgCb Cb;
}UART_MSG_T;

void UartMsgProc();

#endif
