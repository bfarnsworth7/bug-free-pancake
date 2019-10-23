#include <stdio.h>
#include "supportFiles/display.h"
#include "clockDisplay.h"

enum states {
    WAITING_FOR_TOUCH,
    TOUCHED,
    ADC_TIMER_COMPLETE,
    TIMER_START_HELD,
    TIMER_END_HELD,
} currentState;

int32_t adcCounter, autoCounter, rateCounter, secondCounter;
int hasBeenTouched;
#define TIMER_PERIOD 50.0E-3
#define ADC_COUNTER_MAX_VALUE (50 / (TIMER_PERIOD * 1000))
#define AUTO_COUNTER_MAX_VALUE (450 / (TIMER_PERIOD * 1000))
#define RATE_COUNTER_MAX_VALUE (50 / (TIMER_PERIOD * 1000))
#define SECOND_COUNTER_MAX_VALUE (850 / (TIMER_PERIOD * 1000))

// Standard tick function.
void clockControl_tick() {
    switch(currentState) {
    case WAITING_FOR_TOUCH:
        if (display_isTouched()) {
            currentState = TOUCHED;
            hasBeenTouched = 1;
            display_clearOldTouchData();
        }
        break;
    case TOUCHED:
        if (adcCounter != ADC_COUNTER_MAX_VALUE) {
            break;
        }
        if (display_isTouched()) {
            currentState = ADC_TIMER_COMPLETE;
            break;
        }
        currentState = WAITING_FOR_TOUCH;
        secondCounter = 0;
        break;
    case ADC_TIMER_COMPLETE:
        if(!display_isTouched()) {
            currentState = WAITING_FOR_TOUCH;
            secondCounter = 0;
            clockDisplay_performIncDec();
            break;
        }
        if(autoCounter == AUTO_COUNTER_MAX_VALUE) {
            currentState = TIMER_START_HELD;
            clockDisplay_performIncDec();
            break;
        }
        break;
    case TIMER_START_HELD:
        if(!display_isTouched()) {
            currentState = WAITING_FOR_TOUCH;
            secondCounter = 0;
            break;
        }
        if(rateCounter == RATE_COUNTER_MAX_VALUE) {
            currentState = TIMER_END_HELD;
            break;
        }
        break;
    case TIMER_END_HELD:
        if(!display_isTouched()) {
            currentState = WAITING_FOR_TOUCH;
            secondCounter = 0;
            break;
        }
        currentState = TIMER_START_HELD;
        clockDisplay_performIncDec();
        break;
    }

    switch(currentState) {
    case WAITING_FOR_TOUCH:
        adcCounter = autoCounter = rateCounter = 0;
        secondCounter++;
        if (secondCounter >= SECOND_COUNTER_MAX_VALUE && hasBeenTouched) {
            clockDisplay_advanceTimeOneSecond();
            secondCounter = 0;
        }
        break;
    case TOUCHED:
        adcCounter++;
        break;
    case ADC_TIMER_COMPLETE:
        autoCounter++;
        break;
    case TIMER_START_HELD:
        rateCounter++;
        break;
    case TIMER_END_HELD:
        rateCounter = 0;
        break;
    }
    return;
}

// Call this before you call clockControl_tick().
void clockControl_init() {
    currentState = WAITING_FOR_TOUCH;
    adcCounter = autoCounter = rateCounter = secondCounter = hasBeenTouched = 0;
    return;
}
