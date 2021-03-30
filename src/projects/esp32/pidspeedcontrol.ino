/******************************************************************
 * PID Simple Example (Augmented with Processing.org Communication)
 * Version 0.3
 * by Brett Beauregard
 * License: Creative-Commons Attribution Share-Alike
 * April 2011
 ******************************************************************/
 /* Alterado por Josenalde Oliveira em 18.04.2018 e 01/05/2018 e 06/05/2019 para FAN CONTROL */
 
/* PID control RPM Fan */
#include "PID_v1.h"

// signal pins
int inputPin = 2; // Hall sensor speed signal Digital Pin 2
int outputPin = 3; //D3~ for pwm control signal - blue wire 4-wire fan

unsigned long serialTime; //this will help us know when to talk with processing

double rpmSetpoint, Setpoint, Input, Output;

double rpmInput;
volatile double aux; //Váriavel que armazena as rotações. manipulada pela interrupção
unsigned long elapsedTime = 0;
int intervalTimer = 1000; // 1 second

const int rpmMax = 2970; // set the maximum speed for each fan 
                         // to measure, send pwmDuty(79) and measure RPM
byte dutyValue; // for pwmDuty()

//Specify the links and initial tuning parameters
PID myPID(&Input, &Output, &Setpoint, 1,0.1,0.01, DIRECT); // 2.28, 5.49 0

void setup()
{
  pinMode(outputPin, OUTPUT); // pwm signal (control signal)
  
  // from https://www.arduino.cc/reference/pt/language/functions/external-interrupts/attachinterrupt/
  attachInterrupt(digitalPinToInterrupt(2), ctt, RISING);//Habilita a interrupção 0 no pino 2. O UNO tem 02 interrupções
  
  pwm25kHzBegin(); // changes pwm frequency from 490 Hz to 25 kHz
  
  Serial.begin(115200); // 115200 for rpm readings !!!
  //initialize the variables we're linked to
  
  Input = 0;
  rpmSetpoint = 2000; // RPM
  // map(valor, deMenor, deMaior, paraMenor, paraMaior);
  Setpoint = map(rpmSetpoint, 0, rpmMax, 0, 255); 

  //turn the PID on
  myPID.SetMode(AUTOMATIC);
}

void loop()
{
  if (millis() - elapsedTime >= intervalTimer) {
      //Serial.print(rpm * 30);//Mostra o RPM.
      //Serial.println(" RPM");//Mostra o RPM.
      elapsedTime = millis(); //reset
      rpmInput = aux;
      aux = 0; //Reseta a váriavel para proxima leitura.
   }
  // read speed signal and convert it to RPM (bipolar hall effect sensor)
  // rpmInput*60/2, since its 2 readings each cycle
  Input = map(rpmInput*30, 0, rpmMax, 0, 255); 

  myPID.Compute();
  
  dutyValue = map(Output, 0, 255, 0, 79); // linear mapping
  
  pwmDuty(dutyValue); 
  
  if(millis() > serialTime)
  {
    SerialReceive();
    SerialSend();
    serialTime+=50; // communicate at each 100 ms = 0.1s
  }
}

/********************************************
 * Serial Communication functions / helpers
 ********************************************/


union {                // This Data structure lets
  byte asBytes[24];    // us take the byte array
  float asFloat[6];    // sent from processing and
}                      // easily convert it to a
foo;                   // float array



// getting float values from processing into the arduino
// was no small task.  the way this program does it is
// as follows:
//  * a float takes up 4 bytes.  in processing, convert
//    the array of floats we want to send, into an array
//    of bytes.
//  * send the bytes to the arduino
//  * use a data structure known as a union to convert
//    the array of bytes back into an array of floats

