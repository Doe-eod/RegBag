![grafik](https://github.com/user-attachments/assets/24e58b97-9007-4072-8249-64b7e11a98b5)



# RegBag für Windows
```
RegBag
  von: Sven Herz
```
* Author: Sven Herz
* Getestet: Windows 11


## Beschreibung

RegBag ist eine simple Terminal-Anwendung, mit der Shellbags unter Windows angezeigt werden können. Dabei werden die hinterlegten Hex-Werte extrahiert und übersetzt. 


## Programmstart

Das Programm ist für Windows programmiert worden. Es kann sowohl mit einer CMD, aber auch mit Powershell ausgeführt werden. Die bessere Darstellung wird mit Powershell erreicht. Um das Programm zu startet, muss lediglich in den Ordner navigiert und folgender Befehl ausgeführt werden:

```
./regbag.exe
./regbag.exe -v -a1
./regbag.exe -v -a2
```


## Optionen

Insgesamt stehen drei Optionen zur Auswahl:

```
-v (verbose)
-a1 (approximation 1)
-a2 (approximation 2)
```

- Verbose zeigt eine detalierte Ausgabe der aktuell durchsuchten Registry-Pfade.
- Approximation 1 versucht menschenlesbare Zeichen aus der Heximaldarstellung auszugeben
- Approximation 2 versucht eine bessere Einrückung

Als Default-Aktion ohne gesetzte Option wird der jeweilige Hex-Wert der Shallbags ausgegeben.


## Projekt kompilieren

Um das Projekt zu bauen, kann MinGW genutzt werden. Bei dem folgenden Befehl müssen die Pfade zum Projekt noch angegeben werden:

```
gcc -o <PFAD>/regbag.exe <PFAD>/regbag.h <PFAD>/regbag.c <PFAD>/banner.c <PFAD>/help_functions.c  -lAdvapi32

```
