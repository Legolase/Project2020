#include <SPI.h>
#include <SD.h>
#include <time.h>
#include <DHT.h>

#define DHTPIN 2
#define DHTTYPE DHT22

DHT dht(DHTPIN, DHTTYPE);

const int analogInPin = A0; // ��������� ���, � �������� ��������� ������
int sensorValue = 0; // ��������� ���������� ��� �������� �������� � ������� 
					 //� ������ �� ��������� �������� 0
					 
const int PIN_CHIP_SELECT = 4;
					 
void setup() {
  Serial.begin(9600);
  Serial.print("Initializing SD card...");
 // ���� ��� ����������� ������ ���� ��������� ��� OUTPUT
  pinMode(10, OUTPUT);
  // �������� ������������������� ������
  if (!SD.begin(PIN_CHIP_SELECT)) {
	Serial.println("Card failed, or not present");
	// ���� ���-�� ����� �� ���, ��������� ������:
	return;
  }
  Serial.println("�ard initialized."); 
  Serial.println("MQ2 Test"); //�������� ����� � ������� �����, ����� ������� �� ����������� ���������  
  Serial.println("DHTxx test!");
  dht.begin();
}
  
void loop() {
  sensorValue = analogRead(analogInPin); //��������� �������� � �������
  int time=millis()/1000; //�������� ���������� ������ ����� ������
  int h; 
  int m;
  int s;
  char strTime[20] = ""; 
  h = time/60/60; //����
  m = time/60%60; //������
  s = time%60;	//�������
  sprintf (strTime, "%.2i:%.2i:%.2i", h, m, s); //���������� ���������� � ������ ������� �������
  float hyd = dht.readHumidity();
  float tem = dht.readTemperature();
  
  
  
//������ � ����
  File dataFile = SD.open("sensor.txt", FILE_WRITE);
  // ���� ��� ������, �� ���������� ������:
  if (dataFile) {
	dataFile.print(strTime);
	dataFile.print(sensorValue);
	dataFile.print(" ");
	dataFile.print(tem);
	dataFile.print(" ");
	dataFile.println(hyd);
	dataFile.close();
	// ��������� � �������� ����� ��� �������
  Serial.print(strTime);
  Serial.print("MQ135 value= "); //��� ������������ ������ � ������� ����������� �� � ������� �����
  Serial.print(sensorValue);
  Serial.print(" H= "); //��� ������������ ������ � ������� ����������� �� � ������� �����
  Serial.print(hyd);
  Serial.print(" T= "); //��� ������������ ������ � ������� ����������� �� � ������� �����
  Serial.println(tem);
  }
  else {
  // �������� �� ������, ���� ��� �����
	Serial.println("error opening sensor.txt");
  }
  
  delay(1000);
  }
 
