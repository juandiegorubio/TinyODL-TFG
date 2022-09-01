#include "Arduino.h"
// #include "RPC.h"
// #include "ModelNN.h"
// #include "dataset.h"


// const int InputSize = 10;
// const int OutputSize = 1; 

// ModelNN model(1);

int value = 0;

// the setup function runs once when you press reset or power the board
void setup() {
    // initialize digital pin LED_BUILTIN as an output.
    pinMode(LED_BUILTIN, OUTPUT);

    Serial.begin(38400);
    while(!Serial.available());
}

// the loop function runs over and over again forever
void loop() {
    Serial.println("Hello Computer");
    analogWrite(LEDG, value); // turn the LED on (LOW is the voltage level)
    delay(1000); // wait for a second
    value = (value++)%255;
}
