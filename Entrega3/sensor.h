#include <avr/io.h>
#define F_CPU 16000000UL
#include <util/delay.h>

uint8_t pedirTH(uint8_t* , uint8_t* , uint8_t* ,uint8_t* , uint8_t*);
void leer8B(uint8_t*);