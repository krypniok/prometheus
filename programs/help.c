#include "../drivers/display.h"

void help() {
    printf("Available commands:\n");
    printf("sne            - ????\n");
    printf("ramdisk_test   - test ramdisk\n");
    printf("tinysql        - tiny SQL shell\n");
    printf("pip            - ????\n");
    printf("restart        - reboot system\n");
    printf("main_c         - main C test\n");
    printf("beep f l       - play beep with frequency and length\n");
    printf("dtmf           - DTMF demo\n");
    printf("editor_main    - text editor\n");
    printf("editor_main2   - secondary editor\n");
    printf("printlogo      - show logo\n");
    printf("ll_main        - linked list demo\n");
    printf("setpal         - set palette\n");
    printf("bga_demo       - graphics demo\n");
    printf("test_framebuffer- test framebuffer access\n");
    printf("txt            - return to text mode (after bga_demo)\n");
    printf("snake_main     - snake game\n");
    printf("sb16_demo      - SoundBlaster test\n");
    printf("help           - display this message\n");
    printf("random         - random numbers\n");
    printf("uptime         - show uptime\n");
    printf("hidecursor     - hide text cursor\n");
    printf("showcursor     - show text cursor\n");
    printf("print_registers- dump CPU registers\n");
    printf("keycodes       - keyboard scancodes\n");
    printf("exit           - stop kernel\n");
    printf("hddtest        - HDD test\n");
    printf("pf             - print file\n");
    printf("hexdump a l    - hexdump memory\n");
    printf("hexviewer a    - hex viewer\n");
    printf("memcpy d s n   - memory copy\n");
    printf("memset a v n   - memory set\n");
    printf("run prog       - run program\n");
    printf("printascii     - ASCII table\n");
    printf("searchb a s b  - search byte\n");
    printf("searchs a s str- search string\n");
}

