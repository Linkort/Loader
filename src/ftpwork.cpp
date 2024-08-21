#include "ftpwork.h"

FtpServer ftp;

void ftpSetup(){
    /* 
    Настройки для работы с файлами
    Выполнить 1 раз при старте
    */
    if (SPIFFS.begin()) ftp.begin(AP_SSID, AP_PASS);//Настройки FTP сервера
}

void ftpClient(){
    ftp.handleFTP(); // FTP запрос
}

std::vector <String> firmwareListUpdate(){
    std::vector <String> firmwareList; //Список
    firmwareList.clear();
    String tempstring;

    Dir dir = SPIFFS.openDir("/");
    while(dir.next()){//перебор файлов в директории
        tempstring = dir.fileName();
        tempstring = tempstring.substring(1,30); //убрать "/" в названии файла
        if (not tempstring.endsWith(".bin")) continue;// поиск файлов "*.bin"
        firmwareList.push_back(tempstring); // добавить в список
    } 
    //Вывод списка доступных прошивок. Отладка
    Serial.print("Firmware list: ");
    for (size_t i = 0; i < firmwareList.size(); i++)
    {
        Serial.print(firmwareList[i]+ " ");
    }
    Serial.println();
    return firmwareList;
}
