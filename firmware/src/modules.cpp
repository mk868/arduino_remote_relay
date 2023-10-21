#include "modules.h"

#include <Arduino.h>

// TODO load from config / SD
module_t relay1 = {"relay1", false, false, false, 7};
module_t relay2 = {"relay2", false, false, false, 6};
module_t relay3 = {"relay3", false, false, false, 5};
module_t relay4 = {"relay4", false, false, false, 8};

static module_t* _modules[]={
        &relay1,
        &relay2,
        &relay3,
        &relay4,
        nullptr
};

void modules_init() {
    for (int i = 0; _modules[i]; i++) {
        auto module = _modules[i];
        modules_set_value(module, module->initial_value);
        pinMode(module->pin, OUTPUT);
    }
}

module_t **modules_get_all()
{
    return _modules;
}

void modules_set_value(module_t* module, bool value) {
    module->value = value;
    digitalWrite(module->pin, value ^ module->negated_value);
}

void modules_set_initial_value(module_t * module, bool initial_value) {
    module->initial_value = initial_value;
}
