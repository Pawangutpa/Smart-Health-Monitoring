#define BLYNK_TEMPLATE_ID "TMPL3KEPKtPtp"
#define BLYNK_TEMPLATE_NAME "Body Monitor"
#define BLYNK_AUTH_TOKEN "tPirNdFrH7DiQ8_9bIwrdu64csAUyjkS"
#include<DHT.h>
#include<BlynkSimpleEsp8266.h>
#include <Wire.h>              // Include Wire library for I2C

// MPU6050 I2C address
#define MPU6050_ADDR           0x68

// Registers for gyroscope and accelerometer data
#define MPU6050_REG_ACCEL_XOUT_H 0x3B
#define MPU6050_REG_GYRO_XOUT_H  0x43
int m=0;
// MAX30100 I2C address and register for SpO2 and Heart Rate data
#define MAX30100_ADDR          0x57
#define MAX30100_REG_FIFO_DATA 0x07
#include<ESP8266WiFi.h>
#define PIN D13
#define TYPE DHT11
float t=0,tf=0,h=0;
DHT  dht(D5,DHT11) ; //dht is variable
float heart;
float bpspo2;
char auth[]=BLYNK_AUTH_TOKEN;
char wifi[]="One Plus 11R";
char wifipass[]="shivam12345"; 
float ecg=0;
BlynkTimer timer;
BLYNK_WRITE (V1){
ecg= param.asInt( ) ;
}
BLYNK_WRITE (V2){
t = param.asInt( ) ;
}
BLYNK_WRITE (V3){
tf = param.asInt( ) ;
}
BLYNK_WRITE (V4){
h = param.asInt( ) ;
}
BLYNK_WRITE (V5){
m= param.asInt( ) ;
}
BLYNK_WRITE (V6){
bpspo2= param.asInt( ) ;
}
void mytimer(){
Blynk.virtualWrite(V1,ecg);
Blynk.virtualWrite(V2,t);
Blynk.virtualWrite(V3,tf);
Blynk.virtualWrite(V4,h);
Blynk.virtualWrite(V5,m);
Blynk.virtualWrite(V6,bpspo2);

}
void setup() {
pinMode(D8,INPUT);
pinMode(D0,INPUT);
pinMode(D1,INPUT);
pinMode(D2,INPUT);
pinMode(D3,INPUT);
pinMode(A0,INPUT);
pinMode(D4,INPUT);
  Wire.begin();  // Initialize I2C communication
  
  // Initialize MPU6050
  Wire.beginTransmission(MPU6050_ADDR);
  Wire.write(0x6B);            // PWR_MGMT_1 register
  Wire.write(0);               // Set to 0 to wake up MPU6050
  Wire.endTransmission(true);
  
  // Initialize MAX30100
  Wire.beginTransmission(MAX30100_ADDR);
  Wire.write(0x02);            // Mode control register
  Wire.write(0x03);            // Set to enable SpO2 mode
  Wire.endTransmission(true);
dht.begin();
Serial.begin(9600);
Blynk.begin(auth,wifi,wifipass);
timer.setInterval(100L, mytimer);

}

