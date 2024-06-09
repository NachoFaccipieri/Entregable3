#include <avr/io.h>
#include <stdlib.h>
#include <stdio.h>
#include <avr/interrupt.h>
#define F_CPU 16000000UL
#include <util/delay.h> // Retardos por software
#include "lcd.h"
#include "sensor.h"


#define BR9600 (0x67)	//103 en decimal
volatile char RX_Buffer=0;

int main(void)
{	
	uint8_t intRH,decRH,intT,decT,checkS,exito;
	uint8_t guardar, segsUn, segsDec, segs, i;
	//Inicio del LCD
	LCDinit();
	LCDclr();
	LCDGotoXY(0,0);
	
    while (1) 
    {
		LCDclr();
		LCDGotoXY(0,0);
		i2c_start();
		i2c_write(0b11010001);
		guardar = i2c_read(8);

		i2c_stop();
		segsUn = guardar & 0b00001111;
		segsDec = guardar & 0b01110000;
		segs = segsDec*10;
		segs = segs + segsUn;
		LCDescribeDato(segs,2);
		
		_delay_ms(1000);



		
		/*
		//Se llama a la funci�n pedirTH para comenzar la configuracion del sensor y lectura de datos	
		exito=pedirTH(&intRH,&decRH,&intT,&decT,&checkS);
		
		//Si se realiz� una correcta lectura, la funci�n pedirTH devuelve 1. Caso contrario hubo un error
		if(exito){
			LCDclr();
			LCDstring("TEMP: ",6);
			LCDescribeDato(intT,2);
			LCDsendChar(',');
			LCDescribeDato(decT,2);
			LCDGotoXY(0,1);
			LCDstring("HUMEDAD: ",9);
			LCDescribeDato(intRH,2);
			LCDsendChar(',');
			LCDescribeDato(decRH,2);
		}else{
			LCDstring("Error en lectura",16);
		}
		_delay_ms(2000);
		LCDGotoXY(0,0);
		*/
    }
}
