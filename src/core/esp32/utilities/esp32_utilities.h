#pragma once

#include <Arduino.h>
#include <PubSubClient.h>
#include <WiFi.h>
#include <WiFiClientSecure.h>

namespace IoTNet {

class Utilities {
  public:
    // Get formatted timestamp [HH:MM:SS.mmm]
    static String getTimestamp();

  private:
    // Disable instantiation
    Utilities() = delete;
    ~Utilities() = delete;
    Utilities(const Utilities &) = delete;
    Utilities &operator=(const Utilities &) = delete;
};

} // namespace IoTNet
