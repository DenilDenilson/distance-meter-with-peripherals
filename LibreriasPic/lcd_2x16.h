 
#ifndef LCD_2X16_H
#define	LCD_2X16_H

#include "config_device.h"

//****************************************************************************
//    CONFIGURACIÓN DE LOS PINES DE INTERFACE
//****************************************************************************

/* Define el puerto a donde se conectará el BUS de datos del LCD
 * Se utilizará el nible alto del puerto escogido (ejem. RD4,RD5,RD6 Y RD7) */
#define  LCD_DATA_OUT    LATD       // Registro LATx del puerto
#define  LCD_DATA_TRIS   TRISD      // Registro TRISx del puerto

/* Define el puerto a donde se conectarán las líneas de control del LCD
 * EN y RS (Puede ser el mismo puerto del BUS de datos) */
#define  LCD_CTRL_OUT    LATD      // Registro PORTx del puerto
#define  LCD_CTRL_TRIS   TRISD      // Registro TRISx del puerto

//Define los numeros de bits a donde se conectará el modulo LCD
#define RS	2 	// Pin Register Select
#define EN	3	// Pin Enable
#define D4	4	//define el pin del MCU conectado LCD D4
#define D5	5	//define el pin del MCU conectado LCD D5
#define D6	6	//define el pin del MCU conectado LCD D6
#define D7	7	//define el pin del MCU conectado LCD D7

//Prototipos de funciones
void Lcd_Port(unsigned char a);
void Lcd_Cmd(unsigned char a);
void Lcd_Clear( void );
void Lcd_Set_Cursor(unsigned char a, unsigned char b);
void Lcd_Init( void );
void Lcd_Write_Char(unsigned char a);
void Lcd_Write_String(unsigned char *a);
void Lcd_Shift_Right( void );
void Lcd_Shift_Left( void );

#endif	/* LCD_2X16_H */

