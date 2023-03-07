#include "config_device.h"
#include "lcd_2x16.h"

#define vel_sonido 34000.0 // cm / s


void PORT_Init(void); // Inicializamos los pines a utilizar
void TMR0_Init(void); // Configuramos el Timer0

/* V A R I A B L E S */
uint16_t counterTMR0 = 0;
float time = 0, distance = 0;
char strDistance[20];

int main() {
    
    OSCILADOR_Init();
	PORT_Init();
    TMR0_Init();
    Lcd_Init();
    Lcd_Clear();
    
    Lcd_Set_Cursor(1, 1);
    Lcd_Write_String("Distancia: ");
    
    
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
        sprintf(strDistance, "%.04f m", distance);
        Lcd_Write_String(strDistance);
        
    }
}

/* F U N C I O N E S */

void PORT_Init(void) {
    // L C D pines
    ANSELD = 0x00; // Todos los pines del puerto D como digitales
    TRISD = 0b00000010; // Solo RD1 será entrada digital
    // RD0: Echo /\ RD1: Trigger
}

void TMR0_Init(void) {
    T0CONbits.T08BIT = 0; // Resolución de 16 bits
    T0CONbits.T0CS = 0; // Fosc, modo temporizador
    T0CONbits.PSA = 0; // Preescaler activo
    T0CONbits.T0PS = 0b010; // Preescaler x8 necesito un rango de [0:0.2] s
    
    TMR0 = 0; // Que empice desde el inicio
    //T0CONbits.TMR0ON = 1; // Habilito el uso de timers
}