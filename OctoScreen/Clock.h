#include "stdio.h"
#define CLOCK_DURATION 8000
#define CLOCK_REFRESH 60000
typedef void (*Clock)(void);




void doDisplay() {
  char buf[240];

  //tft.setTextDatum(MC_DATUM);
  tft.fillScreen(TFT_BLACK);  // Clear screen

  //  tft.setFreeFont(FF1);
  //  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  //  tft.drawString("Etat :", 0, 10, GFXFF);

  tft.setTextDatum(TC_DATUM);
  tft.setFreeFont(FF6);
  if (dispPrinter.printerStateId == 4) {
    tft.setTextColor(TFT_RED, TFT_BLACK);
  } else if (dispPrinter.printerStateId == 3) {
    tft.setTextColor(TFT_ORANGE, TFT_BLACK);
  } else if (dispPrinter.printerStateId == 2) {
    tft.setTextColor(TFT_BLUE, TFT_BLACK);
  } else {
    tft.setTextColor(TFT_YELLOW, TFT_BLACK);
  }
  tft.drawString(dispPrinter.printerState, 160, 10, GFXFF);

  tft.setTextDatum(TL_DATUM);
  tft.setTextColor(TFT_YELLOW, TFT_BLACK);
  if (String(dispPrinter.jobFileDate).substring(0, 4) != "1970")  {
    tft.setFreeFont(FF1);
    tft.drawString(dispPrinter.jobFileName, 5, 35);
    tft.drawString(dispPrinter.jobFileDate, 5, 50);
    tft.setTextDatum(TR_DATUM);
    tft.drawString(dispPrinter.jobFileSize, 315, 50);
    tft.setTextDatum(TL_DATUM);
  }

  tft.setTextColor(TFT_GREEN, TFT_BLACK);
  tft.setFreeFont(FF6);
  //tft.drawString("Avancement :", 0, 60, GFXFF);
  if (dispPrinter.jobprogress != 0) {
    sprintf(buf, "%.1f%%", dispPrinter.jobprogress);
    tft.drawString(buf, 5, 75);
    tft.drawString(dispPrinter.statTime, 100, 75);
    tft.drawString(dispPrinter.leftTime, 250, 75);
  }

  tft.setTextDatum(TC_DATUM);
  if (dispPrinter.printerStateId == 4) {
    tft.setFreeFont(FMB24);
    tft.drawString(tempInDoor, 160, 150);
  } else {
    sprintf(buf, "%.1f/%.f  %.f/%.f  %s",
            floorf(dispPrinter.Tool0TempActual), floorf(dispPrinter.Tool0TempTarget),
            floorf(dispPrinter.BedTempActual), floorf(dispPrinter.BedTempTarget),
            tempInDoor);
    tft.setFreeFont(FF6);
    tft.drawString(buf, 160, 105);
  }

  tft.setFreeFont(FF6);
  tft.setTextColor(TFT_WHITE, TFT_DARKGREY);
  tft.setTextDatum(TL_DATUM);
  tft.drawString(btnStat.btnGLib, 5, 220);
  tft.setTextDatum(TR_DATUM);
  tft.drawString(btnStat.btnDLib, 315, 220);
}