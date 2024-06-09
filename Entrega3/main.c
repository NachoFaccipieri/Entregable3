#include <avr/io.h>
#include <stdlib.h>
#include <stdio.h>
#define F_CPU 16000000UL
#include <util/delay.h> // Retardos por software
#include "lcd.h"
#include "sensor.h"


//uint8_t pedirTH(uint8_t* , uint8_t* , uint8_t* ,uint8_t* , uint8_t*);
//void leer8B(uint8_t*);

int main(void)
{	
	uint8_t intRH,decRH,intT,decT,checkS,exito;
	
	//DDRC &= ~(1<<PINC0);
	
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

/*

uint8_t pedirTH(uint8_t* intRH, uint8_t* decRH, uint8_t* intT,uint8_t* decT, uint8_t* checkS ){
	uint8_t sum=0,i;
	
	DDRC |= (1<<PINC0);
	PORTC |= (1<<PINC0);
	_delay_us(5);
	PORTC &= ~(1<<PINC0);
	_delay_ms(18);
	PORTC |= (1<<PINC0);
	_delay_us(30);
	
	DDRC &= ~(1<<PINC0);
	_delay_us(160);
	
	leer8B(intRH);
	leer8B(decRH);
	leer8B(intT);
	leer8B(decT);
	leer8B(checkS);
	
	sum=(*intRH)+(*decRH)+(*intT)+(*decT);

	return sum==(*checkS);
}

void leer8B(uint8_t* auxBits){
	int i;
	*auxBits=0;
	for(i=0;i<8;i++){
		while((PINC & (1<<PINC0))==0){}
		_delay_us(40);
		if((PINC & (1<<PINC0))==1){
			*auxBits |= (1<<(7-i));
			_delay_us(30);
		}	
	}
}
*/
