//************************************
//         sensors.cpp       
//************************************

 // This file contains functions used to read and average
 // the sensors.


#include "sensors.h"
#include "Arduino.h"
#include "MedianFilterLib2.h"
#include "DFRobot_BNO055.h"
#include "Wire.h"
//************************************
//Globals  
//************************************
float rangeShortIR_0;
float rangeShortIR_1;
float rangeMedIR_2;
float rangeMedIR_3;
float rangeLongIR_4;
float rangeLongIR_5;
long int rangeUltrasonic_A;
bool dig_IR_0;
BNO bno(&Wire, 0x28);
BNO::sEulAnalog_t    sEulAnalog;

// Local definitions
int windowSize = 6;
MedianFilter2<int> medianFilter0(windowSize);
MedianFilter2<int> medianFilter1(windowSize);
MedianFilter2<int> medianFilter2(windowSize);
MedianFilter2<int> medianFilter3(windowSize);
MedianFilter2<int> medianFilter4(windowSize);
MedianFilter2<int> medianFilter5(windowSize);




typedef enum {
  short_0,
  short_1,
  medium_2,
  medium_3,
  long_4, 
  long_5
} IR_sensor;

//#define 

long microsecondsToMillimeters(long microseconds)
{
  return (microseconds * 10) / 29 / 2;
} 

// show last imu sensor operate status
void printLastOperateStatus(BNO::eStatus_t eStatus)
{
  switch(eStatus) {
  case BNO::eStatusOK:   Serial.println("everything ok"); break;
  case BNO::eStatusErr:  Serial.println("unknow error"); break;
  case BNO::eStatusErrDeviceNotDetect:   Serial.println("device not detected"); break;
  case BNO::eStatusErrDeviceReadyTimeOut:    Serial.println("device ready time out"); break;
  case BNO::eStatusErrDeviceStatus:    Serial.println("device internal status error"); break;
  default: Serial.println("unknow status"); break;
  }
}

void imu_init()
{
  bno.reset();
  while(bno.begin() != BNO::eStatusOK) {
    Serial.println("bno begin faild");
    printLastOperateStatus(bno.lastOperateStatus);
    delay(2000);
  }
  Serial.println("bno begin success");
}

void read_imu()
{
  BNO::sAxisAnalog_t   sAccAnalog, sMagAnalog, sGyrAnalog, sLiaAnalog, sGrvAnalog;
  BNO::sQuaAnalog_t    sQuaAnalog;
  sAccAnalog = bno.getAxis(BNO::eAxisAcc);    // read acceleration
  sMagAnalog = bno.getAxis(BNO::eAxisMag);    // read geomagnetic
  sGyrAnalog = bno.getAxis(BNO::eAxisGyr);    // read gyroscope
  sLiaAnalog = bno.getAxis(BNO::eAxisLia);    // read linear acceleration
  sGrvAnalog = bno.getAxis(BNO::eAxisGrv);    // read gravity vector
  sEulAnalog = bno.getEul();                  // read euler angle
  sQuaAnalog = bno.getQua();                  // read quaternion
  
  
}

// Read ultrasonic value
void read_ultrasonic(/* Parameters */){
  long duration;
 
  // The sensor is triggered by a HIGH pulse of 10 or more microseconds.
  // Give a short LOW pulse beforehand to ensure a clean HIGH pulse:
  digitalWrite(ULTRASOUND_A_TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(ULTRASOUND_A_TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(ULTRASOUND_A_TRIG_PIN, LOW);
 
  // Read the signal from the sensor: a HIGH pulse whose
  // duration is the time (in microseconds) from the sending
  // of the ping to the reception of its echo off of an object.
  duration = pulseIn(ULTRASOUND_A_ECHO_PIN, HIGH);
 
  // convert the time into a distance
  rangeUltrasonic_A = microsecondsToMillimeters(duration);
}


// Read infrared value
void read_infrared(/* Parameters */){
  dig_IR_0 = digitalRead(DIG_IR_0_PIN);

  int short0Raw_IR = analogRead(SHORTIR_0_ADDRESS);
  int filterd0 = medianFilter0.AddValue(short0Raw_IR);
  rangeShortIR_0 = IR_to_range(short_0, filterd0);
  
  int short1Raw_IR = analogRead(SHORTIR_1_ADDRESS);
  int filterd1 = medianFilter1.AddValue(short1Raw_IR);
  rangeShortIR_1 = IR_to_range(short_1, filterd1);
  
	int medium2Raw_IR = analogRead(MEDIUMIR_2_ADDRESS);
  int filterd2 = medianFilter2.AddValue(medium2Raw_IR);
  rangeMedIR_2 = IR_to_range(medium_2, filterd2);
  
  int medium3Raw_IR = analogRead(MEDIUMIR_3_ADDRESS);
  int filterd3 = medianFilter3.AddValue(medium3Raw_IR);
  rangeMedIR_3 = IR_to_range(medium_3, filterd3);
  
  int long4Raw_IR = analogRead(LONGIR_4_ADDRESS);
  int filterd4 = medianFilter4.AddValue(long4Raw_IR);
  rangeLongIR_4 = IR_to_range(long_4, filterd4);
  
  int long5Raw_IR = analogRead(LONGIR_5_ADDRESS);
  int filterd5 = medianFilter5.AddValue(long5Raw_IR);
  rangeLongIR_5 = IR_to_range(long_5, filterd5);
  
  /*Serial3.print(sEulAnalog.head);
  Serial3.print("|  0: ");
  Serial3.print(rangeShortIR_0);
  Serial3.print("|  1: ");
  Serial3.print(rangeShortIR_1);
  */
  //Serial3.print("|  2: ");
  //Serial3.print(rangeMedIR_2);
  //Serial3.print("|  3: ");
  //Serial3.println(rangeMedIR_3);
  //Serial3.print("|  4: ");
  //Serial3.print(rangeLongIR_4);
  //Serial3.print("|  5: ");
  //Serial3.println(rangeLongIR_5);

 
  
  
}

// Read colour sensor value
void read_colour(/* Parameters */){
  Serial.println("colour value \n");  
}

// Pass in data and average the lot
void sensor_average(/* Parameters */){
  Serial.println("Averaging the sensors \n");
}

float IR_to_range(int sensorNum, int filtered_IR){
	float range;
	switch(sensorNum) {
    case short_0:
    range = 21648*pow(filtered_IR, -0.982);
    break;
    case short_1:
    range = 22837*pow(filtered_IR, -0.992);
    break;
    case medium_2:
    range = 14805*pow(filtered_IR, -0.768);
    break;
    case medium_3:
    range = 15863*pow(filtered_IR, -0.784);
    break;
    case long_4:
    range = 73660*pow(filtered_IR, -0.925);
    break;
    case long_5:
    range = 97629*pow(filtered_IR, -0.978);
    break;
	}
	return range;
}
