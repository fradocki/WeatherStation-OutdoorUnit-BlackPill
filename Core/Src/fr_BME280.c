/*
 * fr_BME280.c
 *
 *      Author: Filip
 */

#include <fr_BME280.h>

#define BME280_I2C &hi2c1
#define BME280_ADDRESS 0xEC  // SDIO is grounded, the 7 bit address is 0x76 and 8 bit address = 0x76<<1 = 0xEC

extern float Temperature, Pressure, Humidity;

int32_t rawTemperature, rawPressure, rawHumidity;
uint8_t chipID;
uint8_t TrimParam[36];

uint16_t CalibrationT1,
         CalibrationP1,
         CalibrationH1, CalibrationH3;
int16_t  CalibrationT2, CalibrationT3,
         CalibrationP2, CalibrationP3, CalibrationP4, CalibrationP5,
		 CalibrationP6, CalibrationP7, CalibrationP8, CalibrationP9,
		 CalibrationH2,  CalibrationH4, CalibrationH5, CalibrationH6;

// Read the Trimming parameters saved in the NVM ROM of the device
void Trim_Read(void)
{
	uint8_t TrimTab[32];

	// Read NVM from 0x88 to 0xA1
	HAL_I2C_Mem_Read(BME280_I2C, BME280_ADDRESS, 0x88, 1, TrimTab, 25, 500);

	// Read NVM from 0xE1 to 0xE7
	HAL_I2C_Mem_Read(BME280_I2C, BME280_ADDRESS, 0xE1, 1, (uint8_t *)TrimTab+25, 7, 500);

	CalibrationT1 = (TrimTab[1]<<8) | TrimTab[0];
	CalibrationT2 = (TrimTab[3]<<8) | TrimTab[2];
	CalibrationT3 = (TrimTab[5]<<8) | TrimTab[4];
	CalibrationP1 = (TrimTab[7]<<8) | TrimTab[5];
	CalibrationP2 = (TrimTab[9]<<8) | TrimTab[6];
	CalibrationP3 = (TrimTab[11]<<8) | TrimTab[10];
	CalibrationP4 = (TrimTab[13]<<8) | TrimTab[12];
	CalibrationP5 = (TrimTab[15]<<8) | TrimTab[14];
	CalibrationP6 = (TrimTab[17]<<8) | TrimTab[16];
	CalibrationP7 = (TrimTab[19]<<8) | TrimTab[18];
	CalibrationP8 = (TrimTab[21]<<8) | TrimTab[20];
	CalibrationP9 = (TrimTab[23]<<8) | TrimTab[22];
	CalibrationH1 = TrimTab[24];
	CalibrationH2 = (TrimTab[26]<<8) | TrimTab[25];
	CalibrationH3 = (TrimTab[27]);
	CalibrationH4 = (TrimTab[28]<<4) | (TrimTab[29] & 0x0f);
	CalibrationH5 = (TrimTab[30]<<4) | (TrimTab[29]>>4);
	CalibrationH6 = (TrimTab[31]);
}

