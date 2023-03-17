#ifndef MY_IR_CONFIGS_INCLUDED
#define MY_IR_CONFIGS_INCLUDED

#define MY_IR_RECEIVE_PIN      15  // GPIO15 to be compatible with the Arduino Nano RP2040 Connect (pin3)
#define MY_IR_SEND_PIN         16  // GPIO16
#define TONE_PIN            17
#define APPLICATION_PIN     18
#define ALTERNATIVE_IR_FEEDBACK_LED_PIN 19 // E.g. used for examples which use LED_BUILDIN for example output.
#define _IR_TIMING_TEST_PIN 20

#include <IRremote.hpp>

/*
Pin mapping
 * Platform     IR input    IR output   Tone      Core/Pin schema
 * --------------------------------------------------------------
 * RP2040       3|GPIO15    4|GPIO16    5|GPIO17
 */

 #endif