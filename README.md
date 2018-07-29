# Break-it-Game

Ein [Breakout](https://de.wikipedia.org/wiki/Breakout_(Computerspiel)) Klon entstanden in der Freizeit als Hobby-Projekt. 

Es wurde die Bibliothek [SFML](https://www.sfml-dev.org/) benutzt. 

### Projekt selbst erstellen

Unter `Releases` ist der aktuellste Build für Windows-Systeme zu finden. Wer jedoch möchte kann das Projekt selbst erstellen. Dafür wird eine aktuelle Version des `GCC` Compilers benötigt. Unter Windows z. B. [MinGW](https://sourceforge.net/projects/mingw-w64/). (Damit wurde auch der Build unter `Releases` erstellt.) Außerdem wird eine aktuelle Version des Tool `CMake` (>3.5) benötigt.

Im Folgenden wird davon ausgegangen, dass der benutze Compiler MinGW ist. Für andere Compiler sind die Schritte ggf. etwas anders.

Da das Spiel mit `SFML` als Grundlage geschrieben wurde, werden zunächst die Binaries von `SFML` benötigt. Vorkompilierte Distributionen sind [hier](https://www.sfml-dev.org/download/sfml/2.4.2/) zu finden. Es ist auch möglich die Bibliothek selbst für den eigenen Compiler zu erstellen, dafür siehe SFML-Dokumentation. 

Zunächst muss eine lokale Kopie der Repository erzeugt werden.
```
git clone https://github.com/fayras/Break-it-Game.git
```

Danach kann das Projekt mit folgenden Befehlen erzeugt werden:
```
cd Break-it-Game
mkdir build
cd build
cmake .. -G "MinGW Makefiles"
cmake --build .
```

Hinweis: Sollte `SFML` nicht in einem Ordner liegen, welcher automatisch gefunden werden kann, so kann die CMake-Variable `SFML_ROOT_PATH=/path/to/sfml` gesetzt werden.

Nachdem das Projekt erfolgreich erstellt wurde muss noch die Datei `openal32.dll` aus dem Verzeichnis `path/to/sfml/bin` kopiert werden. (Das hat damit zu tun, dass diese Datei dynamisch gelinkt werden muss auf Grund ihrer Lizenz.)

Dann ist es auch geschafft und das Spiel kann gestartet werden. Viel Spass!
