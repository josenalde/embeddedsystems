//https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/system/sleep_modes.html

#define factor 1000000 //10^6 us = 1s
#define sleep_time 6 //6s

#define RED 2
#define GREEN 4

RTC_DATA_ATTR int boot_number = 0;

void setup()
{
  
  Serial.begin(115200);
  Serial.println(boot_number);
  pinMode(RED,OUTPUT);
  pinMode(GREEN,OUTPUT);
  delay(500);  
  if(boot_number == 0) { //first boot
      digitalWrite(RED,HIGH);
      boot_number++;
  }
  else  {
      digitalWrite(GREEN,HIGH);
       boot_number++;
  }
  delay(4000);
  digitalWrite(GREEN,LOW);
  digitalWrite(RED,LOW);

  Serial.println("ESP32 going to Deep Sleep");
  esp_sleep_enable_timer_wakeup(sleep_time * factor); //6s
  esp_deep_sleep_start();
}
void loop()
{
}