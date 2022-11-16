const int LED = 13;
const int BUTTON = 2;

bool StatusLed = false;
void setup() {
  Serial.begin(9600);
  pinMode(LED, OUTPUT);
  
  pinMode(BUTTON, INPUT);
  
  attachInterrupt(digitalPinToInterrupt(BUTTON),buttonPress,RISING);
}
void loop() {
 digitalWrite(LED, StatusLed);
}

void buttonPress(){
  Serial.println("Interrupt");
  StatusLed = !StatusLed;
}
