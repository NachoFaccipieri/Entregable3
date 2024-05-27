#include <avr/io.h>


int main(void)
{
	
	uint16_t tSubida, tBajada;
	
	//Se configura el timer en modo CTC con un prescaler de 1024
	TCCR1B = (1<<WGM12) | (1<<CS12) | (1<<CS10);	//ICP1 flanco subida. No prescaler
	
	//------Host's Signal------
	//Para encuestar el sensor se lo configura:
	//Pin como salida -> Timer como temporizador -> Enviar 0 (18ms) "start signal" y un 1 (20-40us) "wait for sensor's response"
	DDRC |= (1<<PINC0);	
	
	TCCR1A=0;							//Modo normal
	
	
	//------Sensor's signal-----
	//DDRC se configura como entrada para recibir respuesta del sensor
	DDRC &= ~(1<<PINC0);
	
    while (1) 
    {
		
    }
}

