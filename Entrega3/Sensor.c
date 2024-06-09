#include "sensor.h"

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
