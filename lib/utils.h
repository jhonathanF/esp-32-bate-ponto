#ifndef UTILS_H
#define UTILS_H

#include <RTClib.h>
#include <stdint.h>
#include "../lib/MyEEPROM.h"

RTC_DS1307 rtc;

typedef struct time{
    uint8_t ano;
    uint8_t mes;
    uint8_t dia;
    uint8_t hora;
    uint8_t minuto;
    uint8_t segundo;
} time;

time CurrentTime;

void adjustDateTime(int ano, int mes, int dia, int hora, int minuto, int segundo);

#endif