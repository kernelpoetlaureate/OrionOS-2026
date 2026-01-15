#define PAGE_SIZE 4096
#define BITMAP_ADDRESS 0x100000 
#define MEM_MAP_ENTRIES *((int*)0x8000)
#define MEM_MAP_DATA    ((struct e820_entry*)0x8004)

// --- VGA PRINTING DRIVER ---
int cursor_pos = 0;
void putc(char c) {
    char* video_memory = (char*) 0xb8000;
    if (c == '\n') {
        cursor_pos = (cursor_pos / 160 + 1) * 160;
    } else {
        video_memory[cursor_pos++] = c;
        video_memory[cursor_pos++] = 0x0F;
    }
}

void print(char* str) {
    for (int i = 0; str[i] != '\0'; i++) putc(str[i]);
}

void print_hex(unsigned int n) {
    char* chars = "0123456789ABCDEF";
    print("0x");
    for (int i = 7; i >= 0; i--) putc(chars[(n >> (i * 4)) & 0xF]);
}

// --- PHYSICAL MEMORY MANAGER ---
struct e820_entry {
    unsigned long long base;
    unsigned long long length;
    unsigned int type;
    unsigned int acpi;
};

unsigned char* bitmap = (unsigned char*)BITMAP_ADDRESS;

void bitmap_set(int page_index) {
    bitmap[page_index / 8] |= (1 << (page_index % 8));
}

int bitmap_test(int page_index) {
    return bitmap[page_index / 8] & (1 << (page_index % 8));
}

void bitmap_free(int page_index) {
    bitmap[page_index / 8] &= ~(1 << (page_index % 8));
}

void pmm_init() {
    struct e820_entry* map = MEM_MAP_DATA;
    int entries = MEM_MAP_ENTRIES;

    // Mark 128MB as reserved by default
    for(int i = 0; i < 4096; i++) bitmap[i] = 0xFF;

    // Free usable sections
    for(int i = 0; i < entries; i++) {
        if(map[i].type == 1) {
            unsigned int start_page = (unsigned int)(map[i].base / PAGE_SIZE);
            unsigned int pages = (unsigned int)(map[i].length / PAGE_SIZE);
            for(unsigned int j = 0; j < pages; j++) bitmap_free(start_page + j);
        }
    }
    // Protect first 1MB
    for(int i = 0; i < 256; i++) bitmap_set(i);
}

void* pmm_alloc_page() {
    for(int i = 0; i < 32768; i++) {
        if (!bitmap_test(i)) {
            bitmap_set(i);
            return (void*)(i * PAGE_SIZE);
        }
    }
    return 0;
}

// --- MAIN ---
void main() {
    cursor_pos = 0; // Reset screen
    pmm_init();
    
    print("OrionOS Physical Memory Manager\n");
    print("-------------------------------\n");

    void* p1 = pmm_alloc_page();
    void* p2 = pmm_alloc_page();

    print("Page 1: "); print_hex((unsigned int)p1); print("\n");
    print("Page 2: "); print_hex((unsigned int)p2); print("\n");

    while(1);
}