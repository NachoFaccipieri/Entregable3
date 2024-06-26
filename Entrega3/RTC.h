#ifndef RTC_H_
#define RTC_H_

#include <avr/io.h>
#include "I2C.h"

typedef struct {
	uint8_t Second;  /* 0..59 */
	uint8_t Minute;  /* 0..59 */
	uint8_t Hour;    /* 0..23 */
} Hora_t;

typedef struct {
	uint8_t Day;     /* 1..31 */
	uint8_t Month;   /* 1..12 */
	uint8_t Year;    /* 00..99 */
} Fecha_t;

typedef struct {
	Hora_t hora;
	Fecha_t fecha;
} RTC_t;


#define DS3231_ADDRESS 0x68
#define DS3231_SECONDS 0x00
#define DS3231_MINUTES 0x01
#define DS3231_HOURS 0x02

#define DS3231_WEEKDAY 0x03
#define DS3231_DAYS 0x04
#define DS3231_MONTHS 0x05
#define DS3231_YEARS 0x06

#define MASK_SEC 0b01111111
#define MASK_MIN 0b01111111
#define MASK_HORA 0b00111111
#define MASK_DAY 0b00111111
#define MASK_MES 0b00011111
#define MASK_YEAR 0b11111111

#define DS3231_READ (DS3231_ADDRESS << 1) | 0x01
#define DS3231_WRITE (DS3231_ADDRESS << 1)

void RTC_Init(void);
uint8_t DS3231_GetReg(uint8_t address);
void DS3231_SetReg(uint8_t address, uint8_t val);

void RTC_SetHora(Hora_t *hora);
void RTC_SetFecha(Fecha_t *fecha);
void RTC_GetHora(Hora_t *hora);
void RTC_GetFecha(Fecha_t *fecha);

void RTC_GetTime(RTC_t *rtc);
void RTC_SetTime(RTC_t *rtc);

uint8_t dec_to_bcd(uint8_t val);
uint8_t bcd_to_dec(uint8_t val);

#endif /* RTC_H_ */