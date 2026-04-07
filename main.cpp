#include "pico/stdlib.h"
#include <cstdio> // Include the C standard IO functions

int main() {
    stdio_init_all(); // Initialize standard IO

    while (true) {
        printf("Hello, World!\n");
        sleep_ms(1000);
    }
}
