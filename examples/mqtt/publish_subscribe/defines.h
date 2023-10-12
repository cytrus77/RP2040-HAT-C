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
#define MAC      { 0x00, 0x08, 0xDC, 0x12, 0x34, 0x56 }
#define IP       { 192, 168, 38, 22 }
#define SUBNET   { 255, 255, 255, 0 }
#define GATEWAY  { 192, 168, 38, 1 }
#define DNS      { 8, 8, 8, 8 }

/* MQTT IP */
#define MQTT_PUBLISH_PERIOD (1000 * 1) // 1 seconds
#define MQTT_KEEP_ALIVE 60 // 60 milliseconds
#define MQTT_SERVER { 192, 168, 38, 11 }
const uint16_t MQTT_PORT = 1883;

// MQTT
const string MQTT_CLIENT_ID = "rpi-pico";
const string MQTT_USERNAME = "wiznet";
const string MQTT_PASSWORD = "0123456789";

const string deviceName = "CT01";
const string cmndSufix  = "/cmnd";

const string uptimeTopic      = deviceName + "/uptime";

const string pir1Topic        = deviceName + "/move1";
const string pir2Topic        = deviceName + "/move2";
const string pir3Topic        = deviceName + "/move3";
const string pir4Topic        = deviceName + "/move4";
const string pir5Topic        = deviceName + "/move5";

const string light1Topic      = deviceName + "/light1";
const string light2Topic      = deviceName + "/light2";
const string light3Topic      = deviceName + "/light3";
const string light4Topic      = deviceName + "/light4";
const string light5Topic      = deviceName + "/light5";

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


// Status LED config
// const int StatusPin = 8; // GPIO3 -> PB0
// PIR config
// const int Pir1Pin   = 7;  // GPIO2 -> PD7
// const int Pir2Pin   = 8;  // GPIO3 -> PB0
// const int Pir3Pin   = A7; // ADC7 -> P
// const int Pir4Pin   = 1;  // TX -> PD1
// const int Pir5Pin   = 0;  // RX -> PD0
// const int Pir6Pin   = 4;  // GPIO1 -> PD4
// const int Pir7Pin   = 2;  // GPIO0 -> PD2

// // Light config
// const int Light1Pin = A2; // ADC2 -> PC2
// const int Light2Pin = A3; // ADC3 -> PC3
// const int Light3Pin = A6; // ADC6 -> P
// const int Light4Pin = A4; // ADC4 -> PC4
// const int Light5Pin = A5; // ADC5 -> PC5
// const int Light6Pin = A1; // ADC1 -> PC1
// const int Light7Pin = A0; // ADC0 -> PC0

// const int Led1Pin = 9;  // PWM1 -> PB1
// const int Led2Pin = 6;  // PWM2 -> PD6
// const int Led3Pin = 5;  // PWM3 -> PD5
// const int Led4Pin = 3;  // PWM0 -> PD3