//  the bytes coming from the arduino follow the following
//  format:
//  0: 0=Manual, 1=Auto, else = ? error ?
//  1: 0=Direct, 1=Reverse, else = ? error ?
//  2-5: float setpoint
//  6-9: float input
//  10-13: float output  
//  14-17: float P_Param
//  18-21: float I_Param
//  22-245: float D_Param
void SerialReceive()
{

  // read the bytes sent from Processing
  int index=0;
  byte Auto_Man = -1;
  byte Direct_Reverse = -1;
  while(Serial.available()&&index<26)
  {
    if(index==0) Auto_Man = Serial.read();
    else if(index==1) Direct_Reverse = Serial.read();
    else foo.asBytes[index-2] = Serial.read();
    index++;
  } 
  
  // if the information we got was in the correct format, 
  // read it into the system
  if(index==26  && (Auto_Man==0 || Auto_Man==1)&& (Direct_Reverse==0 || Direct_Reverse==1))
  {
    Setpoint=double(foo.asFloat[0]);
    //Input=double(foo.asFloat[1]);       // * the user has the ability to send the 
                                          //   value of "Input"  in most cases (as 
                                          //   in this one) this is not needed.
    if(Auto_Man==0)                       // * only change the output if we are in 
    {                                     //   manual mode.  otherwise we'll get an
      Output=double(foo.asFloat[2]);      //   output blip, then the controller will 
    }                                     //   overwrite.
    
    double p, i, d;                       // * read in and set the controller tunings
    p = double(foo.asFloat[3]);           //
    i = double(foo.asFloat[4]);           //
    d = double(foo.asFloat[5]);           //
    myPID.SetTunings(p, i, d);            //
    
    if(Auto_Man==0) myPID.SetMode(MANUAL);// * set the controller mode
    else myPID.SetMode(AUTOMATIC);             //
    
    if(Direct_Reverse==0) myPID.SetControllerDirection(DIRECT);// * set the controller Direction
    else myPID.SetControllerDirection(REVERSE);          //
  }
  Serial.flush();                         // * clear any random data from the serial buffer
}

// unlike our tiny microprocessor, the processing ap
// has no problem converting strings into floats, so
// we can just send strings.  much easier than getting
// floats from processing to here no?
void SerialSend()
{
  Serial.print("PID ");
  //Serial.print(" ");
  Serial.print(Setpoint);   
  Serial.print(" ");
  Serial.print(Input);   
  Serial.print(" ");
  Serial.print(Output);   
  Serial.print(" ");
  Serial.print(myPID.GetKp());   
  Serial.print(" ");
  Serial.print(myPID.GetKi());   
  Serial.print(" ");
  Serial.print(myPID.GetKd());   
  Serial.print(" ");
  if(myPID.GetMode()==AUTOMATIC) Serial.print("Automatic");
  else Serial.print("Manual");  
  Serial.print(" ");
  if(myPID.GetDirection()==DIRECT) Serial.println("Direct");
  else Serial.println("Reverse");
}

// from https://fritzenlab.com.br/2018/05/controlar-cooler-intel-4-fios-arduino/
void pwm25kHzBegin() {
  TCCR2A = 0;                               // TC2 Control Register A
  TCCR2B = 0;                               // TC2 Control Register B
  TIMSK2 = 0;                               // TC2 Interrupt Mask Register
  TIFR2 = 0;                                // TC2 Interrupt Flag Register
  TCCR2A |= (1 << COM2B1) | (1 << WGM21) | (1 << WGM20);  // OC2B cleared/set on match when up/down counting, fast PWM
  TCCR2B |= (1 << WGM22) | (1 << CS21);     // prescaler 8
  OCR2A = 79;                               // TOP overflow value (Hz)
  OCR2B = 0;
}
// set duty cycle 25%= 19, 50%=39, 75%=59. 100%=79 (range = 0-79 = 1.25-100%)
void pwmDuty(byte ocrb) {
  OCR2B = ocrb;                             // PWM Width (duty)
}

// ISR fan rotations
void ctt()
{
   aux++;//Incrementa a váriavel.
}
