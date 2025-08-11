#include "../drivers/ports.h"
#include "../drivers/display.h"
#include "../kernel/util.h"

#define BGA_INDEX_PORT 0x1CE
#define BGA_DATA_PORT  0x1CF

#define BGA_REG_XRES   0x01
#define BGA_REG_YRES   0x02
#define BGA_REG_BPP    0x03
#define BGA_REG_ENABLE 0x04

#define BGA_ENABLED        0x01
#define BGA_LFB_ENABLED    0x40

int bga_demo() {
    uint16_t width = 800;
    uint16_t height = 600;
    uint16_t bpp = 32;

    /* Disable display */
    port_word_out(BGA_INDEX_PORT, BGA_REG_ENABLE);
    port_word_out(BGA_DATA_PORT, 0);

    /* Set resolution */
    port_word_out(BGA_INDEX_PORT, BGA_REG_XRES);
    port_word_out(BGA_DATA_PORT, width);
    port_word_out(BGA_INDEX_PORT, BGA_REG_YRES);
    port_word_out(BGA_DATA_PORT, height);
    port_word_out(BGA_INDEX_PORT, BGA_REG_BPP);
    port_word_out(BGA_DATA_PORT, bpp);

    /* Enable display with LFB */
    port_word_out(BGA_INDEX_PORT, BGA_REG_ENABLE);
    port_word_out(BGA_DATA_PORT, BGA_ENABLED | BGA_LFB_ENABLED);

    uint32_t *lfb = (uint32_t *)0xE0000000; // Standard LFB address in QEMU
    for (uint32_t y = 0; y < height; y++) {
        for (uint32_t x = 0; x < width; x++) {
            uint8_t r = (x * 255) / width;
            uint8_t g = (y * 255) / height;
            uint8_t b = 0;
            lfb[y * width + x] = (r << 16) | (g << 8) | b;
        }
    }

    return 0;
}

