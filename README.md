# Demo-Firmware für Servocontroller Lötkurs 2026

## Installieren mit PlatformIO

`platformio run -t upload`

## Installieren mit Arduino-IDE

main.cpp in neuen Sketch kopieren, htmlPage.hpp in neue Datei im Sketch-Ordner (ganz rechts neben den tabs, die drei Punkte, neue Datei), sollte so aussehen:

![alt text](figs/image-4.png)

Library installieren: 

![alt text](figs/image.png)


Board installieren: 

![ll](figs/image-3.png)

Auf dem Board: Sicherstellen, das der Power-Schalter auf "on" steht

Reset + Download-Knopf gedrückt halten, Reset loslassen, dann Download loslassen

Board im Dropdown auswählen

Dann auswählen:

![alt text](figs/image-1.png)

Dann kompilieren und herunterladen

Dann Reset drücken, wenn fertig