byte dutyValue; // for pwmDuty()
byte pwmPin = 3;

void setup() {
  pinMode(pwmPin, OUTPUT);
  pwm25kHzBegin();
  Serial.begin(115200);
  
}

void loop() {
  pwmDuty(19);
}
// from https://fritzenlab.com.br/2018/05/controlar-cooler-intel-4-fios-arduino/
void pwm25kHzBegin() {
  TCCR2A = 0;                               // TC2 Control Register A
  TCCR2B = 0;                               // TC2 Control Register B
  TIMSK2 = 0;                               // TC2 Interrupt Mask Register
  TIFR2 = 0;                                // TC2 Interrupt Flag Register
  TCCR2A |= (1 << COM2B1) | (1 << WGM21) | (1 << WGM20);  // OC2B cleared/set on match when up/down counting, fast PWM
  TCCR2B |= (1 << WGM22) | (1 << CS21);     // prescaler 8
  OCR2A = 79;                               // TOP overflow value (Hz)
  OCR2B = 0;
}
// set duty cycle 25%= 19, 50%=39, 75%=59. 100%=79 (range = 0-79 = 1.25-100%)
void pwmDuty(byte ocrb) {
  OCR2B = ocrb;                             // PWM Width (duty)
}
