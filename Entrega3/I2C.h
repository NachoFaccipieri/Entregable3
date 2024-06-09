#include <avr/io.h>

// Inicializa el m�dulo I2C
void i2c_init(void);

// Envia una condici�n de inicio I2C y espera la confirmaci�n
void i2c_start(void);

// Escribe un byte de datos en el bus I2C
void i2c_write(unsigned char data);

// Lee un byte de datos del bus I2C
// `isLast` indica si es el �ltimo byte a leer (env�a NACK si es el �ltimo, ACK si no)
unsigned char i2c_read(unsigned char isLast);

// Envia una condici�n de parada I2C
void i2c_stop(void);