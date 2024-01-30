/* ---------------------------------------- INCLUDES ---------------------------------------- */

#include "../lib/data.h"
#include "string.h"

/* ---------------------------------------- STRUCTURE ---------------------------------------- */

typedef struct
{
    int code;
    char* args[10];
    int nbArgs;
} send_t;
