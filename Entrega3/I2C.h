#include <avr/io.h>

// Inicializa el módulo I2C
void i2c_init(void);

// Envia una condición de inicio I2C y espera la confirmación
void i2c_start(void);

// Escribe un byte de datos en el bus I2C
void i2c_write(unsigned char data);

// Lee un byte de datos del bus I2C
// `isLast` indica si es el último byte a leer (envía NACK si es el último, ACK si no)
unsigned char i2c_read(unsigned char isLast);

// Envia una condición de parada I2C
void i2c_stop(void);