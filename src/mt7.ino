void setup() {
 
  Serial.begin(112500);
  delay(1000);
 
  Serial.print("Setup: priority = ");
  Serial.println(uxTaskPriorityGet(NULL));
 
  TaskHandle_t myTask;
 
  xTaskCreate(
                    genericTask,        /* Task function. */
                    "genericTask",      /* String with name of task. */
                    10000,              /* Stack size in words. */
                    NULL,               /* Parameter passed as input of the task */
                    12,                 /* Priority of the task. */
                    &myTask);           /* Task handle. */
 
  Serial.print("Setup: created Task priority = ");
  Serial.println(uxTaskPriorityGet(myTask));
 
}
 
void loop() {
  Serial.print("Main Loop: priority = ");
  Serial.println(uxTaskPriorityGet(NULL));
  delay(1000);
}
 
void genericTask( void * parameter ){
    delay(20000);
    vTaskDelete(NULL);
}
