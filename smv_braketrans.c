#include "smv_braketrans.h"

// Helper function to convert adc data -> psi
static void ADCtoPSI(BrakeTrans *bt)
{
	double v_sensor = ((double)bt->adc_raw * BRAKE_TRANS_VREF * BRAKE_TRANS_DIVIDER) / BRAKE_TRANS_ADC_MAX;
	double psi = (BRAKE_TRANS_SLOPE * v_sensor) - BRAKE_TRANS_OFFSET;
	if (psi < PSI_MIN) psi = PSI_MIN;
	if (psi > PSI_MAX) psi = PSI_MAX;

	bt->psi_value = psi;
}

// Return psi value
static double BRAKE_getPSI(BrakeTrans *bt)
{
	return bt->psi_value;
}

// Return psi value
static uint16_t BRAKE_getADC(BrakeTrans *bt)
{
	return bt->adc_raw;
}

/*
 * @brief: Initializes brake transducer object with default values
 * @param [out] BrakeTrans bt: struct object that holds adc values and fault flags
*/
BrakeTrans BRAKE_new(void) 
{
	BrakeTrans bt = {
		.psi_value = 0,
		.device_id = 0,
		.adc_raw = 0,
		.data_type = HSMESSAGE_PRESSURE,
		.fault_flag = 0
	};
	bt.getPsi = BRAKE_getPSI;
	bt.getAdc = BRAKE_getADC;
	return bt;
}
