#pragma once

#include <stdbool.h>

#include "unistd.h"
#include "port.h"
#include "pic.h"

typedef struct mouse_data {
    uint32_t x_pos;
    uint32_t y_pos;
    bool click;
} mouse_data_t;

void InitialiseMouse();