void loop() {
  //
   t=dht.readTemperature();//celsius
 tf=dht.readTemperature(true);//fahren
   h=dht.readHumidity();
Serial.print("temp in cel is =");
Serial.println(t);
Serial.print("temp in fahren is =");
Serial.println(t);
Serial.print("humidity is =");
Serial.println(h);
//ecg
  if(digitalRead(D3)==1 ||digitalRead(D4)==1 ){
  Serial.println("!");
  }
  else{
     ecg=analogRead(A0);
Serial.println(ecg);
  }
  //heat rate
    // Read accelerometer data from MPU6050
  int16_t accel_x, accel_y, accel_z;
  Wire.beginTransmission(MPU6050_ADDR);
  Wire.write(MPU6050_REG_ACCEL_XOUT_H);
  Wire.endTransmission(false);
  Wire.requestFrom(MPU6050_ADDR, 6);
  accel_x = Wire.read() << 8 | Wire.read();
  accel_y = Wire.read() << 8 | Wire.read();
  accel_z = Wire.read() << 8 | Wire.read();
  
  // Read gyroscope data from MPU6050
  int16_t gyro_x, gyro_y, gyro_z;
  Wire.beginTransmission(MPU6050_ADDR);
  Wire.write(MPU6050_REG_GYRO_XOUT_H);
  Wire.endTransmission(false);
  Wire.requestFrom(MPU6050_ADDR, 6);
  gyro_x = Wire.read() << 8 | Wire.read();
  gyro_y = Wire.read() << 8 | Wire.read();
  gyro_z = Wire.read() << 8 | Wire.read();
  
  // Read data from MAX30100
  uint8_t fifo_data[4];
  Wire.beginTransmission(MAX30100_ADDR);
  Wire.write(MAX30100_REG_FIFO_DATA);
  Wire.endTransmission(false);
  delay(10); // Add a small delay to allow the MAX30100 sensor to prepare data
  Wire.requestFrom(MAX30100_ADDR, 4);
  for (int i = 0; i < 4; i++) {
    fifo_data[i] = Wire.read();
  }
  
  // Print sensor data
  Serial.print("MPU6050 Accelerometer: ");
  Serial.print("X = "); Serial.print(accel_x);
  Serial.print(", Y = "); Serial.print(accel_y);
  Serial.print(", Z = "); Serial.println(accel_z);
  
  Serial.print("MPU6050 Gyroscope: ");
  Serial.print("X = "); Serial.print(gyro_x);
  Serial.print(", Y = "); Serial.print(gyro_y);
  Serial.print(", Z = "); Serial.println(gyro_z);
  
  Serial.print("MAX30100 SpO2: ");
  Serial.print(fifo_data[0]); Serial.print(", ");
  heart=fifo_data[1];
  Serial.print("Heart Rate: ");
  Serial.println(fifo_data[1]);
  bpspo2=fifo_data[0];
  if(accel_y>0 and accel_y<1000){
    Serial.println("no movement ");
    m=0;
  }
  else if(accel_y<-10000 and accel_y>-17000 ){
    Serial.print(" right");
    m=1;
  }
   else if(accel_y<15000 and accel_y>10000 ){
    Serial.print(" left");
    m=-1;
  }
   Blynk.run();
    timer.run();
    delay(500);
  
}

                /////-------------------------------------With LM35 Sensor comments the above code  and uncomment the below code-------------------------------------------/////



// #define BLYNK_TEMPLATE_ID "TMPL3KEPKtPtp"
// #define BLYNK_TEMPLATE_NAME "Body Monitor"
// #define BLYNK_AUTH_TOKEN "tPirNdFrH7DiQ8_9bIwrdu64csAUyjkS"
// #include <DHT.h>
// #include <BlynkSimpleEsp8266.h>
// #include <Wire.h>
// #define MPU6050_ADDR 0x68
// #define MPU6050_REG_ACCEL_XOUT_H 0x3B
// #define MPU6050_REG_GYRO_XOUT_H 0x43
// #define MAX30100_ADDR 0x57
// #define MAX30100_REG_FIFO_DATA 0x07
// #include <ESP8266WiFi.h>
// #define PIN D13
// #define TYPE DHT11
// float t = 0, tf = 0, h = 0;
// DHT dht(D5, DHT11);
// float heart = 0;
// float bpspo2 = 0;
// float ecg = 0;
// float lm35_temp = 0;
// int m = 0;

// char auth[] = BLYNK_AUTH_TOKEN;
// char wifi[] = "One Plus 11R";
// char wifipass[] = "shivam12345";

// BlynkTimer timer;

// BLYNK_WRITE(V1) { ecg = param.asInt(); }
// BLYNK_WRITE(V2) { t = param.asInt(); }
// BLYNK_WRITE(V3) { tf = param.asInt(); }
// BLYNK_WRITE(V4) { h = param.asInt(); }
// BLYNK_WRITE(V5) { m = param.asInt(); }
// BLYNK_WRITE(V6) { bpspo2 = param.asInt(); }
// BLYNK_WRITE(V7) { lm35_temp = param.asInt(); }

