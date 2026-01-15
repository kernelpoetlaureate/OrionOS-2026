// Screen constants
#define VGA_WIDTH 80
#define VGA_COLOR_WHITE 0x0F

int cursor_pos = 0;

// Print a single character to the screen
void putc(char c) {
    char* video_memory = (char*) 0xb8000;
    
    if (c == '\n') {
        // Simple newline: move to start of next line
        cursor_pos = (cursor_pos / (VGA_WIDTH * 2) + 1) * (VGA_WIDTH * 2);
    } else {
        video_memory[cursor_pos++] = c;
        video_memory[cursor_pos++] = VGA_COLOR_WHITE;
    }
}

// Print a null-terminated string
void print(char* str) {
    for (int i = 0; str[i] != '\0'; i++) {
        putc(str[i]);
    }
}

// Convert an integer to a Hexadecimal string and print it
// Very useful for memory addresses!
void print_hex(unsigned int n) {
    char* chars = "0123456789ABCDEF";
    print("0x");
    for (int i = 7; i >= 0; i--) {
        putc(chars[(n >> (i * 4)) & 0xF]);
    }
}

// Convert an integer to decimal and print it
void print_int(int n) {
    if (n == 0) {
        putc('0');
        return;
    }
    char buf[11];
    int i = 10;
    buf[i--] = '\0';
    while (n > 0) {
        buf[i--] = (n % 10) + '0';
        n /= 10;
    }
    print(&buf[i+1]);
}




















void main() {
    // Reset cursor
    cursor_pos = 0;

    // Clear screen (optional but recommended)
    char* vga = (char*) 0xb8000;
    for(int i = 0; i < 80 * 25 * 2; i++) vga[i] = 0;

    print("OrionOS Memory Manager Initialized\n");
    print("----------------------------------\n");

    struct memory_block {
        int start_address;
        int size;
        int is_free;
    };

    #define MAX_BLOCKS 5
    struct memory_block memory_map[MAX_BLOCKS];

    // Initialize first block
    memory_map[0].start_address = 0x100000; 
    memory_map[0].size = 64 * 1024;
    memory_map[0].is_free = 1;

    // For demo: create a second "used" block
    memory_map[1].start_address = 0x110000;
    memory_map[1].size = 4096;
    memory_map[1].is_free = 0;

    // Loop and print the status of blocks
    for (int i = 0; i < 2; i++) {
        print("Block ");
        print_int(i);
        print(": Addr: ");
        print_hex(memory_map[i].start_address);
        print(" | Size: ");
        print_int(memory_map[i].size);
        print(" bytes | Status: ");
        
        if (memory_map[i].is_free) {
            print("FREE\n");
        } else {
            print("USED\n");
        }
    }

    while(1);
}