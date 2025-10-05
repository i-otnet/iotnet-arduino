#pragma once

// IoTNet - Main abstraction layer for IoT connectivity
// This header combines all core libraries and provides a unified interface

#if defined(ESP32)
#include "core/esp32/IoTNetESP32.h"
#elif defined(ESP8266)
// Future support for ESP8266
#error "ESP8266 not yet supported"
#elif defined(ARDUINO_ARCH_AVR)
// Future support for Arduino AVR (Uno, Mega, Nano)
#error "Arduino AVR not yet supported"
#elif defined(ARDUINO_ARCH_SAMD)
// Future support for Arduino SAMD (MKR, Zero)
#error "Arduino SAMD not yet supported"
#elif defined(STM32F1xx) || defined(STM32F4xx) || defined(STM32F7xx)
// Future support for STM32
#error "STM32 not yet supported"
#else
#error "Unsupported platform"
#endif

// Main namespace that exposes all IoTNet functionality
namespace IoTNet {
// The IoTNetAgent is already defined in the platform-specific header
// (e.g., IoTNetESP32.h for ESP32)
// This file just includes the right implementation based on the platform
}

using namespace IoTNet;
