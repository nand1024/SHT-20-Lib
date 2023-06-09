/*
 * author Dus Oleh Viktorovych
 * created 11.01.2023
 * cross platform library for SHT20
 */

#include <stdlib.h>
#include "sht20.h"



double calcTemperatureFromRaw(uint16_t rawTemperature)
{
	return -46.85 + 175.72 * ((double)rawTemperature / 65536);
}



double calcHumidityFromRaw(uint16_t rawHumidity)
{
	return -6 + 125 * ((double)rawHumidity / 65536);
}



uint8_t setUserRegistr(Resolution resolution,
					   StatusEOB statusEOB,
					   ChipPower chipPower,
					   OTPReload otpReload)
{
	uint8_t res = 0;

	if (resolution >= resolutionCNT
		|| statusEOB >= endOfBatteryCNT
		|| chipPower >= chipPowerCNT
		|| otpReload >= otpCNT) {
		return res;
	}

	res = (resolution << 6) | (statusEOB << 5) | (chipPower << 1) | otpReload;

	return res;
}



void sht20Measurment(SHT20CB *shtCB, uint8_t configUserReg, double *temperature, double *humidity)
{
	const uint8_t chipEnable = 0b00000010;

	uint8_t addrRegistr = cmdWriteUserRegistr;
	uint8_t data[2];
	uint16_t rawT, rawH;

	if ((configUserReg & chipEnable) == 0) {
		configUserReg |= chipEnable; //enable chip;
	}

	shtCB->i2cWrite(SHT20_I2C_ADDRESS, &addrRegistr, 1, &configUserReg, 1);
	
	addrRegistr = cmdTmeasurementNoHold;
	shtCB->i2cRead(SHT20_I2C_ADDRESS, &addrRegistr, 1, data, 2);
	rawT = data[0];
	rawT <<= 8;
	rawT |= data[1];

	*temperature = calcTemperatureFromRaw(rawT);

	addrRegistr = cmdRHmeasurementNoHold;
	shtCB->i2cRead(SHT20_I2C_ADDRESS, &addrRegistr, 1, data, 2);
	rawH = data[0];
	rawH <<= 8;
	rawH |= data[1];

	*humidity = calcHumidityFromRaw(rawH);
	configUserReg &= ~chipEnable; //disable chip;
	addrRegistr = cmdWriteUserRegistr;
	shtCB->i2cWrite(SHT20_I2C_ADDRESS, &addrRegistr, 1, &configUserReg, 1);
}


void sht20MeasurmentDefSet(SHT20CB *shtCB, double *temperature, double *humidity)
{
	uint8_t userReg = setUserRegistr(resolutionT14bitRH12bit,
		    						 endOfBattery_Vdd_larger_2250mV,
									 chipPowerEnable,
									 otpDisable);
									 
	sht20Measurment(shtCB, userReg, temperature, humidity);
}
