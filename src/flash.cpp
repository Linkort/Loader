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
    
    if (!stm32Write(UploadFile, result)) return result; //Запись Прошивки
    Serial.println("Write -OK-");

    UploadFile.close(); //Закрыть файл.
    return 200;
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
        36 - Очистка памяти. STM32 вернул NACT ответ по завершению очистки. Ошибка.
        37 - Очистка памяти. STM32 вернул не ожидаемый фрейм по завершению очистки.

        40 - Запись Прошивки. Файл прошивки пуст.
        41 - Запись Прошивки. Нет ответа от платы после отправки команды.
        42 - Запись Прошивки. STM32 вернул NACT ответ после отправки команды. Команда не поддерживается
        43 - Запись Прошивки. STM32 вернул не ожидаемый фрейм после отправки команды.
        44 - Запись Прошивки  Нет ответа от платы поcле отправки адреса в памяти.
        45 - Запись Прошивки. STM32 вернул NACT ответ поcле отправки адреса в памяти. Ошибка CRC.
        46 - Запись Прошивки. STM32 вернул не ожидаемый фрейм поcле отправки адреса в памяти.
        47 - Запись Прошивки  Нет ответа от платы поcле отправки данных.
        48 - Запись Прошивки. STM32 вернул NACT ответ поcле отправки данных. Ошибка CRC.
        49 - Запись Прошивки. STM32 вернул не ожидаемый фрейм поcле отправки данных.

        120 - Подключение успешно
        130 - Команда очистки 0x44 Поддерживается
        134 - Очистка Памяти прошла успешно
        200 - Успешная загрузка прошивки
    */
}

bool stm32Connect(uint8_t& err){
    Sport->write(STM32INIT);
    if (!checkAct(20, err)) return false; //Проверка ответного фрейма
    return true; //Ошибка
}

bool checkAct(uint8_t errtype, uint8_t& err){
    byte buf[1];//ответ
    if (!(*Sport).readBytes(buf, 1)){//Таймаут
        err = errtype + 1;
        return false;
    }

    switch (buf[0]){
        case STM32ACT: //Ответ Успешный
            err = errtype + 100;
            delay(50); //костыль, иначе след команда не обрабат. STM32. ?
            return true;
            break;
        case STM32NACT: // Ответ отриц. Команда не поддерживается
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

uint8_t CRC(const uint8_t (&buf)[], size_t cnt){
    uint8_t crc = 0x00;
    for (size_t i = 0; i < cnt; i++)
    {
        crc ^= buf[i];
    }
    return crc;
}

bool stm32Read(){
    
    return false;
}

bool stm32Write(File& firmware, uint8_t& err){
    uint32_t addr;
    size_t fsize = firmware.size(); //Размер файла
    uint16_t packlen;
    uint8_t buf[256];

    if (fsize == 0){ //Защита от пустого файла
        err = 40;
        return false;
    }
    
    Serial.printf("Firmware size: %u bytes.\n", fsize);
   // Serial.println(firmware);
    for (uint i = 0; i < fsize;)
    {
        //Запись идет пакетами по 256 байт, последний по остатку.
        //определение длины след пакета.
        packlen = (fsize - i);
        if (packlen >= 256) packlen = 256;
        addr = FlashAddrOffset + i;
        i+= 256;
        sendCmd(STM32WRITE);
        if (!checkAct(40, err)) return false; //ожид. подтверждения
        //Адрес + CRC
        buf[0] = (addr >> 24) & 0xFF;
        buf[1] = (addr >> 16) & 0xFF;
        buf[2] = (addr >> 8) & 0xFF;
        buf[3] = (addr ) & 0xFF;
        buf[4] = CRC(buf, 4);
        (*Sport).write(buf, 5);
        if (!checkAct(43, err)) return false; //ожид. подтверждения
        //данные
        (*Sport).write(packlen-1);
        firmware.read(buf, packlen);
        (*Sport).write(buf, packlen);
        (*Sport).write(CRC(buf, packlen) ^ (packlen-1));
        if (!checkAct(46, err)) return false; //ожид. подтверждения
    }
    
    return true;
}

bool stm32ExErase(uint8_t& err){
    sendCmd(STM32EXTERASE);
    if (!checkAct(30, err)) return false; //ожид. подтверждения
    //Команда на Global Erase + байт CRC(через XOR)
    (*Sport).write(0xFF); 
    (*Sport).write(0xFF); 
    (*Sport).write(0x00); 
    if (!checkAct(34, err)) return false; //ожид. подтверждения очистки
    return true;
}

