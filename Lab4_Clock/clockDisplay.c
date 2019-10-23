#include <stdbool.h>
#include <stdio.h>
#include "supportFiles/display.h"
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

#define CLOCK_TEXT_SIZE 5
#define NUMBER_OF_CHARS 8

#define CLOCK_CENTER_HEIGHT 120
#define CLOCK_CENTER_WIDTH 160

#define CHAR_WIDTH (6 * CLOCK_TEXT_SIZE)
#define CHAR_HEIGHT (8 * CLOCK_TEXT_SIZE)

#define TEXT_CURSOR_X (CLOCK_CENTER_WIDTH - (4 * CHAR_WIDTH))
#define TEXT_CURSOR_Y (CLOCK_CENTER_HEIGHT - (CHAR_HEIGHT / 2))

#define CLEAR_CHAR_X (TEXT_CURSOR_X + i * CHAR_WIDTH)
#define CLEAR_CHAR_Y (TEXT_CURSOR_Y)

#define TOP_ROW_BOTTOW_LINE (CLOCK_CENTER_HEIGHT - CHAR_HEIGHT)
#define TOP_ROW_TOP_POINT (TOP_ROW_BOTTOW_LINE - (CHAR_WIDTH * 1.5))
#define BOTTOM_ROW_TOP_LINE (CLOCK_CENTER_HEIGHT + CHAR_HEIGHT)
#define BOTTOM_ROW_BOTTOM_POINT (BOTTOM_ROW_TOP_LINE + (CHAR_WIDTH * 1.5))
#define ARROW_ONE_LEFT_X (CLOCK_CENTER_WIDTH - (4 * CHAR_WIDTH))
#define ARROW_ONE_RIGHT_X (ARROW_ONE_LEFT_X + (CHAR_WIDTH * 2))
#define ARROW_ONE_POINT_X ((ARROW_ONE_LEFT_X + ARROW_ONE_RIGHT_X) / 2)
#define ARROW_TWO_LEFT_X (CLOCK_CENTER_WIDTH - CHAR_WIDTH)
#define ARROW_TWO_RIGHT_X (ARROW_TWO_LEFT_X + (CHAR_WIDTH * 2))
#define ARROW_TWO_POINT_X ((ARROW_TWO_LEFT_X + ARROW_TWO_RIGHT_X) / 2)
#define ARROW_THREE_LEFT_X (CLOCK_CENTER_WIDTH + (CHAR_WIDTH * 2))
#define ARROW_THREE_RIGHT_X (ARROW_THREE_LEFT_X + (CHAR_WIDTH * 2))
#define ARROW_THREE_POINT_X ((ARROW_THREE_LEFT_X + ARROW_THREE_RIGHT_X) / 2)

#define ADD_SECOND if(second>=59) {second = 0;} else second++
#define ADD_MINUTE if(minute>=59) {minute = 0;} else minute++
#define ADD_HOUR   if(hour>11)   {hour = 1;}   else hour++
#define REMOVE_SECOND if(second<=0) {second = 59;} else second--
#define REMOVE_MINUTE if(minute<=0) {minute = 59;} else minute--
#define REMOVE_HOUR   if(hour<=1)   {hour = 12;}   else hour--

#define DETERMINE_INC if(touchYCoordinate <= CLOCK_CENTER_HEIGHT) {isInc = true;} else {isInc = false;}
#define DETERMINE_THIRD if(touchXCoordinate <= 110) {thirdOfScreen = 1;} else if(touchXCoordinate <= 210) {thirdOfScreen = 2;} else {thirdOfScreen = 3;}

char currentTime[20] = "01:00:00";
char previousTime[20] = "01:00:00";
int hour = 1; int minute = 0; int second = 0;
bool isInc; int thirdOfScreen;

int16_t touchXCoordinate, touchYCoordinate; uint8_t touchPressureCoordinate;

