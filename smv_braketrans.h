#ifndef _SMV_BRAKETRANS_H
#define _SMV_BRAKETRANS_H

#include <stdint.h>

/* ---- Hardware Constants ---- */
#define BRAKE_TRANS_ADC_MAX     4095U
#define BRAKE_TRANS_VREF        3.3       /* STM32 ADC reference voltage */
#define BRAKE_TRANS_DIVIDER     2.0       /* Voltage divider ratio (sensor -> ADC) */
#define BRAKE_TRANS_VMAX        4.5       /* Sensor full-scale voltage */
#define BRAKE_TRANS_VMIN        0.5       /* Sensor zero-pressure voltage */

/* ---- Calibration Constants ---- */
#define BRAKE_TRANS_SLOPE       125.0
#define BRAKE_TRANS_OFFSET      62.5

/* ---- Physical Limits ---- */
#define PSI_MAX					500.0
#define PSI_MIN					0.0

/* ---- HSMessage for SMV Can Communication ---- */
#define BT_PRESSURE             6           /* HSMessage.Pressure from "smv_board_enums.h" */

typedef struct BrakeTrans
{
    /* Virtual Table */
    void (*init)(BrakeTrans*, int, ADC_HandleTypeDef*);
    void (*begin)(BrakeTrans*);
    void (*collect)(BrakeTrans*);
    double (*getPsi)(BrakeTrans);
    uint16_t (*getAdc)(BrakeTrans);

    /* Data (Largest Alignment First)*/
    double psi_value;       /* calculated brake pressure [psi] */
	int device_id;
    uint16_t adc_raw;       /* last ADC reading (0-4095) */
	uint8_t data_type;      /* Pressure datatype (6) for HS Message */
    uint8_t fault_flag;     /* 0 = OK, 1 = sensor fault, etc. */
} BrakeTrans;


/* Constructor for Brake Trans which also includes ADC setup */
BrakeTrans BRAKE_new(void);

#endif /* _SMV_BRAKETRANS_H */