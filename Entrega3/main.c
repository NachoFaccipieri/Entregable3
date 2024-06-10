#include <avr/io.h>
#include <stdlib.h>
#include <stdio.h>
#include <avr/interrupt.h>
#define F_CPU 16000000UL
#include <util/delay.h> // Retardos por software
#include "lcd.h"
#include "sensor.h"
#include "I2C.h"
#include "RTC.h"
#include "SerialPort.h"

#define BR9600 (0x67)	// 0x67=103 configura BAUDRATE=9600@16MHz

//comunicación con la ISR
volatile char RX_Buffer=0;
uint8_t dejarDeRecibir = 0;
RTC_t tiempo;

void Iniciar_Sistema(void);
void chequeoEntrada(void);

int main(void)
{		
	uint8_t intRH,decRH,intT,decT,checkS,exito;
	
	Iniciar_Sistema();
	RTC_Init();
	tiempo.fecha.Day = dec_to_bcd((10));
	tiempo.fecha.Month = dec_to_bcd((06));
	tiempo.fecha.Year = dec_to_bcd((24));
	tiempo.hora.Second  = dec_to_bcd(20);
	tiempo.hora.Minute= dec_to_bcd(48);
	tiempo.hora.Hour  = dec_to_bcd(15);
	RTC_SetTime(&tiempo);
	
    while (1) 
    {
		chequeoEntrada();	
		if(!dejarDeRecibir){
			//Se llama a la función pedirTH para comenzar la configuracion del sensor y lectura de datos
			exito=pedirTH(&intRH,&decRH,&intT,&decT,&checkS);
					
			//Si se realizó una correcta lectura, la función pedirTH devuelve 1. Caso contrario hubo un error
			if(exito){
				RTC_GetHora(&tiempo.hora);
				RTC_GetFecha(&tiempo.fecha);
				SerialPort_Send_String("TEMP: ");
				//SerialPort_Send_Data("A");
				SerialPort_Send2_uint8_t(intT);
				SerialPort_Send_String("   HUMEDAD: ");
				SerialPort_Send2_uint8_t(intRH);
				SerialPort_Send_String("\n\r");
				SerialPort_Send_String("Fecha: ");
				SerialPort_Send2_uint8_t(bcd_to_dec(tiempo.fecha.Day));
				SerialPort_Send_String("/");
				SerialPort_Send2_uint8_t(bcd_to_dec(tiempo.fecha.Month));
				SerialPort_Send_String("/");
				SerialPort_Send2_uint8_t(bcd_to_dec(tiempo.fecha.Year));
				//SerialPort_Send_String("\n\r");
				SerialPort_Send_String("   Hora: ");
				SerialPort_Send2_uint8_t(bcd_to_dec(tiempo.hora.Hour));
				SerialPort_Send_String(":");
				SerialPort_Send2_uint8_t(bcd_to_dec(tiempo.hora.Minute));
				SerialPort_Send_String(":");
				SerialPort_Send2_uint8_t(bcd_to_dec(tiempo.hora.Second));
				
				SerialPort_Send_String("\n\r");
			}else{
				SerialPort_Send_String("\n\rERROR DE LECTURA\n\r");
			}
			_delay_ms(2000);		
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