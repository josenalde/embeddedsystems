//#include <freertos/FreeRTOS.h>
//#include <freertos/task.h>
//#include <esp_system.h>

int pin_task1 = 2;
int pin_task2 = 25;
int pin_task3 = 26;
 
void task1(void* z)
{
    while (1)
    {
        digitalWrite(pin_task1, 1);
        delay(100);
        digitalWrite(pin_task1, 0);
        delay(100);
    }
}
 
void task2(void*z)
{
    while (1)
    {
        digitalWrite(pin_task2, 1);
        delay(100);
        digitalWrite(pin_task2, 0);
        delay(100);
    }
}
 
void task3(void*z)
{
    while (1)
    {
        for (uint8_t i = 0; i < 200; i++)
        {
            digitalWrite(pin_task3, 1);
            delay(500);
            digitalWrite(pin_task3, 0);
            delay(500);
        }
 
        vTaskDelay(pdMS_TO_TICKS(200)); //ticks for ms
    }
}
 
void setup() {
    Serial.begin(115200);
    pinMode(pin_task1, OUTPUT);
    pinMode(pin_task2, OUTPUT);
    pinMode(pin_task3, OUTPUT);
} 
 
void loop()
{
    //Cria as tarefas
    xTaskCreatePinnedToCore(task1, "task1", 2048, NULL, 1, NULL, 0);//Tarefa 1 com prioridade UM (1) no core 0 (4kB)
    xTaskCreatePinnedToCore(task2, "task2", 2048, NULL, 1, NULL, 0);//Tarefa 2 com prioridade UM (1) no core 0
    xTaskCreatePinnedToCore(task3, "task3", 2048, NULL, 2, NULL, 0);//Tarefa 3 com prioridade DOIS (2) no core 0
//(function to implement the task, name of the task, stack size (words), task input, priority, task handle, core)

}