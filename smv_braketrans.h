#ifndef _SMV_BRAKETRANS_H
#define _SMV_BRAKETRANS_H

#include "stm32f4xx_hal_adc.h"
#include <stdint.h>

typedef struct
{
	// stm32 adc setup
	ADC_HandleTypeDef *hadc;
	ADC_ChannelConfTypeDef sConfig;
	int device_id;
	uint8_t data_type; // Pressure datatype (6) for HS Message

    uint16_t adc_raw;      // last ADC reading (0-4095)
    double psi_value;    // calculated brake pressure [psi]
    uint8_t fault_flag;   // 0 = OK, 1 = sensor fault, etc.

    void (*init)(BrakeTransTypeDef*, int, ADC_HandleTypeDef*);
    void (*begin)(BrakeTransTypeDef*);
    void (*collect)(BrakeTransTypeDef*);
    double (*getPsi)(BrakeTransTypeDef);
    uint16_t (*getAdc)(BrakeTransTypeDef);

} BrakeTransTypeDef;


// ---- Hardware Constants ----
#define BRAKE_TRANS_ADC_MAX     4095U
#define BRAKE_TRANS_VREF        3.3       // STM32 ADC reference voltage
#define BRAKE_TRANS_DIVIDER     2.0       // Voltage divider ratio (sensor -> ADC)
#define BRAKE_TRANS_VMAX        4.5       // Sensor full-scale voltage
#define BRAKE_TRANS_VMIN        0.5       // Sensor zero-pressure voltage

// ---- Calibration Constants ----
#define BRAKE_TRANS_SLOPE       125.0
#define BRAKE_TRANS_OFFSET      62.5

// ---- Physical Limits ----
#define PSI_MAX					500.0
#define PSI_MIN					0.0

// Constructor for Brake Trans which also includes ADC setup
BrakeTransTypeDef BRAKE_new(void);

#endif // _SMV_BRAKETRANS_H
