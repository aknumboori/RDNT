# RDNT_Firmware


## ESP32.zip

This is the final code shipped for the project.

## PCB Verification Files

These files are used for testing the project as we solder on components. They are described below:

### TestMCU_CaseLED.c
This program requires FastLED library and runs in the Arduino framework. It blinks the LEDs white and black.

### TestMCU_CaseLEDNoLibraries.c
This program requies no libraries and implements the same functions as above by manually controlling the PWM signals. This program uses Espressif-IDF framework and is closer to the final implementation.

### TestMCU_ModeButton.c
This program is used to test that the mode button has been implemented correctly. It is lacking state reflection on the case LED, that will be implemented in the next file.


## Prototyping Tools

These are main.c or main.cpp files that have were used in early prototyping. Many of these files are lacking dependencies but these are clear from reading the files. None of these files are fit for release and exist here solely for archival purposes. Use at your own caution.
