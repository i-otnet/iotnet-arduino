#include "IoTNetESP32.h"
#include "certs/mqtt_ca_cert.h"

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
    Serial.printf("WiFi Config set: %s\n", ssid);
}

void IoTNetAgent::setMqttConfig(const char *server, uint16_t port, const char *user,
                                const char *pass) {
    mqttServer = server;
    mqttPort = port;
    mqttUser = user;
    mqttPass = pass;
    customMqttConfigSet = true;
    Serial.printf("MQTT Config set: %s:%d\n", server, port);
}

void IoTNetAgent::defaultCallback(char *topic, byte *payload, unsigned int length) {
    Serial.printf("%s Topic: %s\n", Utilities::getTimestamp().c_str(), topic);
    Serial.printf("%s Payload (%d bytes): ", Utilities::getTimestamp().c_str(), length);
    for (unsigned int i = 0; i < length; i++) {
        Serial.write(payload[i]);
    }
    Serial.println();
}

void IoTNetAgent::begin() {
    // Step 1: Initialize WiFi
    const char *ssid = customWifiConfigSet ? wifiSsid : IoTNetConfig::kWifiSsid;
    const char *password = customWifiConfigSet ? wifiPassword : IoTNetConfig::kWifiPass;

    Serial.printf("%s IoTNet ESP32 Initializing...\n", Utilities::getTimestamp().c_str());

    // Connect to WiFi
    while (!wifi.begin(ssid, password)) {
        Serial.printf("%s Retrying WiFi in 5s...\n", Utilities::getTimestamp().c_str());
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
    Serial.printf("%s Connecting to MQTT broker...\n", Utilities::getTimestamp().c_str());
    while (!mqtt.reconnect()) {
        Serial.printf("%s Retrying in 5s...\n", Utilities::getTimestamp().c_str());
        delay(5000);
    }

    // Subscribe to default topic
    mqtt.subscribe("iotnet/test");

    lastReconnectAttempt = millis();
    Serial.printf("%s ✅ IoTNet ESP32 Ready!\n", Utilities::getTimestamp().c_str());
}

void IoTNetAgent::loop() {
    // Auto-reconnect WiFi if disconnected
    wifi.loop();

    // Auto-reconnect MQTT if disconnected
    if (!mqtt.isConnected()) {
        unsigned long now = millis();
        if (now - lastReconnectAttempt > reconnectInterval) {
            lastReconnectAttempt = now;

            Serial.printf("%s Reconnecting to MQTT...\n", Utilities::getTimestamp().c_str());
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
