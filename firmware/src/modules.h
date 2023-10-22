#include <stdint.h>

typedef struct
{
    const char* id;
    bool value; // current value
    bool initial_value;
    bool negated_value;
    uint8_t pin;
} module_t;

void modules_init();
module_t **modules_get_all();
void modules_set_initial_value(module_t * module, bool initial_value);
void modules_set_value(module_t * module, bool value);
