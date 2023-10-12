/**
 * Copyright (c) 2021 WIZnet Co.,Ltd
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/**
 * ----------------------------------------------------------------------------------------------------
 * Includes
 * ----------------------------------------------------------------------------------------------------
 */
#include <stdio.h>
#include <string.h>
#include <iostream>
#include <sstream> 

#include "port_common.h"

#include "wizchip_conf.h"
// #include "w5x00_spi.h" // covered by extern "C

#include "mqtt_interface.h"
#include "MQTTClient.h"

// #include "timer.h"     // covered by extern "C

#include "defines.h"


/**
 * ----------------------------------------------------------------------------------------------------
 * Variables
 * ----------------------------------------------------------------------------------------------------
 */
/* Network */
static wiz_NetInfo g_net_info =
    {
        .mac = MAC,                         // MAC address
        .ip  = IP,                          // IP address
        .sn  = SUBNET,                      // Subnet Mask
        .gw  = GATEWAY,                     // Gateway
        .dns = DNS,                         // DNS server
        .dhcp = NETINFO_STATIC              // DHCP enable/disable
};

/* MQTT */
static uint8_t g_mqtt_send_buf[ETHERNET_BUF_MAX_SIZE] = {
    0,
};
static uint8_t g_mqtt_recv_buf[ETHERNET_BUF_MAX_SIZE] = {
    0,
};
static uint8_t g_mqtt_broker_ip[4] = MQTT_SERVER;
static Network g_mqtt_network;
static MQTTClient g_mqtt_client;
static MQTTPacket_connectData g_mqtt_packet_connect_data = MQTTPacket_connectData_initializer;
static MQTTMessage g_mqtt_message;

/* Timer  */
static volatile uint32_t g_msec_cnt = 0;

/**
 * ----------------------------------------------------------------------------------------------------
 * Functions
 * ----------------------------------------------------------------------------------------------------
 */
/* Clock */
static void set_clock_khz(void);

/* MQTT */
static void message_arrived(MessageData *msg_data);

/* Timer  */
static void repeating_timer_callback(void);
static time_t millis(void);

void networkConnect();
void mqttConnect();
bool sendMqtt(const string& topic, const string& data);
bool sendMqtt(const string& topic, const int value);

void log(const string& domain, const string& log) { std::cout << domain << ": " << log << std::endl; }

/**
 * ----------------------------------------------------------------------------------------------------
 * Main
 * ----------------------------------------------------------------------------------------------------
 */
int main()
{
    /* Initialize */
    int32_t retval = 0;
    uint32_t start_ms = 0;
    uint32_t end_ms = 0;

    start_ms = millis();

    /* Infinite loop */
    while (1)
    {
        if (g_mqtt_client.isconnected)
        {
            bool result = true;
            if ((retval = MQTTYield(&g_mqtt_client, g_mqtt_packet_connect_data.keepAliveInterval)) < 0)
            {
                log("MQTT", "Yield error: " + retval);
                result = false;
            }

            end_ms = millis();

            if (end_ms > start_ms + MQTT_PUBLISH_PERIOD)
            {
                log("MQTT", "sending BRO");
                result = sendMqtt("ELDO","MORDO");
                start_ms = millis();
            }

            if (!result)
            {
                g_mqtt_client.isconnected = 0;
            }
        }
        else
        {
            networkConnect();
            mqttConnect();
        }
    }
}

/**
 * ----------------------------------------------------------------------------------------------------
 * Functions
 * ----------------------------------------------------------------------------------------------------
 */
/* Clock */
static void set_clock_khz(void)
{
    // set a system clock frequency in khz
    set_sys_clock_khz(PLL_SYS_KHZ, true);

    // configure the specified clock
    clock_configure(
        clk_peri,
        0,                                                // No glitchless mux
        CLOCKS_CLK_PERI_CTRL_AUXSRC_VALUE_CLKSRC_PLL_SYS, // System PLL on AUX mux
        PLL_SYS_KHZ * 1000,                               // Input frequency
        PLL_SYS_KHZ * 1000                                // Output (must be same as no divider)
    );
}

/* Timer */
static void repeating_timer_callback(void)
{
    g_msec_cnt++;

    MilliTimer_Handler();
}

static time_t millis(void)
{
    return g_msec_cnt;
}

extern "C" void wizchip_spi_initialize(void);
extern "C" void wizchip_cris_initialize(void);
extern "C" void wizchip_reset(void);
extern "C" void wizchip_initialize(void);
extern "C" void wizchip_check(void);
extern "C" void network_initialize(wiz_NetInfo net_info);
extern "C" void print_network_information(wiz_NetInfo net_info);
extern "C" void wizchip_1ms_timer_initialize(void (*callback)(void));

