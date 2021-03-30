// Variáveia para armazenar os resultados
 
unsigned long tempo_inicio;
unsigned long tempo_fim;
unsigned long valor;
double Ts;
 
// constante para configuração do prescaler ADPS2 = 2: 0010, ADPS1 = 1: 0001, ADPS = 0: 0000
const unsigned char PS_16 = (1 << ADPS2); // 2^ADPS2 = 4 = 100
const unsigned char PS_32 = (1 << ADPS2) | (1 << ADPS0); // 2^ADPS2 = 100 | 2^ADPS0 = 001, 100 | 001 = 101
const unsigned char PS_64 = (1 << ADPS2) | (1 << ADPS1); // 100 | 010 = 110
const unsigned char PS_128 = (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0); // 111
 
void setup() {
 Serial.begin(115200);
 Serial.println(ADPS2);
 Serial.println(ADPS1);
 Serial.println(ADPS0);
 
 // configura o preescaler do ADC
 ADCSRA &= ~PS_128;  //limpa configuração da biblioteca do arduino ADCSRA = ADCSRA & NOT(111) = x x x x x x x x
                     //                                                                       & 1 1 1 1 1 0 0 0
                     //                                                                         x x x x x 0 0 0
 
 // valores possiveis de prescaler só deixar a linha com prescaler desejado
 // PS_16, PS_32, PS_64 or PS_128
 //ADCSRA |= PS_128; // 64 prescaler
 ADCSRA |= PS_64; // 64 prescaler ADCSRA = ADCSRA OR 00000110
 //ADCSRA |= PS_32; // 32 prescaler
 //ADCSRA |= PS_16; // 16 prescaler
}
 
void loop() {  
 // leitura
   tempo_inicio = micros(); //marca tempo de inicio de leitura
   valor = analogRead(0);  //le valor convertido
   tempo_fim = micros();   //le tempo no fim da conversão
   Ts = (1.0/(tempo_fim - tempo_inicio))*1000;
 
   //exibe valor lido e tempo de conversão
   Serial.print("Valor = ");
   Serial.print(valor);
   Serial.print(" -- Ts = ");
   Serial.print(Ts, 2); // 112 us
   Serial.println(" kS/s");
   delay(500);
}