// Called only once - performs any necessary inits.
// This is a good place to draw the triangles and any other
// parts of the clock display that will never change.
void clockDisplay_init() {
    display_init();
    display_fillScreen(BLACK);
    display_setTextSize(CLOCK_TEXT_SIZE);
    display_fillTriangle(ARROW_ONE_LEFT_X,TOP_ROW_BOTTOW_LINE,ARROW_ONE_RIGHT_X,TOP_ROW_BOTTOW_LINE,ARROW_ONE_POINT_X,TOP_ROW_TOP_POINT,GREEN);
    display_fillTriangle(ARROW_TWO_LEFT_X,TOP_ROW_BOTTOW_LINE,ARROW_TWO_RIGHT_X,TOP_ROW_BOTTOW_LINE,ARROW_TWO_POINT_X,TOP_ROW_TOP_POINT,GREEN);
    display_fillTriangle(ARROW_THREE_LEFT_X,TOP_ROW_BOTTOW_LINE,ARROW_THREE_RIGHT_X,TOP_ROW_BOTTOW_LINE,ARROW_THREE_POINT_X,TOP_ROW_TOP_POINT,GREEN);

    display_fillTriangle(ARROW_ONE_LEFT_X,BOTTOM_ROW_TOP_LINE,ARROW_ONE_RIGHT_X,BOTTOM_ROW_TOP_LINE,ARROW_ONE_POINT_X,BOTTOM_ROW_BOTTOM_POINT,GREEN);
    display_fillTriangle(ARROW_TWO_LEFT_X,BOTTOM_ROW_TOP_LINE,ARROW_TWO_RIGHT_X,BOTTOM_ROW_TOP_LINE,ARROW_TWO_POINT_X,BOTTOM_ROW_BOTTOM_POINT,GREEN);
    display_fillTriangle(ARROW_THREE_LEFT_X,BOTTOM_ROW_TOP_LINE,ARROW_THREE_RIGHT_X,BOTTOM_ROW_TOP_LINE,ARROW_THREE_POINT_X,BOTTOM_ROW_BOTTOM_POINT,GREEN);

    display_setCursor(TEXT_CURSOR_X, TEXT_CURSOR_Y);
    display_setTextColor(GREEN);
    sprintf(currentTime, "%2hd:%02d:%02d", hour, minute, second);
    display_println(currentTime);
    return;
}

// Updates the time display with latest time, making sure to update only those digits that
// have changed since the last update.
// if forceUpdateAll is true, update all digits.
void clockDisplay_updateTimeDisplay(bool forceUpdateAll) {
    if (forceUpdateAll) {
        display_fillRect(TEXT_CURSOR_X, TEXT_CURSOR_Y, CHAR_WIDTH * NUMBER_OF_CHARS, CHAR_HEIGHT * NUMBER_OF_CHARS, BLACK);
        display_setCursor(TEXT_CURSOR_X, TEXT_CURSOR_Y);
        display_println(currentTime);
        return;
    }
    for (int i = 0; i < 8; i++) {
        if (previousTime[i] != currentTime[i]) {
            display_fillRect(CLEAR_CHAR_X, CLEAR_CHAR_Y, CHAR_WIDTH, CHAR_HEIGHT, BLACK);
            display_setCursor(TEXT_CURSOR_X, TEXT_CURSOR_Y);
        }
    }
    display_println(currentTime);
    return;
}

void updateTimeArray() {
    for (int i = 0; i < 10; i++) {
        previousTime[i] = currentTime[i];
    }
    sprintf(currentTime, "%2hd:%02d:%02d", hour, minute, second);
    return;
}

// Reads the touched coordinates and performs the increment or decrement,
// depending upon the touched region.
void clockDisplay_performIncDec() {
    display_getTouchedPoint(&touchXCoordinate, &touchYCoordinate, &touchPressureCoordinate);

    DETERMINE_INC;
    DETERMINE_THIRD;
    if (isInc) {
        switch(thirdOfScreen) {
        case 1: ADD_HOUR;
            break;
        case 2: ADD_MINUTE;
            break;
        case 3: ADD_SECOND;
            break;
        }
    }
    else {
        switch(thirdOfScreen) {
        case 1: REMOVE_HOUR;
            break;
        case 2: REMOVE_MINUTE;
            break;
        case 3: REMOVE_SECOND;
            break;
        }
    }

    updateTimeArray();
    clockDisplay_updateTimeDisplay(0);
    return;
}

// Advances the time forward by 1 second and update the display.
void clockDisplay_advanceTimeOneSecond() {
    if (second>=59) {
        ADD_SECOND;
        if (minute>=59) {
            ADD_HOUR;
        }
        ADD_MINUTE;
    }
    else
        ADD_SECOND;
    updateTimeArray();
    clockDisplay_updateTimeDisplay(0);
    return;
}

// Run a test of clock-display functions.
void clockDisplay_runTest() {
    for (int i = 0; i < 60; i++) {
        ADD_SECOND;
        utils_msDelay(100);
        updateTimeArray();
        clockDisplay_updateTimeDisplay(0);
    }
    for (int i = 0; i < 59; i++) {
        ADD_MINUTE;
        utils_msDelay(100);
        updateTimeArray();
        clockDisplay_updateTimeDisplay(0);
    }
    for (int i = 0; i < 12; i++) {
        ADD_HOUR;
        utils_msDelay(100);
        updateTimeArray();
        clockDisplay_updateTimeDisplay(0);
    }
    for (int i = 0; i < 60; i++) {
        REMOVE_SECOND;
        utils_msDelay(100);
        updateTimeArray();
        clockDisplay_updateTimeDisplay(0);
    }
    for (int i = 0; i < 59; i++) {
        REMOVE_MINUTE;
        utils_msDelay(100);
        updateTimeArray();
        clockDisplay_updateTimeDisplay(0);
    }
    for (int i = 0; i < 12; i++) {
        REMOVE_HOUR;
        utils_msDelay(100);
        updateTimeArray();
        clockDisplay_updateTimeDisplay(0);
    }
    return;
}