// Function to configure BME280 sensor
int BME280_Config (uint8_t tempOversampling, uint8_t pressureOversampling, uint8_t humidityOversampling, uint8_t filter)
{
	// Read the Trimming parameters
	Trim_Read();

	// Define local variables for the data to be written and the data checked
	uint8_t dataToWrite = 0;
	uint8_t dataCheck = 0;

	// Reset the device
	dataToWrite = 0xB6;  // reset sequence
	if (HAL_I2C_Mem_Write(BME280_I2C, BME280_ADDRESS, RESET_REG, 1, &dataToWrite, 1, 1000) != HAL_OK)
	{
		return -1; // return error if writing to I2C bus failed
	}
	HAL_Delay (100); // Delay required for stable operation after reset

	// Write the humidity oversampling to 0xF2 register
	dataToWrite = humidityOversampling;
	if (HAL_I2C_Mem_Write(BME280_I2C, BME280_ADDRESS, CTRL_HUM_REG, 1, &dataToWrite, 1, 1000) != HAL_OK)
	{
		return -1; // return error if writing to I2C bus failed
	}
	HAL_Delay (100); // Delay to ensure the write is completed
	HAL_I2C_Mem_Read(BME280_I2C, BME280_ADDRESS, CTRL_HUM_REG, 1, &dataCheck, 1, 1000); // Read the value back to check if write was successful
	if (dataCheck != dataToWrite)
	{
		return -1; // return error if the read data does not match the written data
	}

	// Write the IIR filter coeff to 0xF5 register
	dataToWrite = (filter << 2);
	if (HAL_I2C_Mem_Write(BME280_I2C, BME280_ADDRESS, CONFIG_REG, 1, &dataToWrite, 1, 1000) != HAL_OK)
	{
		return -1; // return error if writing to I2C bus failed
	}
	HAL_Delay (100); // Delay to ensure the write is completed
	HAL_I2C_Mem_Read(BME280_I2C, BME280_ADDRESS, CONFIG_REG, 1, &dataCheck, 1, 1000); // Read the value back to check if write was successful
	if (dataCheck != dataToWrite)
	{
		return -1; // return error if the read data does not match the written data
	}

	// Write the pressure and temperature oversampling along with mode to 0xF4 register
	dataToWrite = (tempOversampling << 5) | (pressureOversampling << 2) | MODE_FORCED;
	if (HAL_I2C_Mem_Write(BME280_I2C, BME280_ADDRESS, CTRL_MSG_REG, 1, &dataToWrite, 1, 1000) != HAL_OK)
	{
		return -1; // return error if writing to I2C bus failed
	}
	HAL_Delay (100); // Delay to ensure the write is completed
	HAL_I2C_Mem_Read(BME280_I2C, BME280_ADDRESS, CTRL_MSG_REG, 1, &dataCheck, 1, 1000); // Read the value back to check if write was successful
	if (dataCheck != dataToWrite - 1)
	{
		return -1; // return error if the read data does not match the written data
	}

	return 0; // return 0 for successful configuration
}

// Read raw data from BME sensor
int BMESensorRead(void)
{
	uint8_t rawData[8];

	// Check the chip ID before reading
	HAL_I2C_Mem_Read(BME280_I2C, BME280_ADDRESS, ID_REG, 1, &chipID, 1, 1000);

	// Check if the read chip ID is correct
	if (chipID == 0x60)
	{
		// Read registers 0xF7 to 0xFE
		HAL_I2C_Mem_Read(BME280_I2C, BME280_ADDRESS, PRESS_MSB_REG, 1, rawData, 8, 500);

		/* Calculate raw data for the parameters.
		 * Pressure and Temperature are in 20 bit format and Humidity in 16 bit format.
		 */
		rawPressure = (rawData[0]<<12)|(rawData[1]<<4)|(rawData[2]>>4);
		rawTemperature = (rawData[3]<<12)|(rawData[4]<<4)|(rawData[5]>>4);
		rawHumidity = (rawData[6]<<8)|(rawData[7]);

		return 0;  // Success
	}

	else return -1;  // Failure
}

// Wake up BME280 sensor from sleep mode
void BME280_WakeUP(void)
{
	uint8_t controlData = 0;

	// First, read the control measure register
	HAL_I2C_Mem_Read(BME280_I2C, BME280_ADDRESS, CTRL_MSG_REG, 1, &controlData, 1, 1000);

	// Modify the data with the forced mode
	controlData = controlData | MODE_FORCED;

	// Write the new data to the control measure register
	HAL_I2C_Mem_Write(BME280_I2C, BME280_ADDRESS, CTRL_MSG_REG, 1, &controlData, 1, 1000);

	HAL_Delay (100);  // Short delay for changes to take effect
}

// Global variable for storing fine temperature
int32_t temperatureFine;

// Compensate temperature
int32_t BME280_compensateTemperature(int32_t rawTemp)
{
	int32_t intermediateTemp1, intermediateTemp2, compensatedTemp;

	intermediateTemp1 = ((((rawTemp>>3) - ((int32_t)CalibrationT1<<1))) * ((int32_t)CalibrationT2)) >> 11;
	int32_t rawTempShifted = rawTemp >> 4;
	int32_t calibDiff = rawTempShifted - ((int32_t)CalibrationT1);
	int32_t calibDiffSquared = (calibDiff * calibDiff) >> 12;
	int32_t calibProduct = calibDiffSquared * ((int32_t)CalibrationT3);
	intermediateTemp2 = calibProduct >> 14;

	temperatureFine = intermediateTemp1 + intermediateTemp2;

	compensatedTemp = (temperatureFine * 5 + 128) >> 8;
	return compensatedTemp;
}

