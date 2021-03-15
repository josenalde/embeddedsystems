double v = 0;
double volt = 0;
                        
void setup() {
   pinMode(8, OUTPUT);
   pinMode(A0, INPUT);
   Serial.begin(9600);
}
                        
void loop() {
   digitalWrite(8, HIGH);         
   do {
       volt = analogRead(A0);
       v = (5*volt)/1024;
        //Serial.print("Voltagem: ");
       Serial.println(v);
       //Serial.print("V;  Tempo: ");
       unsigned long currentMillis = millis();    //Tempo atual em ms
       //Serial.print(currentMillis);
       //Serial.println("  s");
    } while (volt != 1021);
   
    digitalWrite(8, LOW);
    do {
        volt = analogRead(A0);
        v = (5*volt)/1024;
        //Serial.print("Voltagem: ");
        Serial.println(v);
        //Serial.print("V;  Tempo: ");
        unsigned long currentMillis = millis();    //Tempo atual em ms
        //Serial.print(currentMillis);
        //Serial.println("  s");
     } while (volt != 1);
}