// void mytimer() {
//   Blynk.virtualWrite(V1, ecg);
//   Blynk.virtualWrite(V2, t);
//   Blynk.virtualWrite(V3, tf);
//   Blynk.virtualWrite(V4, h);
//   Blynk.virtualWrite(V5, m);
//   Blynk.virtualWrite(V6, bpspo2);
//   Blynk.virtualWrite(V7, lm35_temp);
//   Blynk.virtualWrite(V8, heart);
// }

// void setup() {
//   pinMode(D8, INPUT);
//   pinMode(D0, INPUT);
//   pinMode(D1, INPUT);
//   pinMode(D2, INPUT);
//   pinMode(D3, INPUT);
//   pinMode(A0, INPUT);
//   pinMode(A1, INPUT);

//   Wire.begin();
//   Wire.beginTransmission(MPU6050_ADDR);
//   Wire.write(0x6B);
//   Wire.write(0);
//   Wire.endTransmission(true);

//   Wire.beginTransmission(MAX30100_ADDR);
//   Wire.write(0x02);
//   Wire.write(0x03);
//   Wire.endTransmission(true);

//   dht.begin();
//   Serial.begin(9600);
//   Blynk.begin(auth, wifi, wifipass);
//   timer.setInterval(100L, mytimer);
// }

// void loop() {
//   t = dht.readTemperature();
//   tf = dht.readTemperature(true);
//   h = dht.readHumidity();
//   Serial.print("Temperature in Celsius: ");
//   Serial.println(t);
//   Serial.print("Temperature in Fahrenheit: ");
//   Serial.println(tf);
//   Serial.print("Humidity: ");
//   Serial.println(h);

//   if (digitalRead(D3) == 1 || digitalRead(D4) == 1) {
//     Serial.println("!");
//   } else {
//     ecg = analogRead(A0);
//     Serial.print("ECG Value: ");
//     Serial.println(ecg);
//   }

//   int lm35_value = analogRead(A1);
//   lm35_temp = (lm35_value * 5.0 / 1023.0) * 100.0;
//   Serial.print("LM35 Temperature: ");
//   Serial.println(lm35_temp);

//   int16_t accel_x, accel_y, accel_z;
//   Wire.beginTransmission(MPU6050_ADDR);
//   Wire.write(MPU6050_REG_ACCEL_XOUT_H);
//   Wire.endTransmission(false);
//   Wire.requestFrom(MPU6050_ADDR, 6);
//   accel_x = Wire.read() << 8 | Wire.read();
//   accel_y = Wire.read() << 8 | Wire.read();
//   accel_z = Wire.read() << 8 | Wire.read();

//   int16_t gyro_x, gyro_y, gyro_z;
//   Wire.beginTransmission(MPU6050_ADDR);
//   Wire.write(MPU6050_REG_GYRO_XOUT_H);
//   Wire.endTransmission(false);
//   Wire.requestFrom(MPU6050_ADDR, 6);
//   gyro_x = Wire.read() << 8 | Wire.read();
//   gyro_y = Wire.read() << 8 | Wire.read();
//   gyro_z = Wire.read() << 8 | Wire.read();

//   uint8_t fifo_data[4];
//   Wire.beginTransmission(MAX30100_ADDR);
//   Wire.write(MAX30100_REG_FIFO_DATA);
//   Wire.endTransmission(false);
//   delay(10);
//   Wire.requestFrom(MAX30100_ADDR, 4);
//   for (int i = 0; i < 4; i++) {
//     fifo_data[i] = Wire.read();
//   }

//   bpspo2 = fifo_data[0];
//   heart = fifo_data[1];

//   Serial.print("MAX30100 SpO2: ");
//   Serial.print(bpspo2);
//   Serial.print(", Heart Rate: ");
//   Serial.println(heart);

//   if (accel_y > 0 && accel_y < 1000) {
//     Serial.println("No Movement");
//     m = 0;
//   } else if (accel_y < -10000 && accel_y > -17000) {
//     Serial.println("Movement Right");
//     m = 1;
//   } else if (accel_y > 10000 && accel_y < 15000) {
//     Serial.println("Movement Left");
//     m = -1;
//   }

//   Blynk.run();
//   timer.run();
//   delay(500);
// }

