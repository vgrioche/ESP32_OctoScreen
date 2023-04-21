#define BUTTON_GAUCHE 25
#define BUTTON_DROITE 26



void getUrl(String url) {
  HTTPClient http;
  http.begin(url.c_str());

  int httpResponseCode = http.GET();

  if (httpResponseCode > 0) {
    Serial.print("HTTP Response code: ");
    Serial.println(httpResponseCode);
    String payload = http.getString();
    Serial.println(payload);
  } else {
    Serial.print("Error code: ");
    Serial.println(httpResponseCode);
  }
  // Free resources
  http.end();
}


void doBtnState() {

  btnStat.btnGState = digitalRead(BUTTON_GAUCHE);
  btnStat.btnDState = digitalRead(BUTTON_DROITE);
  btnStat.btnGOn = false;
  btnStat.btnDOn = false;

  if (btnStat.btnGState == 0 && btnStat.btnGLastState == 1) {
     btnStat.btnGOn = true;
     Serial.println("Bouton Gauche");
   }
   if (btnStat.btnDState == 0 && btnStat.btnDLastState == 1) {
     btnStat.btnDOn = true;
     Serial.println("Bouton Droite");
   }
  btnStat.btnGLastState = btnStat.btnGState;
  btnStat.btnDLastState = btnStat.btnDState;


  if (btnStat.btnGOn) {
    Serial.println("PS=" + dispPrinter.printerStats.printerState);
    Serial.println("PS=" + dispPrinter.printerState);

    if (dispPrinter.printerStateId == 4) {
      Serial.println("Switch ON");
      getUrl(UrlSWOn);
      delay(5000);
      updOctopi();
      if (api.getPrintJob()) {
        api.octoPrintConnectionAutoConnect();
      }
    } else if (dispPrinter.printerStateId == 0) { //"Operational"
      Serial.println("Do octoPrintJobStart");
      api.octoPrintJobStart();
    } else if (dispPrinter.printerStats.printerStatepaused || dispPrinter.printerStats.printerStatepausing) {
      Serial.println("Do octoPrintJobResume");
      api.octoPrintJobResume();
    } else if (dispPrinter.printerStats.printerStatePrinting) {
      Serial.println("Do octoPrintJobPause");
      api.octoPrintJobPause();
    }

    delay(3000);
    updOctopi();
  }

  if (btnStat.btnDOn) {
    if (dispPrinter.printerStateId == 0 || dispPrinter.printerStateId == 4) {  //Operational ou Error
      Serial.println("Switch OFF");
      getUrl(UrlSWOff);
      delay(1000);
      updOctopi();
    } else if (dispPrinter.printerStats.printerStatePrinting) {
      Serial.println("Do octoPrintJobCancel");
      api.octoPrintJobCancel();
    } else if (dispPrinter.printerStats.printerStatepaused) {
      Serial.println("Do octoPrintJobCancel");
      api.octoPrintJobCancel();
    }
    delay(3000);
    updOctopi();
  }



  if (btnStat.btnGOn || btnStat.btnDOn) {
    if (dispPrinter.printerStats.printerStateclosedOrError) {
      Serial.println("printerStateclosedOrError");
    }
    if (dispPrinter.printerStats.printerStateerror) {
      Serial.println("printerStateerror");
    }
    if (dispPrinter.printerStats.printerStatefinishing) {
      Serial.println("printerStatefinishing");
    }
    if (dispPrinter.printerStats.printerStateoperational) {
      Serial.println("printerStateoperational");
    }
    if (dispPrinter.printerStats.printerStatepaused) {
      Serial.println("printerStatepaused");
    }
    if (dispPrinter.printerStats.printerStatepausing) {
      Serial.println("printerStatepausing");
    }
    if (dispPrinter.printerStats.printerStatePrinting) {
      Serial.println("printerStatePrinting");
    }
    if (dispPrinter.printerStats.printerStateresuming) {
      Serial.println("printerStateresuming");
    }
    if (dispPrinter.printerStats.printerStatesdReady) {
      Serial.println("printerStatesdReady");
    }
  }
}