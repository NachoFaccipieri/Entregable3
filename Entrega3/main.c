#include <avr/io.h>
#include <stdlib.h>
#include <stdio.h>
#include <avr/interrupt.h>
#define F_CPU 16000000UL
#include <util/delay.h> // Retardos por software
#include "lcd.h"
#include "sensor.h"

#include "SerialPort.h"
#define BR9600 (0x67)	// 0x67=103 configura BAUDRATE=9600@16MHz

//comunicación con la ISR
volatile char RX_Buffer=0;
uint8_t dejarDeRecibir = 0;

void Iniciar_Sistema(void);
void chequeoEntrada(void);


int main(void)
{	
	uint8_t intRH,decRH,intT,decT,checkS,exito;
	
	uint8_t sec, min, hs, dia, date, mes, anio, i;
	
	Iniciar_Sistema();
	
    while (1) 
    {

		
		chequeoEntrada();	
		if(!dejarDeRecibir){
			//Se llama a la función pedirTH para comenzar la configuracion del sensor y lectura de datos
			exito=pedirTH(&intRH,&decRH,&intT,&decT,&checkS);
			
			i2c_start();
			i2c_write(0b11010001);
			i2c_write(0x00);
			sec = i2c_read(1);
			min = i2c_read(1);
			hs = i2c_read(1);
			dia= i2c_read(1);
			date = i2c_read(1);
			mes = i2c_read(1);
			anio = i2c_read(1);
			i2c_stop();
			
			sec = sec & 0x7F;
			min = min & 0x7F;
					
			//Si se realizó una correcta lectura, la función pedirTH devuelve 1. Caso contrario hubo un error
			if(exito){
				SerialPort_Send_String("TEMP: ");
				//SerialPort_Send_Data("A");
				SerialPort_Send2_uint8_t(intT);
				SerialPort_Send_String("\n\rHUMEDAD: ");
				SerialPort_Send2_uint8_t(intRH);
				SerialPort_Send_String("\n\r");
				SerialPort_Send_String("Hora: ");
				SerialPort_Send2_uint8_t(sec);
				SerialPort_Send2_uint8_t(min);
				SerialPort_Send_String("\n\r");
				}else{
				SerialPort_Send_String("\n\rERROR DE LECTURA");
			}
			_delay_ms(1000);		
		}
    }
}

void Iniciar_Sistema(){	
	SerialPort_Init(BR9600); 		// Se inicializa en formato 8N1 y BAUDRATE = 9600bps
	SerialPort_TX_Enable();			// Activo el Transmisor del Puerto Serie
	SerialPort_RX_Enable();			// Activo el Receptor del Puerto Serie
	SerialPort_Send_String("Presionando S se detiene/reanuda el sistema\n\r");   // Envío el mensaje
	SerialPort_RX_Interrupt_Enable();	// Activo Interrupción de recepcion.
	sei();								// Activo la mascara global de interrupciones (Bit I del SREG en 1)
}

void chequeoEntrada(){
	if(RX_Buffer){ // recepción NO Bloqueante
		// Si presiona 's', se deja de recibir o se comienza a recibir
		if(RX_Buffer == 's'){
			dejarDeRecibir = !dejarDeRecibir;
			
			if(dejarDeRecibir){
				SerialPort_Send_String("Recepcion detenida\n\r");
			} else {
				SerialPort_Send_String("Recepcion reanudada\n\r");
			}
		} 		
		RX_Buffer=0;
	}
}

ISR(USART_RX_vect){
	RX_Buffer = UDR0; //la lectura del UDR borra flag RXC
}


/*		
i2c_start();
i2c_write(0b11010001);
sec = i2c_read(1);
min = i2c_read(1);
hs = i2c_read(1);
dia= i2c_read(1);
date = i2c_read(1);
mes = i2c_read(1);
anio = i2c_read(1);
i2c_stop();

segsUn = guardar & 0b00001111;
segsDec = guardar & 0b01110000;
segs = segsDec*10;
segs = segs + segsUn;
LCDescribeDato(segs,2);
*/