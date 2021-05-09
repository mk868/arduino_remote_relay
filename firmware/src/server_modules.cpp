#include "server_modules.h"

#include <Arduino.h>

#include "modules.h"

void send_header_200(Print &res)
{
    res.println(F("HTTP/1.1 200 OK"));
    res.println(F("Content-Type: application/json"));
    res.println(F("Connection: close"));
    res.println();
}

static void send_error404(Print &res)
{
    res.println(F("HTTP/1.1 404 Not Found"));
    res.println(F("Content-Type: application/json"));
    res.println(F("Connection: close"));
    res.println();
    res.print(F("{\"error\":\"404 Not Found\"}"));
}

void send_body_module(Print &res, uint8_t id)
{
    module_t *module = modules_get(id);
    if (module == NULL)
    {
        res.print("NULL");
        return;
    }

    res.print(F("{\"name\": \""));
    res.print(module->name);
    res.print(F("\",\"value\": "));
    res.print(module->value);
    res.print(F("}"));
}

void send_body_list(Print &res)
{
    res.print(F("["));
    int count = modules_count();
    for (int i = 0; i < count; i++)
    {
        send_body_module(res, i);
        res.print(i < (count - 1) ? "," : "");
    }
    res.print(F("]"));
}

void server_modules_handler(server_request_t req, Print &res)
{
    if (req.path.equals(F("/modules")))
    {
        send_header_200(res);
        send_body_list(res);
        return;
    }

    for (int i = 0; i < modules_count(); i++)
    {
        module_t *module = modules_get(i);
        String module_path = String(F("/modules/")) + module->name;

        if (!req.path.equals(module_path))
        {
            continue;
        }

        int query_value_pos = req.query.indexOf(F("value="));
        if (query_value_pos >= 0)
        {
            String value_str = req.query.substring(query_value_pos + 6);
            value_str.trim();
            modules_set_value(i, value_str.toInt());
            send_header_200(res);
            send_body_module(res, i);
            return;
        }

        send_header_200(res);
        send_body_module(res, i);
        return;
    }
    send_error404(res);
}