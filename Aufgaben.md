Aufgaben:

1) Die C++-Klasse WeatherStation wird über "import Backend 1.0" in die main.qml eingebunden und ein Objekt erstellt
2) Links - analog zur Tür rechts - soll ein Zahnradsymbol in grauer Farbe eingefügt werden. Klickt man auf das Symbol öffnet sich ein neues Fester. Dort gibt es drei Werte :  1) Ort - hier kann der Ort (einschließlich Breiten-/Längengrad) hinterlegt Werden. Die Voreinstellung ist Berlin. 2) Datenlieferant:  Open-Meteo - kann noch nicht verändert werden. Beim Bestätigen und gleichzeitigen Schließen wird der Wert in das Objekt der Klasse WeatherStation geschrieben. 3) Ob die Uhzeit auch die Sekunden anzeigen soll
3)  Alle 10 Minuten fragt die "main.qml" die Messwerte (Temperatur, Luftfeuchte und Luftdruck) bei der "WeatherStation" ab und aktualisiert die Anzeige. Die "WeatherStation" wiederum fragt die Messwerte bei Daten-Lieferanten Open-Meteo ab.
4) "main.qml" Zusätzlich soll der Ort angegeben werden.