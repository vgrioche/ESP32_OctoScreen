//-=-=-=-=-=-=-=
// Octoprint
WiFiClient client;

#include <OctoPrintAPI.h>  //This is where the magic happens... shazam!

#include <Time.h>  //We will need these two just to do some rough time math on the timestamps we get
#include <TimeLib.h>


// Use one of the following:
OctoprintApi api(client, octoprint_ip, octoprint_httpPort, octoprint_apikey);  //If using IP address
//OctoprintApi api(client, octoprint_host, octoprint_httpPort, octoprint_apikey);  //If using hostname. Comment out one or the other.


struct displayPrinter {
  int printerStateId;
  String printerState;
  String jobFileName;
  char jobFileSize[15];
  char jobFileDate[31];
  float jobprogress;
  char statTime[31];
  char leftTime[31];
  float Tool0TempActual;
  float Tool0TempTarget;
  float BedTempActual;
  float BedTempTarget;

  printerStatistics printerStats;
};

displayPrinter dispPrinter;


struct structBtn {
  int btnGState;
  int btnDState;
  int btnGLastState;
  int btnDLastState;
  bool btnGOn;
  bool btnDOn;
  String btnGLib;
  String btnDLib;
};

structBtn btnStat;



void updOctopi();  // Prototype so PlatformIO doesn't complain
Task taskUpdOctopi(TASK_MINUTE * 0.4, TASK_FOREVER, &updOctopi);

void updOctopi() {

  dispPrinter.printerState = "";
  dispPrinter.printerStateId = 0;

  Serial.println("---updOctopi()------------------------");
  if (api.getPrintJob()) {  //Get the print job API endpoint
    Serial.println("--------------getPrintJob()-------------");


    /* Example use case of data being returned, outputting to serial monitor. 
         * Could easily by an OLED screen, LEDs, Neopixel bar, servo motor etc.
         */
    Serial.print("Printer current state string:\t\t");
    Serial.println(api.printJob.printerState);
    Serial.print("File:\t\t\t");
    Serial.println(api.printJob.jobFileName);

    //Human readable MB size:
    const float bytes_in_megabytes = 1048576;
    const float temp_size_mb = api.printJob.jobFileSize / bytes_in_megabytes;
    Serial.print("File size:\t\t");
    Serial.print(temp_size_mb);
    Serial.println("MB");

    sprintf(dispPrinter.jobFileSize, "%0.2fMb", temp_size_mb);

    //File upload time in human readable date and time YYYY-MM-DD HH:MM:SS
    //char bufDate[31];
    Serial.print("Uploaded:\t\t");
    sprintf(dispPrinter.jobFileDate, "%04d-%02d-%02d %02d:%02d",
            year(api.printJob.jobFileDate),
            month(api.printJob.jobFileDate),
            day(api.printJob.jobFileDate),
            hour(api.printJob.jobFileDate),
            minute(api.printJob.jobFileDate));
    //          second(api.printJob.jobFileDate));
    Serial.println(dispPrinter.jobFileDate);

    dispPrinter.printerState = api.printJob.printerState;
    dispPrinter.jobFileName = api.printJob.jobFileName;

    //Percentage of current print job complete
    const float temp_percent = floor(api.printJob.progressCompletion * 100) / 100;
    Serial.print("Percentage complete:\t");
    Serial.print(temp_percent);
    Serial.println("%");

    dispPrinter.jobprogress = temp_percent;


    //Print time left (if printing) in human readable time HH:MM:SS
    int runHours = api.printJob.progressPrintTime / 3600;
    int runSecs = api.printJob.progressPrintTime % 3600;
    int runMinutes = runSecs / 60;
    //int runSeconds = runSecs % 60;
    //char buf[31];
    Serial.print("Print time start:\t");
    sprintf(dispPrinter.statTime, "%02d:%02d", runHours, runMinutes);
    Serial.println(dispPrinter.statTime);

    //Print time left (if printing) in human readable time HH:MM:SS
    runHours = api.printJob.progressPrintTimeLeft / 3600;
    runSecs = api.printJob.progressPrintTimeLeft % 3600;
    runMinutes = runSecs / 60;
    //runSeconds = runSecs % 60;
    //char buf[31];
    Serial.print("Print time left:\t");
    sprintf(dispPrinter.leftTime, "%02d:%02d", runHours, runMinutes);
    Serial.println(dispPrinter.leftTime);
    Serial.println("----------------------------------------");
  }
  if (api.getPrinterStatistics()) {  //Get the print job API endpoint
    Serial.println("--------------getPrinterStats()-------------");
    Serial.print("Printer current state string:\t\t");
    Serial.println(api.printerStats.printerState);
    Serial.print("Tools:\t");
    Serial.print(api.printerStats.printerTool0TempActual);
    Serial.print("/");
    Serial.println(api.printerStats.printerTool0TempTarget);

    Serial.print("Bed:\t");
    Serial.print(api.printerStats.printerBedTempActual);
    Serial.print("/");
    Serial.println(api.printerStats.printerBedTempTarget);

    dispPrinter.Tool0TempActual = api.printerStats.printerTool0TempActual;
    dispPrinter.Tool0TempTarget = api.printerStats.printerTool0TempTarget;
    dispPrinter.BedTempActual = api.printerStats.printerBedTempActual;
    dispPrinter.BedTempTarget = api.printerStats.printerBedTempTarget;
    dispPrinter.printerStats = api.printerStats;

    Serial.println("----------------------------------------");

    btnStat.btnGLib = " ";
    btnStat.btnDLib = " ";

    if (dispPrinter.printerState.substring(0, 5) == "Error") {
      dispPrinter.printerState = "Erreur";
      dispPrinter.printerStateId = 4;
      taskUpdOctopi.setInterval(TASK_MINUTE * 0.4);
      btnStat.btnGLib = "- On -";
      btnStat.btnDLib = "- Off ";
    } else if (dispPrinter.printerState.substring(0, 7) == "Offline") {
      dispPrinter.printerState = "Hors ligne";
      dispPrinter.printerStateId = 4;
      taskUpdOctopi.setInterval(TASK_MINUTE * 0.4);
      btnStat.btnGLib = "- On -";
      btnStat.btnDLib = "";
    } else if (dispPrinter.printerState.substring(0, 11) == "Operational") {
      dispPrinter.printerState = "Opérationnel";
      dispPrinter.printerStateId = 0;
      taskUpdOctopi.setInterval(TASK_SECOND * 15);
      btnStat.btnGLib = "- Imprimmer -";
      btnStat.btnDLib = "- Off -";
    } else if (dispPrinter.printerStats.printerStatepaused || dispPrinter.printerStats.printerStatepausing) {
      dispPrinter.printerState = "En pause";
      dispPrinter.printerStateId = 3;
      taskUpdOctopi.setInterval(TASK_SECOND * 2);
      btnStat.btnGLib = "- Reprendre -";
      btnStat.btnDLib = "- Annuler -";
    } else if (dispPrinter.printerStats.printerStatePrinting) {
      dispPrinter.printerState = "Impression";
      dispPrinter.printerStateId = 2;
      taskUpdOctopi.setInterval(TASK_SECOND * 5);
      btnStat.btnGLib = "- Pause -";
      btnStat.btnDLib = "- Annuler -";
    }

    Serial.println(btnStat.btnGLib + " " + btnStat.btnDLib);
  }

  UpdDisplay = true;
}