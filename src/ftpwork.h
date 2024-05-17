#pragma once
#include "main.h"
#include <FS.h>
#include <ESP8266FtpServer.h>
//#include <vector>



//Стартовые настройки
void ftpSetup();

//Обработка запросов
void ftpClient();

//Обновить список прошивок *.bin
std::vector <String> firmwareListUpdate();

