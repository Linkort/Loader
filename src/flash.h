#pragma once
#include "main.h"

#define FlashAddrOffset 0x8000000 //Область памяти для загрузки прошивки

//Байт/Команда
#define STM32ACT        0x79 //Байт подтверждения
#define STM32NACT       0x1F //Байт отказа
#define STM32INIT       0x7F //Байт инициализации
#define STM32READ       0x11 //Байт чтения
#define STM32WRITE      0x31 //Байт чтения
#define STM32ERASE      0x43 //Байт очистки
#define STM32EXTERASE   0x44 //Байт очистки

//Команды stm32ERASE и stm32EXTERASE - взаимоисключающие

#define FlashBauds      128000//Скорость прошивки.

//AN3155 - протокол загрузчика USART STM32
//https://www.st.com/resource/en/application_note/an3155-usart-protocol-used-in-the-stm32-bootloader-stmicroelectronics.pdf


//Загрузка прошивки
void flashSetup(SoftwareSerial &port);

//main фукция
uint8_t flash(bool cmd, String fileName);

//Подключение к STM32
bool stm32Connect(uint8_t& err);

//Проверка на байт подтверждения
bool checkAct(uint8_t errtype, uint8_t& err);

//Отправить команду
void sendCmd(byte cmd);

//рассчет CRC
uint8_t CRC(const uint8_t (&buf)[], size_t bytenum);

//Запись в память (Команда 0x31 - WriteMemory)
bool stm32Write(File& firmware, uint8_t& err);

//Очистка Памяти (Команда 0x43 - Erase)
bool stm32Erase();

//Очистка Памяти (Команда 0x44 - Extended Erase)
bool stm32ExErase(uint8_t& err);


