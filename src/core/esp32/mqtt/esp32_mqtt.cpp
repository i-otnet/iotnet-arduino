#include "esp32_mqtt.h"
#include "esp32_mqtt_config.h"
#include <Arduino.h>
#include <sstream>

namespace IoTNet {

ESP32MQTT::ESP32MQTT()
    : mqttClient(nullptr), clientId(generateRandomClientId()),
      mqttServer(IoTNetConfig::kMqttServer), mqttPort(IoTNetConfig::kMqttPort),
      mqttUser(IoTNetConfig::kMqttUser), mqttPass(IoTNetConfig::kMqttPass) {}
std::string ESP32MQTT::generateRandomClientId() {
    // Use millis and random for uniqueness
    std::stringstream ss;
    ss << "ESP32Client-" << (uint32_t)ESP.getEfuseMac() << "-" << random(0xFFFFF);
    return ss.str();
}

ESP32MQTT::~ESP32MQTT() {
    if (mqttClient) {
        delete mqttClient;
    }
}

void ESP32MQTT::begin(WiFiClientSecure &wifiClient, const char *caCert) {
    begin(wifiClient, caCert, mqttServer, mqttPort, mqttUser, mqttPass);
}

void ESP32MQTT::begin(WiFiClientSecure &wifiClient, const char *caCert, const char *server,
                      uint16_t port, const char *user, const char *pass) {
    // Update config
    mqttServer = server;
    mqttPort = port;
    mqttUser = user;
    mqttPass = pass;

    // Set CA certificate
    wifiClient.setCACert(caCert);

    // Create MQTT client instance
    mqttClient = new PubSubClient(wifiClient);

    // Set server and port
    mqttClient->setServer(mqttServer, mqttPort);

    // Set buffer size
    mqttClient->setBufferSize(IoTNetConfig::kMqttMaxPacketSize);

    Serial.println("MQTT initialized");
    Serial.printf("Server: %s:%d\n", mqttServer, mqttPort);
}

void ESP32MQTT::setCallback(MQTT_CALLBACK_SIGNATURE) {
    if (mqttClient) {
        mqttClient->setCallback(callback);
    }
}

bool ESP32MQTT::subscribe(const char *topic) {
    if (mqttClient && mqttClient->connected()) {
        bool result = mqttClient->subscribe(topic);
        if (result) {
            Serial.printf("Subscribed to: %s\n", topic);
        }
        return result;
    }
    return false;
}

bool ESP32MQTT::publish(const char *topic, const char *payload) {
    return publish(topic, payload, false);
}

bool ESP32MQTT::publish(const char *topic, const char *payload, bool retained) {
    if (mqttClient && mqttClient->connected()) {
        return mqttClient->publish(topic, payload, retained);
    }
    return false;
}

void ESP32MQTT::loop() {
    if (mqttClient) {
        mqttClient->loop();
    }
}

bool ESP32MQTT::isConnected() {
    return mqttClient && mqttClient->connected();
}

bool ESP32MQTT::reconnect() {
    if (!mqttClient) {
        return false;
    }

    if (mqttClient->connected()) {
        return true;
    }

    // Regenerate clientId for each reconnect for extra uniqueness
    clientId = generateRandomClientId();

    Serial.print("Connecting to MQTT with clientId: ");
    Serial.println(clientId.c_str());

    if (mqttClient->connect(clientId.c_str(), mqttUser, mqttPass)) {
        Serial.println();
        Serial.println("✅ MQTT Connected!");
        return true;
    } else {
        Serial.printf(" ❌ Failed (rc=%d)\n", mqttClient->state());
        return false;
    }
}

int ESP32MQTT::getState() {
    if (mqttClient) {
        return mqttClient->state();
    }
    return -1;
}

} // namespace IoTNet
