#include "server.h"

#include <SPI.h>
#include <Ethernet.h>
#include "config.h"

static EthernetServer server(SERVER_PORT);
static server_request_handler_t request_handler = NULL;

bool server_init(IPAddress address)
{
    Ethernet.init(10);
    Ethernet.begin(ETH_MAC, address);

    // Check for Ethernet hardware present
    if (Ethernet.hardwareStatus() == EthernetNoHardware)
    {
        Serial.println(F("Ethernet shield was not found."));
        return false;
    }

    Serial.print(F("Server inited at "));
    Serial.println(address);
    return true;
}

void server_begin()
{
    server.begin();
}

void server_loop()
{
    // listen for incoming clients
    EthernetClient client = server.available();
    if (client)
    {
        client.readStringUntil(' ');
        String path = client.readStringUntil(' ');
        // get query from path
        String query;
        if (path.indexOf('?') > 0)
        {
            query = path.substring(path.indexOf('?') + 1);
            path = path.substring(0, path.indexOf('?'));
        }
        // remove double '/'
        while (path.indexOf("//") >= 0)
        {
            path.replace("//", "/");
        }
        // remove '/' at the end of path
        if (path.endsWith("/"))
        {
            path = path.substring(0, path.length() - 1);
        }
        Serial.println(path);
        Serial.println(query);

        int ch;
        while ((ch = client.read()) != -1)
        {
        }

        if (request_handler != NULL)
        {
            server_request_t req;
            req.path = path;
            req.query = query;
            request_handler(req, client);
        }

        delay(1);
        client.flush();
        client.stop();
        Serial.println(F("client disconnected"));
    }
}

void server_set_request_handler(server_request_handler_t cb)
{
    request_handler = cb;
}
