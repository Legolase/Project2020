#include <SPI.h>
#include <SD.h>
#include <time.h>
#include <DHT.h>

#define DHTPIN 2
#define DHTTYPE DHT22

DHT dht(DHTPIN, DHTTYPE);

const int analogInPin = A0; // Указываем пин, к которому подключен датчик
int sensorValue = 0; // Объявляем переменную для хранения значений с датчика  
					 //и задаем ее начальное значение 0
					 
const int PIN_CHIP_SELECT = 4;
					 
void setup() {
  Serial.begin(9600);
  Serial.print("Initializing SD card...");
 // Этот пин обязательно должен быть определен как OUTPUT
  pinMode(10, OUTPUT);
  // Пытаемся проинициализировать модуль
  if (!SD.begin(PIN_CHIP_SELECT)) {
	Serial.println("Card failed, or not present");
	// Если что-то пошло не так, завершаем работу:
	return;
  }
  Serial.println("Card initialized."); 
  Serial.println("MQ2 Test"); //Посылаем текст в монитор порта, чтобы следить за выполнением программы
  Serial.println("DHTxx test!");
  dht.begin();
}
  
void loop() {
  sensorValue = analogRead(analogInPin); //считываем значения с датчика
  int time=millis()/1000; //получаем количество секунд после старта
  int h; 
  int m;
  int s;
  char strTime[20] = ""; 
  h = time/60/60; //часы
  m = time/60%60; //минуты
  s = time%60;	//секунды
  sprintf (strTime, "%.2i:%.2i:%.2i", h, m, s); //записываем переменные в строку нужного формата
  float hyd = dht.readHumidity();
  float tem = dht.readTemperature();
  
  
  
//Запись в файл
  File dataFile = SD.open("sensor.txt", FILE_WRITE);
  // Если все хорошо, то записываем строку:
  if (dataFile) {
	dataFile.print(strTime);
	dataFile.print(sensorValue);
	dataFile.print(" ");
	dataFile.print(tem);
	dataFile.print(" ");
	dataFile.println(hyd);
	dataFile.close();
	// Публикуем в мониторе порта для отладки
  Serial.print(strTime);
  Serial.print("MQ135 value= "); //Для отслеживания данных с датчика транслируем их в монитор порта
  Serial.print(sensorValue);
  Serial.print(" H= "); //Для отслеживания данных с датчика транслируем их в монитор порта
  Serial.print(hyd);
  Serial.print(" T= "); //Для отслеживания данных с датчика транслируем их в монитор порта
  Serial.println(tem);
  }
  else {
  // Сообщаем об ошибке, если все плохо
	Serial.println("error opening sensor.txt");
  }
  
  delay(1000);
  }
 
