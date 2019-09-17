
#ifndef MyEEPROM_H
#define MyEEPROM_H

#include <stdint.h>
#include <EEPROM.h>
// Usuário 4 numeros ex. 9242
// Numero de bits para armazenar numeros até 9999
// 0  0  0  0  0  0  0  0   0   0    0    0   0      0   =  14 bits -> 2 Bytes por usuário
// 1  2  4  8 16 32 64 128 256 512 1024 2048 4096   8192
// uint16_t

// Armazenando no maximo 15 Usuários
// 30 bytes para usuários

// 00 - 29 Users
// 28 - 29 Int do ultimo endereço gravado
// 30 - 509 Registros

// 6 Bytes por registro

//////////// 1 Byte /////////
// 00 /   ID USUÁRIO 00    //
// 01 /   ID USUÁRIO 00    //
// .  /        .           //
// 29 /   ID USUÁRIO 14    //
//---------------------------
// 30 /   Last Register    //
// 31 /   Address Position //   -- > Ultimo endereço gravado
//---------------------------
// 32 /   ENDEREÇO ID      //
// 33 /   HORA             //
// 34 /   Minuto           //
// 35 /   Dia              //  -- > Struct
// 36 /   Mes              //
// 37 /   Ano              //
// 38 /   Entrada/Saida    //
/////////////////////////////

#define EEPROM_SIZE 512
#define EEPROM_USER_ADRLIMIT 29
#define MAX_USERS 15
#define EEPROM_REG_ADRSTART 32
#define LAST_REGISTER_POS 30
#define STRUCT_SIZE 7
#define MAX_REGISTERS (EEPROM_SIZE - EEPROM_REG_ADRSTART) / STRUCT_SIZE - 1

typedef struct reg
{
    uint8_t id;
    uint8_t hora;
    uint8_t minuto;
    uint8_t dia;
    uint8_t mes;
    uint8_t ano;
    uint8_t entrada;
} reg;

uint16_t users[MAX_USERS] = {};
reg registers[MAX_REGISTERS] = {};
reg bufferRegister = {};

//Redefine o valor de Last Address para posição inicial
void reset();

//Carrega no vetor users[MAX_USERS] o numero de usuários;
void loadUsersToRAM();

//Carrega no vetor de Struct registros[MAX_REGISTERS] os valores encontrados na EEPROM, até o valor de último enredeço.
void loadRegistersToRAM();

// Ex: addUserToEEPROM(6352);
int addUserToEEPROM(uint16_t user);

//Escreve na EEPROM os valores passados;
int writeRegisterToEEPROM(int id, int ano, int mes, int dia, int hora, int minuto, int entrada);

int userState(int user);

#endif
