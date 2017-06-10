#include "Wire.h"
#include "I2Cdev.h"
#include "HMC5883L.h"
#define M_PI 3.1415926

int16_t x1 ;                 
int16_t x2 ;
HMC5883L mag;
int16_t mx, my, mz;          
byte buf[3] ;

void setup() { 
  //the last byte equals 255, we will use it as a seperator between packets
  buf[2] = 255;
  Wire.begin();
  // initialize serial communication
  // (38400 chosen because it works as well at 8MHz as it does at 16MHz, but
  Serial.begin(38400);
  // initialize device
  Serial.println("Initializing I2C devices...");
  mag.initialize();
} 

int fillBuffer(int16_t x1, int16_t x2) { 
  x1 = x1 ; 
  x2 = x2 ;
  buf[0] = x1;
  buf[2] = x2;
} 
 
void loop() { 
  // read raw heading measurements from device
  mag.getHeading(&mx, &my, &mz);

  // To calculate heading in degrees. 0 degree indicates North
  float heading = atan2(mx, my);
  if(heading < 0)  
    heading += 2 * M_PI;
  x1=heading*180/M_PI/360*256;

  float heading2 = atan2(my, mz);
  if(heading2 < 0)
    heading2 += 2 * M_PI
  x2=heading2*180/M_PI/360*256;
  
  // reads the value of the variable resistor  
  fillBuffer(x1,x2);
  Serial.write(buf,3);
  delay(50); 
} 

