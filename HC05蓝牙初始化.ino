void setup() {
  Serial.begin(38400);//使用波特率为38400的串口进入蓝牙模块AT模式设置参数
}

void sendcmd()
{
  
  Serial.println("AT");//显示是否进入AT模式
  while(Serial.available())
  {
    char ch;
    ch = Serial.read();
    Serial.print(ch);
  } // Get response: OK
  delay(1000); // wait for printing 

  Serial.println("AT+NAME=HC-05");//设置蓝牙名字为HC-05
  while(Serial.available())
  {
    char ch;
    ch = Serial.read();
    Serial.print(ch);
  }
  delay(1000);

  Serial.println("AT+PSWD=0000");//设置蓝牙密码为0000
  while(Serial.available())
  {
    char ch;
    ch = Serial.read();
    Serial.print(ch);
  }
  delay(1000);
  
  Serial.println("AT+UART=38400,0,0");//设置蓝牙工作的波特率为38400
  while(Serial.available())
  {
    char ch;
    ch = Serial.read();
    Serial.print(ch);
  }
  delay(1000);
    
  Serial.println("AT+ADDR?");//显示蓝牙模块的Mac地址
  while(Serial.available())
  {
    char ch;
    ch = Serial.read();
    Serial.print(ch);
  }
  delay(1000);
}

void loop() {
    sendcmd();
}

