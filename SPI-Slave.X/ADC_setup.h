/* 
 * File:  ADC_setup.h Libreria de interrupcion PORTB 
 * Author: Gabriel Carrera 21216
 * Comments: a 
 * Revision history: a
 */

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef ADC_SETUP_H
#define	ADC_SETUP_H

#include <xc.h> // include processor files - each processor file is guarded.  
#include <stdint.h>

// --------------- Setup del ADC --------------- 
void adc_init(int channel);
int adc_read();
void adc_change_channel(int channel);
int adc_get_channel();
int map_adc_volt(int value, int inputmin, int inputmax, int outmin, int outmax); // Funcion para mappear el adc a voltaje 

#endif	/* ADC_SETUP_H */

