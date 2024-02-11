#pragma once

#include <string>

using namespace std;

/**
 * ----------------------------------------------------------------------------------------------------
 * Macros
 * ----------------------------------------------------------------------------------------------------
 */
/* Clock */
#define PLL_SYS_KHZ (133 * 1000)

/* Buffer */
#define ETHERNET_BUF_MAX_SIZE (1024 * 2)

/* Socket */
#define SOCKET_MQTT 0

/* Timeout */
#define DEFAULT_TIMEOUT 1000 // 1 second

// #include "AnalogSensor.h"

// TimerOne configLed
const unsigned long CYCLES_PER_SECOND = 8000;
const unsigned int PIR_PERIOD_MS = 300;
const unsigned int ANALOG_PERIOD_MS = 3000;
const float MAX_ADC_V = 5.0;
// const AnalogSensor::SScaler ANALOG_SCALER = AnalogSensor::SScaler(0, 1024, 100, 0);

// Ethernet config
#define MAC      { 0x00, 0x08, 0xDC, 0x12, 0x34, 0x51 }
#define IP       { 192, 168, 1, 51 }
#define SUBNET   { 255, 255, 255, 0 }
#define GATEWAY  { 192, 168, 1, 1 }
#define DNS      { 8, 8, 8, 8 }

/* MQTT IP */
#define MQTT_PUBLISH_PERIOD (1000 * 1) // 1 seconds
#define MQTT_KEEP_ALIVE 1000 // 1000 milliseconds
#define MQTT_SERVER { 192, 168, 1, 121 }
const uint16_t MQTT_PORT = 1883;

// MQTT
const string MQTT_CLIENT_ID = "rozdzielnia-pico";
const string MQTT_USERNAME = "wiznet";
const string MQTT_PASSWORD = "0123456789";

const string deviceName = "rozdzielnia";
const string cmndSufix  = "/cmnd";

const string statusTopic      = deviceName + "/status";
const string uptimeTopic      = deviceName + "/uptime";

const string pir1Topic        = deviceName + "/move1";
const string pir2Topic        = deviceName + "/move2";
const string pir3Topic        = deviceName + "/move3";
const string pir4Topic        = deviceName + "/move4";
const string pir5Topic        = deviceName + "/move5";
const string pir6Topic        = deviceName + "/move6";
const string pir7Topic        = deviceName + "/move7";
const string pir8Topic        = deviceName + "/move8";
const string pir9Topic        = deviceName + "/move9";

const string light1Topic      = deviceName + "/light1";
const string light2Topic      = deviceName + "/light2";
const string light3Topic      = deviceName + "/light3";

const string pwm1TopicStat    = deviceName + "/pwm1";
const string pwm2TopicStat    = deviceName + "/pwm2";
const string pwm3TopicStat    = deviceName + "/pwm3";
const string pwm4TopicStat    = deviceName + "/pwm4";
const string pwm5TopicStat    = deviceName + "/pwm5";
const string pwm6TopicStat    = deviceName + "/pwm6";
const string pwm1TopicCmnd    = pwm1TopicStat + cmndSufix;
const string pwm2TopicCmnd    = pwm2TopicStat + cmndSufix;
const string pwm3TopicCmnd    = pwm3TopicStat + cmndSufix;
const string pwm4TopicCmnd    = pwm4TopicStat + cmndSufix;
const string pwm5TopicCmnd    = pwm5TopicStat + cmndSufix;
const string pwm6TopicCmnd    = pwm6TopicStat + cmndSufix;

const int PWM1Pin = 3;
const int PWM2Pin = 22;
const int PWM3Pin = 21;
const int PWM4Pin = 11;
const int PWM5Pin = 13;
const int PWM6Pin = 14;

