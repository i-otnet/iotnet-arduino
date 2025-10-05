#pragma once

#include "core/esp32/wifi/esp32_wifi_config.h"
#include "core/esp32/wifi/esp32_wifi.h"
#include "core/esp32/mqtt/esp32_mqtt_config.h"
#include "core/esp32/mqtt/esp32_mqtt.h"
#include "core/esp32/utilities/esp32_utilities.h"

namespace IoTNet {

class IoTNetAgent {
public:
    IoTNetAgent();
    ~IoTNetAgent();
    
    // Set WiFi credentials
    void setWiFiConfig(const char* ssid, const char* password);
    
    // Set MQTT broker configuration
    void setMqttConfig(const char* server, uint16_t port, const char* user, const char* pass);
    
    // Initialize library - auto load CA cert, setup WiFi and MQTT
    void begin();
    
    // Loop - must be called in loop(), auto-reconnect if disconnected
    void loop();
    
    // Publish message
    bool publish(const char* topic, const char* payload);
    bool publish(const char* topic, const char* payload, bool retained);
    
    // Check MQTT connection
    bool isConnected();
    
    // Check WiFi connection
    bool isWiFiConnected();
    
    // Get WiFi IP address
    String getIPAddress();

private:
    ESP32WiFi wifi;
    WiFiClientSecure espClient;
    ESP32MQTT mqtt;
    unsigned long lastReconnectAttempt;
    static constexpr unsigned long reconnectInterval = 5000; // 5 seconds
    
    // WiFi configuration (runtime override)
    const char* wifiSsid;
    const char* wifiPassword;
    bool customWifiConfigSet;
    
    // MQTT configuration (runtime override)
    const char* mqttServer;
    uint16_t mqttPort;
    const char* mqttUser;
    const char* mqttPass;
    bool customMqttConfigSet;
    
    // Default callback for MQTT messages
    static void defaultCallback(char* topic, byte* payload, unsigned int length);
    
    // Disable copy constructor and assignment
    IoTNetAgent(const IoTNetAgent&) = delete;
    IoTNetAgent& operator=(const IoTNetAgent&) = delete;
};

} // namespace IoTNet

using namespace IoTNet;
