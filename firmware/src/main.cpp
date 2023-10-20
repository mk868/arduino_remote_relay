#include <Arduino.h>
#include "server.h"
#include "server_modules.h"
#include "store.h"
#include "modules.h"
#include "config.h"

static void error_loop();

void setup()
{
  Serial.begin(115200);
  while (!Serial)
  {
  }

  Serial.println(F("Initializing SD..."));

  if (!store_init())
  {
    Serial.println(F("SD init failed!"));
    error_loop();
    return;
  }
  Serial.println(F("SD inited"));

  IPAddress address = store_get_ip();
  if (!server_init(address))
  {
    error_loop();
    return;
  }

  modules_init();

  server_set_request_handler(server_modules_handler);
  server_begin();
}

void loop()
{
  server_loop();
}

static void error_loop()
{
  pinMode(ERROR_LED_PIN, OUTPUT);
  while (1)
  {
    digitalWrite(ERROR_LED_PIN, 1);
    delay(1000);
    digitalWrite(ERROR_LED_PIN, 0);
    delay(1000);
  }
}
