//*****************************************************************************
/*
 * File:   main.c
 * Author: Gabriel Carrera 21216
 * Created on July 31, 08:42 PM
 */

 // Código de PIC Esclavo 2

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
#include "SPI.h" // Libreria del SPI
#include "ADC_setup.h" // Libreria del ADC

// --------------- Frecuencia ---------------
#define _XTAL_FREQ 4000000


// --------------- Variables ---------------
unsigned char voltaje1; //Variable para guardar voltaje
int var; //Variable para decidir que dato enviar


// --------------- Prototipos ---------------
void setup(void); // funcion de configuracion


// --------------- Loop principal ---------------
void main(void) {
    setup(); // Llamada a la funcion de configuracion
    adc_init(0); // Se inicializa el ADC para el canal AN0
    
    while(1){
       //adc_var = adc_read();
       
       if (ADCON0bits.GO == 0) { // si la lectura del ADC se desactiva
            ADCON0bits.GO = 1; // activarla
            //__delay_ms(50);
        }
    }
}

// --------------- Rutina de  interrupciones ---------------
void __interrupt() isr(void){ // interrupciones
   if (PIR1bits.ADIF == 1){ // revisar bandera de interrupcion del puerto B
        voltaje1 = adc_read();
        adc_change_channel(0);
        PIR1bits.ADIF = 0; // limpiar bandera
    }
    if (PIR1bits.SSPIF == 1){ //Revisar si es interrupción del SPI
        var = spiRead(); //Leer variable para decidir que dato enviar
        if (var == 2){ //Si se recibe 2 enviar voltaje
            spiWrite(voltaje1); //Enviar valor del potenciómetro
            PIR1bits.SSPIF = 0; //Limpiar bandera 
        }
    }
}

// --------------- Setup General ---------------
void setup(void){
    
// --------------- Definir analogicas ---------------
    ANSELbits.ANS0 = 1; //Canal AN0 como entrada analógica
    ANSELbits.ANS4 = 0; //Pin A5 como pin digital
    ANSELH = 0; //Puertos como I/O digitales
    
    
// --------------- Configurar puertos --------------- 
    TRISAbits.TRISA5 = 1; //Puerto A5 como entrada
    TRISBbits.TRISB7 = 1; //Puerto B6 como entrada
    TRISBbits.TRISB6 = 1; //Puerto B7 como entrada
    TRISD = 0; //Puerto D como salida
    
    PORTB = 0; //Limpiar Puerto B
    PORTD = 0; //Limpiar Puerto D
    
// INTERRUPCIONES
    INTCONbits.GIE = 1; //Activar interrupciones globales
    INTCONbits.PEIE = 1; //Activar interrupciones periféricas
    PIE1bits.ADIE = 1; // Habiliar interrupcion del conversor ADC
    PIR1bits.SSPIF = 0; //Limpiar bandera de interrupción del SPI
    PIE1bits.SSPIE = 1; //Activar bandera del SPI
    
// --------------- Oscilador --------------- 
    OSCCONbits.IRCF = 0b110 ; // establecerlo en 4 MHz
    OSCCONbits.SCS = 1; // utilizar oscilador interno
    
    spiInit(SPI_SLAVE_SS_EN, SPI_DATA_SAMPLE_END, SPI_CLOCK_IDLE_LOW, SPI_IDLE_2_ACTIVE); //Función de configuración del SPI
   
}