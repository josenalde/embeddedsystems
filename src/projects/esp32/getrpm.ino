volatile int rpm;
/* volatile plays an important role in C programming as the compiler can’t guess about the value. 
The main reason behind using volatile is that it can change value any time a user wants it to be changed or
when another thread is running but using the same variable.
It is therefore useful when interrupt service routines can change variable values, instead of the code itself. Moreover, it is used to exchange messages between main code
and ISRs

See examples and explanation here: https://www.educba.com/volatile-in-c/
*/

// quando se usa botões para interrupção, é comum prevenir o efeito bouncing (trepidação do botão), definindo um tempo
#define DEBOUNCETIME 10 //tempo máximo de debounce para o botão (ms)

unsigned long elapsedTime = 0;
int intervalTimer = 1000; //1s

void setup()
{
   Serial.begin(115200);//Inicia a Serial.
   //https://www.filipeflop.com/blog/uso-de-interrupcoes-externas-com-esp32/
   /* attachInterrupt(GPIOPin, ISR, Mode)
      ISR - função a ser executada
      Mode - LOW, HIGH, RISING, FALLING ou CHANGE
   */
   pinMode(2, INPUT); //setar GPIO de interrupção como INPUT
   attachInterrupt(2, ctt, RISING);//Habilita a interrupção 0 no pino 2. UNO. o Uno só possui 2 pinos para interrupção. Esp32 qualquer digital
   elapsedTime = millis();
}
void loop()
{
   if (millis() - elapsedTime >= intervalTimer) { // ou debouncetime
      Serial.print(rpm * 30);//Mostra o RPM.
      Serial.println(" RPM");//Mostra o RPM.
      elapsedTime = millis(); //reset
      rpm = 0; //Reseta a váriavel para proxima leitura.
   }
   //Como estamos lendo a cada UM segundo, usariamos RPM*60 para determinar as rotações por minuto,
   //porém, como foi dito na explicação, é preciso dividir o valor por 2, por isto 30!
}
 

void ctt()
{
   rpm++;//Incrementa a váriavel.
}

// no caso do ESP32, necessário keyword IRAM_ATTR para indicar que a função é executada em RAM interna do ESP32 e não na FLASH, para ser mais rápido
/* 
void IRAM_ATTR ctt() {
   rpm++;//Incrementa a váriavel.
} 
*/

//Outro exemplo interessante, com padrão idf-sdk/freertos: https://www.usinainfo.com.br/blog/esp32-projeto-com-sensor-de-fluxo-de-agua-para-monitoramento-via-wifi/
