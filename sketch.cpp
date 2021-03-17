//Final sketch
#include <SPI.h>
#include <SD.h>
#include <time.h>
#include <DHT.h>
#include <TinyGPS++.h>
#include <SoftwareSerial.h>

//set readers from sensors

//dht
#define DHTPIN 2
#define DHTTYPE DHT22

DHT dht(DHTPIN, DHTTYPE);

//set pin from gas sensor
const int analogInPin = A0;
int sensorValue = 0;

//set SD pin
const int PIN_CHIP_SELECT = 4;

//set GPS settings
static const int RXPin = 5, TXPin = 3;
static const uint32_t GPSBaud = 9600;
TinyGPSPlus gps;
SoftwareSerial ss(RXPin, TXPin);

//
float oldHym = 0, oldTem = 0;
//

void setup() {
  Serial.begin(115200);
  ss.begin(GPSBaud);
  dht.begin();
  pinMode(10, OUTPUT);
  if (!SD.begin(PIN_CHIP_SELECT)) {
    Serial.println("Card failed, or not present");
    // Если что-то пошло не так, завершаем работу:
    return;
  }
  Serial.println("Сard initialized."); 
  Serial.println("MQ2 Test"); //Посылаем текст в монитор порта, чтобы следить за выполнением программы  
  Serial.println("DHTxx test!");
}

void loop() {
  bool isnan = false;
  sensorValue = analogRead(analogInPin); //считываем значения с датчика 
  int time=millis()/1000; //получаем количество секунд после старта 

  //time
  int h = time/60/60, m = time/60%60, s = time%60;

  //GPS
  
  double latitude = (gps.location.isValid()) ? gps.location.lat() : 0.0;
  double longitude = (gps.location.isValid()) ? gps.location.lng() : 0.0; 
  double height = (gps.altitude.isValid()) ? gps.altitude.meters(): 0.0;

  //humidity and temperature
  float hyd = dht.readHumidity();
  float tem = dht.readTemperature(); 

  if ((hyd != hyd) || (tem != tem)){
    hyd = oldHym;
    tem = oldTem;
    isnan= true;
  }
  else{
    oldHym = hyd;
    oldTem = tem;
  }
  
  char strTime[20] = "", strGPS[50] = "";  
  sprintf (strTime, "%.2i:%.2i:%.2i", h, m, s); //записываем переменные в строку нужного формат

  File dataFile = SD.open("sensor.txt", FILE_WRITE); 

  if (dataFile){
    dataFile.print(strTime);
    dataFile.print(' ');
    dataFile.print(sensorValue);
    dataFile.print(' ');
    dataFile.print(hyd);
    dataFile.print(' ');
    dataFile.print(tem);
    dataFile.print(' ');
    dataFile.print(latitude, 15);
    dataFile.print(' ');
    dataFile.print(longitude, 15);
    dataFile.print(' ');
    dataFile.print(height, 1);
    dataFile.println();
    
    dataFile.close();
  }
  else
    Serial.println("error opening sensor.txt"); 
  
  Serial.print(strTime);
  Serial.print(" || ");
  Serial.print(sensorValue);
  Serial.print(" || ");
  Serial.print(hyd);
  Serial.print(' ');
  Serial.print(tem);
  Serial.print(" || ");
  Serial.print(latitude, 10);
  Serial.print(' ');
  Serial.print(longitude, 10);
  Serial.print(' ');
  Serial.print(height, 1);
  if (isnan)
      Serial.print(" <- there was nan");
  Serial.println();

  smartDelay(1000);

  if (millis() > 5000 && gps.charsProcessed() < 10)
    Serial.println(F("No GPS data received: check wiring"));
}

static void smartDelay(unsigned long ms)
{
  unsigned long start = millis();
  do 
  {
    while (ss.available())
      gps.encode(ss.read());
  } while (millis() - start < ms);
}
