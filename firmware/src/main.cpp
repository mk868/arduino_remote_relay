#include <Arduino.h>
#include "server.h"
#include "web_handler.h"
#include "store.h"
#include "modules.h"
#include "config.h"

static void error_loop();

void setup() {
    Serial.begin(SERIAL_SPEED);
    while (!Serial) {
    }

    Serial.println(F("Initializing SD..."));

    if (!store_init()) {
        Serial.println(F("SD init failed!"));
        error_loop();
        return;
    }
    Serial.println(F("SD inited"));

    IPAddress address = store_get_ip();
    if (!server_init(address)) {
        error_loop();
        return;
    }

    auto modules = modules_get_all();
    for (int i = 0; modules[i]; i++) {
        auto module = modules[i];
        bool initial_value = store_get_bool(module->name);
        modules_set_initial_value(module, initial_value);
    }
    modules_init();

    server_set_request_handler(web_handler_handle);
    server_begin();
}

void loop() {
    server_loop();
}

static void error_loop() {
    pinMode(ERROR_LED_PIN, OUTPUT);
    while (true) {
        digitalWrite(ERROR_LED_PIN, 1);
        delay(1000);
        digitalWrite(ERROR_LED_PIN, 0);
        delay(1000);
    }
}
