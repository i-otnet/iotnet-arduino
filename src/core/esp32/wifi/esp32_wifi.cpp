#include "esp32_wifi.h"

namespace IoTNet {

ESP32WiFi::ESP32WiFi() : wifiSsid(nullptr), wifiPassword(nullptr), lastReconnectAttempt(0) {}

ESP32WiFi::~ESP32WiFi() {
    WiFi.disconnect(true);
}

bool ESP32WiFi::begin(const char *ssid, const char *password) {
    wifiSsid = ssid;
    wifiPassword = password;

    Serial.printf("%s Connecting to WiFi...\n", Utilities::getTimestamp().c_str());
    Serial.printf("%s SSID: %s\n", Utilities::getTimestamp().c_str(), ssid);

    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);

    int attempts = 0;
    while (WiFi.status() != WL_CONNECTED && attempts < 20) {
        delay(500);
        Serial.print(".");
        attempts++;
    }

    if (WiFi.status() == WL_CONNECTED) {
        Serial.println();
        Serial.printf("%s IP Address: %s\n", Utilities::getTimestamp().c_str(),
                      WiFi.localIP().toString().c_str());
        Serial.printf("%s Signal Strength: %d dBm\n", Utilities::getTimestamp().c_str(),
                      WiFi.RSSI());
        Serial.printf("%s ✅ WiFi connected!\n", Utilities::getTimestamp().c_str());
        lastReconnectAttempt = millis();
        return true;
    } else {
        Serial.println();
        Serial.printf("%s ❌ WiFi connection failed!\n", Utilities::getTimestamp().c_str());
        return false;
    }
}

bool ESP32WiFi::isConnected() {
    return WiFi.status() == WL_CONNECTED;
}

String ESP32WiFi::getIPAddress() {
    if (isConnected()) {
        return WiFi.localIP().toString();
    }
    return "0.0.0.0";
}

void ESP32WiFi::loop() {
    // Auto-reconnect if disconnected
    if (!isConnected()) {
        unsigned long now = millis();
        if (now - lastReconnectAttempt > reconnectInterval) {
            lastReconnectAttempt = now;
            Serial.printf("%s ⚠️  WiFi disconnected! Reconnecting...\n",
                          Utilities::getTimestamp().c_str());
            WiFi.disconnect();
            WiFi.begin(wifiSsid, wifiPassword);

            int attempts = 0;
            while (WiFi.status() != WL_CONNECTED && attempts < 10) {
                delay(500);
                Serial.print(".");
                attempts++;
            }

            if (WiFi.status() == WL_CONNECTED) {
                Serial.println();
                Serial.printf("%s IP Address: %s\n", Utilities::getTimestamp().c_str(),
                              WiFi.localIP().toString().c_str());
                Serial.printf("%s ✅ WiFi reconnected!\n", Utilities::getTimestamp().c_str());
            } else {
                Serial.println();
                Serial.printf("%s ❌ WiFi reconnection failed!\n",
                              Utilities::getTimestamp().c_str());
            }
        }
    }
}

} // namespace IoTNet
