#include "../drivers/ports.h"
#include "../drivers/display.h"
#include "../drivers/video.h"
#include "../kernel/util.h"
#include "../kernel/mem.h"
#include "../drivers/font8x16.h"
#include "../drivers/keyboard.h"

void showcursor();
int txt();

#define BGA_INDEX_PORT 0x1CE
#define BGA_DATA_PORT  0x1CF

#define BGA_REG_XRES   0x01
#define BGA_REG_YRES   0x02
#define BGA_REG_BPP    0x03
#define BGA_REG_ENABLE 0x04

#define BGA_ENABLED        0x01
#define BGA_LFB_ENABLED    0x40

/*
 * VGA register setup taken from the OSDev wiki to switch the card back
 * to the classic 80x25 text mode.  This is required after the BGA has
 * been enabled because simply disabling the BGA controller does not fully
 * restore the VGA state.  Without reprogramming the registers the screen
 * would stay in graphics mode and our text routines writing to 0xB8000
 * would remain invisible.
 */
static unsigned char text_mode_80x25[] = {
    /* MISC */
    0x67,
    /* SEQ */
    0x03, 0x00, 0x03, 0x00, 0x02,
    /* CRTC */
    0x5F, 0x4F, 0x50, 0x82, 0x55, 0x81, 0xBF, 0x1F,
    0x00, 0x4F, 0x0D, 0x0E, 0x00, 0x00, 0x00, 0x50,
    0x9C, 0x0E, 0x8F, 0x28, 0x40, 0x96, 0xB9, 0xA3,
    0xFF,
    /* GC */
    0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x0E, 0x00, 0xFF,
    /* AC */
    0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
    0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F,
    0x41, 0x00, 0x0F, 0x00, 0x00
};

static void write_registers(unsigned char *regs) {
    unsigned int i;

    /* MISC */
    port_byte_out(0x3C2, *regs++);

    /* SEQ */
    for (i = 0; i < 5; i++) {
        port_byte_out(0x3C4, i);
        port_byte_out(0x3C5, *regs++);
    }

    /* Unlock CRTC registers */
    port_byte_out(0x3D4, 0x03);
    port_byte_out(0x3D5, port_byte_in(0x3D5) | 0x80);
    port_byte_out(0x3D4, 0x11);
    port_byte_out(0x3D5, port_byte_in(0x3D5) & ~0x80);

    /* CRTC */
    for (i = 0; i < 25; i++) {
        port_byte_out(0x3D4, i);
        port_byte_out(0x3D5, *regs++);
    }

    /* GC */
    for (i = 0; i < 9; i++) {
        port_byte_out(0x3CE, i);
        port_byte_out(0x3CF, *regs++);
    }

    /* AC */
    for (i = 0; i < 21; i++) {
        /* Reset flip-flop before each attribute controller access */
        port_byte_in(0x3DA);
        port_byte_out(0x3C0, i);
        port_byte_in(0x3DA);
        port_byte_out(0x3C0, *regs++);
    }

    /* Lock palette and unblank */
    port_byte_in(0x3DA);
    port_byte_out(0x3C0, 0x20);
}

int bga_demo() {
    init_video();

    uint16_t width = 800;
    uint16_t height = 600;
    uint16_t bpp = 32;

    /* Disable display */
    port_word_out(BGA_INDEX_PORT, BGA_REG_ENABLE);
    port_word_out(BGA_DATA_PORT, 0);

    /* Set linear frame buffer address to 0xE0000000 */
    port_word_out(BGA_INDEX_PORT, 0x09);
    port_word_out(BGA_DATA_PORT, 0x0000);
    port_word_out(BGA_INDEX_PORT, 0x0A);
    port_word_out(BGA_DATA_PORT, 0xE000);

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

    volatile uint32_t *lfb = framebuffer;
    for (uint32_t y = 0; y < height; y++) {
        for (uint32_t x = 0; x < width; x++) {
            uint8_t r = (x * 255) / width;
            uint8_t g = (y * 255) / height;
            uint8_t b = 0;
            lfb[y * width + x] = (r << 16) | (g << 8) | b;
        }
    }
    while (getkey() != SC_ESC) {
        // wait for ESC key
    }
    txt();
    return 0;
}

int txt() {
    /* Disable BGA controller */
    port_word_out(BGA_INDEX_PORT, BGA_REG_ENABLE);
    port_word_out(BGA_DATA_PORT, 0);

    /* Reprogram VGA registers back to text mode */
    write_registers(text_mode_80x25);

    load_vga_font(font8x16, 16);

    /* Reload default palette and console state */
    setpal();

    /* Clear the screen and reposition the cursor */
    clear_screen();
    set_cursor(get_offset(0, 0));
    showcursor();

    printf("Text mode restored\n");
    return 0;
}

