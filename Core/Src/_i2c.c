#include <string.h>

#include "_i2c.h"

I2CDrv sensorI2C;

osSemaphoreAttr_t* getOsSemaphoreAttr_t(char* name, void* cb_mem, uint32_t cb_size) {
    static osSemaphoreAttr_t attr;
    memset(&attr, 0, sizeof(attr));
    attr.name = name;
    attr.cb_mem = cb_mem;
    attr.cb_size = cb_size;
    return &attr;
}

void _I2C_Init() {
	sensorI2C.hi2c = &hi2c3;
	sensorI2C.i2cRxDmaSemaphore = osSemaphoreNew(1, 0, getOsSemaphoreAttr_t("SENSORI2C", &sensorI2C.i2cRxDmaSemaphoreBuffer, sizeof(sensorI2C.i2cRxDmaSemaphoreBuffer)));
}


/*! @brief Sensor I2C read function */
int8_t i2cSensorsRead(uint8_t regAddr, uint8_t *regData, uint32_t len, void *intfPtr) {
	HAL_StatusTypeDef status;
	uint16_t DevAddress = *(uint8_t*)intfPtr << 1;
	HAL_I2C_Master_Transmit(sensorI2C.hi2c, DevAddress, &regAddr, 1, 1000);
	status = HAL_I2C_Master_Receive_DMA(sensorI2C.hi2c, DevAddress, regData, len);
	osSemaphoreAcquire(sensorI2C.i2cRxDmaSemaphore, osWaitForever);
	/**
	 * HAL_StatusTypeDef: 0, 1, 2, 3
	 * BMI08X_INTF_RET_TYPE: 0, -1, -2, ..., -9
	 */
	return -status;
}

/*! @brief Sensor I2C write function */
int8_t i2cSensorsWrite(uint8_t regAddr, const uint8_t *regData, uint32_t len, void *intfPtr) {
	static uint8_t sBuffer[256];
	HAL_StatusTypeDef status;
	uint16_t DevAddress = *(uint8_t*)intfPtr << 1;
	memset(sBuffer, 0, 256);
	sBuffer[0] = regAddr;
	memcpy(sBuffer + 1, regData, len);
	status = HAL_I2C_Master_Transmit(sensorI2C.hi2c, DevAddress, sBuffer, len + 1, 1000);
	return -status;
}

void sensorsI2cRxDmaIsr() {
	osSemaphoreRelease(sensorI2C.i2cRxDmaSemaphore);
}