//*****************************************************************************
/*
 * File:   main.c
 * Author: Gabriel Carrera 21216
 * Created on July 31, 05:14 PM
 */

 // Código de PIC Maestro

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
#include "LCD.h" // librería LCD
#include "SPI.h" // librería SPI

// --------------- Frecuencia ---------------
#define _XTAL_FREQ 4000000 // Frecuencia 4MHz


// --------------- Variables ---------------
unsigned char voltaje1; //Variable para guardar valor del voltaje del potenciómetro 1
unsigned char voltaje2; //Variable para guardar valor del voltaje del potenciómetro 2
unsigned char contador = 0; //Variable para guardar valor del voltaje del potenciómetro 2
int vol1; //Variable para guardar valor del voltaje del potenciómetro 1 mapeado
int vol2; //Variable para guardar valor del voltaje del potenciómetro 2 mapeado
unsigned int unidad1; //Variable para guardar unidades del voltaje del AN0
unsigned int decima1; //Variable para guardar decimas del voltaje del AN0
unsigned int centesima1; //Variable para guardar centesimas del voltaje del AN0
unsigned int unidad2; //Variable para guardar unidades del voltaje del AN1
unsigned int decima2; //Variable para guardar decimas del voltaje del AN1
unsigned int centesima2; //Variable para guardar centesimas del voltaje del AN0
char buffer[20]; //Arreglo para guardar valores de voltajes
char buffer1[20]; //Arreglo para guardar valores de voltajes
char buffer2[20]; //Arreglo para guardar valor de contador


// --------------- Prototipos ---------------
void setup(void); // funcion de configuracion
int map(unsigned char value, int inputmin, int inputmax, int outmin, int outmax){ //función para mapear valores
    return ((value - inputmin)*(outmax-outmin)) / (inputmax-inputmin)+outmin;}


// --------------- Loop principal ---------------
void main(void) {
    setup(); // Llamada a la funcion de configuracion
    
    //------------LCD------------------------
    Lcd_Init(); // Iniciar la LCD
    Lcd_Clear(); // Limpiar la LCD
    
    Lcd_Set_Cursor(1,7); //Cursor en (1,7)
    Lcd_Write_String("S2:"); //Escribir S2 en pantalla
    
    Lcd_Set_Cursor(1,1); //Cursor en (1,1)
    Lcd_Write_String("S1:"); //Escribir S1 en pantalla
    
    Lcd_Set_Cursor(1,14); //Cursor en (1,14)
    Lcd_Write_String("S3:"); //Escribir S3 en pantalla
    
    while(1){
         PORTCbits.RC2 = 0; // Iniciar comunicación con esclavo 1 
        __delay_ms(1); //delay de 1ms

        spiWrite(1); // Enviar un 1 al esclavo para solicitar datos del potenciómetro
        voltaje1 = spiRead(); // Leer valor de voltaje del potenciómetro del esclavo 1
         
        __delay_ms(1); // delay de 1ms
        PORTCbits.RC2 = 1; // Terminar comunicación con esclavo 1
        
        PORTCbits.RC2 = 0; // Iniciar comunicación con esclavo 1 
        __delay_ms(1); // delay de 1ms
        
        spiWrite(3); // Enviar un 1 al esclavo para solicitar datos del contador
        contador = spiRead(); // Leer datos del contador
        
        __delay_ms(1); //delay de 1ms
        PORTCbits.RC2 = 1; // Terminar comunicación con esclavo 1
        
        PORTCbits.RC1 = 0; //Iniciar comunicación con esclavo 2 
        __delay_ms(1); //delay de 1ms
        
        spiWrite(2); //Enviar un 2 al esclavo 2 para solicitar datos del potenciómetro
        voltaje2 = spiRead(); //Leer datos del contador
        
        __delay_ms(1); //delay de 1ms
        PORTCbits.RC1 = 1; //Terminar comunicación con esclavo 2
        
        
        vol1 = map(voltaje1, 0, 255, 0, 100); //mapear valor del voltaje de 0 a 100
        unidad1 = (vol1*5)/100; //Separar las unidades del valor del voltaje
        decima1 = ((vol1*5)/10)%10; //Separar las decimas del valor del voltaje
        centesima1 = (vol1*5)%10; //Separar las centesimas del valor del voltaje
        Lcd_Set_Cursor(2,1); //Cursor en (1,7)
        sprintf(buffer, "%d.%d%dV" , unidad1 , decima1 , centesima1 ); //convertir variable a una cadena de caracteres
        Lcd_Write_String(buffer); //Mostrar cadena de caracteres en pantalla
               
        vol2 = map(voltaje2, 0, 255, 0, 100); //mapear valor del voltaje de 0 a 100
        unidad2 = (vol2*5)/100; //Separar las unidades del valor del voltaje
        decima2 = ((vol2*5)/10)%10; //Separar las decimas del valor del voltaje
        centesima2 = (vol2*5)%10; //Separar las centesimas del valor del voltaje
        Lcd_Set_Cursor(2,7); //Cursor en (1,7)
        sprintf(buffer1, "%d.%d%dV" , unidad2 , decima2 , centesima2 ); //convertir variable a una cadena de caracteres
        Lcd_Write_String(buffer1); //Mostrar cadena de caracteres en pantalla
        
        Lcd_Set_Cursor(2,14); //Cursor en (2,14)
        sprintf(buffer2, "%03d" , contador); //convertir variable a una cadena de caracteres
        Lcd_Write_String(buffer2); //Mostrar cadena de caracteres en pantalla
    }
    
}

