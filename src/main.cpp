#include <LiquidCrystal.h>
#include <Arduino.h>
#include <stdint.h>
#include <RTClib.h>
#include <EEPROM.h>
#include <ArduinoJson.h>
#include "string.h"
#include "../lib/MyKeypad/MyKeypad.h"
#include "../lib/MyEEPROM/MyEEPROM.h"
#include "../lib/MyUtils/MyUtils.h"

const int rs = 23, en = 19, d4 = 18, d5 = 5, d6 = 4, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
Keypad customKeypad = setupKeypad();
void loop2(void *z);
void updateLCD(String firstRow);
char buffer[6];
void setup()
{
    startCLock();
    Serial.begin(115200);
    lcd.begin(16, 2);
    begin();
    reset();
    loadUsersToRAM();
    loadRegistersToRAM();
    delay(1000);
    updateLCD("ID: ");
    xTaskCreatePinnedToCore(loop2, "loop2", 8192, NULL, 1, NULL, 0);
}

void loop()
{
    String cmd;

    if (Serial.available() > 0)
    {
        cmd = Serial.readStringUntil('}');
        DynamicJsonDocument doc(1024);
        deserializeJson(doc, cmd);
        if (doc["type"] == 1)
        {
            adjustDateTime(doc['ano'], doc['mes'], doc['dia'], doc['hora'], doc['minuto'], doc['segundo']);

            Serial.print("{\"status\": 200, \"type\": 1,  \"msg\": \"Hora alterada com sucesso!\"} ");
        }
        else if (doc["type"] == 2)
        {
            if (addUserToEEPROM(doc["matricula"]) > 0)
                Serial.print("{\"status\": 200, \"type\": 2,  \"msg\": \"Cadastrado com sucesso!\"} ");
            else
                Serial.print("{\"status\": 500, \"type\": 2,  \"msg\": \"Erro ao cadastrar!\"} ");
        }
        else if (doc["type"] == 3)
        {
            //handle Time

            int i = 0;
            int posicoes = ((getLastAddress() - EEPROM_REG_ADRSTART) / STRUCT_SIZE);
            if (posicoes <= 0)
            {
                DynamicJsonDocument docToSend(1024);
                docToSend["status"] = 400;
                docToSend["type"] = 3;
                docToSend["msg"] = "Sem Registros";
                serializeJson(docToSend, Serial);
            }
            else
            {
                for (i = 0; i < posicoes; i++)
                {
                    DynamicJsonDocument docToSend(1024);
                    docToSend["status"] = 200;
                    docToSend["type"] = 3;
                    docToSend["id"] = registers[i].id;
                    docToSend["ano"] = registers[i].ano;
                    docToSend["mes"] = registers[i].mes;
                    docToSend["dia"] = registers[i].dia;
                    docToSend["hora"] = registers[i].hora;
                    docToSend["minuto"] = registers[i].minuto;
                    docToSend["entrada"] = registers[i].entrada;
                    serializeJson(docToSend, Serial);
                }
            }
        }
    }
}

void loop2(void *z)
{
    Serial.printf("\nloop2() em core: %d", xPortGetCoreID()); // Mostra no monitor em
    while (1)
    {
        char customKey = customKeypad.getKey();
        if (customKey)
        {
            size_t cur_len = strlen(buffer);
            if (cur_len < 4)
            {
                buffer[cur_len] = customKey;
                buffer[cur_len + 1] = '\0';
            }
            Serial.println(buffer);
        }

        if (strlen(buffer) == 4)
        {
            int id;
            sscanf(buffer, "%d", &id);
            if (findUser(id))
            {
                writeRegisterToEEPROM(id, getCurrentTime().year(), getCurrentTime().month(),
                                      getCurrentTime().day(),
                                      getCurrentTime().hour(),
                                      getCurrentTime().minute(),
                                      getCurrentTime().second());

                updateLCD("Bom Dia: ");
            }
            else
            {
                updateLCD("ID nao encontrado: ");
            }
            //Exibir MSGM

            Serial.print(buffer);

            memset(buffer, 0, sizeof buffer);
            delay(1500);
        }
        updateLCD("ID: ");
        delay(50);
    }
}
void updateLCD(String firstRow)
{
    DateTime currentTime = getCurrentTime();
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(firstRow);
    lcd.print(buffer);
    lcd.setCursor(0, 1);
    lcd.print(currentTime.day(), DEC);
    lcd.print("/");
    lcd.print(currentTime.month(), DEC);
    lcd.print("/");
    lcd.print(currentTime.year() - 2000, DEC);
    lcd.print(" ");
    lcd.print(currentTime.hour(), DEC);
    lcd.print(":");
    lcd.print(currentTime.minute(), DEC);
    lcd.print(":");
    lcd.print(currentTime.second(), DEC);
}