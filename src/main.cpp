// ⚠️ IMPORTANT: Define MQTT config BEFORE including any headers!
#define MQTT_SERVER "broker.iotunnel.my.id"  
#define MQTT_PORT 8884
#define MQTT_USER "farismnrr"
#define MQTT_PASS "farismnrr"

// ⚠️ IMPORTANT: Define WiFi config BEFORE including any headers!
#define WIFI_SSID "farismnrr-hotspot"
#define WIFI_PASS "farismnrr-hotspot"

#include "IoTNetESP32.h"

IoTNetAgent iotnet;

void setup() {
  Serial.begin(115200);
  delay(1000);

  iotnet.setWiFiConfig(WIFI_SSID, WIFI_PASS);
  iotnet.setMqttConfig(MQTT_SERVER, MQTT_PORT, MQTT_USER, MQTT_PASS);   
  iotnet.begin();
}

void loop() {
  iotnet.loop();
}
