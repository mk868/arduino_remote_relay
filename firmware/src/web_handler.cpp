#include <Arduino.h>
#include "web_handler.h"

#include "config.h"
#include "common.h"
#include "modules.h"
#include "store.h"

#define CONTENT_TYPE_TEXT "txt"
#define CONTENT_TYPE_HTML "htm"
#define CONTENT_TYPE_JSON "jsn"

void send_header_200(const char * content_type, Print &res) {
    char buffer[20];
    const char *file_prefix = "http/200.";
    strcpy(buffer, file_prefix);
    strcat(buffer, content_type);
    store_read_file(buffer, res);
}

static void send_error404(Print &res) {
    store_read_file("http/err404", res);
}

static void send_error414(Print &res) {
    store_read_file("http/err414", res);
}

static void send_body_module(Print &res, module_t * module) {
    res.print(F("{\"name\": \""));
    res.print(module->name);
    res.print(F("\",\"value\": "));
    res.print(module->value);
    res.print(F(",\"initialValue\": "));
    res.print(module->initial_value);
    res.print('}');
}

static void send_body_list(Print &res) {
    auto modules = modules_get_all();
    res.print('[');
    for (int i = 0; modules[i]; i++) {
        send_body_module(res, modules[i]);
        res.print(modules[i + 1] ? ',' : ']');
    }
}

static bool handle_modules(const server_request_t& req, Print &res) {
    const char *root_path = "/modules";
    const uint8_t root_path_len = strlen(root_path);

    if (!str_prefix(req.path, root_path)) {
        return false;
    }
    if (req.path[root_path_len] == '\0') {
        send_header_200(CONTENT_TYPE_JSON, res);
        send_body_list(res);
        return true;
    }

    const char path_separator = '/';
    const uint8_t path_separator_len = 1;

    if (req.path[root_path_len] != path_separator) {
        return false;
    }
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

        send_header_200(CONTENT_TYPE_JSON, res);
        send_body_module(res, module);
        return true;
    }
    return false;
}

static bool handle_static_www(const server_request_t& req, Print &res) {
    if (strstr(req.path + 1, "/") != nullptr) {
        return false;
    }

    const char *default_file = "/index.htm";

    const char *file = req.path;
    if (!strcmp(file, "/")) {
        file = default_file;
    }

    const char *www_dir = "web";
    char buffer[20];
    strcpy(buffer, www_dir);
    strncat(buffer, file, sizeof(buffer) - strlen(buffer) - 1);

    if (!store_file_exist(buffer)) {
        return false;
    }

    const char *ext = strrchr(buffer, '.');
    if (ext != nullptr) {
        // skip '.' char
        ext = ext + 1;
    } else {
        ext = CONTENT_TYPE_TEXT;
    }

    send_header_200(ext, res);
    store_read_file(buffer, res);
    return true;
}

void web_handler_handle(const server_request_t& request, Print &response) {
    if (strlen(request.path) > HTTP_MAX_PATH_LEN) {
        send_error414(response);
        return;
    }
    if (handle_modules(request, response)) {
        return;
    }
    if (handle_static_www(request, response)) {
        return;
    }

    send_error404(response);
}