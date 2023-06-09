/*
 * author Dus Oleh Viktorovych
 * created 11.01.2023
 * cross platform library for SHT20
 */

#ifndef _SHT_20_H_
#define _SHT_20_H_

#include <stdint.h>
#include <stdbool.h>

#define SHT20_I2C_ADDRESS    0b1000000

typedef struct {
	void (*i2cWrite) (uint8_t adress, uint8_t *regAdr, uint8_t sizeRegAdr, uint8_t *data, uint32_t size);
	void (*i2cRead) (uint8_t adress, uint8_t *regAdr, uint8_t sizeRegAdr, uint8_t *data, uint32_t size);
}SHT20CB;

enum {
	cmdTmeasurementHoldByMaster =  0b11100011,
	cmdRHmeasurementHoldByMaster = 0b11100101,
	cmdTmeasurementNoHold =  0b11110011,
	cmdRHmeasurementNoHold = 0b11110101,
	cmdWriteUserRegistr = 0b11100110,
	cmdReadUserRegistr =  0b11100111,
	cmdSoftReset = 0b11111110,
};

typedef enum {
	resolutionT14bitRH12bit = 0,
	resolutionT12bitRH8bit = 1,
	resolutionT13bitRH10bit = 2,
	resolutionT11bitRH11bit = 3,
	resolutionCNT,
}Resolution;

typedef enum {
	endOfBattery_Vdd_larger_2250mV = 0,
	endOfBattery_Vdd_Smaller_2250mV = 1,
	endOfBatteryCNT,
}StatusEOB;

typedef enum {
	chipPowerDisable = 0,
	chipPowerEnable = 1,
	chipPowerCNT,
}ChipPower;

typedef enum {
	otpDisable = 0,
	otpEnable = 1,
	otpCNT,
}OTPReload;

uint8_t setUserRegistr(Resolution resolution,
					   StatusEOB statusEOB,
					   ChipPower chipPower,
					   OTPReload otpReload);
void sht20Measurment(SHT20CB *shtCB, uint8_t configUserReg, double *temperature, double *humidity);
void sht20MeasurmentDefSet(SHT20CB *shtCB, double *temperature, double *humidity);

#endif
