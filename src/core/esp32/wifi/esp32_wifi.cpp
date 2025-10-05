#include "esp32_wifi.h"

namespace IoTNet {

ESP32WiFi::ESP32WiFi() : wifiSsid(nullptr), wifiPassword(nullptr), lastReconnectAttempt(0) {}

ESP32WiFi::~ESP32WiFi() {
    WiFi.disconnect(true);
}

bool ESP32WiFi::begin(const char *ssid, const char *password) {
    wifiSsid = ssid;
    wifiPassword = password;

    Serial.printf("SSID: %s\n", ssid);
    Serial.print("Connecting to WiFi");

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
        Serial.printf("IP Address: %s\n", WiFi.localIP().toString().c_str());
        Serial.printf("Signal Strength: %d dBm\n", WiFi.RSSI());
        Serial.println("✅ WiFi Connected!");
        lastReconnectAttempt = millis();
        return true;
    } else {
        Serial.println();
        Serial.println("❌ WiFi connection failed!");
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
            Serial.println("⚠️  WiFi disconnected! Reconnecting...");
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
                Serial.printf("IP Address: %s\n", WiFi.localIP().toString().c_str());
                Serial.println("✅ WiFi reconnected!");
            } else {
                Serial.println();
                Serial.println("❌ WiFi reconnection failed!");
            }
        }
    }
}

} // namespace IoTNet
