
#include "lcd_2x16.h"

//Desarrollo de funciones
void Lcd_Port(unsigned char a)
{
	if(a & 1)
	    //D4 = 1;
		LCD_DATA_OUT |= (1<<D4);
	else
		//D4 = 0;
		LCD_DATA_OUT &= ~(1<<D4);
	if(a & 2)
		//D5 = 1;
		LCD_DATA_OUT |= (1<<D5);
	else
		//D5 = 0;
		LCD_DATA_OUT &= ~(1<<D5);
	if(a & 4)
		//D6 = 1;
		LCD_DATA_OUT |= (1<<D6);
	else
		//D6 = 0;
		LCD_DATA_OUT &= ~(1<<D6);
	if(a & 8)
		//D7 = 1;
		LCD_DATA_OUT |= (1<<D7);
	else
		//D7 = 0;
		LCD_DATA_OUT &= ~(1<<D7);
}
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
void Lcd_Cmd(unsigned char a)
{
	LCD_CTRL_OUT &= ~(1<<RS);// => RS = 0           
	Lcd_Port(a);
	LCD_CTRL_OUT |= (1<<EN);// => EN = 1           
    __delay_ms(4);
	LCD_CTRL_OUT &= ~(1<<EN);// => EN = 0          
}
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//Funcion para limpiar la pantalla LCD
void Lcd_Clear( void )
{
	Lcd_Cmd(0);
	Lcd_Cmd(1);
}
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//Funcion para Posicionarse en una dirección específica de la pantalla
void Lcd_Set_Cursor(unsigned char a, unsigned char b)
{
	char temp,z,y;
	if(a == 1)
	{
	  temp = 0x80 + b - 1;
		z = temp>>4;
		y = temp & 0x0F;
		Lcd_Cmd(z);
		Lcd_Cmd(y);
	}
	else if(a == 2)
	{
		temp = 0xC0 + b - 1;
		z = temp>>4;
		y = temp & 0x0F;
		Lcd_Cmd(z);
		Lcd_Cmd(y);
	}
}
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//Funcion para inicializar la pantalla
void Lcd_Init( void )
{
	/* Configurar las direcciones de los pines de interface del LCD */
	LCD_DATA_TRIS &= ~(0xF0); //Configuro los pines del BUS de datos del LCD como salida
	LCD_CTRL_TRIS &= ~((1<<EN)|(1<<RS));//Configuro los pines de control del LCD como salida
	
	Lcd_Port(0x00);
	__delay_ms(20);
	Lcd_Cmd(0x03);
	__delay_ms(5);
	Lcd_Cmd(0x03);
	__delay_ms(11);
	Lcd_Cmd(0x03);
	/////////////////////////////////////////////////////
	Lcd_Cmd(0x02);
	Lcd_Cmd(0x02);
	Lcd_Cmd(0x08);
	Lcd_Cmd(0x00);
	Lcd_Cmd(0x0C);
	Lcd_Cmd(0x00);
	Lcd_Cmd(0x06);
}
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//Funcion para escribir un caracter por pantalla
void Lcd_Write_Char(unsigned char a)
{
   char temp,y;
   temp = a&0x0F;
   y = a&0xF0;
   LCD_CTRL_OUT |= (1<<RS);// => RS = 1                
   Lcd_Port(y>>4);          //Transferencia de datos (formto 4 bits)
   LCD_CTRL_OUT |= (1<<EN);// => EN = 1
   __delay_us(40);
   LCD_CTRL_OUT &= ~(1<<EN);// => EN = 0
   Lcd_Port(temp);          //Transferencia de datos (formto 4 bits)
   LCD_CTRL_OUT |= (1<<EN);// => EN = 1
   __delay_us(40);
   LCD_CTRL_OUT &= ~(1<<EN);// => EN = 0
}
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//Funcion para estribir un string por pantalla
void Lcd_Write_String(unsigned char *a)
{
	int i;
	for(i=0;a[i]!='\0';i++)
	   Lcd_Write_Char(a[i]);
}
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
void Lcd_Shift_Right( void )
{
	Lcd_Cmd(0x01);
	Lcd_Cmd(0x0C);
}
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
void Lcd_Shift_Left( void )
{
	Lcd_Cmd(0x01);
	Lcd_Cmd(0x08);
}
