#include "SevSeg.h"

SevSeg sevseg; // Instantiate a seven segment controller object
const int BUFFER_SIZE = 32;
void write(char *buf, int buf_len, int start);

void setup()
{
    byte numDigits = 4;

    byte digitPins[] = {10, 15, A0, 4};
    byte segmentPins[] = {16, A1, 6, 8, 9, 14, 5, 7};

    bool resistorsOnSegments = false; // not using any resistors... but this works, so don't change it
    byte hardwareConfig = COMMON_CATHODE;
    bool updateWithDelays = false; // This is their recommended value
    bool leadingZeros = false;
    bool disableDecPoint = false;

    sevseg.begin(hardwareConfig, numDigits, digitPins, segmentPins, resistorsOnSegments,
                 updateWithDelays, leadingZeros, disableDecPoint);
    // sevseg.setBrightness(90);
    Serial.begin(9600);
}

void loop()
{
    static char buffer[BUFFER_SIZE];
    static int read_len = 0;
    static int start = 0;
    static int start_time = millis();

    if (Serial.available()) // only run serial data is incoming
    {
        read_len = Serial.readBytes(buffer, BUFFER_SIZE);
    }

    int now = millis();
    if (now - start_time > 500)
    {
        start_time = now;

        if (read_len <= 4)
        {
            start = 0;
        }
        else
        {
            start = start < read_len ? start + 1 : 0; // display scrolling text
        }
        write(buffer, read_len, start);
    }

    sevseg.refreshDisplay();
}

// Write the buffer the the screen, right-justified.
// Doesn't have to be 4 chars, but only the first 4 will be displayed
//
// `buf` is the buffer you are displaying from
//
// `buf_len` is the length of of `buf` that you want to read from.
//
// `start` is the index to start the display at.
void write(char *buf, int buf_len, int start)
{
    static char display_buffer[8];

    int not_counted = 0;
    for (int i = start; i < buf_len && i < 8; i++)
    {
        if (buf[i] == '.')
        {
            not_counted++;
        }
    }

    int shift = 4 - buf_len + not_counted;
    shift = shift < 0 ? 0 : shift;
    
    Serial.print("buf_len: "); 
    Serial.println(buf_len);

    Serial.print("start: ");
    Serial.println(start);

    Serial.print("shift: ");
    Serial.println(shift);

    Serial.print("n-c: ");
    Serial.println(not_counted);

    // FIXME not respecting start for actuall start location
    for (int i = start; i < buf_len && i < 8; i++)
    {
        display_buffer[i + shift] = buf[i];
    }

    if (shift) // 0 == false
    {          // make blank leading space
        for (int i = start; i < shift; i++)
        {
            display_buffer[i] = ' ';
        }
    }
    // FIXME going from 1.2.3.4. -> 1.2.3.4 (no dot) doesn't remove the last dot
    sevseg.setChars(display_buffer);
}