#include <LibRobus.h>

#include "etats.h"

void setup() {
    BoardInit();
    etats_init();
}

void loop() {
    etats_executer();
}
