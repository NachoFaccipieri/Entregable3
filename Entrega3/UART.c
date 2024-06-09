#include <avr/io.h>
#include <avr/interrupt.h>
#include "serialPort.h"

#define BR9600 (0x67)	// 0x67=103 configura BAUDRATE=9600@16MHz

/*
DDRB = 0xFF; //Se setea el puerto B como entrada

//Se habilita la recepci�n
UCSR0B = (1<<RXEN0);

//Longitud de dato
//Para trabajar con 8 bits, UCSZ02 debe valer 0 y UCSZ00/01 deben valer 1
UCSR0B = (0<<UCSZ02);
UCSR0C = UCSR0C = (1<<UCSZ01)|(1<<UCSZ00);

//UBRR0L = 103; //Baud rate 9600bps
*/



//4- transmitir cadenas +  eco usando interrupci�n Rx

//mensajes de bienvenida y despedida
char msg1[] = "Si presiona 's' termina el programa:\n\r";
char msg2[] = "\n\rHasta luego!";

//comunicaci�n con la ISR
volatile char RX_Buffer=0;

volatile char dato = 0;
	
SerialPort_Init(BR9600); 		// Se inicializa en formato 8N1 y BAUDRATE = 9600bps
SerialPort_TX_Enable();			// Activo el Transmisor del Puerto Serie
SerialPort_RX_Enable();			// Activo el Receptor del Puerto Serie
SerialPort_Send_String(msg1);   // Env�o el mensaje de Bienvenida
SerialPort_RX_Interrupt_Enable();	// Activo Interrupci�n de recepcion.
sei();								// Activo la mascara global de interrupciones (Bit I del SREG en 1)

if(RX_Buffer){ // recepci�n NO Bloqueante
	// Si presionan 's' se termina el programa
	if(RX_Buffer == 's'){
		SerialPort_Send_String(msg2);  // Env�o el string de despedida
		while(1);
	}
	else{	// Eco:
		SerialPort_Wait_For_TX_Buffer_Free(); // Espero a que el canal de transmisi�n este libre (bloqueante)
		SerialPort_Send_Data(RX_Buffer);	  // Env�o el dato recibido
	}
	RX_Buffer=0;
}
//otra tarea

// Rutina de Servicio de Interrupci�n de Byte Recibido
ISR(USART_RX_vect){
	RX_Buffer = UDR0; //la lectura del UDR borra flag RXC
}