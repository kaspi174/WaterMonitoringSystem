//Kacper
//B00123855
// Final code used in the system


#include <EEPROM.h>
#include "GravityTDS.h"
#include <SoftwareSerial.h>
#include <ArduinoJson.h>
#include <OneWire.h>
#include <DallasTemperature.h>

GravityTDS gravityTds;

float temperature = 25, tdsValue = 0;


#define ONE_WIRE_BUS 4
#define rxPin 10
#define txPin 11 
#define TdsSensorPin A1
#define SensorPin A2            //pH meter Analog output to Arduino Analog Input 1
#define Offset 0.00            //deviation compensate
#define LED 13
#define samplingInterval 20
#define printInterval 800
#define ArrayLenth  40    //times of collection
#define MESSAGE_SIZE 5
#define DEBUG 1

SoftwareSerial Sigfox(rxPin, txPin);
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

int pHArray[ArrayLenth];   //Store the average value of the sensor feedback
int pHArrayIndex=0;



void setup() {

  Sigfox.begin(9600);
  Serial.begin(9600);

  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(rxPin, INPUT);
  pinMode(txPin, OUTPUT);
  
  delay(100);
  sensors.begin();

   gravityTds.setPin(TdsSensorPin);
   gravityTds.setAref(5.0);  //reference voltage on ADC, default 5.0V on Arduino UNO
   gravityTds.setAdcRange(1024);  //1024 for 10bit ADC;4096 for 12bit ADC
   gravityTds.begin();  //initialization

   pinMode(LED,OUTPUT);


  String id = getID();
  if(DEBUG){
    Serial.print("Sigfox Device ID: ");
    Serial.println(id);
  }
}
void loop(){

  sensors.requestTemperatures(); 
  float temp = sensors.getTempC(0);
  int sensorValue = analogRead(A0);
  float volt = sensorValue * (5.0 / 1024.0);
  float ntu = -1120.4*square(volt)+5742.3*volt-4353.8; 

  gravityTds.setTemperature(temperature);  // set the temperature and execute temperature compensation
  gravityTds.update();  //sample and calculate
  tdsValue = gravityTds.getTdsValue();  // then get the value

  static unsigned long samplingTime = millis();
  static unsigned long printTime = millis();
  static float pHValue,voltage;

  

  if(millis()-samplingTime > samplingInterval)
  {
      pHArray[pHArrayIndex++]=analogRead(SensorPin);
      if(pHArrayIndex==ArrayLenth)pHArrayIndex=0;
      voltage = avergearray(pHArray, ArrayLenth)*5.0/1024;
      pHValue = 3.5*voltage+Offset;
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
  Serial.println("");
  Serial.println("-----");
  Serial.println("Turbidity:");
  Serial.print("V");
  Serial.println(volt);
  Serial.println("ntu");
  Serial.println(ntu);
  Serial.println("-----");

  
  Serial.println("TDS:");
  Serial.print(tdsValue,0);
  Serial.println("ppm");
  Serial.println("-----");
  Serial.print("Celsius temperature: ");
  Serial.print(sensors.getTempCByIndex(0)); 
  Serial.print(" - Fahrenheit temperature: ");
  Serial.println(sensors.getTempFByIndex(0));
  Serial.println("-----");


  uint8_t msg[MESSAGE_SIZE];
  msg[0] = (uint8_t)(pHValue + 128);   // convert pH value to 1-byte hex value
  msg[1] = (uint8_t)(volt * 51);       // convert voltage to 1-byte hex value (multiply by 51 instead of 255 to avoid overflow)
  msg[2] = (uint8_t)(ntu + 128);       // convert ntu to 1-byte hex value
  msg[3] = (uint8_t)(tdsValue / 100);  // convert tdsValue to 1-byte hex value (divide by 100 to fit within 1 byte)
  msg[4] = (uint8_t)(temp);            // convert temperature value to 1-byte hex value


  

  // send the message using the sendMessage function
  sendMessage(msg, MESSAGE_SIZE);
  Serial.write(msg, MESSAGE_SIZE);
  // wait for 10 minutes (600000 milliseconds)
  delay(30000);
  
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
      }//if
    }//for
    avg = (double)amount/(number-2);
  }//if
  return avg;
}
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

  return id;
}

// send a Sigfox message
void sendMessage(uint8_t msg[], int size)
{
  if(DEBUG){
    Serial.println("Sending message...");
  }

  String status = "";
  String hexChar = "";
  String sigfoxCommand = "";
  char output;

  // build the AT command with the message data
  sigfoxCommand += "AT$SF=";
  for (int i = 0; i < size; i++)
  {
    hexChar = String(msg[i], HEX);

    // padding for single-digit hex values
    if (hexChar.length() == 1)
    {
      hexChar = "0" + hexChar;
    }

    sigfoxCommand += hexChar;
  }

  // send the command to the Sigfox module
  Sigfox.println(sigfoxCommand);

  // wait for the response from the module
  while (!Sigfox.available())
  {
    blink();
  }

  // read the response
  while (Sigfox.available())
  {
    output = (char)Sigfox.read();
    status += output;
    delay(10);
  }

  if(DEBUG){
    Serial.print("Status: ");
    Serial.println(status);
  }
}
void blink(){
  digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on
  delay(1000);                       // wait for a second
  digitalWrite(LED_BUILTIN, LOW);    // turn the LED off
  delay(1000);    
}
 
