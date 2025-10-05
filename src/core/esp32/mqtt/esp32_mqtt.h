#pragma once

#include <PubSubClient.h>
#include <WiFiClientSecure.h>

namespace IoTNet {

class ESP32MQTT {
  public:
    ESP32MQTT();
    ~ESP32MQTT();

    // Initialize MQTT with CA certificate
    void begin(WiFiClientSecure &wifiClient, const char *caCert);

    // Initialize MQTT with CA certificate and custom config
    void begin(WiFiClientSecure &wifiClient, const char *caCert, const char *server, uint16_t port,
               const char *user, const char *pass);

    // Set callback for receiving messages
    void setCallback(MQTT_CALLBACK_SIGNATURE);

    // Subscribe to topic
    bool subscribe(const char *topic);

    // Publish message
    bool publish(const char *topic, const char *payload);
    bool publish(const char *topic, const char *payload, bool retained);

    // MQTT loop (must be called in loop())
    void loop();

    // Check MQTT connection
    bool isConnected();

    // Reconnect to MQTT
    bool reconnect();

    // Get client state
    int getState();

  private:
    PubSubClient *mqttClient;
    const char *clientId;

    // MQTT configuration
    const char *mqttServer;
    uint16_t mqttPort;
    const char *mqttUser;
    const char *mqttPass;

    // Disable copy constructor and assignment
    ESP32MQTT(const ESP32MQTT &) = delete;
    ESP32MQTT &operator=(const ESP32MQTT &) = delete;
};

} // namespace IoTNet
