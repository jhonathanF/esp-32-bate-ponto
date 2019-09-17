#include <LiquidCrystal.h>
#include <Arduino.h>

#include <ArduinoJson.h>
#include "string.h"
#include "../lib/MyKeypad/MyKeypad.h"
#include "../lib/MyEEPROM.h"

const int rs = 23, en = 19, d4 = 18, d5 = 5, d6 = 4, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
Keypad customKeypad = setupKeypad();
void loop2(void *z);
char buffer[6];
void setup()
{
    Serial.begin(115200);
    lcd.begin(16, 2);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("ID: ");
    lcd.setCursor(0, 1);
    lcd.print("17/09/2019 02:09");
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
            //handle Time
            Serial.print("{\"status\": 200, \"type\": 1,  \"msg\": \"Hora alterada com sucesso!\"} ");
        }
        else if (doc["type"] == 2)
        {
            //handle Time
            Serial.print("{\"status\": 200, \"type\": 2,  \"msg\": \"Cadastrado com sucesso!\"} ");
        }
        else if (doc["type"] == 3)
        {
            //handle Time
            Serial.print("{\"status\": 200, \"type\": 3,  \"msg\": \"Toma o relatorio Irmão\"} ");
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
            Serial.print("{\"msg\": \"Recebi tudo!\"} ");
            lcd.clear();
            lcd.setCursor(0, 0);
            lcd.print("ID: ");
            lcd.print(buffer);
            lcd.setCursor(0, 1);
            lcd.print("17/09/2019 02:09");
        }

        if (strlen(buffer) == 4)
        {
            //Buscar Cliente
            //Registrar
            //Exibir MSGM
            Serial.print("Bom dia - ");
            Serial.print(buffer);
            Serial.println();
            lcd.clear();
            lcd.setCursor(0, 0);
            lcd.print("Bom dia - ");
            lcd.print(buffer);
            lcd.setCursor(0, 1);
            lcd.print("17/09/2019 02:09");

            memset(buffer, 0, sizeof buffer);
            delay(1500);
            lcd.clear();
            lcd.setCursor(0, 0);
            lcd.print("ID: ");
            lcd.setCursor(0, 1);
            lcd.print("17/09/2019 02:09");
        }
        delay(10);
    }
}