// --------------- Rutina de  interrupciones ---------------
void __interrupt() isr(void){ // interrupciones
    
}

// --------------- Setup General ---------------
void setup(void){
    
// --------------- Definir analogicas ---------------
    ANSEL = 0; //Puertos como I/O digitales
    ANSELH = 0; //Puertos como I/O digitales
    
// --------------- Configurar puertos --------------- 
    
    TRISB = 0; // Puerto B como salida - LCD
    TRISCbits.TRISC1 = 0; // Pin para segundo esclavo
    TRISCbits.TRISC2 = 0; // Pin para primer esclavo
    TRISD = 0; // Puerto D como salida - LCD
    //TRISE = 0;
    
    // Cerrar conexion con ambos esclavos
    PORTCbits.RC1 = 1; // Slave deselect - S2
    PORTCbits.RC2 = 1; // Slave deselect - S1
    
// --------------- Limpiar puertos ---------------    
    PORTA = 0;
    PORTB = 0;
    PORTC = 0;
    PORTD = 0;
    PORTE = 0;    
    
// --------------- Habilitar pullups --------------- 
    //OPTION_REGbits.nRBPU = 0; 
    //WPUBbits.WPUB6 = 1;
    //WPUBbits.WPUB7 = 1; 

// --------------- Banderas e interrupciones --------------- 
    //INTCONbits.GIE = 1;   // Habilitar interrupciones globales
    //INTCONbits.PEIE = 1;  // Habilitar interrupciones de perifericas
    //INTCONbits.RBIE = 1;  // Habilitar interrupciones en PORTB
    
    //IOCBbits.IOCB6 = 1;   // Habilitar interrupciones en RB6
    //IOCBbits.IOCB7 = 1;   // Habilitar interrupciones en RB7
    
    // Utilizar la libreria para habilitar pullup e IOCB de cada boton deseado
    //ioc_init(7);
    //ioc_init(6);
    
    //INTCONbits.RBIF = 0;  // Limpiar bandera de interrupcion de PORTB
    
// --------------- Oscilador --------------- 
    OSCCONbits.IRCF = 0b110 ; // establecerlo en 4 MHz
    OSCCONbits.SCS = 1; // utilizar oscilador interno
    
    // Inicializacion del SPI con la libreria
    spiInit(SPI_MASTER_OSC_DIV4, SPI_DATA_SAMPLE_END, SPI_CLOCK_IDLE_LOW, SPI_IDLE_2_ACTIVE); //Función Librería de Setup de comunicación de ISP

}