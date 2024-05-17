#pragma once
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

#include "main.h"

//Стартовые Настройки
void httpSetup();

//Обработка запросов
void httpClient();