#include "MyEEPROM.h"

reg registers[MAX_REGISTERS] = {};
uint16_t users[MAX_USERS] = {};
reg bufferRegister = {};

void begin()
{
    EEPROM.begin(EEPROM_SIZE);
    return;
}

void reset()
{

    EEPROM.writeShort(LAST_REGISTER_POS, EEPROM_REG_ADRSTART);
    EEPROM.commit();
    return;
}

int getLastAddress()
{
    return EEPROM.readShort(LAST_REGISTER_POS);
}

void loadRegistersToRAM()
{
    Serial.print("Carregando registros na RAM");
    int aux = 0;
    int cont = 0;

    while (aux != (getLastAddress() - EEPROM_REG_ADRSTART))
    {
        int track = aux + EEPROM_REG_ADRSTART;
        EEPROM.get(track, registers[cont]);
        /*
        registros[cont].id = EEPROM.readShort(track);
        registros[cont].hora = EEPROM.readShort(track + 1);
        registros[cont].minuto = EEPROM.readShort(track + 2);
        registros[cont].mes = EEPROM.readShort(track + 3);
        registros[cont].ano = EEPROM.readShort(track + 4);
        registros[cont].estrada = EEPROM.readShort(track + 5);
        */
        aux = aux + STRUCT_SIZE;
        Serial.print(getLastAddress());
        cont++;
    }
}

int writeRegisterToEEPROM(int idU, int ano, int mes, int dia, int hora, int minuto, int entrada)
{

    bufferRegister.id = findUserAddress(bufferRegister.id);
    bufferRegister.ano = ano - 2000;
    bufferRegister.mes = mes;
    bufferRegister.dia = dia;
    bufferRegister.hora = hora;
    bufferRegister.minuto = minuto;
    bufferRegister.entrada = entrada;

    int aux = (getLastAddress() - EEPROM_REG_ADRSTART) / STRUCT_SIZE;
    if (aux > 505)
    {
        return -1; /// Espaço insuficiente para mais registros
    }
    EEPROM.put(getLastAddress(), bufferRegister);
    EEPROM.commit();
    registers[aux] = bufferRegister; // RAM
    EEPROM.writeShort(LAST_REGISTER_POS, aux + STRUCT_SIZE);
    EEPROM.commit(); // att last address
    return 1;
}

uint8_t findUserAddress(int user)
{

    uint8_t aux;

    for (aux = 0; aux < MAX_USERS; aux++)
    {
        if (users[aux] == user)
        {
            return aux;
        }
    }
}

int userState(int user)
{

    int aux = (getLastAddress() - EEPROM_REG_ADRSTART) / STRUCT_SIZE;

    while (aux < 0)
    {
        if (user == registers[aux].id)
        {
            return registers[aux].entrada;
        }
        aux--;
    }
    return -1;
}

void loadUsersToRAM()
{
    Serial.print("Carregando users na RAM");
    int aux = 0;
    int address = 0;

    while (aux < MAX_USERS)
    {
        users[aux] = EEPROM.readShort(address);
        address = address + 2;
        aux++;
    }
    return;
}

int addUserToEEPROM(uint16_t user)
{

    if (findUser(user) == 0)
    {

        uint16_t address = 0;

        while (EEPROM.readShort(address) == 0 && address < EEPROM_USER_ADRLIMIT)
        {
            address = address + 2;
        }

        if (address > EEPROM_USER_ADRLIMIT - 1)
        {

            return -1;
            // Limite de usuárioas cadastrados excedido
        }
        else
        {
            EEPROM.writeShort(address, findUserAddress(user));
            EEPROM.commit();
            loadUsersToRAM();
            return 1;
        }
    }
    else
    {

        return -2; // User já existe
    }
}

int findUser(int user)
{
    int aux;
    for (aux = 0; aux < MAX_USERS; aux++)
    {
        if (users[aux] == user)
        {
            return 1; // achou usuário
        }
    }
    return 0; // n encontrou

}

reg getRegister(int index)
{
    return registers[index];
}