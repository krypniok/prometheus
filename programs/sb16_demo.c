#include "../drivers/ports.h"
#include "../drivers/display.h"
#include <stdint.h>
#include <stddef.h>

static void sb16_delay() {
    for (volatile int i = 0; i < 1000; i++) {
        /* simple delay */
    }
}

static int sb16_reset() {
    port_byte_out(0x226, 1);
    sb16_delay();
    port_byte_out(0x226, 0);
    for (int i = 0; i < 1000; i++) {
        if (port_byte_in(0x22E) == 0xAA) {
            return 1;
        }
        sb16_delay();
    }
    return 0;
}

void sb16_demo() {
    printf("SB16 reset...\n");
    if (!sb16_reset()) {
        printf("SB16 not found\n");
        return;
    }
    printf("SB16 ready\n");

    static unsigned char sample[] = {0x80, 0xFF, 0x80, 0x00, 0x80, 0xFF, 0x80, 0x00};

    port_byte_out(0x22C, 0x10);
    for (size_t i = 0; i < sizeof(sample); i++) {
        port_byte_out(0x22C, sample[i]);
        sb16_delay();
    }
}

