#pragma once

#ifndef WIFI_SSID
#define WIFI_SSID "<your-ssid>"
#endif

#ifndef WIFI_PASS
#define WIFI_PASS "<your-password>"
#endif

namespace IoTNetConfig {
    constexpr const char* kWifiSsid = WIFI_SSID;
    constexpr const char* kWifiPass = WIFI_PASS;
}
