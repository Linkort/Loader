#include "httpwork.h"

//ESP8266WebServer http(80);

void httpSetup(){
    WiFi.mode(WIFI_AP); 
    WiFi.softAP(AP_SSID, AP_PASS);//Включение точки доступа
    //http.begin(); 
}

void httpClient(){
    //http.handleClient();
}