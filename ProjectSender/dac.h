#ifndef DAC_H
#define DAC_H

void dac_init();

//void sine_wave();

/**
 * Sets the output voltage to the specified value
 * @param voltage should be between 0 and 4095
 * @param writeEEPROM <= 1 for data to persist after reset else 0
 */
void dac_set_voltage(unsigned int voltage, unsigned char writeEEPROM);

#endif
