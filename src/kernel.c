#include <stdint.h>
#include <stddef.h>

#define VGA_WIDTH 80
#define VGA_HEIGHT 25
#define VGA_ADDRESS 0xB8000

enum vga_color {
    COLOR_BLACK = 0, COLOR_BLUE = 1, COLOR_GREEN = 2, COLOR_CYAN = 3,
    COLOR_RED = 4, COLOR_MAGENTA = 5, COLOR_BROWN = 6, COLOR_LIGHT_GREY = 7,
    COLOR_DARK_GREY = 8, COLOR_LIGHT_BLUE = 9, COLOR_LIGHT_GREEN = 10,
    COLOR_LIGHT_CYAN = 11, COLOR_LIGHT_RED = 12, COLOR_LIGHT_MAGENTA = 13,
    COLOR_LIGHT_BROWN = 14, COLOR_WHITE = 15,
};

static uint16_t* const VGA_BUFFER = (uint16_t*) VGA_ADDRESS;
static int mouse_x = 40, mouse_y = 12;
static char cmd_buf[64];
static int cmd_idx = 0;
static char notepad_buf[256];
static int notepad_idx = 0;

static inline uint8_t inb(uint16_t port) {
    uint8_t ret;
    asm volatile ("inb %1, %0" : "=a"(ret) : "Nd"(port));
    return ret;
}

static inline void outb(uint16_t port, uint8_t val) {
    asm volatile ("outb %0, %1" : : "a"(val), "Nd"(port));
}

static inline uint8_t vga_color(enum vga_color fg, enum vga_color bg) {
    return fg | (bg << 4);
}

static inline uint16_t vga_entry(unsigned char uc, uint8_t color) {
    return (uint16_t) uc | ((uint16_t) color << 8);
}

void put_char(char c, uint8_t color, size_t x, size_t y) {
    if (x < VGA_WIDTH && y < VGA_HEIGHT) {
        VGA_BUFFER[y * VGA_WIDTH + x] = vga_entry(c, color);
    }
}

void draw_rect(char fill_char, uint8_t color, size_t x, size_t y, size_t width, size_t height) {
    for (size_t r = y; r < y + height; r++) {
        for (size_t c = x; c < x + width; c++) {
            put_char(fill_char, color, c, r);
        }
    }
}

void write_string(const char* str, uint8_t color, size_t x, size_t y) {
    for (size_t i = 0; str[i] != '\0'; i++) {
        put_char(str[i], color, x + i, y);
    }
}

void draw_window(const char* title, size_t x, size_t y, size_t width, size_t height) {
    uint8_t win_bg = vga_color(COLOR_BLACK, COLOR_LIGHT_GREY);
    uint8_t title_bg = vga_color(COLOR_WHITE, COLOR_BLUE);

    draw_rect(' ', win_bg, x, y, width, height);
    draw_rect(' ', title_bg, x, y, width, 1);
    write_string(title, title_bg, x + 2, y);
    write_string("[X]", vga_color(COLOR_WHITE, COLOR_RED), x + width - 4, y);
}

void sys_reboot() {
    outb(0x64, 0xFE);
}

void sys_shutdown() {
    outb(0x604, 0x0 | 0x2000); // QEMU/Virt ACPI shutdown
    outb(0x4004, 0x3400);      // VirtualBox/VMware ACPI shutdown
}

void execute_cmd(const char* cmd) {
    if (cmd[0] == 'h' && cmd[1] == 'e') {
        write_string("Commands: help, ls, clear, notepad, browser, reboot, shutdown", vga_color(COLOR_GREEN, COLOR_BLACK), 2, 22);
    } else if (cmd[0] == 'l' && cmd[1] == 's') {
        write_string("Files: /bin/bash /bin/vga /etc/vayu.conf /apps/browser", vga_color(COLOR_LIGHT_CYAN, COLOR_BLACK), 2, 22);
    } else if (cmd[0] == 'r' && cmd[1] == 'e') {
        sys_reboot();
    } else if (cmd[0] == 's' && cmd[1] == 'h') {
        sys_shutdown();
    } else if (cmd[0] == 'n' && cmd[1] == 'o') {
        draw_window(" Vayu Notepad (Type text directly) ", 42, 3, 35, 8);
    } else if (cmd[0] == 'b' && cmd[1] == 'r') {
        draw_window(" Vayu Web Browser ", 42, 12, 35, 8);
        write_string("URL: http://vayu.os.local", vga_color(COLOR_BLACK, COLOR_WHITE), 44, 14);
        write_string("Status: 200 OK (Bare-Metal)", vga_color(COLOR_GREEN, COLOR_LIGHT_GREY), 44, 16);
    } else {
        write_string("Unknown command! Type 'help'", vga_color(COLOR_LIGHT_RED, COLOR_BLACK), 2, 22);
    }
}

