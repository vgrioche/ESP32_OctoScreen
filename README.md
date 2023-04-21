# ESP32_OctoScreen
Ecran pour OctoPi
Utilisation d'un écran tactile TFT SPI 240 X 320 (ILI9341) avec un ESP32 

Voici comment j'ai branché l'écran à mon module ESP32:
    Broche VCC de l'écran : 3,3 V de l'ESP32
    Broche GND            : GND 
    Broche CS             : D15 
    Broche Reset          : D4 
    Broche DC             : D5
    Broche SDI (MOSI)     : D23
    Broche SCK            : D18
    Broche LED            : 3,3 V 
    Broche CDCK (MISO)    : D19
    Broche T_CLK          : D18
    Broche T_CS           : D21
    Broche T_DIN          : D23
    Broche T_DO           : D19
    Broche T_IRQ          : pas branchée
    
La bibliothèque TFT_eSPI de Bodmer peut être installée au moyen du gestionnaire de bibliothèques de l'IDE Arduino.    
dans le dossier TFT_eSPI/User_Setups : ajouter le fichier Setup42_ILI9341_ESP32.h
dans TFT_eSPI/User_Setup_Select.h :
  - commenter la ligne   #include <User_Setup.h>                        // Default setup is root library folder
  - decommenter la ligne #include <User_Setups/Setup42_ILI9341_ESP32.h> // Setup file for ESP32 and SPI ILI9341 240x320
