#ifndef MYUTILS_H
#define MYUTILS_H

#include <RTClib.h>
#include <Wire.h>
#include <Arduino.h>
#include <stdint.h>

typedef struct Time
{
    uint16_t ano;
    uint8_t mes;
    uint8_t dia;
    uint8_t hora;
    uint8_t minuto;
    uint8_t segundo;
} Time;

void adjustDateTime(uint16_t ano, uint8_t mes, uint8_t dia, uint8_t hora, uint8_t minuto, uint8_t segundo);
void adjustDateTimeBySystem();
DateTime getCurrentTime();
void startCLock();

#endif