void process_keyboard(uint8_t scancode) {
    if (scancode == 0x1C) { // Enter key
        cmd_buf[cmd_idx] = '\0';
        execute_cmd(cmd_buf);
        cmd_idx = 0;
        draw_rect(' ', vga_color(COLOR_WHITE, COLOR_BLACK), 12, 20, 25, 1);
    } else if (scancode == 0x0E) { // Backspace
        if (cmd_idx > 0) {
            cmd_idx--;
            put_char(' ', vga_color(COLOR_WHITE, COLOR_BLACK), 12 + cmd_idx, 20);
        }
    } else if (scancode >= 0x10 && scancode <= 0x32 && cmd_idx < 24) {
        char keymap[] = "qwertyuiop  asdfghjkl   zxcvbnm";
        char c = keymap[scancode - 0x10];
        if (c != ' ') {
            cmd_buf[cmd_idx++] = c;
            put_char(c, vga_color(COLOR_WHITE, COLOR_BLACK), 12 + cmd_idx - 1, 20);
        }
    }
}

void update_mouse(int dx, int dy) {
    // Erase old cursor
    put_char(' ', vga_color(COLOR_WHITE, COLOR_CYAN), mouse_x, mouse_y);

    mouse_x += dx;
    mouse_y += dy;

    if (mouse_x < 0) mouse_x = 0;
    if (mouse_x >= VGA_WIDTH) mouse_x = VGA_WIDTH - 1;
    if (mouse_y < 0) mouse_y = 0;
    if (mouse_y >= VGA_HEIGHT) mouse_y = VGA_HEIGHT - 1;

    // Draw active mouse cursor
    put_char('X', vga_color(COLOR_WHITE, COLOR_RED), mouse_x, mouse_y);
}

void kernel_main(void) {
    // Desktop Background
    draw_rect(' ', vga_color(COLOR_WHITE, COLOR_CYAN), 0, 0, VGA_WIDTH, VGA_HEIGHT);

    // Taskbar & Start Menu
    draw_rect(' ', vga_color(COLOR_WHITE, COLOR_BLUE), 0, 0, VGA_WIDTH, 1);
    write_string("[Vayu Menu]", vga_color(COLOR_BLACK, COLOR_LIGHT_GREY), 1, 0);
    write_string("VayuOS Bare-Metal C Kernel | Mouse & CLI Active", vga_color(COLOR_WHITE, COLOR_BLUE), 15, 0);

    // Built-in Shell Window
    draw_window(" Vayu Shell (vayu_sh) ", 2, 3, 38, 17);
    write_string("VayuOS Bare-Metal Shell v1.0", vga_color(COLOR_BLACK, COLOR_LIGHT_GREY), 4, 5);
    write_string("Type 'help' for command list.", vga_color(COLOR_DARK_GREY, COLOR_LIGHT_GREY), 4, 6);

    draw_rect(' ', vga_color(COLOR_WHITE, COLOR_BLACK), 3, 19, 36, 4);
    write_string("vayu_sh> ", vga_color(COLOR_GREEN, COLOR_BLACK), 4, 20);

    // Initial Default Windows
    draw_window(" Vayu Notepad ", 42, 3, 35, 8);
    write_string("Built-in scratchpad loaded.", vga_color(COLOR_BLACK, COLOR_LIGHT_GREY), 44, 5);

    draw_window(" Vayu Browser ", 42, 12, 35, 8);
    write_string("URL: http://localhost", vga_color(COLOR_BLACK, COLOR_WHITE), 44, 14);

    // Hardware Event Loop (Keyboard & PS/2 Mouse Controller)
    while (1) {
        if (inb(0x64) & 1) {
            uint8_t scancode = inb(0x60);
            process_keyboard(scancode);

            // Arrow keys move mouse pointer
            if (scancode == 0x4B) update_mouse(-1, 0);
            if (scancode == 0x4D) update_mouse(1, 0);
            if (scancode == 0x48) update_mouse(0, -1);
            if (scancode == 0x50) update_mouse(0, 1);
        }
        for (volatile int i = 0; i < 50000; i++);
    }
}
