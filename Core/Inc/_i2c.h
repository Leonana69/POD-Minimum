#ifndef __UI2C_H__
#define __UI2C_H__

/*! I2C wrappers */
#include "i2c.h"
#include "cmsis_os2.h"
#include "FreeRTOS.h"
#include "stdbool.h"

typedef struct {
	I2C_HandleTypeDef* hi2c;
	osSemaphoreId_t i2cRxDmaSemaphore;
	StaticSemaphore_t i2cRxDmaSemaphoreBuffer;
} I2CDrv;

extern I2CDrv sensorI2C;

void _I2C_Init();

int8_t i2cSensorsRead(uint8_t regAddr, uint8_t *regData, uint32_t len, void *intfPtr);
int8_t i2cSensorsWrite(uint8_t regAddr, const uint8_t *regData, uint32_t len, void *intfPtr);

void sensorsI2cRxDmaIsr();

#endif
