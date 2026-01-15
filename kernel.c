void main() {
    // Write 'X' to the top-left of the screen
    // VGA memory starts at 0xB8000
    char* video_memory = (char*) 0xb8000;
    *video_memory = 'X';
    *(video_memory + 1) = 0x0f; // White text on black background

    while(1);
}