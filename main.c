#include "LibreriasPic/config_device.h"
#include "LibreriasPic/lcd_2x16.h"
#include <stdbool.h>

#define vel_sonido 34000.0 // cm / s


void PORT_Init(void); // Inicializamos los pines a utilizar
void TMR0_Init(void); // Configuramos el Timer0
void INTERRUPT_Global_Config(void); // Configuración global de las interrupciones
void INTERRUPT_INTx_Config(void);

/* V A R I A B L E S */
uint16_t counterTMR0 = 0;
float time = 0, distance = 0;
char strDistance[20];
volatile bool is_danger;

int main() {
    
    OSCILADOR_Init();
	PORT_Init();
    TMR0_Init();
    Lcd_Init();
    Lcd_Clear();
    INTERRUPT_Global_Config();
    INTERRUPT_INTx_Config();
    
    Lcd_Set_Cursor(1, 1);
    Lcd_Write_String("Distancia: ");
    
    LATB = 0b00001111;
    
    while ( 1 ) {
        /* Enviamos el pulso de ultrasonido */
        LATDbits.LATD0 = 1; // En alto el pin 
        __delay_us(15);
        LATDbits.LATD0 = 0;
        
        /* Leeremos el tiempo en alza del pin echo */
        while ( PORTDbits.RD1 == 0 ); // Esperamos que la señal pase a alta
        T0CONbits.TMR0ON = 1; // Habilito el timer
        
        while ( PORTDbits.RD1 == 1 ); // Esperamos todo el tiempo que la señal
        // está en alta
        T0CONbits.TMR0ON = 0; // Deshabilito el timer
        
        counterTMR0 = TMR0; // Guardamos el tiempo del pulso estuvo en alta
        TMR0 = 0; // Reiniciamos el valor de TMR0 para la siguiente medida
        
        time = counterTMR0 * 2.0 / 1000000.0; // Convertimos cuentas en tiempo
        time = time / 2; // Entre dos ya que es el tiempo de ida y vuelta
        
        distance = vel_sonido * time;
        
        /* I M P R I M I R    E N    L C D */
        
        Lcd_Set_Cursor(2, 1);
        sprintf(strDistance, "%06.02f", distance);
        Lcd_Write_String(strDistance);
        
        /* V A L I D A C I Ó N  D I S T A N C I A */
        
        if ( distance < 10 ) {
            is_danger = true;
            
            while ( is_danger ) {
                // Recordar que en el entrenador al pulsar los botones
                // dan una señal en baja
                LATBbits.LATB4 = 1;
                LATBbits.LATB5 = 1;
                LATBbits.LATB6 = 0;
                __delay_ms(100);
                LATBbits.LATB5 = 0;
                LATBbits.LATB6 = 1;
                __delay_ms(100);
            }
            
            
        }
        
        
        LATBbits.LATB4 = 0;
        LATBbits.LATB5 = 0;
        LATBbits.LATB6 = 0;
        
    }
}

/* F U N C I O N E S */

// Implementación de rutinas de interrupción
void __interrupt() RutinaServicioInterrupcion_ISR(void) {
    // Se evaluan el bit de bandera de la fuente de interrupción
    if ( INTCONbits.INT0IE == 1 && INTCONbits.INT0IF == 1 ) {
        // Tarea de interrupción
        is_danger = false;
        // Limpiar el flag para habilitar una nueva interrupción
        INTCONbits.INT0IF = 0;
        
    }
}

void PORT_Init(void) {
    // L C D pines y HC-SR04
    ANSELD = 0x00; // Todos los pines del puerto D como digitales
    TRISD = 0b00000010; // Solo RD1 será entrada digital
    // RD0: Trigger /\ RD1: Echo
    
    // LEDS, BOTONES Y BUZZER
    ANSELB = 0x00; // Todos los pines del puerto B como digitales
    TRISB = 0b00001111; // 0: Salidas digitales /\ 1: Entradas digitales
    // RB7 : Pin que no se usa
    // RB6 : Pin para el led verde
    // RB5 : Pin para el led rojo
    // RB4 : Pin para el buzzer
    // RB3 : Pin del boton del entrenador 
    // RB2 : Pin del boton del entrenador
    // RB1 : Pin del boton del entrenador
    // RB0 : Pin del boton del entrenador
}

void TMR0_Init(void) {
    T0CONbits.T08BIT = 0; // Resolución de 16 bits
    T0CONbits.T0CS = 0; // Fosc, modo temporizador
    T0CONbits.PSA = 0; // Preescaler activo
    T0CONbits.T0PS = 0b010; // Preescaler x8 necesito un rango de [0:0.2] s
    
    TMR0 = 0; // Que empice desde el inicio
    //T0CONbits.TMR0ON = 1; // Habilito el uso de timers
}

void INTERRUPT_Global_Config(void) {
    // Modo de trabajo de interrupciones: Modo convencional
    RCONbits.IPEN = 0; // No establece niveles de prioridad en las interrupciones

    // Habilitar las interrupciones globales
    INTCONbits.GIE = 1;
}

void INTERRUPT_INTx_Config(void){
    INTCONbits.INT0IE = 1; // Habilita la interrupción por pin INT0
    INTCONbits.INT0IF = 0; // No manda una interrupción al iniciar
    // INT0 por defecto es de alta prioridad así que no se configura
    INTCON2bits.INTEDG0 = 0; // Config INT0 por flanco de bajada

}