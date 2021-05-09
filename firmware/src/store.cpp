#include "store.h"

#include <SD.h>
#include "config.h"

static const char *file_ip = "/IP";

bool store_init()
{
    return SD.begin(SD_CS_PIN);
}

IPAddress store_get_ip()
{
    File file = SD.open(file_ip);
    String content = file.readString();
    content.trim();
    IPAddress address;
    if (!address.fromString(content))
    {
        Serial.println("Parse IP error");
    }
    file.close();
    return address;
}
