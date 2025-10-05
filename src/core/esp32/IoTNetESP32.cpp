#include "IoTNetESP32.h"
#include "certs/mqtt_ca_cert.h"
#include "mqtt/esp32_mqtt_config.h"
#include "wifi/esp32_wifi_config.h"

namespace IoTNet {

IoTNetAgent::IoTNetAgent()
    : lastReconnectAttempt(0), wifiSsid(nullptr), wifiPassword(nullptr), customWifiConfigSet(false),
      mqttServer(nullptr), mqttPort(0), mqttUser(nullptr), mqttPass(nullptr),
      customMqttConfigSet(false) {}

IoTNetAgent::~IoTNetAgent() {}

void IoTNetAgent::setWiFiConfig(const char *ssid, const char *password) {
    wifiSsid = ssid;
    wifiPassword = password;
    customWifiConfigSet = true;
}

void IoTNetAgent::setMqttConfig(const char *server, uint16_t port, const char *user,
                                const char *pass) {
    mqttServer = server;
    mqttPort = port;
    mqttUser = user;
    mqttPass = pass;
    customMqttConfigSet = true;
}

void IoTNetAgent::defaultCallback(char *topic, byte *payload, unsigned int length) {
    Serial.printf("Topic: %s\n", topic);
    Serial.printf("Payload (%d bytes): ", length);
    for (unsigned int i = 0; i < length; i++) {
        Serial.write(payload[i]);
    }
    Serial.println();
}

void IoTNetAgent::begin() {
    // Step 1: Initialize WiFi
    const char *ssid = customWifiConfigSet ? wifiSsid : IoTNetConfig::kWifiSsid;
    const char *password = customWifiConfigSet ? wifiPassword : IoTNetConfig::kWifiPass;

    Serial.println("IoTNet ESP32 Initializing...");

    // Connect to WiFi
    while (!wifi.begin(ssid, password)) {
        Serial.println("Retrying WiFi in 5s...");
        delay(5000);
    }

    // Step 2: Initialize MQTT
    const char *server = customMqttConfigSet ? mqttServer : IoTNetConfig::kMqttServer;
    uint16_t port = customMqttConfigSet ? mqttPort : IoTNetConfig::kMqttPort;
    const char *user = customMqttConfigSet ? mqttUser : IoTNetConfig::kMqttUser;
    const char *pass = customMqttConfigSet ? mqttPass : IoTNetConfig::kMqttPass;

    // Initialize MQTT with CA certificate and config
    mqtt.begin(espClient, ca_cert, server, port, user, pass);

    // Set default callback
    mqtt.setCallback(defaultCallback);

    // Connect to MQTT broker
    Serial.println("Connecting to MQTT broker...");
    while (!mqtt.reconnect()) {
        Serial.println("Retrying in 5s...");
        delay(5000);
    }

    // Subscribe to default topic
    mqtt.subscribe("iotnet/test");

    lastReconnectAttempt = millis();
    Serial.println("✅ IoTNet ESP32 Ready!");
}

void IoTNetAgent::loop() {
    // Auto-reconnect WiFi if disconnected
    wifi.loop();

    // Auto-reconnect MQTT if disconnected
    if (!mqtt.isConnected()) {
        unsigned long now = millis();
        if (now - lastReconnectAttempt > reconnectInterval) {
            lastReconnectAttempt = now;

            Serial.println("Reconnecting to MQTT...");
            if (mqtt.reconnect()) {
                // Re-subscribe after reconnect
                mqtt.subscribe("iotnet/test");
            }
        }
    } else {
        mqtt.loop();
    }
}

bool IoTNetAgent::publish(const char *topic, const char *payload) {
    return mqtt.publish(topic, payload);
}

bool IoTNetAgent::publish(const char *topic, const char *payload, bool retained) {
    return mqtt.publish(topic, payload, retained);
}

bool IoTNetAgent::isConnected() {
    return mqtt.isConnected();
}

bool IoTNetAgent::isWiFiConnected() {
    return wifi.isConnected();
}

String IoTNetAgent::getIPAddress() {
    return wifi.getIPAddress();
}

} // namespace IoTNet
