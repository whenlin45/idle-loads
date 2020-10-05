
#include <FilterDerivative.h>
#include <FilterOnePole.h>
#include <Filters.h>
#include <FilterTwoPole.h>
#include <FloatDefine.h>
#include <RunningStatistics.h>

#include <LowPower.h>


//int Vo;
//int analogPin= 0;
//int raw= 0;
//int Vin= 5;
//float Vout= 0;
//float R1= 1000;
//float R2= 0;
//float buffer= 0;

float oldCurrent1; 
float newCurrent1;
float oldCurrent2;
float newCurrent2;

#define ACS_Pin1 A1                        //Sensor data pin on A1 analog input
#define ACS_Pin2 A2                        //Sensor data pin on A2 analog input

float ACS_Value;                              //Here we keep the raw data valuess
float testFrequency = 50;                    // test signal frequency (Hz)
float windowLength = 40.0/testFrequency;     // how long to average the signal, for statistist

float intercept = 0; // to be adjusted based on calibration testing
float intercept1 = 0;
float slope = 0.0752; // to be adjusted based on calibration testing
float slope1 = 0.0752;
                      //Please check the ACS712 Tutorial video by SurtrTech to see how to get them because it depends on your sensor, or look below

float Amps_TRMS1; // estimated actual current in amps
float Amps_TRMS2;

float outputCurrent;
float inputResistance;
 
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600); //Start serial communication

  pinMode(2, INPUT); // pin 2 as INPUT
  pinMode(ACS_Pin1,INPUT);  //Define the pin mode
  pinMode(ACS_Pin2,INPUT);  //Define the pin mode
  calibrate();
  readCurrent();
}

void loop() {
  // put your main code here, to run repeatedly:

   // Allow wake up pin to trigger interrupt on rising .
    attachInterrupt(digitalPinToInterrupt(2), wakeUp, RISING);

    LowPower.powerDown(SLEEP_FOREVER, ADC_OFF, BOD_OFF);
    // Disable external pin interrupt on wake up pin.

    detachInterrupt(digitalPinToInterrupt(2));
    
  readCurrent();

  if(didCurrentChange(oldCurrent2, newCurrent2)){
    Serial.print("current changed...");
  } else {
    Serial.print("current has not changed...");
  }
  
}

void wakeUp() {
  Serial.print("INTERRUPT TRIGGERED!!!!");
}

void calibrate(){
  RunningStatistics inputStats;                 // create statistics to look at the raw test signal
  RunningStatistics inputStats1;

  inputStats.setWindowSecs( windowLength );     //Set the window length
  inputStats1.setWindowSecs (windowLength);

  //INPUT STATS FOR 2ND SENSOR
  if(inputStats1.sigma() > 0){   //if sigma > 0, intercept is negative to reduce offset to 0
    intercept1 = -1 * inputStats1.sigma()
  } else if (inputStats1.sigma() < 0) {  //if sigma < 0, intercept is positive to reduce offset to 0
    intercept1 = inputStats1.sigma()
  }

//INPUT STATS FOR 1st SENSOR
  if(inputStats.sigma() > 0){   //if sigma > 0, intercept is negative to reduce offset to 0
    intercept = -1 * inputStats.sigma()
  } else if (inputStats.sigma() < 0) {  //if sigma < 0, intercept is positive to reduce offset to 0
    intercept = inputStats.sigma()
  }
}

/**
 *  This method will update the current Amps value and the old Amps value each time it is invoked
 */
void readCurrent() {
  Amps_TRMS1 = slope * inputStats.sigma();
  oldCurrent1 = newCurrent1;
  newCurrent1 = Amps_TRMS1;

  Amps_TRMS2 = slope1 * inputStats1.sigma();
  oldCurrent2 = newCurrent2;
  newCurrent2 = Amps_TRMS2;
  //return newCurrent;
}

bool didCurrentChange(float prevVal, float currentVal){
  return abs((prevVal - currentVal)) != 0;
}