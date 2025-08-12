#include "../drivers/ports.h"
#include "../drivers/display.h"
#include "../kernel/math.h"
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

    const int sample_rate = 8000;
    const int samples = sample_rate; /* 1 second */
    static unsigned char wave[8000];
    for (int i = 0; i < samples; i++) {
        double t = (double)i / sample_rate;
        wave[i] = 128 + (unsigned char)(127 * sin(2 * PI * 440.0 * t));
    }

    port_byte_out(0x22C, 0x10);
    for (int i = 0; i < samples; i++) {
        port_byte_out(0x22C, wave[i]);
        sb16_delay();
    }
}

