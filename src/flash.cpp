#include "flash.h"


File UploadFile; //Файл прошивки

SoftwareSerial Sport(RX, TX);

uint8_t Flash(String fileName){
    String path = "/" + fileName;
    //Dir dir = SPIFFS.openDir("/");
    //UploadFile = dir.openFile
    if (!SPIFFS.exists(path)) return 1; //Файл не найден.
    UploadFile = SPIFFS.open(path, "r");//Открыть файл на чтение.
    if (!UploadFile) return 2; //Ошибка при открытии.

    if (!stm32Connect())
    //инициализация
    

    UploadFile.close(); //Закрыть файл.
    return 0;
}

void sentByte(byte cmd){
    //Для каждой команды хост отправляет байт и его байт-инверсия
    Sport.write(cmd);
    Sport.write(~cmd);
}

bool check(byte b){
    if (b == stm32ACT) return true;
    return false;
}

bool timeout(){

    return false;
}


bool stm32Connect(){
    byte resByte; //ответ
    sentByte(stm32INIT);
    if (!check(resByte)) return false;
    return true;
}

bool stm32Read(){
    
    return false;
}

bool stm32Wrie(){
    
    return false;
}

bool stm32Erase(){
    
    return false;
}

bool stm32ExErase(){
    
    return false;
}
