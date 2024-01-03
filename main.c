#include "cli.h"
#include <stdio.h>

int main() {
    CLI_init();
    appendCmd("func1", nullCmd, (void*) "func1");
    return CLI_loop();
}
