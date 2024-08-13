#include "flash.h"


File UploadFile; //Файл прошивки

SoftwareSerial *Sport;

void flashSetup(SoftwareSerial &p){
    Sport = &p;
    //Переоткрыть порт с новыми параметрами
    (*Sport).end();
    (*Sport).begin(BAUDS, SWSERIAL_8E1);
}

uint8_t flash(bool cmd, String fileName){
    uint8_t result = 255;
    if (!cmd) return result; //Выход при отсутствии команды

    String path = "/" + fileName;
    if (!SPIFFS.exists(path)) return 11; //Файл не найден.
    UploadFile = SPIFFS.open(path, "r");//Открыть файл на чтение.
    if (!UploadFile) return 12; //Ошибка при открытии файла.

    while ((*Sport).available()) (*Sport).read(); //Очистка входного буфера
    if (!stm32Connect(result)) return result; //Подключение



    UploadFile.close(); //Закрыть файл.
    return result;
    /* result cases:
        255 - Нет команды на прошивку
        11 - Файл не найден
        12 - Ошибка при открытии файла
        21 - Нет ответа от платы при попытке подключения
        22 - STM32 вернул NACT ответ при попытке подключения
        23 - STM32 вернул не ожидаемый фрейм 
    */
}

bool stm32Connect(uint8_t& err){
    Sport->write(stm32INIT);
    if (checkAct(20, err)) return true; //Проверка ответного фрейма
    return false; //Ошибка
}

bool checkAct(uint8_t errtype, uint8_t& err){
    byte buf[1];//ответ
    if (!(*Sport).readBytes(buf, 1)){//Таймаут
        err = errtype + 1;
        return false;
    }

    switch (buf[0]){
        case stm32ACT: //Ответ Успешный
            err = errtype + 100;
            return true;
            break;
        case stm32NACT: // Ответ отриц. Команда не поддерживается
            err = errtype + 2;
            break;
        default: // Неожидаемый фрейм
            err = errtype + 3;
            break;
    }
    return false;
}

void sentCmd(byte cmd){
    //Для каждой команды хост отправляет байт и его байт-инверсия
    Sport->write(cmd);
    Sport->write(~cmd); 
}

bool stm32Read(){
    
    return false;
}

bool stm32Wrie(){
    
    return false;
}

bool stm32ExErase(){
    
    return false;
}



