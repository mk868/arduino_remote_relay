#include "server.h"

#include <SPI.h>
#include <Ethernet.h>
#include "config.h"

static EthernetServer server(HTTP_SERVER_PORT);
static server_request_handler_t request_handler = nullptr;

bool server_init(const IPAddress& address) {
    EthernetClass::init(ETH_SS_PIN);
    EthernetClass::begin((uint8_t *) ETH_MAC, address);

    // Check for Ethernet hardware present
    if (EthernetClass::hardwareStatus() == EthernetNoHardware) {
        Serial.println(F("Ethernet shield was not found."));
        return false;
    }

    Serial.print(F("Server inited at "));
    Serial.println(address);
    return true;
}

void server_begin() {
    server.begin();
}

void server_loop() {
    // listen for incoming clients
    EthernetClient client = server.available();
    if (!client) {
        return;
    }
    char path_buffer[HTTP_MAX_PATH_LEN + 2];
    client.readBytesUntil(' ', path_buffer, sizeof(path_buffer) - 1); // read method
    auto size = client.readBytesUntil(' ', path_buffer, sizeof(path_buffer) - 1); // read path
    path_buffer[size] = 0;

    while (client.read() != -1) {
    }

    if (request_handler != nullptr) {
        server_request_t req{
                .path = path_buffer,
        };
        request_handler(req, client);
    }

    delay(1);
    client.flush();
    client.stop();
    Serial.println(F("client disconnected"));
}

void server_set_request_handler(server_request_handler_t cb) {
    request_handler = cb;
}
