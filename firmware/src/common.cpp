
#include "common.h"
#include <string.h>

bool str_prefix(const char *str, const char *pre)
{
    return strncmp(pre, str, strlen(pre)) == 0;
}