int globalIntVar = 5;
 
void setup() {
 
  Serial.begin(112500);
  delay(1000);
 
  xTaskCreate(
                    globalIntTask,             /* Task function. */
                    "globalIntTask",           /* String with name of task. */
                    10000,                     /* Stack size in words. */
                    (void*)&globalIntVar,      /* Parameter passed as input of the task */
                    1,                         /* Priority of the task. */
                    NULL);                     /* Task handle. */
 
  int localIntVar = 9;
 
  xTaskCreate(
                    localIntTask,              /* Task function. */
                    "localIntTask",            /* String with name of task. */
                    10000,                     /* Stack size in words. */
                    (void*)&localIntVar,       /* Parameter passed as input of the task */
                    1,                         /* Priority of the task. */
                    NULL);                     /* Task handle. */
 
}
 
void loop() {
  delay(1000);
}
 
void globalIntTask( void * parameter ){
 
    Serial.print("globalIntTask: ");
    Serial.println(*((int*)parameter));            
 
    vTaskDelete( NULL );
 
}
 
void localIntTask( void * parameter ){
 
    Serial.print("localIntTask: ");
    Serial.println(*((int*)parameter));            
 
    vTaskDelete( NULL );
 
}
