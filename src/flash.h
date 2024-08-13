#pragma once
#include "main.h"

#define stm32offset 0x8000000 //Область памяти Flash

//Байт/Команда
#define stm32ACT        0x79 //Байт подтверждения
#define stm32NACT       0x1F //Байт отказа
#define stm32INIT       0x7F //Байт инициализации
#define stm32READ       0x11 //Байт чтения
#define stm32WRITE      0x31 //Байт чтения
#define stm32ERASE      0x43 //Байт очистки
#define stm32EXTERASE   0x44 //Байт очистки

//Команды stm32ERASE и stm32EXTERASE - взаимоисключающие



//AN3155 - протокол загрузчика USART STM32
//https://www.st.com/resource/en/application_note/an3155-usart-protocol-used-in-the-stm32-bootloader-stmicroelectronics.pdf


//Загрузка прошивки
void flashSetup(SoftwareSerial &port);

uint8_t flash(bool cmd, String fileName);

//Подключение к STM32
bool stm32Connect(uint8_t& err);

//Проверка на байт подтверждения
bool checkAct(uint8_t errtype, uint8_t& err);

//Отправить команду
void sentCmd(byte cmd);

//Чтение памяти (Команда 0x11 - GET)
bool stm32Read();

//Запись в память (Команда 0x31 - WriteMemory)
bool stm32Write();

//Очистка Памяти (Команда 0x43 - Erase)
bool stm32Erase();

//Очистка Памяти (Команда 0x44 - Extended Erase)
bool stm32ExErase();


