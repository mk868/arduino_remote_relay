#include "modules.h"

#include <Arduino.h>

// TODO load from config / SD
module_t relay1 = {"relay1", false, false, 7};
module_t relay2 = {"relay2", false, false, 6};
module_t relay3 = {"relay3", false, false, 5};
module_t relay4 = {"relay4", false, false, 8};

static module_t* _modules[]={
        &relay1,
        &relay2,
        &relay3,
        &relay4,
        nullptr
};

void modules_init() {
    for (int i = 0; _modules[i]; i++) {
        auto phy_module = _modules[i];
        digitalWrite(phy_module->pin, phy_module->negated_value);
        pinMode(phy_module->pin, OUTPUT);
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
