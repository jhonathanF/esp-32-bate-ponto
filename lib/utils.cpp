

#include "../lib/utils.h"

void attBufferRegister()
{

    DateTime now = rtc.now;

    bufferRegister.hora = now.hour;
    bufferRegister.minuto = now.minute;
    bufferRegister.dia = now.day;
    bufferRegister.mes = now.month;
    bufferRegister.ano = now.year;
    return;
}
