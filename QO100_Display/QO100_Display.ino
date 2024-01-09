// CIV_template.ino

// Bearbeitet von PA5MR && PE1OFO
// z_userprog von VK3PE17

/*
CIV_template – Ein einfaches Beispiel zur Handhabung eines Kommunikationskanals zu ICOM-Funkgeräten

Erstellt von Wilfried Dilling, DK8RW, Mai 2022
Freigegeben für die Öffentlichkeit

Diese Arduino-Skizze basiert auf der "CIVmasterLib"-Bibliothek, für die sie geschrieben wurde und steht unter der gemeinfreien Lizenz von Wilfried Dillig, DK8RW, im Mai 2022 oder neuer.

Die Skizze ist funktionsfähig und enthält zusätzliche Kommentare für besseres Verständnis. Die Software ist in Registerkarten unterteilt, um dem Benutzer das Erstellen eigener Skizzen auf Basis von CIVmasterLib zu erleichtern. Fügen Sie Ihren eigenen Code in den fast leeren Tab "z_userprog.ino" ein und setzen Sie die Aufrufe Ihrer Prozeduren an eine der mit "//!//" markierten Stellen.

Aufgaben:
- Vorlage zur Veranschaulichung der Funktionsweise der CIV-Klasse (Punkt-zu-Punkt-Verbindung)
- Ideal für die Verbindung zwischen einem CI-V-Master (Arduino oder ESP32) und einem ICOM-Radio unter Verwendung von CIVmasterLib
- Standardmäßig verarbeitet es Informationen wie Radio-Ein/Aus, TX/RX-Modus und verwendete Frequenz
- Optional können auch Modulationsmodus und RX-Filter verarbeitet werden (aktiviert mit "modmode" in a_defines.h)

Um einen ersten Eindruck zu erhalten, kompilieren Sie den unveränderten Sketch und führen Sie ihn auf einem ESP32 oder einem anderen unterstützten Arduino-Board aus. Werfen Sie einen Blick in den Seriellen Monitor (Baudrate 115200).

Hinweis: Der Sketch kann auch zur Steuerung der Radios verwendet werden, indem Sie Ihre Befehle im Abschnitt "CiV_sendCmds()" hinzufügen. Dies wird jedoch in diesem Beispiel nicht behandelt.

!!! Neu !!! 28.05.22 fastPTT!

Dank der Informationen von PU5VEN über VK3PE zu einem bestimmten ICOM-Befehl konnte ich einen "FastPTT"-Modus implementieren. Dadurch wurde die PTT-Verzögerung von durchschnittlich 80 ms ... 130 ms auf durchschnittlich 10 ms ... 30 ms reduziert! Das ist viel schneller und liegt im Bereich, den der IC705 kompensieren kann.

Das Risiko, etwas in die Luft zu jagen, ist also deutlich gesunken. Beachten Sie jedoch, dass dies alles Software ist, und der unten stehende Hinweis bleibt gültig.

Hinweis: Bitte seien Sie vorsichtig, wenn Sie RX/TX-Informationen als PTT für PAs verwenden möchten. Die Verzögerung ist leider ziemlich hoch (fastPTT: max. 30 ms). Der IC705 kann nur 30 ms kompensieren. Es liegt in Ihrer eigenen Verantwortung sicherzustellen, dass an Ihrem Radio oder der PA kein Schaden entsteht, wenn der PTT-Wechsel von RX zu TX später erfolgt als die HF-Leistung vom Funkgerät.

*/
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
// Datei "settings.h" einbinden (Standardmäßig ist der Debug-Modus aktiviert) !
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
#include "globals.h"
#include "Display.h"
#include "Touch.h"
#include <CIVmaster.h> // CIVcmds.h wird automatisch zusätzlich eingebunden
#include "FrequencyDigits.h"
#include <cstdint> 
#include "z_userprog.h"

