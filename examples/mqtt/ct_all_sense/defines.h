#pragma once

#include <string>
#include <vector>

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
#define SOCKET_DHCP 0
#define SOCKET_DNS 1

/* Retry count */
#define DHCP_RETRY_COUNT 5
#define DNS_RETRY_COUNT 5

/* Timeout */
#define DEFAULT_TIMEOUT 1000 // 1 second

//======================================================================================

std::string charToHexString(char input) {
    const char hexDigits[] = "0123456789ABCDEF";
    std::string result;
    result += hexDigits[(input >> 4) & 0x0F];  // Extract the high nibble
    result += hexDigits[input & 0x0F];         // Extract the low nibble
    return result;
}

// GENERIC CONF
#define IP       { 192, 168, 38, 111 }
#define SUBNET   { 255, 255, 255, 0 }
#define GATEWAY  { 192, 168, 38, 1 }
#define DNS      { 8, 8, 8, 8 }

// Ethernet config
// // Bolus Ratusz
// #define MAC      { 0x00, 0x08, 0xDC, 0x45, 0xA8, 0x11 }
// #define IP       { 192, 168, 89, 101 }
// #define SUBNET   { 255, 255, 254, 0 }
// #define GATEWAY  { 192, 168, 88, 1 }
// #define MQTT_SERVER { 192, 168, 89, 100 }


// Bolus Ogrodek Alarm
// #define MAC      { 0x00, 0x08, 0xDC, 0x75, 0xB0, 0x21 }
// #define MQTT_SERVER { 192, 168, 89, 100 }

// Bolus Ratusz Alarm
// #define MAC      { 0x00, 0x08, 0xDC, 0x75, 0xE3, 0x27 }
// #define MQTT_SERVER { 192, 168, 89, 100 }

// Kleczko
#define MAC      { 0x2C, 0x08, 0xDC, 0xAD, 0x3D, 0x94 }
#define MQTT_SERVER { 192, 168, 38, 11 }

// #define IP       { 192, 168, 89, 104 }
// #define SUBNET   { 255, 255, 254, 0 }
// #define GATEWAY  { 192, 168, 88, 1 }
// #define MQTT_SERVER { 192, 168, 89, 100 }


// // WLKP
// #define MAC      { 0x00, 0x08, 0xDC, 0xC8, 0x76, 0x21 }
// #define IP       { 192, 168, 116, 199 }
// #define SUBNET   { 255, 255, 255, 0 }
// #define GATEWAY  { 192, 168, 116, 1 }
// #define MQTT_SERVER { 192, 168, 116, 135 }

//======================================================================================

/* MQTT IP */
#define MQTT_PUBLISH_PERIOD (1000 * 1) // 1 seconds
#define MQTT_KEEP_ALIVE 10 // milliseconds
const uint16_t MQTT_PORT = 1883;

// MQTT
const std::vector<char> macAddr = MAC;
const string MQTT_CLIENT_ID = "CT-pico-sens-" + charToHexString(macAddr[3]) + charToHexString(macAddr[4]) + charToHexString(macAddr[5]);
const string deviceName = "CTpicoSens_" + charToHexString(macAddr[3]) + charToHexString(macAddr[4]) + charToHexString(macAddr[5]);
const string MQTT_USERNAME = "wiznet";
const string MQTT_PASSWORD = "0123456789";

const string cmndSufix  = "/cmnd";

const string statusTopic      = deviceName + "/status";
const string uptimeTopic      = deviceName + "/uptime";
const string willTopic        = deviceName + "/LWT";
const string willMessageOff   = "offline";
const string willMessageOn    = "online";

const string In1A_Topic       = deviceName + "/in1a";
const string In1B_Topic       = deviceName + "/in1b";
const string In2A_Topic       = deviceName + "/in2a";
const string In2B_Topic       = deviceName + "/in2b";
const string In3A_Topic       = deviceName + "/in3a";
const string In3B_Topic       = deviceName + "/in3b";
const string In4A_Topic       = deviceName + "/in4a";
const string In4B_Topic       = deviceName + "/in4b";
const string In5A_Topic       = deviceName + "/in5a";
const string In5B_Topic       = deviceName + "/in5b";
const string In6A_Topic       = deviceName + "/in6a";
const string In6B_Topic       = deviceName + "/in6b";
const string In7A_Topic       = deviceName + "/in7a";
const string In7B_Topic       = deviceName + "/in7b";
const string In8A_Topic       = deviceName + "/in8a";
const string In8B_Topic       = deviceName + "/in8b";
const string In9A_Topic       = deviceName + "/in9a";
const string In9B_Topic       = deviceName + "/in9b";

const int In1aPin = 2;
const int In1bPin = 3;
const int In2aPin = 4;
const int In2bPin = 5;
const int In3aPin = 6;
const int In3bPin = 7;
const int In4aPin = 8;
const int In4bPin = 9;
const int In5aPin = 10;
const int In5bPin = 11;
const int In6aPin = 12;
const int In6bPin = 13;
const int In7aPin = 28;
const int In7bPin = 14;
const int In8aPin = 27;
const int In8bPin = 15;
const int In9aPin = 26;
const int In9bPin = 22;


typedef struct Input
{
    bool initDone;
    bool measuredValue;
    bool lastReportedValue;
    const string& topicStat;
    const uint8_t pinNo;

    Input(const string& _topicStat, const uint8_t _pinNo)
     : initDone(false) 
     , measuredValue(false)
     , lastReportedValue(false)
     , topicStat(_topicStat)
     , pinNo(_pinNo)
    {}
} Input;


Input in1a = Input(In1A_Topic, In1aPin);
Input in1b = Input(In1B_Topic, In1bPin);
Input in2a = Input(In2A_Topic, In2aPin);
Input in2b = Input(In2B_Topic, In2bPin);
Input in3a = Input(In3A_Topic, In3aPin);
Input in3b = Input(In3B_Topic, In3bPin);
Input in4a = Input(In4A_Topic, In4aPin);
Input in4b = Input(In4B_Topic, In4bPin);
Input in5a = Input(In5A_Topic, In5aPin);
Input in5b = Input(In5B_Topic, In5bPin);
Input in6a = Input(In6A_Topic, In6aPin);
Input in6b = Input(In6B_Topic, In6bPin);
Input in7a = Input(In7A_Topic, In7aPin);
Input in7b = Input(In7B_Topic, In7bPin);
Input in8a = Input(In8A_Topic, In8aPin);
Input in8b = Input(In8B_Topic, In8bPin);
Input in9a = Input(In9A_Topic, In9aPin);
Input in9b = Input(In9B_Topic, In9bPin);