const int Port1PirLPin = 15;   // [x][][][][][] PIN 3 Left
const int Port1PirRPin = 12;   // [x][][][][][] PIN 4 Right
const int Port2PirLPin = 10;   // [][x][][][][] PIN 3 Left
const int Port2PirRPin = 9;    // [][x][][][][] PIN 4 Right
const int Port3PirLPin = 8;    // [][][x][][][] PIN 3 Left
const int Port3PirRPin = 7;    // [][][x][][][] PIN 4 Right
const int Port4PirRPin = 6;    // [][][][x][][] PIN 4 Right
const int Port5PirRPin = 5;    // [][][][][x][] PIN 4 Right
const int Port6PirRPin = 4;    // [][][][][][x] PIN 4 Right

const int Port4ADCLPin = 26;   // [][][][x][][] PIN 3 Left
const int Port5ADCLPin = 27;   // [][][][][x][] PIN 3 Left
const int Port6ADCLPin = 28;   // [][][][][][x] PIN 3 Left

const int Port4ADCinput = 0;
const int Port5ADCinput = 1;
const int Port6ADCinput = 2;

typedef struct ADCchannel
{
    bool initDone;
    uint16_t measuredValue;
    uint16_t lastReportedValue;
    const string& topicStat;
    const uint8_t pinNo;
    const uint8_t channelNo;

    ADCchannel(const string& _topicStat, const uint8_t _pinNo, const uint8_t _channelNo)
     : initDone(false)
     , measuredValue(0)
     , lastReportedValue(0)
     , topicStat(_topicStat)
     , pinNo(_pinNo)
     , channelNo(_channelNo)
    {}
} ADCchannel;

typedef struct PIR
{
    bool initDone;
    bool measuredValue;
    bool lastReportedValue;
    const string& topicStat;
    const uint8_t pinNo;

    PIR(const string& _topicStat, const uint8_t _pinNo)
     : initDone(false) 
     , measuredValue(false)
     , lastReportedValue(false)
     , topicStat(_topicStat)
     , pinNo(_pinNo)
    {}
} PIR;

typedef struct PWMchannel
{
    uint8_t targetPWM;
    uint8_t currentPWM;
    const string& topicStat;
    const string& topicCmnd;
    const uint8_t pinNo;

    PWMchannel(const string& _topicStat, const string& _topicCmnd, const uint8_t _pinNo)
     : targetPWM(0)
     , currentPWM(0)
     , topicStat(_topicStat)
     , topicCmnd(_topicCmnd)
     , pinNo(_pinNo)
    {}
} PWMchannel;

PWMchannel pwm1 = PWMchannel(pwm1TopicStat, pwm1TopicCmnd, PWM1Pin);
PWMchannel pwm2 = PWMchannel(pwm2TopicStat, pwm2TopicCmnd, PWM2Pin);
PWMchannel pwm3 = PWMchannel(pwm3TopicStat, pwm3TopicCmnd, PWM3Pin);
PWMchannel pwm4 = PWMchannel(pwm4TopicStat, pwm4TopicCmnd, PWM4Pin);
PWMchannel pwm5 = PWMchannel(pwm5TopicStat, pwm5TopicCmnd, PWM5Pin);
PWMchannel pwm6 = PWMchannel(pwm6TopicStat, pwm6TopicCmnd, PWM6Pin);

ADCchannel adc1 = ADCchannel(light1Topic, Port4ADCLPin, Port4ADCinput);
ADCchannel adc2 = ADCchannel(light2Topic, Port5ADCLPin, Port5ADCinput);
ADCchannel adc3 = ADCchannel(light3Topic, Port6ADCLPin, Port6ADCinput);

PIR pir1 = PIR(pir1Topic, Port1PirLPin);
PIR pir2 = PIR(pir2Topic, Port1PirRPin);
PIR pir3 = PIR(pir3Topic, Port2PirLPin);
PIR pir4 = PIR(pir4Topic, Port2PirRPin);
PIR pir5 = PIR(pir5Topic, Port3PirLPin);
PIR pir6 = PIR(pir6Topic, Port3PirRPin);
PIR pir7 = PIR(pir7Topic, Port4PirRPin);
PIR pir8 = PIR(pir8Topic, Port5PirRPin);
PIR pir9 = PIR(pir9Topic, Port6PirRPin);
