#include <stdio.h>
#include "supportFiles/display.h"
#include "clockDisplay.h"
#include "clockControl.h"
#include "supportFiles/utils.h"

// Color definitions
#define BLACK 0x0000
#define BLUE 0x001F
#define RED 0xF800
#define GREEN 0x07E0
#define CYAN 0x07FF
#define MAGENTA 0xF81F
#define YELLOW 0xFFE0
#define WHITE 0xFFFF

int main() {
    clockDisplay_init();
    clockControl_init();
    while (1) {
        clockControl_tick();
        utils_msDelay(1);
    }
    printf("Test 1\n");
}

// This function must be defined but can be left empty for now.
// You will use this function in a later lab.
// It is called in the timer interrupt service routine (see interrupts.c in supportFiles).
void isr_function() {
    // Empty for now.
}
