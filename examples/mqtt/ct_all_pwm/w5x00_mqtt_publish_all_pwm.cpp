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
// #include "timer.h"     // covered by extern "C
#include "mqtt_interface.h"
#include "MQTTClient.h"

#include "defines.h"

/* pico include */
#include "pico/stdlib.h"
#include "pico/time.h"
#include "hardware/adc.h"
#include "hardware/gpio.h"
#include "hardware/irq.h"
#include "hardware/pwm.h"
#include "hardware/watchdog.h"
/* pico include end */

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
// Last Will Testament for MQTT
static MQTTPacket_willOptions g_mqtt_will_options = MQTTPacket_willOptions_initializer;

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
static void message_arrived_pwm1(MessageData *msg_data);
static void message_arrived_pwm2(MessageData *msg_data);
static void message_arrived_pwm3(MessageData *msg_data);
static void message_arrived_pwm4(MessageData *msg_data);
static void message_arrived_pwm5(MessageData *msg_data);
static void message_arrived_pwm6(MessageData *msg_data);
static void message_arrived_pwm7(MessageData *msg_data);
static void message_arrived_pwm8(MessageData *msg_data);

static void message_arrived_pwm9(MessageData *msg_data);
static void message_arrived_pwm10(MessageData *msg_data);
static void message_arrived_pwm11(MessageData *msg_data);
static void message_arrived_pwm12(MessageData *msg_data);
static void message_arrived_pwm13(MessageData *msg_data);
static void message_arrived_pwm14(MessageData *msg_data);
static void message_arrived_pwm15(MessageData *msg_data);
static void message_arrived_pwm16(MessageData *msg_data);

/* Timer  */
static void repeating_timer_callback(void);
static time_t millis(void);

void networkConfig();
void mqttConnect();
bool sendMqtt(const string& topic, const string& data);
bool sendMqtt(const string& topic, const int value);

void configPWM(const uint8_t pinNo);
void on_pwm_wrap();

void log(const string& domain, const string& log) { std::cout << domain << ": " << log << std::endl; }
void log(const string& domain, const uint32_t value) { std::cout << domain << ": " << value << std::endl; }

/**
 * ----------------------------------------------------------------------------------------------------
 * Main
 * ----------------------------------------------------------------------------------------------------
 */
