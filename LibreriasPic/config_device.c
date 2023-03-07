
#include "config_device.h"

// Desarrollo de funciones
void OSCILADOR_Init(void) {
    // Configuración del configurador interno a 16Mhz
    OSCCONbits.IRCF = 0b111; // Osc interno a 16Mhz
    OSCCONbits.SCS0 = 0;
    OSCCONbits.SCS1 = 1; // Determina el osc interno para la cpu y periféricos
}

