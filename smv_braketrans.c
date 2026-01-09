#include "../Inc/smv_braketrans.h"
#include "../Inc/smv_board_enums.h"

static void ADCtoPSI(BrakeTrans *bt); // private helper

static void BRAKE_QuickSetup(BrakeTrans *bt, int hardware, ADC_HandleTypeDef *adc_obj)
{
	bt->device_id = hardware;
	bt->data_type = Pressure;
	bt->psi_value = 0;
	bt->fault_flag = 0;

	bt->hadc = adc_obj;
	ADC_Init(&adc_obj);
	memset(&(bt->sConfig), 0, sizeof(bt->sConfig));


}

static void ADC_Init(BrakeTrans *adc_obj)
{
	
	bt->hadc->Instance = ADC1;
	bt->hadc->Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV2;
	bt->hadc->Init.Resolution = ADC_RESOLUTION_12B;
	bt->hadc->Init.ScanConvMode = DISABLE;
	bt->hadc->Init.ContinuousConvMode = ENABLE;
	bt->hadc->Init.DiscontinuousConvMode = DISABLE;
	bt->hadc->Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
	bt->hadc->Init.ExternalTrigConv = ADC_SOFTWARE_START;
	bt->hadc->Init.DataAlign = ADC_DATAALIGN_RIGHT;
	bt->hadc->Init.NbrOfConversion = 1;
	bt->hadc->Init.DMAContinuousRequests = DISABLE;
	bt->hadc->Init.EOCSelection = ADC_EOC_SINGLE_CONV;
	if (HAL_ADC_Init(bt->hadc) != HAL_OK)
	{
	  Error_Handler();
	}

	/* Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time. */
	bt->sConfig.Channel = ADC_CHANNEL_0;
	bt->sConfig.Rank = 1;
	bt->sConfig.SamplingTime = ADC_SAMPLETIME_84CYCLES;
	if (HAL_ADC_ConfigChannel(bt->hadc, &(bt->sConfig)) != HAL_OK)
	{
	  Error_Handler();
	}
}

static void BRAKE_Run(BrakeTrans *bt)
{
	// Start ADC once for continuous mode
	if (HAL_ADC_Start(bt->hadc) != HAL_OK) {
		Error_Handler();
	}
}

static void BRAKE_Collect(BrakeTrans *bt) {
	if (HAL_ADC_PollForConversion(bt->hadc, 0) == HAL_OK)
	{
	    bt->adc_raw = (uint16_t)HAL_ADC_GetValue(bt->hadc);
	    ADCtoPSI(bt);
	}
}

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

BrakeTrans BRAKE_new(void) 
{
	BrakeTrans bt = {0};
	bt.init = BRAKE_QuickSetup;
	bt.begin = BRAKE_Run;
	bt.collect = BRAKE_Collect;
	bt.getPsi = BRAKE_getPSI;
	bt.getAdc = BRAKE_getADC;
	return bt;
}
