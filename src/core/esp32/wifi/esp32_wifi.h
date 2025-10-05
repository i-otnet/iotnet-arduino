#pragma once

#include "../utilities/esp32_utilities.h"

namespace IoTNet {

class ESP32WiFi {
public:
    ESP32WiFi();
    ~ESP32WiFi();
    
    // Initialize WiFi with custom credentials
    bool begin(const char* ssid, const char* password);
    
    // Check WiFi connection status
    bool isConnected();
    
    // Get IP address
    String getIPAddress();
    
    // Auto-reconnect if disconnected
    void loop();
    
private:
    const char* wifiSsid;
    const char* wifiPassword;
    unsigned long lastReconnectAttempt;
    static constexpr unsigned long reconnectInterval = 5000; // 5 seconds
    
    // Disable copy constructor and assignment
    ESP32WiFi(const ESP32WiFi&) = delete;
    ESP32WiFi& operator=(const ESP32WiFi&) = delete;
};

} // namespace IoTNet
