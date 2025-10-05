#include "esp32_utilities.h"

namespace IoTNet {

String Utilities::getTimestamp() {
    unsigned long ms = millis();
    unsigned long seconds = ms / 1000;
    unsigned long minutes = seconds / 60;
    unsigned long hours = minutes / 60;
    ms %= 1000;
    seconds %= 60;
    minutes %= 60;
    
    char buffer[16];
    sprintf(buffer, "[%02lu:%02lu:%02lu.%03lu]", hours, minutes, seconds, ms);
    return String(buffer);
}

} // namespace IoTNet