// Compensate pressure
uint32_t BME280_compensatePressure(int32_t rawPressure)
{
	int64_t intermediatePressure1, intermediatePressure2, compensatedPressure;

	intermediatePressure1 = ((int64_t)temperatureFine) - 128000;
	intermediatePressure2 = intermediatePressure1 * intermediatePressure1 * (int64_t)CalibrationP6;
	intermediatePressure2 = intermediatePressure2 + ((intermediatePressure1*(int64_t)CalibrationP5)<<17);
	intermediatePressure2 = intermediatePressure2 + (((int64_t)CalibrationP4)<<35);
	intermediatePressure1 = ((intermediatePressure1 * intermediatePressure1 * (int64_t)CalibrationP3)>>8) +
			((intermediatePressure1 * (int64_t)CalibrationP2)<<12);
	intermediatePressure1 = (((((int64_t)1)<<47)+intermediatePressure1))*((int64_t)CalibrationP1)>>33;

	// Avoid exception caused by division by zero
	if (intermediatePressure1 == 0)
	{
		return 0;
	}

	compensatedPressure = 1048576 - rawPressure;
	compensatedPressure = (((compensatedPressure<<31) - intermediatePressure2) * 3125) / intermediatePressure1;
	intermediatePressure1 = (((int64_t)CalibrationP9) * (compensatedPressure>>13) * (compensatedPressure>>13)) >> 25;
	intermediatePressure2 = (((int64_t)CalibrationP8) * compensatedPressure) >> 19;
	compensatedPressure = ((compensatedPressure + intermediatePressure1 + intermediatePressure2) >> 8)
			+ (((int64_t)CalibrationP7)<<4);

	return (uint32_t)compensatedPressure;
}

// Compensate humidity
uint32_t BME280_compensateHumidity(int32_t rawHumidity)
{
	int32_t intermediateHumidity;

	intermediateHumidity = (temperatureFine - ((int32_t)76800));
	int32_t tempHumid = temperatureFine - (int32_t)76800;
	int32_t calc1 = (rawHumidity << 14) - (((int32_t)CalibrationH4) << 20) - (((int32_t)CalibrationH5) * tempHumid);
	calc1 = (calc1 + (int32_t)16384) >> 15;
	int32_t calc2 = (tempHumid * ((int32_t)CalibrationH6)) >> 10;
	int32_t calc3 = ((tempHumid * ((int32_t)CalibrationH3)) >> 11) + ((int32_t)32768);
	int32_t calc4 = (((calc2 * calc3) >> 10) + ((int32_t)2097152)) * ((int32_t)CalibrationH2) + 8192;
	calc4 = calc4 >> 14;
	intermediateHumidity = calc1 * calc4;
	int32_t calc5 = ((intermediateHumidity >> 15) * (intermediateHumidity >> 15)) >> 7;
	intermediateHumidity -= calc5 * ((int32_t)CalibrationH1) >> 4;

	intermediateHumidity = (intermediateHumidity < 0 ? 0 : intermediateHumidity);
	intermediateHumidity = (intermediateHumidity > 419430400 ? 419430400 : intermediateHumidity);

	return (uint32_t)(intermediateHumidity>>12);
}

//measure the temp, pressure and humidity
void BME280_Measure (void)
{
	if (BMESensorRead() == 0)
	{
		  if(rawTemperature == 0x800000)
		  {
			   Temperature = 0; // value in case temp measurement was disabled
		  }
		  else
		  {
			  Temperature = (BME280_compensateTemperature(rawTemperature))/100.0;//compensation of values using trimming data
		  }

		  if(rawPressure == 0x800000){
			  Pressure = 0; // value in case temp measurement was disabled
		  }

		  else
		  {
			  Pressure = (BME280_compensatePressure(rawPressure))/256.0;//compensation of values using trimming data
		  }

		  if(rawHumidity == 0x8000){
			  Humidity = 0; // value in case temp measurement was disabled
		  }
		  else
		  {
			  Humidity = (BME280_compensateHumidity(rawHumidity))/1024.0;//compensation of values using trimming data
		  }
	}
	else
	{
		Temperature = Pressure = Humidity = 0;
	}

	//convert measured values for UI labels format
	sprintf(str_temp, "%2.1f", Temperature);
	sprintf(str_cis, "%4.0f", Pressure/100);
	sprintf(str_wil, "%2.1f", Humidity);
	sprintf(tab_polecenie[5].msg, "%s:W%s:T%s:P%04u:B%04d:D#\r\n",str_wil,str_temp, str_cis, bat_lvl_int, dust);
	sprintf(tab_polecenie[4].msg, "AT+CIPSEND=0,%i\r\n",strlen(tab_polecenie[5].msg));//channel
	sprintf(tab_polecenie[6].msg,  "AT+CIPCLOSE=0\r\n");//=%dchannel

}
