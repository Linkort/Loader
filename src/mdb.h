#pragma once
#include "main.h"

#include "ModbusMaster.h"

//Моdbus

#define BAUDS 115200    //Скорость для подключений
#define SLAVE_ADDR 255  //Адрес платы 255-broadcast
#define BAUDS_DIV 240   //Записываемый делитель скорости для плат V1 - 52, V2 - 240
#define REG_OFFSET 1    //Регистр для чтение. Регистр, где расположен адрес платы

//Стартовые настройки Modbus
void mdbSetup(SoftwareSerial& port);

//Опрос платы
uint8_t mdbPoll(bool cmdRead, bool cmdWrite, uint8_t &Addr);