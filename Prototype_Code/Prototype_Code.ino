//Kacper Czajka
//B00123855
//Sample Code to test sensors

#include <EEPROM.h>
#include "GravityTDS.h"

GravityTDS gravityTds;

float temperature = 19.9,tdsValue = 0;
 
#define TdsSensorPin A1

#define SensorPin A2            //pH meter Analog output to Arduino Analog Input 1
#define Offset 0.82            //deviation compensate
#define LED 13
#define samplingInterval 20
#define printInterval 800
#define ArrayLenth  40    //times of collection
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
      }//if
    }//for
    avg = (double)amount/(number-2);
  }//if
  return avg;
}
 
 
