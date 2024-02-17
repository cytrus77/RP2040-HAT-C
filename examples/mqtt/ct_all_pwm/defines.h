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

// Ethernet config
#define MAC      { 0x00, 0x08, 0xDC, 0x18, 0x34, 0x56 }
#define IP       { 192, 168, 116, 222 }
#define SUBNET   { 255, 255, 255, 0 }
#define GATEWAY  { 192, 168, 116, 1 }
#define DNS      { 8, 8, 8, 8 }
#define MQTT_SERVER { 192, 168, 116, 135 }

/* MQTT IP */
#define MQTT_PUBLISH_PERIOD (1000 * 1) // 1 seconds
#define MQTT_KEEP_ALIVE 10 // milliseconds
const uint16_t MQTT_PORT = 1883;

// MQTT
const string MQTT_CLIENT_ID = "rpi-pico";
const string MQTT_USERNAME = "wiznet";
const string MQTT_PASSWORD = "0123456789";

const string deviceName = "CT01X";
const string cmndSufix  = "/cmnd";

const string statusTopic      = deviceName + "/status";
const string uptimeTopic      = deviceName + "/uptime";
const string willTopic        = deviceName + "/LWT";
const string willMessageOff   = "offline";
const string willMessageOn    = "online";

const string pwm1TopicStat    = deviceName + "/pwm1";
const string pwm2TopicStat    = deviceName + "/pwm2";
const string pwm3TopicStat    = deviceName + "/pwm3";
const string pwm4TopicStat    = deviceName + "/pwm4";
const string pwm5TopicStat    = deviceName + "/pwm5";
const string pwm6TopicStat    = deviceName + "/pwm6";
const string pwm7TopicStat    = deviceName + "/pwm7";
const string pwm8TopicStat    = deviceName + "/pwm8";
const string pwm9TopicStat    = deviceName + "/pwm9";
const string pwm10TopicStat   = deviceName + "/pwm10";
const string pwm11TopicStat   = deviceName + "/pwm11";
const string pwm12TopicStat   = deviceName + "/pwm12";
const string pwm13TopicStat   = deviceName + "/pwm13";
const string pwm14TopicStat   = deviceName + "/pwm14";
const string pwm15TopicStat   = deviceName + "/pwm15";
const string pwm16TopicStat   = deviceName + "/pwm16";

const string pwm1TopicCmnd    = pwm1TopicStat + cmndSufix;
const string pwm2TopicCmnd    = pwm2TopicStat + cmndSufix;
const string pwm3TopicCmnd    = pwm3TopicStat + cmndSufix;
const string pwm4TopicCmnd    = pwm4TopicStat + cmndSufix;
const string pwm5TopicCmnd    = pwm5TopicStat + cmndSufix;
const string pwm6TopicCmnd    = pwm6TopicStat + cmndSufix;
const string pwm7TopicCmnd    = pwm7TopicStat + cmndSufix;
const string pwm8TopicCmnd    = pwm8TopicStat + cmndSufix;
const string pwm9TopicCmnd    = pwm9TopicStat + cmndSufix;
const string pwm10TopicCmnd   = pwm10TopicStat + cmndSufix;
const string pwm11TopicCmnd   = pwm11TopicStat + cmndSufix;
const string pwm12TopicCmnd   = pwm12TopicStat + cmndSufix;
const string pwm13TopicCmnd   = pwm13TopicStat + cmndSufix;
const string pwm14TopicCmnd   = pwm14TopicStat + cmndSufix;
const string pwm15TopicCmnd   = pwm15TopicStat + cmndSufix;
const string pwm16TopicCmnd   = pwm16TopicStat + cmndSufix;

const int PWM1Pin = 28;
const int PWM2Pin = 27;
const int PWM3Pin = 26;
const int PWM4Pin = 22;
const int PWM5Pin = 21;
const int PWM6Pin = 11;
const int PWM7Pin = 14;
const int PWM8Pin = 15;

const int PWM9Pin  = 3;
const int PWM10Pin = 4;
const int PWM11Pin = 5;
const int PWM12Pin = 6;
const int PWM13Pin = 7;
const int PWM14Pin = 8;
const int PWM15Pin = 9;
const int PWM16Pin = 10;

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
PWMchannel pwm7 = PWMchannel(pwm7TopicStat, pwm7TopicCmnd, PWM7Pin);
PWMchannel pwm8 = PWMchannel(pwm8TopicStat, pwm8TopicCmnd, PWM8Pin);

PWMchannel pwm9  = PWMchannel(pwm9TopicStat,  pwm9TopicCmnd, PWM9Pin);
PWMchannel pwm10 = PWMchannel(pwm10TopicStat, pwm10TopicCmnd, PWM10Pin);
PWMchannel pwm11 = PWMchannel(pwm11TopicStat, pwm11TopicCmnd, PWM11Pin);
PWMchannel pwm12 = PWMchannel(pwm12TopicStat, pwm12TopicCmnd, PWM12Pin);
PWMchannel pwm13 = PWMchannel(pwm13TopicStat, pwm13TopicCmnd, PWM13Pin);
PWMchannel pwm14 = PWMchannel(pwm14TopicStat, pwm14TopicCmnd, PWM14Pin);
PWMchannel pwm15 = PWMchannel(pwm15TopicStat, pwm15TopicCmnd, PWM15Pin);
PWMchannel pwm16 = PWMchannel(pwm16TopicStat, pwm16TopicCmnd, PWM16Pin);