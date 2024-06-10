#include <avr/io.h>
#include <avr/interrupt.h>
#include "SerialPort.h"

#define BR9600 (0x67)	// 0x67=103 configura BAUDRATE=9600@16MHz

//mensajes de bienvenida y despedida
char msg1[] = "Si presiona 's' termina el programa:\n\r";
char msg2[] = "\n\rSe detuvo la recepcion";

//comunicación con la ISR
volatile char RX_Buffer=0;

volatile char dato = 0;
	
SerialPort_Init(BR9600); 		// Se inicializa en formato 8N1 y BAUDRATE = 9600bps
SerialPort_TX_Enable();			// Activo el Transmisor del Puerto Serie
SerialPort_RX_Enable();			// Activo el Receptor del Puerto Serie
SerialPort_Send_String(msg1);   // Envío el mensaje de Bienvenida
SerialPort_RX_Interrupt_Enable();	// Activo Interrupción de recepcion.
sei();								// Activo la mascara global de interrupciones (Bit I del SREG en 1)

if (UCSR0A & (1<<RXC0)) {  // Se verifica si hay datos disponibles en el buffer de recepción
	char receivedChar = UDR0;
	if (receivedChar == 's') {
		//Toggle_Receiving();
	}
}

//Si RX_BUFFER es distinto de 0, es porque se recibió un dato
if(RX_Buffer){ // recepción NO Bloqueante
	// Si presionan 's' se termina el programa
	if(RX_Buffer == 's'){
		SerialPort_Wait_For_TX_Buffer_Free(); // Espero a que el canal de transmisión este libre (bloqueante)
		SerialPort_Send_Data(RX_Buffer);	  // Envío el dato recibido
		RX_Buffer = 0;
	}
}

//Cuando se recibe un dato, la interrupción ISR(USART_RX_vect) se activa
// Rutina de Servicio de Interrupción de Byte Recibido
ISR(USART_RX_vect){
	RX_Buffer = UDR0; //la lectura del UDR borra flag RXC
}





/*
DDRB = 0xFF; //Se setea el puerto B como entrada

//Se habilita la recepción
UCSR0B = (1<<RXEN0);

//Longitud de dato
//Para trabajar con 8 bits, UCSZ02 debe valer 0 y UCSZ00/01 deben valer 1
UCSR0B = (0<<UCSZ02);
UCSR0C = UCSR0C = (1<<UCSZ01)|(1<<UCSZ00);

//UBRR0L = 103; //Baud rate 9600bps
*/
