#ifdef CORE_CM7

int myLED;

void setup_m7() {
  // put your setup code here, to run once:
  bootM4();
  myLED = LEDB;
  Serial.begin(9600);
}

void loop_m7() {
  Serial.println("M7 core");
}
#endif