int main()
{
    set_clock_khz();
    stdio_init_all();

    watchdog_enable(10000, false);

    configPWM(pwm1.pinNo);
    configPWM(pwm2.pinNo);
    configPWM(pwm3.pinNo);
    configPWM(pwm4.pinNo);
    configPWM(pwm5.pinNo);
    configPWM(pwm6.pinNo);
    configPWM(pwm7.pinNo);
    configPWM(pwm8.pinNo);

    configPWM(pwm9.pinNo);
    configPWM(pwm10.pinNo);
    configPWM(pwm11.pinNo);
    configPWM(pwm12.pinNo);
    configPWM(pwm13.pinNo);
    configPWM(pwm14.pinNo);
    configPWM(pwm15.pinNo);
    configPWM(pwm16.pinNo);

    networkConfig();

    /* Initialize */
    int32_t retval = 0;
    uint32_t start_ms = 0;
    uint32_t end_ms = 0;
    bool initDone = false;

    start_ms = millis();

    /* Infinite loop */
    while (1)
    {
        if (g_mqtt_client.isconnected)
        {
            watchdog_update();

            if (!initDone)
            {
                auto initRet = sendMqtt(willTopic, willMessageOn);
                initDone = initRet;
            }

            bool result = true;
            if ((retval = MQTTYield(&g_mqtt_client, g_mqtt_packet_connect_data.keepAliveInterval)) < 0)
            {
                log("MQTT", "Yield error: ");// + retval);
                result = false;
            }

            end_ms = millis();

            if (end_ms > start_ms + MQTT_PUBLISH_PERIOD)
            {
                result = sendMqtt(uptimeTopic, millis()/1000) && result;
                start_ms = millis();
            }

            if (!result)
            {
                g_mqtt_client.isconnected = 0;
            }
        }
        else
        {
            mqttConnect();
            initDone = false;
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


// MQTT
extern "C" void wizchip_spi_initialize(void);
extern "C" void wizchip_cris_initialize(void);
extern "C" void wizchip_reset(void);
extern "C" void wizchip_initialize(void);
extern "C" void wizchip_check(void);
extern "C" void network_initialize(wiz_NetInfo net_info);
extern "C" void print_network_information(wiz_NetInfo net_info);
extern "C" void wizchip_1ms_timer_initialize(void (*callback)(void));

///////////===================================================================

void networkConfig()
{
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
}

string getNetInfo(wiz_NetInfo net_info)
{
    stringstream stream;
    ctlnetwork(CN_GET_NETINFO, (void *)&net_info);

    if (net_info.dhcp == NETINFO_DHCP)
    {
        stream << "network config : DHCP\n";
    }
    else
    {
        stream << "network config : static\n";
    }

    stream << " MAC         : " << net_info.mac << "\n";
    // printf(" IP          : %d.%d.%d.%d\n", net_info.ip[0], net_info.ip[1], net_info.ip[2], net_info.ip[3]);
    // printf(" Subnet Mask : %d.%d.%d.%d\n", net_info.sn[0], net_info.sn[1], net_info.sn[2], net_info.sn[3]);
    // printf(" Gateway     : %d.%d.%d.%d\n", net_info.gw[0], net_info.gw[1], net_info.gw[2], net_info.gw[3]);
    // printf(" DNS         : %d.%d.%d.%d\n", net_info.dns[0], net_info.dns[1], net_info.dns[2], net_info.dns[3]);
    string str;
    stream >> str;
    return str;
}

void mqttConnect()
{
    int32_t retval = 0;

    retval = ConnectNetwork(&g_mqtt_network, g_mqtt_broker_ip, MQTT_PORT);

    if (retval != 1)
    {
        log("Network", "Network connect failed");
        return;
    }

    log("MQTT", "mqttConnect start");
    /* Initialize MQTT client */
    MQTTClientInit(&g_mqtt_client, &g_mqtt_network, DEFAULT_TIMEOUT, g_mqtt_send_buf, ETHERNET_BUF_MAX_SIZE, g_mqtt_recv_buf, ETHERNET_BUF_MAX_SIZE);

    /* Connect to the MQTT broker */
    g_mqtt_packet_connect_data.MQTTVersion = 3;
    g_mqtt_packet_connect_data.cleansession = 1;
    g_mqtt_packet_connect_data.willFlag = 1;
    g_mqtt_packet_connect_data.keepAliveInterval = MQTT_KEEP_ALIVE;
    g_mqtt_packet_connect_data.clientID.cstring = const_cast<char*>(MQTT_CLIENT_ID.c_str());
    g_mqtt_packet_connect_data.username.cstring = const_cast<char*>(MQTT_USERNAME.c_str());
    g_mqtt_packet_connect_data.password.cstring = const_cast<char*>(MQTT_PASSWORD.c_str());
    g_mqtt_packet_connect_data.will = g_mqtt_will_options;
    g_mqtt_will_options.topicName.cstring = const_cast<char*>(willTopic.c_str());
    g_mqtt_will_options.message.cstring   = const_cast<char*>(willMessageOff.c_str());
    g_mqtt_will_options.retained = 1;
    
    retval = MQTTConnect(&g_mqtt_client, &g_mqtt_packet_connect_data);

    if (retval < 0)
    {
        log("MQTT", "MQTT connect failed: ");// + retval);
        return;
    }

    log("MQTT", "MQTT connected");

    sendMqtt(statusTopic, getNetInfo(g_net_info));

    /* Subscribe */
    retval = MQTTSubscribe(&g_mqtt_client, pwm1.topicCmnd.c_str(), QOS0, message_arrived_pwm1);
    retval = MQTTSubscribe(&g_mqtt_client, pwm2.topicCmnd.c_str(), QOS0, message_arrived_pwm2);
    retval = MQTTSubscribe(&g_mqtt_client, pwm3.topicCmnd.c_str(), QOS0, message_arrived_pwm3);
    retval = MQTTSubscribe(&g_mqtt_client, pwm4.topicCmnd.c_str(), QOS0, message_arrived_pwm4);
    retval = MQTTSubscribe(&g_mqtt_client, pwm5.topicCmnd.c_str(), QOS0, message_arrived_pwm5);
    retval = MQTTSubscribe(&g_mqtt_client, pwm6.topicCmnd.c_str(), QOS0, message_arrived_pwm6);
    retval = MQTTSubscribe(&g_mqtt_client, pwm7.topicCmnd.c_str(), QOS0, message_arrived_pwm7);
    retval = MQTTSubscribe(&g_mqtt_client, pwm8.topicCmnd.c_str(), QOS0, message_arrived_pwm8);

    retval = MQTTSubscribe(&g_mqtt_client, pwm9.topicCmnd.c_str(), QOS0, message_arrived_pwm9);
    retval = MQTTSubscribe(&g_mqtt_client, pwm10.topicCmnd.c_str(), QOS0, message_arrived_pwm10);
    retval = MQTTSubscribe(&g_mqtt_client, pwm11.topicCmnd.c_str(), QOS0, message_arrived_pwm11);
    retval = MQTTSubscribe(&g_mqtt_client, pwm12.topicCmnd.c_str(), QOS0, message_arrived_pwm12);
    retval = MQTTSubscribe(&g_mqtt_client, pwm13.topicCmnd.c_str(), QOS0, message_arrived_pwm13);
    retval = MQTTSubscribe(&g_mqtt_client, pwm14.topicCmnd.c_str(), QOS0, message_arrived_pwm14);
    retval = MQTTSubscribe(&g_mqtt_client, pwm15.topicCmnd.c_str(), QOS0, message_arrived_pwm15);
    retval = MQTTSubscribe(&g_mqtt_client, pwm16.topicCmnd.c_str(), QOS0, message_arrived_pwm16);

    if (retval < 0)
    {
        log("MQTT", "Subscribe failed: ");//+ retval);
        return;
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
        log("MQTT", "Publish failed");
        return false;
    }
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
void setMqttPwmTarget(PWMchannel& channel, uint8_t targetPwm)
{
    channel.targetPWM = targetPwm;
}

uint8_t getValueFromMsg(MessageData *msg_data)
{
    MQTTMessage *message = msg_data->message;
    string payload(static_cast<char*>(message->payload), message->payloadlen);
    return stoi(payload);
}

static void handle_pwm_message(MessageData *msg_data, PWMchannel& channel)
{
    log("MQTT", "received: " + channel.topicCmnd);
    auto value = getValueFromMsg(msg_data) * 2.55;
    log("MQTT", value);
    setMqttPwmTarget(channel, value);
    sendMqtt(channel.topicStat, value);
}

static void message_arrived_pwm1(MessageData *msg_data) {handle_pwm_message(msg_data, pwm1);};
static void message_arrived_pwm2(MessageData *msg_data) {handle_pwm_message(msg_data, pwm2);};
static void message_arrived_pwm3(MessageData *msg_data) {handle_pwm_message(msg_data, pwm3);};
static void message_arrived_pwm4(MessageData *msg_data) {handle_pwm_message(msg_data, pwm4);};
static void message_arrived_pwm5(MessageData *msg_data) {handle_pwm_message(msg_data, pwm5);};
static void message_arrived_pwm6(MessageData *msg_data) {handle_pwm_message(msg_data, pwm6);};
static void message_arrived_pwm7(MessageData *msg_data) {handle_pwm_message(msg_data, pwm7);};
static void message_arrived_pwm8(MessageData *msg_data) {handle_pwm_message(msg_data, pwm8);};

static void message_arrived_pwm9(MessageData *msg_data)  {handle_pwm_message(msg_data, pwm9);};
static void message_arrived_pwm10(MessageData *msg_data) {handle_pwm_message(msg_data, pwm10);};
static void message_arrived_pwm11(MessageData *msg_data) {handle_pwm_message(msg_data, pwm11);};
static void message_arrived_pwm12(MessageData *msg_data) {handle_pwm_message(msg_data, pwm12);};
static void message_arrived_pwm13(MessageData *msg_data) {handle_pwm_message(msg_data, pwm13);};
static void message_arrived_pwm14(MessageData *msg_data) {handle_pwm_message(msg_data, pwm14);};
static void message_arrived_pwm15(MessageData *msg_data) {handle_pwm_message(msg_data, pwm15);};
static void message_arrived_pwm16(MessageData *msg_data) {handle_pwm_message(msg_data, pwm16);};

void message_arrived(MessageData *msg_data)
{
    log("MQTT", "received");
    MQTTMessage *message = msg_data->message;
    string topic(msg_data->topicName->lenstring.data, msg_data->topicName->lenstring.len);
    log("MQTT", topic);
    string payload(static_cast<char*>(message->payload), message->payloadlen);
    log("MQTT", payload);

    int value = stoi(payload);
}

void configPWM(const uint8_t pinNo)
{
    // Tell the LED pin that the PWM is in charge of its value.
    gpio_set_function(pinNo, GPIO_FUNC_PWM);
    // Figure out which slice we just connected to the LED pin
    uint slice_num = pwm_gpio_to_slice_num(pinNo);

    // Mask our slice's IRQ output into the PWM block's single interrupt line,
    // and register our interrupt handler
    pwm_clear_irq(slice_num);
    pwm_set_irq_enabled(slice_num, true);
    irq_set_exclusive_handler(PWM_IRQ_WRAP, on_pwm_wrap);
    irq_set_enabled(PWM_IRQ_WRAP, true);

    // Get some sensible defaults for the slice configuration. By default, the
    // counter is allowed to wrap over its maximum range (0 to 2**16-1)
    pwm_config config = pwm_get_default_config();
    // Set divider, reduces counter clock to sysclock/this value
    pwm_config_set_clkdiv(&config, 4.f);
    // Load the configuration into our PWM slice, and set it running.
    pwm_init(slice_num, &config, true);

    // Everything after this point happens in the PWM interrupt handler, so we
    // can twiddle our thumbs
}

uint16_t calculatePWMValue(PWMchannel& channel)
{
    if (channel.targetPWM != channel.currentPWM)
    {
        if (channel.targetPWM > channel.currentPWM)
            channel.currentPWM++;
        else if (channel.targetPWM < channel.currentPWM)
            channel.currentPWM--;
    }

    return channel.currentPWM * channel.currentPWM;
}

void setPWMValue(PWMchannel& channel)
{
    pwm_clear_irq(pwm_gpio_to_slice_num(channel.pinNo));
    uint16_t value = calculatePWMValue(channel);
    pwm_set_gpio_level(channel.pinNo, value);
}

void on_pwm_wrap() {
    static int fade = 0;
    static bool going_up = true;
    // Clear the interrupt flag that brought us here
    setPWMValue(pwm1);
    setPWMValue(pwm2);
    setPWMValue(pwm3);
    setPWMValue(pwm4);
    setPWMValue(pwm5);
    setPWMValue(pwm6);
    setPWMValue(pwm7);
    setPWMValue(pwm8);

    setPWMValue(pwm9);
    setPWMValue(pwm10);
    setPWMValue(pwm11);
    setPWMValue(pwm12);
    setPWMValue(pwm13);
    setPWMValue(pwm14);
    setPWMValue(pwm15);
    setPWMValue(pwm16);
}