///////////===================================================================

void networkConnect()
{
    int32_t retval = 0;

    set_clock_khz();

    stdio_init_all();

    wizchip_spi_initialize();
    wizchip_cris_initialize();

    wizchip_reset();
    wizchip_initialize();
    wizchip_check();

    wizchip_1ms_timer_initialize(repeating_timer_callback);

    network_initialize(g_net_info);

    /* Get network information */
    print_network_information(g_net_info);

    NewNetwork(&g_mqtt_network, SOCKET_MQTT);

    retval = ConnectNetwork(&g_mqtt_network, g_mqtt_broker_ip, MQTT_PORT);

    if (retval != 1)
    {
        log("Network", "Network connect failed");
    }
}


void mqttConnect()
{
    int32_t retval = 0;

    log("MQTT", "mqttConnect start");
    /* Initialize MQTT client */
    MQTTClientInit(&g_mqtt_client, &g_mqtt_network, DEFAULT_TIMEOUT, g_mqtt_send_buf, ETHERNET_BUF_MAX_SIZE, g_mqtt_recv_buf, ETHERNET_BUF_MAX_SIZE);

    /* Connect to the MQTT broker */
    g_mqtt_packet_connect_data.MQTTVersion = 3;
    g_mqtt_packet_connect_data.cleansession = 1;
    g_mqtt_packet_connect_data.willFlag = 0;
    g_mqtt_packet_connect_data.keepAliveInterval = MQTT_KEEP_ALIVE;
    g_mqtt_packet_connect_data.clientID.cstring = const_cast<char*>(MQTT_CLIENT_ID.c_str());
    g_mqtt_packet_connect_data.username.cstring = const_cast<char*>(MQTT_USERNAME.c_str());
    g_mqtt_packet_connect_data.password.cstring = const_cast<char*>(MQTT_PASSWORD.c_str());

    retval = MQTTConnect(&g_mqtt_client, &g_mqtt_packet_connect_data);

    if (retval < 0)
    {
        log("MQTT", "MQTT connect failed: " + retval);
    }

    log("MQTT", "MQTT connected");

    /* Subscribe */
    retval = MQTTSubscribe(&g_mqtt_client, pwm1TopicCmnd.c_str(), QOS0, message_arrived);

    if (retval < 0)
    {
        log("MQTT", "Subscribe failed: "+ retval);
    }

    log("MQTT", "Subscribed");
}

bool sendMqtt(const string& topic, const string& data)
{
    /* Configure publish message */
    g_mqtt_message.qos = QOS0;
    g_mqtt_message.retained = 0;
    g_mqtt_message.dup = 0;
    g_mqtt_message.payload = const_cast<char*>(data.c_str());
    g_mqtt_message.payloadlen = strlen(data.c_str());

    int32_t retval = 0;
    /* Publish */
    retval = MQTTPublish(&g_mqtt_client, topic.c_str(), &g_mqtt_message);
    if (retval < 0)
    {
        stringstream logStream;
        logStream << "Publish failed: " << retval;
        string logString;
        logStream >> logString;
        log("MQTT", logString);
        return false;
    }

    log("MQTT", "Published");
    return true;
}

bool sendMqtt(const string& topic, const int value)
{
  char dataChar[6];
  itoa(value, dataChar, 10);
  const string dataStr(dataChar);
  return sendMqtt(topic, dataStr);
}

/* MQTT */
static void message_arrived(MessageData *msg_data)
{
    MQTTMessage *message = msg_data->message;

    printf("%.*s", (uint32_t)message->payloadlen, (uint8_t *)message->payload);
}

// void messageReceived(String &topic, String &payload)
// {
//   #ifdef DEBUG
//   Serial.print(topic);
//   Serial.print(" => ");
//   Serial.print(payload);
//   Serial.println();
//   #endif

//   if (topic.indexOf(ledDimmer1.getMqttTopic()) >= 0)
//   {
//     int value = atoi(payload.c_str());
//     ledDimmer1.setValue(value);
//   }
//   else if (topic.indexOf(ledDimmer2.getMqttTopic()) >= 0)
//   {
//     int value = atoi(payload.c_str());
//     ledDimmer2.setValue(value);
//   }
//   else if (topic.indexOf(ledDimmer3.getMqttTopic()) >= 0)
//   {
//     int value = atoi(payload.c_str());
//     ledDimmer3.setValue(value);
//   }
//   else if (topic.indexOf(ledDimmer4.getMqttTopic()) >= 0)
//   {
//     int value = atoi(payload.c_str());
//     ledDimmer4.setValue(value);
//   }
//   else
//   {
//     #ifdef DEBUG
//     Serial.println("unknown");
//     #endif
//   }
// }
