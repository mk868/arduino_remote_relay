#include "modules.h"

#include <Arduino.h>

// TODO load from config / SD
static module_t _modules[] = {
    {"relay1", 0, false, 7},
    {"relay2", 0, false, 6},
    {"relay3", 0, false, 5},
    {"relay4", 0, false, 8}};

void modules_init()
{
    for (int i = 0; i < modules_count(); i++)
    {
        module_t module = _modules[i];
        digitalWrite(module.pin, module.negated_value);
        pinMode(module.pin, OUTPUT);
    }
}

int modules_count()
{
    return sizeof(_modules) / sizeof(module_t);
}

module_t *modules_get(int id)
{
    if (id < 0 || id >= modules_count())
    {
        return nullptr;
    }
    return &_modules[id];
}

void modules_set_value(int id, uint8_t value)
{
    value = !(!value);
    module_t *module = modules_get(id);
    if (module == nullptr)
    {
        return;
    }
    module->value = value;
    digitalWrite(module->pin, value ^ module->negated_value);
}
