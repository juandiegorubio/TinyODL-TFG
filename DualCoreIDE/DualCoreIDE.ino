void setup() {
  randomSeed(analogRead(0));
  #ifdef CORE_CM7  
    setup_m7();
  #else
    setup_m4();
  #endif
}

void loop() {
  #ifdef CORE_CM4
    loop_m4();
  #else
    loop_m7();
  #endif
}
