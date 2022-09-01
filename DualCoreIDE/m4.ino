#ifdef CORE_CM4

int myLED;

void setup_m4() {
  // put your setup code here, to run once:
  myLED = LEDG;
}

void loop_m4() {
  // put your main code here, to run repeatedly:
  digitalWrite(myLED, LOW); // turn the LED on 
  delay(198); 
  digitalWrite(myLED, HIGH); // turn the LED off 
  delay(198); // wait for a random amount of time between 1 and 3 seconds.
}
#endif
