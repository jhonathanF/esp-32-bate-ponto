#include "MYUTILS.h"
RTC_DS1307 rtc;

DateTime getCurrentTime()
{
    return rtc.now();
}

void adjustDateTime(uint16_t ano, uint8_t mes, uint8_t dia, uint8_t hora, uint8_t minuto, uint8_t segundo)
{
    // rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
     Serial.print("Setando nova Data");
    rtc.adjust(DateTime(ano, mes, dia, hora, minuto, segundo));
    return;
}

void adjustDateTimeBySystem()
{
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    return;
}
void startCLock()
{
    Serial.print("Iniciando Relogio");
    rtc.begin();

    if (!rtc.isrunning())
        Serial.print("Relogio não tá rodando");
}