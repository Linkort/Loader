#include "flash.h"


File UploadFile; //Файл прошивки

SoftwareSerial Sport;

void flashSetup(SoftwareSerial &p){
    //Переоткрыть порт с новыми параметрами
   // *port = &p;
    p.end();
    p.begin(BAUDS, SWSERIAL_8E1);
}

uint8_t flash(String fileName){

    String path = "/" + fileName;
    if (!SPIFFS.exists(path)) return 1; //Файл не найден.
    UploadFile = SPIFFS.open(path, "r");//Открыть файл на чтение.
    if (!UploadFile) return 2; //Ошибка при открытии.

    //инициализация
    if (!stm32Connect()) return 0; //Нет ответа от STM
    
    UploadFile.close(); //Закрыть файл.

    return 255;
}

void sentCmd(byte cmd){
    //Для каждой команды хост отправляет байт и его байт-инверсия
   // Sport.write(cmd);
   // Sport.write(~cmd);
}

void sentByte(byte cmd){
   // Sport.write(cmd);
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
