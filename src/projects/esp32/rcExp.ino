double v = 0;
double volt = 0;
                        
void setup() {
   pinMode(9, OUTPUT);
   pinMode(A0, INPUT);
   Serial.begin(115200);
   //para emular esp32
   analogReference(EXTERNAL);
}
                        
void loop() {
   //esp32 pwm
   //ledcAttachPin(pin_pwm, channel)
   //ledcSetup(channel, freq, resolution)
   //ledcWrite(0, 255)
   analogWrite(9, (3.3*255)/5); //equiv 3.3
   //digitalWrite(8, HIGH); //5V         
   do {
       volt = analogRead(A0);
       v = (3.3*volt)/1024;
       Serial.println(v);
       unsigned long currentMillis = millis();    //Tempo atual em ms
    } while (volt < 1021);
      
   analogWrite(9, 0); 
   //digitalWrite(9, LOW);
    do {
        volt = analogRead(A0);
        v = (3.3*volt)/1024;
        //Serial.print("Voltagem: ");
        Serial.println(v);
        //Serial.print("V;  Tempo: ");
        unsigned long currentMillis = millis();    //Tempo atual em ms
        //Serial.print(currentMillis);
        //Serial.println("  s");
     } while (volt > 1);
   volt = 0;
}

/* no esp32, para controle é melhor usar o DAC, que vai até 3,3V de contínuo "puro"
   dacWrite(Canal_DAC, Valor digital);
   Onde:
       Canal_DAC: pode ser 25 (para usar o GPIO 25, tensão de saída do DAC0) ou 26 (para usar o GPIO 26, tensão de saída do DAC1);
       Valor digital: valores de 0 até 255, sendo que 0 produz 0V na saída do DAC e 255 produz 3.3V na saída do DAC.
*/       
