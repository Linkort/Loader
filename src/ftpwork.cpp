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
        tempstring = tempstring.substring(1,30);
        if (not tempstring.endsWith(".bin")) continue;// поиск файлов "*.bin"
        firmwareList.push_back(tempstring);
    }
    //отладка
    for (size_t i = 0; i < firmwareList.size(); i++)
    {
        Serial.print(firmwareList[i]+ " ");
    }

    return firmwareList;
}
