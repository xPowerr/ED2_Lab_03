//*****************************************************************************
/*
 * File:   main.c
 * Author: Gabriel Carrera 21216
 * Created on July 31, 05:18 PM
 */

 // Código de PIC Esclavo

// --------------- Palabra de Configuracion ---------------
// CONFIG1
#pragma config FOSC = INTRC_NOCLKOUT// Oscillator Selection bits (INTOSCIO oscillator: I/O function on RA6/OSC2/CLKOUT pin, I/O function on RA7/OSC1/CLKIN)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled and can be enabled by SWDTEN bit of the WDTCON register)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config MCLRE = OFF      // RE3/MCLR pin function select bit (RE3/MCLR pin function is digital input, MCLR internally tied to VDD)
#pragma config CP = OFF         // Code Protection bit (Program memory code protection is disabled)
#pragma config CPD = OFF        // Data Code Protection bit (Data memory code protection is disabled)
#pragma config BOREN = OFF      // Brown Out Reset Selection bits (BOR disabled)
#pragma config IESO = OFF       // Internal External Switchover bit (Internal/External Switchover mode is disabled)
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enabled bit (Fail-Safe Clock Monitor is disabled)
#pragma config LVP = OFF        // Low Voltage Programming Enable bit (RB3 pin has digital I/O, HV on MCLR must be used for programming)

// CONFIG2
#pragma config BOR4V = BOR40V   // Brown-out Reset Selection bit (Brown-out Reset set to 4.0V)
#pragma config WRT = OFF        // Flash Program Memory Self Write Enable bits (Write protection off)

// --------------- Librerias ---------------
#include <xc.h>
#include <stdint.h>
#include <stdio.h>
#include "SPI.h"
#include "ADC_setup.h"

// --------------- Frecuencia ---------------
#define _XTAL_FREQ 4000000


// --------------- Variables ---------------
uint8_t temporal = 0;
int adc_var = 0; // variable para almacenar el valor leido del ADC

// --------------- Prototipos ---------------
void setup(void); // funcion de configuracion


// --------------- Loop principal ---------------
void main(void) {
    setup(); // Llamada a la funcion de configuracion
    adc_init(0); // Se inicializa el ADC para el canal AN0
    
    while(1){
       //adc_var = adc_read();
       __delay_ms(5);
       
       if (ADCON0bits.GO == 0) { // si la lectura del ADC se desactiva
            ADCON0bits.GO = 1; // activarla
            __delay_ms(50);
        }
    }
    return;
}

// --------------- Rutina de  interrupciones ---------------
void __interrupt() isr(void){ // interrupciones
   if (PIR1bits.ADIF == 1){ // revisar bandera de interrupcion del puerto B
        adc_var = adc_read();
        PIR1bits.ADIF = 0; // limpiar bandera
    }
    if(SSPIF == 1){
        spiWrite(adc_var);
        SSPIF = 0;
    }
}

// --------------- Setup General ---------------
void setup(void){
    
// --------------- Definir analogicas ---------------
    ANSEL   =   0;
    ANSELH  =   0;
    
// --------------- Configurar puertos --------------- 
    
    INTCONbits.GIE = 1;         // Habilitamos interrupciones
    INTCONbits.PEIE = 1;        // Habilitamos interrupciones PEIE
    PIR1bits.SSPIF = 0;         // Borramos bandera interrupción MSSP
    PIE1bits.SSPIE = 1;         // Habilitamos interrupción MSSP
    TRISAbits.TRISA5 = 1;       // Slave Select
    spiInit(SPI_SLAVE_SS_EN, SPI_DATA_SAMPLE_MIDDLE, SPI_CLOCK_IDLE_LOW, SPI_IDLE_2_ACTIVE);
   
}