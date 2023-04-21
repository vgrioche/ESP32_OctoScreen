#include "Free_Fonts.h" // Include the header file attached to this sketch
#include "SPI.h"
#include "TFT_eSPI.h"
TFT_eSPI tft = TFT_eSPI();

bool UpdDisplay = false;

//************************************************************
#include <WiFi.h>
#include <HTTPClient.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>

const char* ssid     = "???????????";
const char* password = "???????????";

AsyncWebServer server(80);

//************************************************************
#include <TaskScheduler.h>
Scheduler userScheduler;  // to control your personal task

String tempInDoor = "";
String humInDoor = "";

//-=-=-=-=-=-=-=

IPAddress octoprint_ip(192, 168, 1, ???);  // Your IP address of your OctoPrint server (inernal or external)
const int octoprint_httpPort = 80;                             //If you are connecting through a router this will work, but you need a random port forwarded to the OctoPrint server from your router. Enter that port here if you are external
String octoprint_apikey = "???????????";  //See top of file or GIT Readme about getting API key

//Url jeddom pour allumer l'imp et la lumière.
String UrlSWOn  = "???????????";
String UrlSWOff = "???????????";

#include "Octopi.h"
#include "DHT.h"
#include "Temperature.h"
#include "Clock.h"
#include "btn.h"


void setup() {
  Serial.begin(115200);

  tft.begin();
  tft.setRotation(1);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.fillScreen(TFT_BLACK);            // Clear screen

  tft.setTextDatum(TC_DATUM);
  tft.setFreeFont(FF1);
  tft.drawString("Demmarage...", 160, 120, GFXFF);

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }
  // Print ESP Local IP Address
  Serial.println(WiFi.localIP());

  //Async webserver
  server.on("/", HTTP_GET, [](AsyncWebServerRequest* request) {
    if (request->hasArg("Get")) {
      String msg;
      //      msg = JSON.stringify(allDoc);
      request->send(200, "text/html", msg);
      Serial.printf("Get=%s\n", msg.c_str());
    }
  });

  server.begin();
  dht.begin();

  userScheduler.addTask(taskUpdOctopi);
  userScheduler.addTask(taskUpdTemperatures);

  taskUpdOctopi.enable();
  taskUpdTemperatures.enable();

  pinMode(BUTTON_GAUCHE, INPUT_PULLUP);
  pinMode(BUTTON_DROITE, INPUT_PULLUP);
  btnStat.btnGLastState = 1;
  btnStat.btnDLastState = 1;
}

void loop() {
  userScheduler.execute();
  doBtnState();

  if (UpdDisplay) {
    doDisplay();
    UpdDisplay = false;
  }
}
