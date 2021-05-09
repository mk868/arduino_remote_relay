#include <stdint.h>
#include <WString.h>

typedef struct
{
    String name;
    uint8_t value;
    // hardware specific fields, should be more abstract
    bool negated_value;
    uint8_t pin;
} module_t;

void modules_init();
int modules_count();
module_t *modules_get(int id);
void modules_set_value(int id, uint8_t value);
