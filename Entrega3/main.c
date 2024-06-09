#include <avr/io.h>
#include <stdlib.h>
#include <stdio.h>
#define F_CPU 16000000UL
#include <util/delay.h> // Retardos por software
#include "lcd.h"
#include "sensor.h"

int main(void)
{	
	uint8_t intRH,decRH,intT,decT,checkS,exito;
	
	//Inicio del LCD
	LCDinit();
	LCDclr();
	LCDGotoXY(0,0);
	
    while (1) 
    {
		//Se llama a la función pedirTH para comenzar la configuracion del sensor y lectura de datos	
		exito=pedirTH(&intRH,&decRH,&intT,&decT,&checkS);
		
		//Si se realizó una correcta lectura, la función pedirTH devuelve 1. Caso contrario hubo un error
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
    }
}