#include "SevSeg.h"

SevSeg sevseg; //Instantiate a seven segment controller object

void setup() {
  byte numDigits = 4;

  // byte digitPins[] = {4, 7, 8, 10};
  // byte segmentPins[] = {5,   9, 14, A0, A1,  6, 16, 15};

  byte digitPins[] = {10, 15, A0, 4};
  byte segmentPins[] = {16, A1, 6, 8, 9, 14, 5, 7};

  bool resistorsOnSegments = false; // 'false' means resistors are on digit pins
  byte hardwareConfig = COMMON_CATHODE; // See README.md for options
  bool updateWithDelays = false; // Default 'false' is Recommended
  bool leadingZeros = false; // Use 'true' if you'd like to keep the leading zeros
  bool disableDecPoint = false; // Use 'true' if your decimal point doesn't exist or isn't connected

  sevseg.begin(hardwareConfig, numDigits, digitPins, segmentPins, resistorsOnSegments,
  updateWithDelays, leadingZeros, disableDecPoint);
  // sevseg.setBrightness(90);
  Serial.begin(9600);
}

const int BUFFER_SIZE = 32;
char buffer[BUFFER_SIZE];

void loop() {

  if (Serial.available()) // only run if a change is needed
  {
    int read_len = Serial.readBytes(buffer, BUFFER_SIZE);
    
    char display_buffer[4]; 

    int word_len = read_len > 4 ? 4 : read_len;
    // the amount of chars the word needs to be shifted to the right
    int shift = 4 - word_len;

    for (int i = 0; i < shift; i++)
    {
      display_buffer[i] = ' ';
    }
    for (int i = shift; i < 4; i++) 
    {
      display_buffer[i] = buffer[i-shift];  
    }
    
    Serial.print(display_buffer); 
 
    sevseg.setChars(display_buffer);
  }

  sevseg.refreshDisplay();
}
