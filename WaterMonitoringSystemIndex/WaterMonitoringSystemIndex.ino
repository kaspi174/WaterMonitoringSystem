//Kacper Czajka
//B00123855
//Technological University Dublin
//Water Monitoring System

#include <EEPROM.h>
#include "GravityTDS.h"
#include <SoftwareSerial.h>



GravityTDS gravityTds;

float temperature = 19.9,tdsValue = 0;
 
#define rxPin 10
#define txPin 11 
#define TdsSensorPin A1
#define SensorPin A2            //pH meter Analog output to Arduino Analog Input 1
#define Offset 0.82            //deviation compensate
#define LED 13
#define samplingInterval 20
#define printInterval 800
#define ArrayLenth  40    //times of collection

// set up a new serial port
SoftwareSerial Sigfox =  SoftwareSerial(rxPin, txPin);
//Set to 0 if you don't need to see the messages in the console
#define DEBUG 1
//Message buffer
uint8_t msg[12];





int pHArray[ArrayLenth];   //Store the average value of the sensor feedback
int pHArrayIndex=0;

void setup() {
  Serial.begin(9600);

   gravityTds.setPin(TdsSensorPin);
   gravityTds.setAref(5.0);  //reference voltage on ADC, default 5.0V on Arduino UNO
   gravityTds.setAdcRange(1024);  //1024 for 10bit ADC;4096 for 12bit ADC
   gravityTds.begin();  //initialization

   pinMode(LED,OUTPUT);
}
void loop() {
  int sensorValue = analogRead(A0);
  float volt = sensorValue * (5.0 / 1024.0);
  float ntu = -1120.4*square(volt)+5742.3*volt-4353.8; 

  gravityTds.setTemperature(temperature);  // set the temperature and execute temperature compensation
  gravityTds.update();  //sample and calculate
  tdsValue = (gravityTds.getTdsValue())-32;  // then get the value

  static unsigned long samplingTime = millis();
  static unsigned long printTime = millis();
  static float pHValue,voltage;
  if(millis()-samplingTime > samplingInterval)
  {
      pHArray[pHArrayIndex++]=analogRead(SensorPin);
      if(pHArrayIndex==ArrayLenth)pHArrayIndex=0;
      voltage = avergearray(pHArray, ArrayLenth)*5.0/1024;
      pHValue = 3.6*voltage+Offset;
      samplingTime=millis();
  }
  if(millis() - printTime > printInterval)   //Every 800 milliseconds, print a numerical, convert the state of the LED indicator
  {
      Serial.println("-----");
    Serial.print("Voltage:");
        Serial.print(voltage,2);
        Serial.print("    pH value: ");
    Serial.println(pHValue,2);
        digitalWrite(LED,digitalRead(LED)^1);
        printTime=millis();
      Serial.println("-----");
  }

  Serial.println("-----");
  Serial.println("Turbidity:");
  Serial.print("V");
  Serial.println(volt);
  Serial.println("ntu");
  Serial.println(ntu);
  Serial.println("-----");

  Serial.println("-----");
  Serial.println("TDS:");
  Serial.print(tdsValue,0);
  Serial.println("ppm");
  Serial.println("-----");
  delay(3000);
  
}
double avergearray(int* arr, int number){
  int i;
  int max,min;
  double avg;
  long amount=0;
  if(number<=0){
    Serial.println("Error number for the array to avraging!/n");
    return 0;
  }
  if(number<5){   //less than 5, calculated directly statistics
    for(i=0;i<number;i++){
      amount+=arr[i];
    }
    avg = amount/number;
    return avg;
  }else{
    if(arr[0]<arr[1]){
      min = arr[0];max=arr[1];
    }
    else{
      min=arr[1];max=arr[0];
    }
    for(i=2;i<number;i++){
      if(arr[i]<min){
        amount+=min;        //arr<min
        min=arr[i];
      }else {
        if(arr[i]>max){
          amount+=max;    //arr>max
          max=arr[i];
        }else{
          amount+=arr[i]; //min<=arr<=max
        }
      }
    }
    avg = (double)amount/(number-2);
  }
  return avg;
}
----------------------------------

// the setup function runs once when you press reset or power the board
void setup() {
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(LED_BUILTIN, OUTPUT);

  if(DEBUG){
    Serial.begin(9600);
  }

  // open Wisol communication
   // define pin modes for tx, rx:
  pinMode(rxPin, INPUT);
  pinMode(txPin, OUTPUT);
  Sigfox.begin(9600);
  delay(100);
  getID();
  delay(100);
  getPAC();
}

// the loop function runs over and over again forever
void loop() {
  msg[0]=0xC0;
  msg[1]=0xFF;
  msg[2]=0xEE;

  sendMessage(msg, 3);

  // In the ETSI zone, due to the reglementation, an object cannot emit more than 1% of the time hourly
  // So, 1 hour = 3600 sec
  // 1% of 3600 sec = 36 sec
  // A Sigfox message takes 6 seconds to emit
  // 36 sec / 6 sec = 6 messages per hours -> 1 every 10 minutes
  delay(60000);
}

void blink(){
  digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(1000);                       // wait for a second
  digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
  delay(1000);    
}

//Get Sigfox ID
String getID(){
  String id = "";
  char output;

  Sigfox.print("AT$I=10\r");
  while (!Sigfox.available()){
     blink();
  }

  while(Sigfox.available()){
    output = Sigfox.read();
    id += output;
    delay(10);
  }

  if(DEBUG){
    Serial.println("Sigfox Device ID: ");
    Serial.println(id);
  }

  return id;
}
//Get PAC number
String getPAC(){
  String pac = "";
  char output;

  Sigfox.print("AT$I=11\r");
  while (!Sigfox.available()){
     blink();
  }

  while(Sigfox.available()){
    output = Sigfox.read();
    pac += output;
    delay(10);
  }

  if(DEBUG){
    Serial.println("PAC number: ");
    Serial.println(pac);
  }

  return pac;
}


//Send Sigfox Message
void sendMessage(uint8_t msg[], int size)
{
  Serial.println("Inside sendMessage");

  String status = "";
  String hexChar = "";
  String sigfoxCommand = "";
  char output;

  sigfoxCommand += "AT$SF=";

  for (int i = 0; i < size; i++)
  {
    hexChar = String(msg[i], HEX);

    //padding
    if (hexChar.length() == 1)
    {
      hexChar = "0" + hexChar;
    }

    sigfoxCommand += hexChar;
  }

  Serial.println("Sending...");
  Serial.println(sigfoxCommand);
  Sigfox.println(sigfoxCommand);

  while (!Sigfox.available())
  {
    Serial.println("Waiting for response");
    delay(1000);
  }

  while (Sigfox.available())
  {
    output = (char)Sigfox.read();
    status += output;
    delay(10);
  }

  Serial.println();
  Serial.print("Status \t");
  Serial.println(status);
}


