#include "sensor.h"

uint8_t pedirTH(uint8_t* intRH, uint8_t* decRH, uint8_t* intT,uint8_t* decT, uint8_t* checkS ){
	uint8_t sum=0,i;
	
	//------Host's Signal------
	//Se configura el sensor para encuestarlo:
	//Pin como salida -> Enviar un 1 (5ms) -> enviar un 0 (18ms) "start signal" y un 1 (20-40us) "wait for sensor's response"
	
	DDRC |= (1<<PINC0);
	PORTC |= (1<<PINC0);
	_delay_us(5);
	PORTC &= ~(1<<PINC0);
	_delay_ms(18);
	PORTC |= (1<<PINC0);
	_delay_us(30);
	
	
	//------Sensor's signal-----
	//DDRC se configura como entrada para recibir respuesta del sensor
	
	//El sensor envía dos señales de 80us cada una y comienza el envío de datos
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
