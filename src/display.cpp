#include "main.h"

GyverShift<OUTPUT, 3> disp(CS, DAT, CLK);

std::map <String, uint8_t> symb = {
    {"0", 0b11000000},  {"1", 0b11111001},  {"2", 0b10100100},  {"3", 0b10110000},  {"4", 0b10011001},
    {"5", 0b10010010},  {"6", 0b10000010},  {"7", 0b11111000},  {"8", 0b10000000},  {"9", 0b10010000},
    {"0.", 0b01000000}, {"1.", 0b01111001}, {"2.", 0b00100100}, {"3.", 0b00110000}, {"4.", 0b00011001},
    {"5.", 0b00010010}, {"6.", 0b00000010}, {"7.", 0b01111000}, {"8.", 0b00000000}, {"9.", 0b00010000},
    {"A", 0b10001000},  {"C", 0b11000110},  {"E", 0b10000110},  {"F", 0b10001110},  {"G", 0b11000010},
    {"H", 0b10001001},  {"I", 0b11111001},  {"J", 0b11110001},  {"L", 0b11000111},  {"O", 0b11000000},
    {"P", 0b10001100},  {"S", 0b10010010},  {"U", 0b11000001},  {"Y", 0b10010001},
    {"b", 0b10000011},  {"c", 0b10100111},  {"d", 0b10100001},  {"h", 0b10001011},  {"i", 0b11111011},
    {"n", 0b10101011},  {"o", 0b10100011},  {"t", 0b10000111},  {"u", 0b11100011},  {"r", 0b10101111},
    {"-", 0b10111111},
    {" ", 0b11111111}, {"allon", 0b00000000}, {".", 0b01111111}
};


void display(String s1, String s2, String  s3){
    disp.buffer[0] = symb[s1];
    disp.buffer[1] = symb[s2];
    disp.buffer[2] = symb[s3];
    disp.update();
}


void display(uint16_t val){
    disp.buffer[0] = symb[String((val / 100) % 10)];
    disp.buffer[1] = symb[String((val / 10) % 10)];
    disp.buffer[2] = symb[String(val % 10)];
    disp.update();
}

void display(String text){
    for (size_t i = 0; i < 3; i++)
    {
        disp.buffer[i] = symb[text.substring(i,i+1)];
    }
    disp.update();
}