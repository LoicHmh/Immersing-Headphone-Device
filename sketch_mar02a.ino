//将三轴加速计数据通过蓝牙传输到手机
#include "CurieBLE.h"//
#include "CurieIMU.h"

float ax,ay,az,gx,gy,gz;
int Rax,Ray,Raz,Rgx,Rgy,Rgz;
int dur=1;
float anglex=0,angley=0,anglez=0;

BLEPeripheral blePeripheral;//
BLEService heartRateService("180D");//
BLECharacteristic heartRateChar("2A37",BLERead | BLENotify, 2);//
int oldHeartRate = 0; //
long previousMillis = 0;//

void setup() {
  Serial.begin(9600); 
  while (!Serial);
  pinMode(13, OUTPUT);//
   
  CurieIMU.begin();
  CurieIMU.setAccelerometerRate(1600);
  CurieIMU.setAccelerometerRange(8);  
  CurieIMU.autoCalibrateAccelerometerOffset(X_AXIS,0);
  CurieIMU.autoCalibrateAccelerometerOffset(Y_AXIS,0);
  CurieIMU.autoCalibrateAccelerometerOffset(Z_AXIS,1); 
  blePeripheral.setLocalName("HeartRateSketch");//
  blePeripheral.setAdvertisedServiceUuid(heartRateService.uuid());//
  blePeripheral.addAttribute(heartRateService);//
  blePeripheral.addAttribute(heartRateChar); //
  blePeripheral.begin();
  Serial.println("Bluetooth device active, waiting for connections...");//
}

void loop() {
  BLECentral central = blePeripheral.central();
  if (central) 
  {
    Serial.print("Connected to central: ");
    Serial.println(central.address());
    digitalWrite(13, HIGH);
    while (central.connected())
    {
      long currentMillis = millis();
      if (currentMillis - previousMillis >= 200) 
     {
        previousMillis = currentMillis;
        getAcce();
      }
    }
    
    digitalWrite(13, LOW);
    Serial.print("Disconnected from central: ");
    Serial.println(central.address());
  }  
}

void getAcce()
{
  CurieIMU.readAccelerometer( Rax, Ray, Raz);
  ax=convertAcce(Rax);
  ay=convertAcce(Ray);
  az=convertAcce(Raz);

  anglex=ang(az,ay);
  angley=ang(ax,az);
  anglez=ang(ay,ax);
  
  //int heartRate = map(anglex, 0, 1023, 0, 100);
  float heartRate=anglex;
  if (heartRate != oldHeartRate) 
  {      
    Serial.print("Angle x is now: "); 
    Serial.println(heartRate);
    const unsigned char heartRateCharArray[2] = { 0, (char)heartRate };
    heartRateChar.setValue(heartRateCharArray, 2);  
    oldHeartRate = heartRate;
  }
  
  //Serial.print(anglex);
  //Serial.print("\t");
  //Serial.print(angley);
  //Serial.print("\t");
  //Serial.print(anglez);
  //Serial.println();  
  //delay(dur);
}



float convertAcce( int a)
{
  return (a * 8.0) / 32768.0;
}

float ang(float a1,float a2)
{
  float x1=acos(a1)/3.14*180.0;
  float x2=-asin(a2)/3.14*180.0;
  if (x1>45)
    return x1;
  else
    return x2;
}
