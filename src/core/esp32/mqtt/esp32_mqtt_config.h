#pragma once

#include <cstddef>
#include <cstdint>
#include <functional>

#ifndef MQTT_SERVER
#define MQTT_SERVER "<your-mqtt-broker>"
#endif

#ifndef MQTT_PORT
#define MQTT_PORT 8884
#endif

#ifndef MQTT_USER
#define MQTT_USER "<your-mqtt-username>"
#endif

#ifndef MQTT_PASS
#define MQTT_PASS "<your-mqtt-password>"
#endif

#ifndef MQTT_MAX_PACKET_SIZE
#define MQTT_MAX_PACKET_SIZE 8192
#endif

namespace IoTNetConfig {
constexpr const char *kMqttServer = MQTT_SERVER;
constexpr uint16_t kMqttPort = static_cast<uint16_t>(MQTT_PORT);
constexpr const char *kMqttUser = MQTT_USER;
constexpr const char *kMqttPass = MQTT_PASS;

constexpr size_t kMqttMaxPacketSize = static_cast<size_t>(MQTT_MAX_PACKET_SIZE);
} // namespace IoTNetConfig