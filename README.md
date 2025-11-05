# SMV_STM32_BrakeTrans

Bruin Racing Supermileage – Brake Transducer Driver for STM32 CANbus

This driver reads a **brake pressure transducer** using the STM32’s ADC, converts it to PSI, and sends it through the "SMV_STM32_CANbus" library https://github.com/UCLA-Bruin-Supermileage/SMV_STM32_CANbus.  
It’s written for the **STM32F446RE**; other STM32 boards may need slight adjustments (ADC channel, instance, or clock setup).

---

## ⚙️ Installation

1. Configure **ADC1** in STM32CubeIDE:
   - Resolution: 12 bits  
   - Channel: 5  
   - Sampling Time: 84 cycles  
   - Continuous Conversion: Enabled  
   - External Trigger: None  

2. Enable **CAN1** at your desired baud rate (typically **500 kbps**).  
3. Place these files in your project:
