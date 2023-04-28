#include <port.h>

void PORTInit(PORT_ConfigType * PORT_Config)
{
  //set up interrup
  (PORT_Config->PORTn)->PCR[PORT_Config->pin] &= ~PORT_PCR_IRQC_MASK;
  (PORT_Config->PORTn)->PCR[PORT_Config->pin] |= PORT_PCR_IRQC(PORT_Config->interrupt_config_type);
  //set up mux: GPIO, ...
  (PORT_Config->PORTn)->PCR[PORT_Config->pin] &= ~PORT_PCR_MUX_MASK;
  (PORT_Config->PORTn)->PCR[PORT_Config->pin] |= PORT_PCR_MUX(PORT_Config->pin_mux_control_type);
  //set up pull enable
  (PORT_Config->PORTn)->PCR[PORT_Config->pin] &= ~PORT_PCR_PE_MASK;
  (PORT_Config->PORTn)->PCR[PORT_Config->pin] |= PORT_PCR_PE(PORT_Config->pull_enable_type);
  //set up pull-up or pull-down
  (PORT_Config->PORTn)->PCR[PORT_Config->pin] &= ~PORT_PCR_PS_MASK;
  (PORT_Config->PORTn)->PCR[PORT_Config->pin] |= PORT_PCR_PS(PORT_Config->pull_select_type);
}

void PORT_EXTI_Config (PORT_ConfigType * PORT_Config)
{
  //set up interrupt
  (PORT_Config->PORTn)->PCR[PORT_Config->pin] &= ~PORT_PCR_IRQC_MASK;
  (PORT_Config->PORTn)->PCR[PORT_Config->pin] |= PORT_PCR_IRQC(PORT_Config->interrupt_config_type);
}

//PORT_EXTI_ClearFlag