
#include "MyEEPROM.h"

void reset()
{
    EEPROM.writeShort(LAST_REGISTER_POS, EEPROM_REG_ADRSTART);
    return;
}

void loadRegistersToRAM()
{
    int aux = 0;
    int cont = 0;

    while (aux != getLastAddress() - EEPROM_REG_ADRSTART)
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
        cont++;
    }
}

int writeRegisterToEEPROM(int id, int ano, int mes, int dia, int hora, int minuto, int entrada)
{

    bufferRegister.id = id;
    bufferRegister.ano = ano - 2000;
    bufferRegister.mes = mes;
    bufferRegister.dia = dia;
    bufferRegister.hora = hora;
    bufferRegister.minuto = minuto;
    bufferRegister.entrada = entrada;


    int aux = getLastAddress();
    if (aux > 505)
    {
        return -1; /// Espaço insuficiente para mais registros
    }
    EEPROM.put(aux, bufferRegister);
    registers[(aux - EEPROM_REG_ADRSTART) / STRUCT_SIZE] = bufferRegister; // escreve na RAM o valor do buffer
    EEPROM.writeShort(LAST_REGISTER_POS, aux + STRUCT_SIZE);               // att last address
    return 1;
}

int getLastAddress()
{
    return EEPROM.readShort(LAST_REGISTER_POS);
}

void loadUsersToRAM()
{

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
        EEPROM.writeShort(address, user);
        return 1;
    }
}

