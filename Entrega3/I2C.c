#include "I2C.h"

void i2c_init(void){
	TWSR=0x00;    // set prescaler bits to zero
	TWBR=152;     // SCL frequency is 50K for XTAL = 16M
	TWCR=0x04;    // enable the TWI module
}

void I2C_Start(void){
	TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN);
	while ((TWCR & (1 << TWINT)) == 0);
}
void I2C_Write(unsigned char data){
	TWDR = data;
	TWCR = (1 << TWINT) | (1 << TWEN);
	while ((TWCR & (1 << TWINT)) == 0);
}

uint8_t I2C_ReStart( void )
{
	TWCR = (1<<TWINT)|(1<<TWSTA)|(1<<TWEN);

	while ( !(TWCR & (1<<TWINT)) );

	return 1;
}


unsigned char I2C_Read(unsigned char isLast){
	if (isLast == 0){  // send ACK
		TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWEA);
	}
	else{  // send NACK
		TWCR = (1 << TWINT) | (1 << TWEN);
	}
	while ((TWCR & (1 << TWINT)) == 0);
	return TWDR;
}

void I2C_Stop(void){
	TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWSTO);
}