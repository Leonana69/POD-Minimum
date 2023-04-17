#ifndef __DEBUG_H__
#define __DEBUG_H__

#include "eprintf.h"
#include "_uart.h"

#ifdef DEBUG_MODULE
#define DEBUG_FMT(fmt) DEBUG_MODULE ": " fmt
#endif

#ifndef DEBUG_FMT
#define DEBUG_FMT(FMT) FMT
#endif


#define DEBUG_PRINT_UART(FMT, ...) eprintf(debugUartPutchar, FMT, ## __VA_ARGS__)

#endif