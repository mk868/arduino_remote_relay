#include <Arduino.h>
#include "server_modules.h"

#include "common.h"
#include "modules.h"
#include "store.h"

void send_header_200(Print &res) {
    store_read_file("/200", res);
}

static void send_error404(Print &res) {
    store_read_file("/404", res);
}

void send_body_module(Print &res, module_t * module) {
    res.print(F("{\"name\": \""));
    res.print(module->name);
    res.print(F("\",\"value\": "));
    res.print(module->value);
    res.print(F(",\"initialValue\": "));
    res.print(module->initial_value);
    res.print('}');
}

void send_body_list(Print &res) {
    auto modules = modules_get_all();
    res.print('[');
    for (int i = 0; modules[i]; i++) {
        send_body_module(res, modules[i]);
        res.print(modules[i + 1] ? ',' : ']');
    }
}

void server_modules_handler(const server_request_t& req, Print &res) {
    const char *root_path = "/modules";
    const uint8_t root_path_len = strlen(root_path);

    if (str_prefix(req.path, root_path)) {
        if (req.path[root_path_len] == '\0') {
            send_header_200(res);
            send_body_list(res);
            return;
        }

        const char path_separator = '/';
        const uint8_t path_separator_len = 1;

        if (req.path[root_path_len] == path_separator) {
            auto module_all = modules_get_all();
            for (int i = 0; module_all[i]; i++) {
                module_t *module = module_all[i];

                if (!str_prefix(req.path + root_path_len + path_separator_len, module->name)) {
                    continue;
                }
                auto module_name_len = strlen(module->name);
                if (req.path[root_path_len + path_separator_len + module_name_len] != 0 &&
                    req.path[root_path_len + path_separator_len + module_name_len] != '?') {
                    continue;
                }

                const char *value_query = "?value=";
                const uint8_t value_query_len = strlen(value_query);

                if (str_prefix(req.path + root_path_len + path_separator_len + module_name_len, value_query)) {
                    char value = req.path[root_path_len + path_separator_len + module_name_len + value_query_len];
                    Serial.print("Update value: ");
                    Serial.println(value);
                    modules_set_value(module, value == '1');
                }

                const char *initial_value_query = "?initialValue=";
                const uint8_t initial_value_query_len = strlen(initial_value_query);

                if (str_prefix(req.path + root_path_len + path_separator_len + module_name_len, initial_value_query)) {
                    char value = req.path[root_path_len + path_separator_len + module_name_len +
                                          initial_value_query_len];
                    Serial.print("Initial value: ");
                    Serial.println(value);
                    bool prev_value = module->initial_value;
                    bool new_value = value == '1';
                    if (prev_value != new_value) {
                        modules_set_initial_value(module, new_value);
                        store_set_bool(module->name, new_value);
                    }
                }

                send_header_200(res);
                send_body_module(res, module);
                return;
            }
        }
    }

    send_error404(res);
}