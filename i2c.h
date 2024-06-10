#include <avr/io.h>
#define F_CPU 16000000UL
#define F_SCL 400000UL
#define TWBRX0 (((F_CPU/F_SCL)-16)/2)
#define TWBRX1 TWBRX0/4
#define TWBRX2 TWBRX0/16
#define TWBRX3 TWBRX0/64

#define NACK 0
#define ACK 1

// Inicializa el m�dulo I2C
void i2c_init(void);

// Envia una condici�n de inicio I2C y espera la confirmaci�n
void i2c_start(void);

// Escribe un byte de datos en el bus I2C
void i2c_write(unsigned char data);

uint8_t I2C_ReStart(void);

// Lee un byte de datos del bus I2C
// `isLast` indica si es el �ltimo byte a leer (env�a NACK si es el �ltimo, ACK si no)
unsigned char i2c_read(unsigned char isLast);

// Envia una condici�n de parada I2C
void i2c_stop(void);

