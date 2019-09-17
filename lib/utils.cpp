

#include "../lib/utils.h"

void attCurrentTime(){
    DateTime now = rtc.now;

    CurrentTime.hora = now.hour;
    CurrentTime.minuto = now.minute;
    CurrentTime.segundo = now.second;
    CurrentTime.dia = now.day;
    CurrentTime.mes = now.month;
    CurrentTime.ano = now.year;
    return;
    
};

void adjustDateTime(int ano, int mes, int dia, int hora, int minuto, int segundo){
    rtc.adjust(DateTime(ano,mes,dia,hora,minuto,segundo));
    return;
}
