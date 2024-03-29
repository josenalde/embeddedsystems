/* Alterado por Josenalde Oliveira em 16.03.2021/19.01.2022 */
                            
unsigned long lastTime;
double u=0, y=0, sp; // u: input, y: output (and sensor measure), sp: setpoint
double iTerm=0, lastY=0;
double kp, ki, kd;
unsigned long sampleTime = 1000; // 1000 ms or 1 sec
double uMin, uMax; // control signal saturation
double voltageY;

double VCC = 3.3; // For ESP32 3.3, For Uno 5
int yPin = 32; //GPIO32 ADC1 (ESP) 
//A0 for sensor pin (pv sensor)
int uPin = 25; // PWM or DAC0 (25) and DAC1 (26)

void computeU() {
    unsigned long now = millis();
    int timeChange = (now - lastTime);
    if (timeChange >= sampleTime) {
        double e = sp - y; // tracking error
        iTerm += (ki * e); // integral term I
        // integral term is always increasing, therefore it is limited
        if (iTerm > uMax) iTerm = uMax;
        else if (iTerm < uMin) iTerm = uMin;
        // y difference term
        double dY = (y - lastY);

        // compute control signal
        u = kp*e + iTerm - kd*dY;
        if (u > uMax) u = uMax;
        else if (u < uMin) u = uMin;

        lastY = y;
        lastTime = now;
    }
}

void setTunings(double kP, double kI, double kD) {
    double sampleTimeInSec = ((double)sampleTime/1000);
    kp = kP;
    ki = kI * sampleTimeInSec;
    kd = kD / sampleTimeInSec;
}

void setSampleTime(int newSampleTime) {
    if (newSampleTime > 0) {
        double ratio = (double)newSampleTime / (double)sampleTime;
        ki *= ratio;
        kd /= ratio;
        sampleTime = (unsigned long)newSampleTime;
    }
}

void setControlLimits(double min, double max) {
    if (min > max) return;
    uMin = min;
    uMax = max;
    if (u > uMax) u = uMax;
    else if (u < uMin) u = uMin;
    if (iTerm > uMax) iTerm = uMax;
    else if (iTerm < uMin) iTerm = uMin;
}


                     
void setup() {
  Serial.begin(9600);
  analogReadResolution(10);
  ledcAttachPin(uPin, 0); //channel 0 (pin 32)
  ledcSetup(0, 1000, 8); //channel, freq (Hz), resolution (bits)
  sp = 400; // considering 1-1023
  pinMode(uPin, OUTPUT);
  pinMode(yPin, INPUT);
  setControlLimits(0, 255); // 255 = 5V PWM
  setSampleTime(10);
  //setTunings(0.05, 0.5, 0.1);
  //setTunings(2, 0.8, 0.2); // boa resposta
  //setTunings(1.41, 1.41/4.04, kp*2.92 );
  //setTunings(1.5, 0.8, 0.2); // boa resposta
  setTunings(1.5, 0.8, 1);
  dacWrite(uPin, 0);
  delay(5000);
  
}  
                            
void loop() {
  y = analogRead(yPin); // 0 - 1023
  // voltageInput
  voltageY = (VCC/1024)*y;
  computeU();
  //ledcWrite(0, u); //ESP32
  //analogWrite(uPin, u); // for open loop, use 255 for instance (UNO)
  dacWrite(uPin,u); //ESP32 pure DAC
  Serial.print(1023); //upper limit
  Serial.print(" ");
  Serial.print(0); //lower limit
  Serial.print(" ");
  Serial.print(y);
  Serial.print(" ");
  Serial.print(sp);
  Serial.println();  //Serial.println(kp);
  delay(1);
}
