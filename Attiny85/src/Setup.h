#ifndef _SETUP_h
#define _SETUP_h

#include <Arduino.h>

/* 
	Включение логирования с TinySerial: 
	3 pin TX -> RX (TTL-USB 3.3 или 5в), 9600 8N1
	При логировании не работает счетчик2 на 3-м пине (Вход 2).

	Для логирования раскомментируйте LOG_LEVEL_DEBUG
*/
// #define LOG_LEVEL_ERROR
// #define LOG_LEVEL_INFO
// #define LOG_LEVEL_DEBUG

#if defined (LOG_LEVEL_DEBUG) || defined (LOG_LEVEL_INFO) || defined (LOG_LEVEL_ERROR)
#include "TinyDebugSerial.h"
#endif

//#define TEST_WATERIUS   // Тестирование счетчика при помощи Arduino


/*
	Период отправки данных на сервер, мин
*/
#ifdef TEST_WATERIUS
#define WAKE_EVERY_MIN   10
#else
#define WAKE_EVERY_MIN   24 * 60U  // 1U 
#endif

/*
	Аварийное отключение, если ESP зависнет и не пришлет команду "сон".
*/
#define WAIT_ESP_MSEC    120000UL      

/*
	Сколько милисекунд пользователь может 
	настраивать ESP. Если не закончил, питание ESP выключится.
*/
#define SETUP_TIME_MSEC  300000UL 


struct Data {
    uint32_t value0;
    uint32_t value1;
};

struct CounterState {
    uint8_t  state0;  // состояние входа
    uint8_t  state1;  // не добавляем в Data, т.к. та в буфере кольцевом
};

struct Header {
    uint8_t       version;
    uint8_t       service;
    uint32_t      voltage;
    uint8_t       resets;
    uint8_t       reserved;
    CounterState  states;
    Data          data;
};


/*
	define для логирования. Не менять.
*/
#define LOG_DEBUG(x)
#define LOG_INFO(x)  
#define LOG_ERROR(x) 
#define DEBUG_CONNECT(x)  


#if defined (LOG_LEVEL_DEBUG) || defined (LOG_LEVEL_INFO) || defined (LOG_LEVEL_ERROR)
    #define DEBUG
    class TinyDebugSerial;
    extern TinyDebugSerial mySerial;
    #undef DEBUG_CONNECT
    #define DEBUG_CONNECT(x)  mySerial.begin(x)
    #define PRINT_NOW(x) mySerial.print(millis()); mySerial.print(x);
#endif

#ifdef LOG_LEVEL_DEBUG
    #undef LOG_DEBUG
    #define LOG_DEBUG(x) { PRINT_NOW(F(" [D]: ")); mySerial.println(x); }
    #define LOG_LEVEL_INFO
#endif
#ifdef LOG_LEVEL_INFO
    #undef LOG_INFO
    #define LOG_INFO(x) { PRINT_NOW(F(" [I]: ")); mySerial.println(x); }
    #define LOG_LEVEL_ERROR
#endif
#ifdef LOG_LEVEL_ERROR
    #undef LOG_ERROR
    #define LOG_ERROR(x) { PRINT_NOW(F(" [E]: ")); mySerial.println(x); }
#endif

#endif