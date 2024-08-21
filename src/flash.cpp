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

    while ((*Sport).available()) (*Sport).read(); //Очистка входного буфера
    if (!stm32Connect(result)) return result; //Подключение
    Serial.println("Init -OK-");

    //Подготовка файла только после подключения к STM
    String path = "/" + fileName;
    if (!SPIFFS.exists(path)) return 11; //Файл не найден.
    UploadFile = SPIFFS.open(path, "r");//Открыть файл на чтение.
    if (!UploadFile) return 12; //Ошибка при открытии файла.
    Serial.println("Firmware -OK-");
    if (!stm32ExErase(result)) return result; //Очистка памяти
    Serial.println("Erase -OK-");
    if (!stm32Write(UploadFile, result)) return result; //Очистка памяти
    Serial.println("Write -OK-");

    UploadFile.close(); //Закрыть файл.
    return result;
    /* result cases:
        255 - Нет команды на прошивку.
        11 - Файл не найден.
        12 - Ошибка при открытии файла.

        21 - Подключение. Нет ответа от платы.
        22 - Подключение. STM32 вернул NACT ответ при попытке подключения. 
        23 - Подключение. STM32 вернул не ожидаемый фрейм.

        31 - Очистка памяти. Нет ответа от платы после отправки команды.
        32 - Очистка памяти. STM32 вернул NACT ответ после отправки команды. Команда не поддерживается
        33 - Очистка памяти. STM32 вернул не ожидаемый фрейм после отправки команды.
        35 - Очистка памяти. Нет ответа от платы по завершению очистки.
        36 - Очистка памяти. STM32 вернул NACT ответ по завершению очистки. Ошибка 
        37 - Очистка памяти. STM32 вернул не ожидаемый фрейм по завершению очистки.

        40 - Запись Прошивки. Файл прошивки пуст.
        41 - Запись Прошивки. Нет ответа от платы после отправки команды.
        42 - Запись Прошивки. STM32 вернул NACT ответ после отправки команды. Команда не поддерживается
        43 - Запись Прошивки. STM32 вернул не ожидаемый фрейм после отправки команды.

        44 - Запись Прошивки Нет ответа от платы поcле отправки адреса в памяти.
        45 - Запись Прошивки. STM32 вернул NACT ответ поcле отправки адреса в памяти. Ошибка CRC адреса.
        46 - Запись Прошивки. STM32 вернул не ожидаемый фрейм поcле отправки адреса в памяти.


        120 - Подключение успешно
        130 - Команда очистки 0x44 Поддерживается
        134 - Очистка Памяти прошла успешно
        200 - Успешная загрузка прошивки
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
            delay(50); //костыль, иначе след команда не обрабат. STM32. ?
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

void sendCmd(byte cmd){
    //Для каждой команды хост отправляет байт и его байт-инверсия
    Sport->write(cmd);
    Sport->write(~cmd); 
}

bool stm32Read(){
    
    return false;
}

bool stm32Write(File& firmware, uint8_t& err){
    sendCmd(stm32WRITE);
    if (!checkAct(40, err)) return false; //ожид. подтверждения
    return false;
}

bool stm32SendData(){

    return false;
}

bool stm32ExErase(uint8_t& err){
    sendCmd(stm32EXTERASE);
    if (!checkAct(30, err)) return false; //ожид. подтверждения
    //Команда на Global Erase + байт CRC(через XOR)
    (*Sport).write(0xFF); 
    (*Sport).write(0xFF); 
    (*Sport).write(0x00); 
    if (checkAct(34, err)) return true; //ожид. подтверждения очистки
    return false;
}