/*
Parameter der Struktur CIVresult_t:
.retVal // Zusammenfassung/Ergebnis der Befehlsverarbeitung wie CIV_OK, CIV_NOK, ...
.address // Adresse des Radios, das die Nachricht empfangen/gesendet hat
.cmd[5] // Befehl, der vom Radio gesendet/empfangen wurde; cmd[0]= Länge des Befehls
.datafield[10] // Daten, die vom Radio gesendet/empfangen wurden; Datenfeld[0]= Länge der Daten
.value // Wenn das Datenfeld gültige Daten enthält, ist hier die Binärzahl. Andernfalls auf 0 setzen

Mögliche Rückgabewerte (retVal_t):
   CIV_OK = 0,     // Gültiger Befehl vom Funkgerät empfangen und akzeptiert
   CIV_OK_DAV = 1, // Gültiger Befehl vom Radio empfangen, akzeptiert und Daten (z. B. Frequenz)
                   // stehen zur Verfügung

   // Dies ist die Grenze zwischen Gut und Böse und wird als solche verwendet -----------------

   CIV_NOK = 2,          // Befehl vom Funkgerät empfangen, aber NICHT akzeptiert; Irgendwas stimmt nicht
   CIV_HW_FAULT = 3,     // Onewire-Bus: Datenleitung ist eine Abkürzung zur Erde
   CIV_BUS_BUSY = 4,     // Onewire-Bus: Versuch, Daten zu senden, ist aufgrund des Datenverkehrs auf dem Bus fehlgeschlagen
   CIV_BUS_CONFLICT = 5, // Onewire-Bus: Versuch, Daten zu senden, ist aufgrund einer Buskollision fehlgeschlagen
   CIV_NO_MSG = 6        // Keine Antwort vom Funkgerät!

   ... siehe auch CIVmaster.h ...
*/

// uint16_t wird hier verwendet, um RAM-Platz zu sparen (wegen der kleinen Atmel-Prozessoren)

void setup() {
  // Initialisieren der seriellen Schnittstelle (für Debug-Nachrichten)
  Serial.begin(debugBdRate);
  Serial.println("");
  delay(100);
  Serial.println(VERSION_STRING);

  // Civ-Objekt/Modul initialisieren (true bedeutet "Bluetooth verwenden" im Falle des ESP32)
  #ifdef useBT
    // Modifiziert von DL1BZ
    civ.setupp(true, NAME); // Bluetooth-Identifier am IC705 anzeigen
  #else
    civ.setupp(); // Onewire (HW)
  #endif

  // Civ mitteilen, welche Adresse des Radios verwendet werden soll
  civ.registerAddr(civAddr);

  // Hinweis: Im fastPTT-Modus kann dies signifikant langsamer sein, da dies in diesem Fall die PTT-Verzögerung nicht beeinflusst
  init_delayMeas; // Debugging auf ESP32: PTT-Verzögerungsmessung einrichten
  
  init_TFT();
  //!// Setzen Sie Ihren Setup-Code (unabhängig von Civ) hier ein ... oder legen Sie ihn in die Datei "z_userprog.ino"

  // Baseloop-Timer initialisieren...
  t_curr_lp = millis();
  ts_last_lp = t_curr_lp;
}

//============================ Hauptloop ====================================================

void loop() {
  t_curr_lp = millis();

  if (uint16_t(t_curr_lp - ts_last_lp) >= BASELOOP_TICK) {
    // Payload ------------------------------------------------------

    set_delayMeas; // Debugging auf ESP32: Überprüfen des Trigger-Eingangs und Speichern der Zeit, wenn sich der Eingang ändert

    // Zuerst die Antworten vom Radio lesen und verarbeiten (falls verfügbar)
    CIV_getProcessAnswers();

    // Dann Befehle an das Radio schreiben (falls möglich und erforderlich)
    CIV_sendCmds();

    // An diesem Punkt wurden alle verfügbaren Daten des Radios verarbeitet - CIV-Verarbeitung abgeschlossen!
    // Die Variablen in der "Datenbank" wurden aktualisiert, falls erforderlich, und Sie können mit ihnen arbeiten.

    //!// Setzen Sie Ihren Baseloop-Code (unabhängig von Civ) hier ein ... oder legen Sie ihn in die Datei "z_userprog.ino"
    userBaseLoop();

    // Wenn Sie Touch-Funktionen implementiert haben
    touchloop();

    // Payload ------------------------------------------------------
    ts_last_lp = t_curr_lp;
  } // if BASELOOP_TICK
} // Ende loop
