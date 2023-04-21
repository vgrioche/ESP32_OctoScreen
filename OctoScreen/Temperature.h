//Temperature sensor
#define DHTPIN 0 //19
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);


unsigned long templastTime = 300000;
unsigned long temptimerDelay = 90000;
unsigned long temptimerDelayErr = 30000;


void updTemperatures();  // Prototype so PlatformIO doesn't complain
Task taskUpdTemperatures(TASK_SECOND * 50, TASK_FOREVER, &updTemperatures);


void updTemperatures() {
  //humInDoor = String(dht.readHumidity());
  char buf[4];
  sprintf(buf,"%.f", dht.readHumidity());
  humInDoor = String(buf);
  sprintf(buf,"%.1f", dht.readTemperature());
  tempInDoor = String(buf);
  if (humInDoor == "nan" || tempInDoor == "nan") {
    humInDoor = "";
    tempInDoor = "";
  }
  Serial.println("T in:" + tempInDoor);
  Serial.println("H in:" + humInDoor);

  UpdDisplay = true;
}