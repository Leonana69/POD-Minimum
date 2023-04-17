#include "_uart.h"

int debugUartPutchar(int c) {
	HAL_UART_Transmit(&huart3, (uint8_t*) &c, 1, 100);
	return (unsigned char) c;
}