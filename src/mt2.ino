// setup() and loop() runs at APP_CPU by default
void setup() {
  Serial.begin(115200);
  Serial.println(xPortGetCoreID());
}
 
void loop() {
      //Default loop runs on APP_CPU
    Serial.print("This loop runs on APP_CPU which id is:");
    Serial.println(xPortGetCoreID()); // ID 1 (APP_CPU)   
}
