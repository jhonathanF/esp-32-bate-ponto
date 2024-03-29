#include "MyEEPROM.h"

reg registers[MAX_REGISTERS] = {};
uint16_t users[MAX_USERS] = {};
reg bufferRegister = {};

void begin()
{
    EEPROM.begin(EEPROM_SIZE);
    return;
}

void wipeUsers()
{

    int aux = 0;

    while (aux < MAX_USERS)
    {
        EEPROM.writeShort(aux, 0);
        aux = aux + 2;
    }
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

    if(getLastAddress()== EEPROM_REG_ADRSTART){
        Serial.println("EEPROM vazia");

    }

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

int writeRegisterToEEPROM(int id, int ano, int mes, int dia, int hora, int minuto, int entrada)
{

    bufferRegister.id = findUserAddress(id);
    bufferRegister.ano = ano - 2000;
    bufferRegister.mes = mes;
    bufferRegister.dia = dia;
    bufferRegister.hora = hora;
    bufferRegister.minuto = minuto;
    bufferRegister.entrada = !userState(id);

    int aux = getLastAddress();
    if (aux > 505)
    {
        return -1; /// Espaço insuficiente para mais registros
    }
    EEPROM.put(aux, bufferRegister);
    EEPROM.commit();

    registers[(aux - EEPROM_REG_ADRSTART) / STRUCT_SIZE] = bufferRegister;
    registers[(aux - EEPROM_REG_ADRSTART) / STRUCT_SIZE].id = id; // escreve na RAM o valor do buffer
    EEPROM.writeByte(aux - 1, !bufferRegister.entrada);
    EEPROM.commit();

    EEPROM.writeShort(LAST_REGISTER_POS, aux + STRUCT_SIZE); // att last address
    EEPROM.commit();
    return 1;
}

uint8_t findUserAddress(uint16_t user)
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

uint8_t userState(int user)
{

    int aux = (getLastAddress() - EEPROM_REG_ADRSTART) / STRUCT_SIZE;
    aux = aux - 1;
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

        while (EEPROM.readShort(address) != 0)
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

int findUser(uint16_t user)
{
    int aux;
    for (aux = 0; aux < MAX_USERS; aux++)
    {
        if (users[aux] == user)
        {
            return 1; // achou usuário
        }
    }
    return 0;
}