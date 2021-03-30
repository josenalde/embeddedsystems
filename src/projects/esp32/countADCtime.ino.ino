#include <ESP8266WiFi.h>

ADC_MODE(ADC_VCC);

unsigned long tempo_inicio;
unsigned long tempo_fim;
unsigned long valor;
 
void setup() {
 Serial.begin(9600);    //inicia a comunicação serial
}
 
void loop() {  
   // leitura
   tempo_inicio = micros(); //marca tempo de inicio de leitura
   float vdd = ESP.getVcc();  //le valor convertido
   tempo_fim = micros();   //le tempo no fim da conversão
 
   //exibe valor lido e tempo de conversão
   Serial.print("Valor = ");
   Serial.print(vdd);
   Serial.print(" -- Tempo leitura = ");
   Serial.print(tempo_fim - tempo_inicio);
   Serial.println(" us");
   delay(500);
}
